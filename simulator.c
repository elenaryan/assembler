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
#define NUMREGS 8
#define numMemory 65536
int main(int argc, char **argv)
{

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

        //primary execution goes here
        int i = 0;
        fscanf(f, "%d", &i);

        while (!feof(f)) {
            //printf("%d\n", i);

        
            int op = i>>22;
            //Might have to do some additional error checking to avoid .fill stuff
            switch(op) {
                case 0:
                    printf("command is add\n");
                    break;
                case 1:
                    printf("nand\n");
                    break;
                case 2:
                    printf("lw\n");
                    break;
                case 3:
                    printf("sw\n");
                    break;
                case 4:
                    printf("beq\n");
                    break;
                case 5:
                    printf("jalr\n");
                    break;
                case 6:
                    printf("halt\n");
                    break;
                case 7:
                    printf("noop\n");
                    break;
            }//switch


            fscanf(f, "%d", &i);
        }
        fclose(f);
    } else {
        printf("Too many inputs given");


    }//end file reading
    return 0;
}//main

