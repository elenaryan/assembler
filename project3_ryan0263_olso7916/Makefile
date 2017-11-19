CC=gcc
CFLAGS=-pipe
INC=

all: pipelinesimulator
	$(CC) $(CFLAGS) pipeline_simulator.o -o pipeline_simulator

pipelinesimulator: pipeline_simulator.c
	$(CC) $(CFLAGS) -c pipeline_simulator.c

clean:
	rm pipeline_simulator.o pipeline_simulator
