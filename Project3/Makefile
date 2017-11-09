CC=gcc
CFLAGS=-pipe
INC=

all: pipeline_simulator
	$(CC) $(CFLAGS) pipeline_simulator.o -o pipeline_simulator

simulator: pipeline_simulator.c
	$(CC) $(CFLAGS) -c pipeline_simulator.c

clean:
	rm pipeline_simulator.o pipeline_simulator
