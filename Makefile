CC=gcc
CFLAGS=-pipe
INC=

all: assembler simulator
	$(CC) $(CFLAGS) assembler.o -o assembler
	$(CC) $(CFLAGS) simulator.o -o simulator

assembler: assembler.c
	$(CC) $(CFLAGS) -c assembler.c

simulator: simulator.c
	$(CC) $(CFLAGS) -c simulator.c

clean:
	rm  assembler.o assembler simulator.o simulator
