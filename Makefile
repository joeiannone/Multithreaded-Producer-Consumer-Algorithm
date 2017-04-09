CC=gcc
CFLAGS=-pthread -Wall

all: pc input

pc: prodcons.c
	$(CC) $(CFLAGS) prodcons.c -o pc -lm

input: prod_input.c
	$(CC) prod_input.c -o input
clean:
	rm -f *.o pc input

.PHONY: clean
