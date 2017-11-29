CC=gcc
CFLAGS=-pipe
INC=

all: simulator
	$(CC) $(CFLAGS) simulator_cache.o -o simulator

simulator: simulator_cache.c
	$(CC) $(CFLAGS) -c simulator_cache.c

clean:
	rm simulator_cache.o simulator
