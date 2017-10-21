CC=gcc
CFLAGS=-pipe
INC=

all: assembler
	$(CC) $(CFLAGS) assembler.o -o assembler

assembler: assembler.c
	$(CC) $(CFLAGS) -c assembler.c

clean:
	rm  assembler.o assembler
