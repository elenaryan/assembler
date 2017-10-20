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

void add(int inst, stateType *statePtr);

void nand(int inst, stateType *statePtr);

void lw(int inst, stateType *statePtr);

void sw(int inst, stateType *statePtr);

void beq(int inst, stateType *statePtr);

void jalr(int inst, stateType *statePtr);

int main(int argc, char **argv)
{

    stateType stat;
    int i = 0;

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
            //printf("At memory address %d is: %d\n", j, i);
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

    while(stat.pc < stat.numMemory) {
        
       //put all of the appropriate commands here
        /*HERE's WHAT NEEDS TO HAPPEN:
            1. evaluate each opcode, esp make sure that we can tell the difference
                bt add and .fills
            2. update all necessary state variables
            3. print state
        */

        //implement switch, but also discern bt opcode and fill int somehow...
        //or with 32 bits ones to ensure its more than an immediate value or just >32767

        if(stat.mem[stat.pc] > 32767) {
            //just makes sure we're using real instructions not .fills
            int op = stat.mem[stat.pc]>>22;
            //printf("opcodes %d\n", op);
            if(op == 0) {
            //add
                add(stat.mem[stat.pc], &stat);
                stat.pc++;
                
            } else if(op == 1) {
                nand(stat.mem[stat.pc], &stat);
                stat.pc++;//nand
            } else if(op == 2) {
                lw(stat.mem[stat.pc], &stat);
                stat.pc++;//lw
            } else if(op == 3) {
                sw(stat.mem[stat.pc], &stat);
                stat.pc++;//sw
            } else if(op == 4) {
                beq(stat.mem[stat.pc], &stat);
                //stat.pc++;//beq
            } else if(op == 5) {
                jalr(stat.mem[stat.pc], &stat);
                
            } else if(op == 6) {
                printf("Halt\n");
                stat.pc++;
            } else if(op == 7) {
                stat.pc++;
            } 
            
                   
        } else {
            stat.pc++;
        }
    }//end while

    return 0;
}//main


void add(int inst, stateType *statePtr) {
    int dest = (inst & 7);
    int regA = (inst >> 19) & 7;
    int regB = (inst >> 16) & 7;
    statePtr->reg[dest] = statePtr->reg[regA] +statePtr->reg[regB];
        
    //printf("in statePtr->mem[dest] is %d (should be 0)\n", statePtr->reg[dest]);

}//add

void nand(int inst, stateType *statePtr) {
    int dest = (inst & 7);
    int regA = (inst >> 19) & 7;
    int regB = (inst >> 16) & 7;
    statePtr->reg[dest] = ~(statePtr->reg[regA] & statePtr->reg[regB]);
        
    //printf("in statePtr->mem[dest] is %d (should be 0)\n", statePtr->reg[dest]);

}//nand

void lw(int inst, stateType *statePtr) {
    int regA = (inst >> 19) & 7;
    int regB = (inst >> 16) & 7;
    int immed = convertNum(inst & 0xFFFF);
    statePtr->reg[regA] = statePtr->mem[statePtr->reg[regB] + immed];

}//lw

void sw(int inst, stateType *statePtr) {
    int regA = (inst >> 19) & 7;
    int regB = (inst >> 16) & 7;
    int immed = convertNum(inst & 0xFFFF);
    statePtr->mem[statePtr->reg[regB] + immed] = statePtr->reg[regA];

}//sw


void beq(int inst, stateType *statePtr) {
    int regA = (inst >> 19) & 7;
    int regB = (inst >> 16) & 7;
    int immed = convertNum(inst & 0xFFFF);
    //printf("inst is %d and regA is %d and regB is %d and immed is%d\n", inst, regA, regB, immed);
    if(statePtr->reg[regA] == statePtr->reg[regB]) {
        statePtr->pc = statePtr->pc + 1 + immed;
    } else {
        statePtr->pc = statePtr->pc +1;
    }
}//beq


void jalr(int inst, stateType *statePtr) {
    int regA = (inst >> 19) & 7;
    int regB = (inst >> 16) & 7;
    statePtr->reg[regA] = statePtr->pc +1;
    statePtr->pc = statePtr->reg[regB];
}//jalr







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


