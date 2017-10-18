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





 * Given functions:


 * offset field number convertor 
 *
 * int convertNum(int num) {
        if(num &(1<<15)) {
            num-=(1<<16);
        }
    return num;
    }



 * state struct
 *
 * typedef struct stateStruct {
       int pc;
       int mem[NUMMEMORY];
       int reg[NUMREGS];
       int numMemory;
    } stateType;


 * print state
 *
 * void printState(stateType *statePTr) {
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
*/



