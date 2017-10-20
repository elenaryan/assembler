#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strmap.h"
#include "strmap.c"

/* 
 * Elena Ryan and Jenna Olson
 * Computer Architecture
 * Simulator
 * Project Part II | 10/20/2017
 * 
 * Takes as input assembled code
 * In the form of decimal numbers
 * Parses out opcodes and executes instructions
 *
 */
#define NUMREGS 8
#define NUMMEMORY 65536


typedef struct stateStruct {
       int pc;
       
       int mem[NUMMEMORY];
       int reg[NUMREGS];
       int numMemory;
    } stateType;

void printState(stateType *statePTr);


int convertNum(int num);



int main(int argc, char **argv)
{

    stateType stat;
    int i = 0;
    //printf("MADE IT AFTER ALL\n");
    while(i<NUMREGS){
        stat.reg[i] = 0;
        i++;
    }// init regs to 0;
    //printf("after regs");
    int j = 0;//mem counter
    if(argc == 1) {
        printf("No input given.\n");
	    return 0;
    } else if (argc == 2 ) {
        FILE *f;
        f = fopen(argv[1], "r");

        if (f == NULL)   {
            printf("Invalid file or path.\n");
            exit(0);
        }
        i = 0;
        fscanf(f, "%d", &i);
        while (!feof(f)) {
            stat.mem[j] = i;
            printf("At memory address %d is: %d\n", j, i);
            fscanf(f, "%d", &i);
            j++;            
        }
        fclose(f);
    } else {
        printf("Too many inputs given");
    }//end file reading and memory initialization

    stat.numMemory = j;//actual necessary mem size
    //probably toss an error if this exceeds machine memory



    stat.pc = 0;//initialize program counter

    //while pc (maybe plus 1) is less than file size

    return 0;
}//main




void printState(stateType *statePtr) {
        int i;
        printf("\n@@@\nstate:\n");
        printf("\tpc %d\n", statePtr->pc);
        printf("tmemory:\n");
        for(i=0; i<statePtr->numMemory; i++) {
            printf("\t\tmem[%d]=%d\n", i, statePtr->mem[i]);
        }
        printf("\tregisters:\n");
        for(i=0; i<NUMREGS;i++) {
            printf("\t\treg[%d]=%d\n", i, statePtr->reg[i]);
        }
        printf("end state\n");
}


int convertNum(int num) {
        if(num &(1<<15)) {
            num-=(1<<16);
        }
    return num;
    }


