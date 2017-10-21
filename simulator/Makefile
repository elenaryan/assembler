CC=gcc
CFLAGS=-pipe
INC=

all: simulator
	$(CC) $(CFLAGS) simulator.o -o simulator

simulator: simulator.c
	$(CC) $(CFLAGS) -c simulator.c

clean:
	rm simulator.o simulator
