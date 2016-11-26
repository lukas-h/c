#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include "minimp3.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/soundcard.h>
#include <pthread.h>

/* --- defaults & definitions --- */
#define CONFIG_PATH "/home/lukas/Projekte/c/rpi-audio/"
#define HTTP_PORT	8976
#define MAX_CONNECTIONS	20
#define FILE_CHUNK_SIZE	4096U

/* --- HTTP messages --- */
const char *HTTP_ERR_501 = "HTTP/1.0 501 Not Implemented\r\nContent-Type: text/html\r\nContent-length: 119\r\n\r\n"
	"<html><head><title>Error</title></head><body><hr><h1>HTTP method not implemented.</h1><hr><p>toy-http</p></body></html>";

const char *HTTP_ERR_404 = "HTTP/1.0 404 Not Found\r\nContent-Type: text/html\r\nContent-length: 106\r\n\r\n"
	"<html><head><title>Error</title></head><body><hr><h1>File not found.</h1><hr><p>toy-http</p></body></html>";

const char *HTTP_ERR_403 = "HTTP/1.0 403 Forbidden\r\nContent-Type: text/html\r\nContent-length: 105\r\n\r\n"
	"<html><head><title>Error</title></head><body><hr><h1>No permission.</h1><hr><p>toy-http</p></body></html>";

const char *HTTP_SUC_200 = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nContent-length: 95\r\n\r\n"
	"<html><head><title>OK</title></head><body><hr><h1>Success</h1><hr><p>toy-http</p></body></html>";

char *files[] = {
	"Bach-arr_Partita-E_I.mp3",
	"NOTHING"
};
size_t i =0;

ssize_t file_size(void);
int parse_head_line(const char *src, char *method, char *filepath);
ssize_t recv_line(int fd, char *buf, size_t len);
void *serve(void *id);
void *play_file(void *id);

/* --- globals --- */
int http_port = HTTP_PORT;
int max_connections = MAX_CONNECTIONS;
int serve_dir = 0;
pthread_t th; /* audio thread */
pthread_t http_th[MAX_CONNECTIONS];
int http_c[MAX_CONNECTIONS];
size_t http_th_c;
bool _stop=true;
bool _pause=false;
pthread_cond_t cnd;
pthread_mutex_t mtx;

/* --- error and signal handling --- */
#define error(msg, ...)		fprintf(stderr, "\033[1;41merror:\033[0m" msg, ##__VA_ARGS__)
#define warning(msg, ...)	fprintf(stderr, "\033[1;43mwarning:\033[0m" msg, ##__VA_ARGS__)
#define info(msg, ...)		fprintf(stdout, "\033[1;44minfo:\033[0m" msg, ##__VA_ARGS__)

#define SET_FD 1
#define CLOSE_FD 2
static void handle_fd(int fd, int instr){
	static int f;
	switch(instr){
		case SET_FD:
			f=fd? fd : -1;
		break;
		case CLOSE_FD:
			close(f);
		break;
	}
}
static void abort_program(int signum){
	handle_fd(0, CLOSE_FD);

	switch(signum){
		case SIGABRT: case SIGHUP:
			error("abnormal termination: exit");
		break;
		case SIGILL: case SIGSEGV:
			error("invalid instruction: please contact the maintainer");
		break;
		case SIGINT:
			info("exit");
		break;
		case SIGTERM: case SIGKILL:
			info("termination request: exit");
			signum = 0;
		break;
	}

	_exit((signum > 0) ? 1 : 0);
}

/* --- SERVER MAIN --- */
int main(int argc, char *argv[]){
	
	int fd, client, pid;
	struct sockaddr_in addr, client_addr;
	socklen_t siz;

	signal(SIGABRT, abort_program); //signal handling
	signal(SIGILL, abort_program);
	signal(SIGINT, abort_program);
	signal(SIGSEGV, abort_program);
	signal(SIGTERM, abort_program);
	signal(SIGCHLD, SIG_IGN);

	if(http_port > 65535){
		error("illegal port\n");
		abort_program(-1);
	}

	fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(fd == -1){
		error("can not create new socket\n");
		abort_program(-1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(http_port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in))==-1){
		error("bind() failed.\n >Is another server running on this port?\n");
		abort_program(-1);
	}

	if(listen(fd, max_connections)==-1){
		error("listen() failed\n");
		abort_program(-1);
	}

	if(chdir(CONFIG_PATH)){
		error("invalid path or can not set\n");
		abort_program(-1);
	}

	info("Server running!\n"
	" >host: http://127.0.0.1:%d\n"
	" >\033[1mCtrl-C\033[0m to abort.\n", http_port);
	handle_fd(fd, SET_FD);

	while(1){
		siz = sizeof(struct sockaddr_in);
		client = accept(fd, (struct sockaddr *)&client_addr, &siz);

		if(client==-1){
			error("accept() failed\n");
			continue;
		}

		pthread_create(&(http_th[http_th_c--]), NULL, serve, NULL);
	}
	close(fd);

	pthread_exit(NULL);
	return 0;
}


/* --- server procedures and file handling --- */
ssize_t file_size(){
	struct stat info;

    if(stat(CONFIG_PATH "index.html", &info)==-1){
        info("index.html not existing in config path: %s\n", CONFIG_PATH);
        abort_program(SIGINT);
    }
	return info.st_size;
}

ssize_t recv_line(int fd, char *buf, size_t len){
	size_t i=0;
	ssize_t err=1;
	while((i < len-1) && err==1){
		err = recv(fd, &(buf[i]), 1, 0);
		if(buf[i]=='\n'){ break; }
		else{ i++; }
	}
	if(i && (buf[i-1]=='\r')){ i--; }
	buf[i] = '\0';

	return i;
}

int parse_head_line(const char *src, char *method, char *filepath){
	if(strlen(src)<5)return 1;
	while((*method++ = *src++) && *src!=' ');

	src++; /* whitespace */
	while((*filepath++ = *src++) && *src!=' ');
	return 0;
}

#define EXIT() \
	shutdown(client, SHUT_RDWR); \
	close(client); \
	pthread_exit(NULL);

void *serve(void *id){
	int client = http_c[http_th_c-1];
	char buf[FILE_CHUNK_SIZE]="\0", request[8]="\0", url[256]="\0";
	ssize_t len;
	FILE *f;

	if(recv_line(client, buf, (sizeof(buf)-1) )<=3){
		warning("can not receive request\n");
		EXIT();
	}
	if(parse_head_line(buf, request, url)){
		warning("parsing error\n >request: `%s`\n", buf);
		EXIT();
	}

	while(recv_line(client, buf, (sizeof(buf)-1) ) > 0);

	if( strcmp(request, "GET")!=0 ){
		send(client, HTTP_ERR_501, strlen(HTTP_ERR_501), 0);
		warning("request method not supported\n >method: `%s`\n", request);
		EXIT();
	}

    if(strcmp(url, "/next/")==0){
        send(client, HTTP_ERR_501, strlen(HTTP_ERR_501), 0);
    }
    else if(strcmp(url, "/previous/")==0){
        send(client, HTTP_ERR_501, strlen(HTTP_ERR_501), 0);
    }
    else if(strcmp(url, "/pause/")==0){
        send(client, HTTP_SUC_200, strlen(HTTP_SUC_200), 0);

		/*pthread_mutex_lock(&mtx);
		_pause=true;
		pthread_mutex_unlock(&mtx);*/
    }
	else if(strcmp(url, "/resume/")==0){		
        send(client, HTTP_SUC_200, strlen(HTTP_SUC_200), 0);

		/*pthread_mutex_lock(&mtx);
		_pause=false;
		pthread_mutex_unlock(&mtx);*/
	}
	else if(strcmp(url, "/start/")==0){
        send(client, HTTP_SUC_200, strlen(HTTP_SUC_200), 0);
		if(_stop==true){
			_stop=false;
			if(pthread_create(&th, NULL, play_file, NULL)){
				error("can not create audio thread!");
			}
		}
	}
    else if(strcmp(url, "/stop/")==0){        
        send(client, HTTP_SUC_200, strlen(HTTP_SUC_200), 0);
		_stop=true;
    }
    else if(strcmp(url, "/")==0){
        f=fopen("index.html", "r");
        if(f==NULL){
            send(client, HTTP_ERR_404, strlen(HTTP_ERR_404), 0);
            EXIT();
        }
	    send(client, "HTTP/1.0 200 OK\r\nContent-type: text/html\r\n", 17, 0);
	    sprintf(buf, "Content-length: %ld\r\n\r\n", file_size());
        len = strlen(buf);
        send(client, buf, len, 0);
        while(!feof(f)){
			len = fread(buf, 1, FILE_CHUNK_SIZE, f);
			if(len){ send(client, buf, len, 0); }
		}
        fclose(f);
    }
    else{
        send(client, HTTP_ERR_404, strlen(HTTP_ERR_404), 0);
    }

	EXIT();
}

void *play_file(void *id){
	mp3_decoder_t mp3;
    mp3_info_t info;
    int fd, pcm;
    void *file_data;
    unsigned char *stream_pos;
    signed short sample_buf[MP3_MAX_SAMPLES_PER_FRAME];
    int bytes_left;
    int frame_size;
    int value;

	printf("should play now!\n");

    fd = open(files[i], O_RDONLY);
    if (fd < 0) {
		printf("can not open file\n");
        pthread_exit(NULL); 
    }
    
    bytes_left = lseek(fd, 0, SEEK_END);    
    file_data = mmap(0, bytes_left, PROT_READ, MAP_PRIVATE, fd, 0);
    stream_pos = (unsigned char *)file_data;
    bytes_left -= 100;

    mp3 = mp3_create();
    frame_size = mp3_decode(mp3, stream_pos, bytes_left, sample_buf, &info);
    if (!frame_size){
		error("can not get frame size\n");
        pthread_exit(NULL); 
    }
    
    pcm = open("/dev/dsp", O_WRONLY);
    if (pcm < 0){
		error("can not open /dev/dsp\n");
		pthread_exit(NULL); 
	}

    value = AFMT_S16_LE;
    if (ioctl(pcm, SNDCTL_DSP_SETFMT, &value) < 0){
		error("can not set audio format\n");
		pthread_exit(NULL); 
	}

    if (ioctl(pcm, SNDCTL_DSP_CHANNELS, &info.channels) < 0){
        error("can not set audio channels\n");
		pthread_exit(NULL);
	}
    if (ioctl(pcm, SNDCTL_DSP_SPEED, &info.sample_rate) < 0){
        error("can not set audio sample rate\n");
		pthread_exit(NULL); 
	}
	printf("HI");
    while((bytes_left >= 0) && (frame_size > 0)) {
		printf("*");
		/*if(_stop==true){
			close(pcm);
			pthread_exit(NULL);
		}*/
		/*pthread_mutex_lock(&mtx);
		while(_pause==true){
			pthread_cond_wait(&cnd, &mtx);
		}
		pthread_mutex_unlock(&mtx);*/

        stream_pos += frame_size;
        bytes_left -= frame_size;
        write(pcm, (const void *) sample_buf, info.audio_bytes);
        frame_size = mp3_decode(mp3, stream_pos, bytes_left, sample_buf, NULL);
    }

    close(pcm);
	pthread_exit(NULL); 
}