calc: tinyexpr.o main.o
	gcc -O3 -Wall -c tinyexpr.c -lm -pthread
	gcc -O3 -Wall -c main.c -lm
	gcc -O3 -Wall -o calc main.o tinyexpr.o -lm -pthread

install: calc
	install -m 0755 calc /usr/local/bin
