	#include <stdio.h>
	#include <string.h>
	#include <ctype.h>
	#include <stdlib.h>
	#include <stdbool.h>

	#include "lexer.h"

	const char *keywords[] ={
		"do",
		"while",
		"if",
		"else",
		"for"
	};

static void lexer_output_create(lexer_out_t *output){
    output->tokc = 0;
    output->tokv = (lexer_tok_t *)malloc(sizeof(lexer_tok_t) * (output->tokc + 1));
    output->tokv[0] = T_ENDOFAPP;
    output->tokv[0].content = NULL;
    
}
static void lexer_output_push(lexer_out_t *output, lexer_tok_t tok){
    output->tokv[output->tokc++] = tok;
    output->tokv = realloc(output->tokv, sizeof(lexer_tok_t) * (output->tokc + 1));
    output->tokv[output->tokc].type = T_ENDOFAPP;
    output->tokv[output->tokc].content = NULL;
}
static lexer_tok_t lexer_output_pop(lexer_out_t *output){

}
void lexer_output_destroy(lexer_out_t *output){

}

	static void oneline_comment(char **p){
		while(**p!='\n'){
			
			(*p)++;
		}
	}
	static void multiline_comment(char **p){
		(*p)++;
		while(**p!='~'){
			(*p)++;
		}
		(*p)++;
	}
	static char escaped_char(char **p){
		char c;
		switch(**p){
			case '\"':  return '\"';	 break;
			case '\'':	return '\'';	 break;
			case '\\':	return '\\';	 break;
			case 'a': 	return '\a';	 break;
			case 'b': 	return '\b';	 break;
			case 'e': 	return '\033';	 break;
			case 'f': 	return '\f';	 break;
			case 'n': 	return '\n';	 break;
			case 'r': 	return '\r';	 break;
			case 't': 	return '\t';	 break;
			case 'v': 	return '\v';	 break;

			/* octal value */
			case '0' ... '7':
				for (c = 0; isdigit(**p) && **p < '8'; (*p)++){
					c = c * 8 + (**p - '0');
				}
				return c;
			break;

			default:
				warning("escaped character `%c` not supported\n", **p);
				return **p;
			break;
		}
	}

	hgs_tok_t get_string_literal(char **p){
		hgs_tok_t tok;
		size_t i=1;
		char buf[LITERAL_MAX_SIZE];
		bool ctrl=true;
		tok.type = T_STR_LITERAL;
		
		while(i < LITERAL_MAX_SIZE && ctrl){
			(*p)++;
			switch(**p){
				case '\\':
					(*p)++;
					buf[i++] = escaped_char(p);
				break;
				case '\"':
					(*p)++;
					buf[i] = '\0';
					ctrl=false;
				break;
				default:
					buf[i++] = **p;
				break;
			}
		}
		buf[i] = '\0';

		tok.content = (char *)malloc(strlen(buf) + 1);
		strcpy(tok.content, buf);
		
		return tok;
	}

	hgs_tok_t get_identifier(char **p){
		hgs_tok_t tok;
		size_t i=1;
		char buf[VARIABLE_MAX_SIZE];
		bool ctrl=true;
		tok.type = T_NAME;
		buf[0] = **p;

		while(i < VARIABLE_MAX_SIZE && ctrl){
			(*p)++;
			if(IS_LEGAL_ID_CHAR(**p, buf[0])){
				buf[i++] = **p;
			} else{
				ctrl=false;
			}
		}
		buf[i] = '\0';

		tok.content = (char *)malloc(strlen(buf) + 1);
		strcpy(tok.content, buf);
		
		return tok;
	}

	hgs_tok_t get_numeric_literal(char **p){
		hgs_tok_t tok;
		size_t i=1;
		char buf[NUMERIC_MAX_SIZE];
		bool ctrl=true;
		tok.type = T_INT_LITERAL;
		buf[0] = **p;

		while(i < NUMERIC_MAX_SIZE && ctrl){
			(*p)++;
			switch(**p){
				case '0' ... '9':
					buf[i++] = **p;
				break;
				case '.':
					tok.type = T_FLOAT_LITERAL;
					buf[i++] = **p;
				break;
				default:
					ctrl=false;
				break;
			}
		}

		buf[i] = '\0';

		tok.content = (char *)malloc(strlen(buf) + 1);
		strcpy(tok.content, buf);
		
		return tok;
	}

	hgs_tok_t get_meta(char **p){
		hgs_tok_t tok;
		tok.content = NULL;
		switch(**p){
			case '{':
				tok.type = T_BLOCK_OPEN;
			break;
			case '}':
				tok.type = T_BLOCK_CLOSE;
			break;
			case '(':
				tok.type = T_BR_OPEN;
			break;
			case ')':
				tok.type = T_BR_CLOSE;
			break;
			case ',':
				tok.type = T_COMMA;
			break;
			case ';':
				tok.type = T_SEPARATOR;
			break;
			default:
				tok.type = T_ILLEGAL;
				return tok;
			break;
		}
		(*p)++;
		
		return tok;
	}

	int check_keywords(char *str){
		size_t i;
		for(i=0; i < NUMBER_OF_KEYWORDS; i++){
			if(strcmp(keywords[i], str)==0){
				return 1;
			}
		}
		return 0;
	}
	
int lexer(lexer_in_t *input, lexer_out_t *output){

		char *p = ->content;
		hgs_tok_t tok;
		bool ctrl=true;

		tok_create(out);

		while(ctrl){
			switch(*p){

				case '\0': case EOF:
					ctrl=false;
					if(tok.type==T_BR_CLOSE){
						tok.type = T_SEPARATOR;
						tok.content = NULL;
					}
				break;

				case '#':
					oneline_comment(&p);
					continue;
				break;
				
				case ...:
					multiline_comment(&p);
					continue;
				break;
				
				case '\"':
					tok = get_string_literal(&p);
				break;
				
				case ' ': case '\t': case '\v': case '\f':
					/* nothing */
					p++;
					continue;
				break;
				
				case 'a' ... 'z':
				case 'A' ... 'Z':
				case '_': case '$':
					tok = get_identifier(&p);
					
					if(check_keywords(tok.content)){
						tok.type = T_KEYWORD;
					}
					if(check_packages(tok.content)){
						tok.type = T_IDENTIFIER;
					}
				break;
				
				case '0' ... '9':
					tok = get_numeric_literal(&p);
				break;

				case '\n':
					p++;
					if(tok.type==T_BR_CLOSE){
						tok.type = T_SEPARATOR;
						tok.content = NULL;
					} else{
						continue;
					}
				break;

				default:
					tok = get_meta(&p);
					if(tok.type==T_ILLEGAL){
						error("unexpected expression `%c` while tokenizing", *p);
						input_destroy(input);
						tok_destroy(out);
						return 1;
					}
				break;
			}
			tok_push(out, tok);
		}

		tok.type = T_ENDOFAPP;
		tok.content = NULL;
		tok_push(out, tok);

		input_destroy(input);
		
		return 0;
	}