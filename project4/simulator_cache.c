#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * Elena Ryan and Jenna Olson
 * Computer Architecture
 * Simulator
 * Project IV | 12/6/2017
 *
 * Takes as input a file of assembled machine code
 * In the form of decimal numbers
 * Parses out opcodes and executes instructions
 *
 * The main method takes the file input and loads
 * to memory.  As it steps through, the program identifies each
 * opcode and executes the instruction by parsing out registers
 * and immediates and updating the registers and memory accordingly


 * updated 11/28 to take specified inputs (e.g. machine file, block size, num sets, associativity
 */

#define NUMREGS 8
#define NUMMEMORY 65536

enum actionType {cacheToProcessor, processorToCache, memoryToCache, cacheToMemory, cacheToNowhere};

typedef struct stateStruct {
       int pc;
       int mem[NUMMEMORY];
       int reg[NUMREGS];
       int numMemory;
    } stateType;

void printState(stateType *statePTr);

void printAction(int address, int size, enum actionType type);

int convertNum(int num);

void add(int inst, stateType *statePtr);

void nand(int inst, stateType *statePtr);

void lw(int inst, stateType *statePtr);

void sw(int inst, stateType *statePtr);

void beq(int inst, stateType *statePtr);

void jalr(int inst, stateType *statePtr);

int cacheSim(int *cache, int n_sets, int assoc, int w_block, int act);

void print_stats(int n_instrs);

int main(int argc, char **argv)
{

    stateType stat;
    int i = 0;
    int j = 0;
    while(i<NUMREGS){
        stat.reg[i] = 0;
        i++;
    }

        char file[255];
        printf("Enter the machine code program to simulate: ");
        scanf("%s", file);
        FILE *f;
        f = fopen(file, "r");

        if (f == NULL)   {
            printf("Invalid file or path.\n");
            exit(0);
        }
        i = 0;
        fscanf(f, "%d", &i);
        while (!feof(f) && j< NUMMEMORY) {
            stat.mem[j] = i;
            fscanf(f, "%d", &i);
            j++;
        }
        fclose(f);

        printf("Please enter cache settings\n Note that total # blocks must be <= 256, and that all entries must be powers of 2\n");

        int b_size;  //block size
        int num_s;   //number of sets in cache
        int c_assoc; //associativity of the cache
        printf("Enter the block size of the cache in words: ");
        scanf("%d", &b_size);
        printf("Enter the number of sets in the cache: ");
        scanf("%d", &num_s);
        printf("Enter the associativity of the cache: ");
        scanf("%d", &c_assoc);

        int cache[num_s][c_assoc][b_size+3];//three d array, set x associativity x blocksize+ room for valid/dirty/tag


        stat.numMemory = j;//actual necessary mem size
        stat.pc = 0;//initialize program counter
        int c = 0;//inst counter
    while(stat.pc < stat.numMemory) {

       if(stat.mem[stat.pc] > 32767) {
          //probably need a better way to figure this out, and also store the value in the function cass



          
          int curri = stat.mem[stat.pc];
/* ----- Implementing Cache Instruction Fetch ------ */

          int addr = stat.pc;
          int block= stat.pc/b_size;
          int set  = block % num_s;
          







          int op = curri>>22;
          int regA = (curri >> 19) & 7;
          int regB = (curri >> 16) & 7;
          int immed = convertNum(curri & 0xFFFF);
          c++;
          if(op == 0) {
             add(curri, &stat);
             stat.pc++;
          } else if(op == 1) {
             nand(curri, &stat);
             stat.pc++;//nand
          } else if(op == 2) {
             stat.reg[regA] = stat.mem[stat.reg[regB] + immed];
             //lw(curri, &stat);
             stat.pc++;//lw
          } else if(op == 3) {
             stat.mem[stat.reg[regB] + immed] = stat.reg[regA];
             //sw(curri, &stat);
             stat.pc++;//sw
          } else if(op == 4) {
             beq(curri, &stat);
          } else if(op == 5) {

             jalr(curri, &stat);    
          } else if(op == 6) {
             printf("Halt\n");
             stat.pc++;
          } else if(op == 7) {
             stat.pc++;
          }
        } else {
            stat.pc++;
        }//takes .fill into account
        printState(&stat);
    }//end while
    print_stats(c);
    return 0;
}//main


void add(int inst, stateType *statePtr) {
    int dest = (inst & 7);
    int regA = (inst >> 19) & 7;
    int regB = (inst >> 16) & 7;
    statePtr->reg[dest] = statePtr->reg[regA] +statePtr->reg[regB];
}//add

void nand(int inst, stateType *statePtr) {
    int dest = (inst & 7);
    int regA = (inst >> 19) & 7;
    int regB = (inst >> 16) & 7;
    statePtr->reg[dest] = ~(statePtr->reg[regA] & statePtr->reg[regB]);
}//nand




void beq(int inst, stateType *statePtr) {
    int regA = (inst >> 19) & 7;
    int regB = (inst >> 16) & 7;
    int immed = convertNum(inst & 0xFFFF);
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
}//printState
int cacheSim(int *cache, int n_sets, int assoc, int w_block, int act) {
//this should be the only place that memory is ever reffed













}//send in the cache, number of sets, associativity, block size, and action to take

/*
* Log the specifics of each cache action.
*
* address is the starting word address of the range of data being transferred.
* size is the size of the range of data being transferred.
* type specifies the source and destination of the data being transferred.
*
* cacheToProcessor: reading data from the cache to the processor
* processorToCache: writing data from the processor to the cache
* memoryToCache: reading data from the memory to the cache
* cacheToMemory: evicting cache data by writing it to the memory
* cacheToNowhere: evicting cache data by throwing it away
*/

void printAction(int address, int size, enum actionType type)
{
	printf("transferring word [%i-%i] ", address, address + size - 1);
	if (type == cacheToProcessor) {
		printf("from the cache to the processor\n");
	} else if (type == processorToCache) {
		printf("from the processor to the cache\n");
	} else if (type == memoryToCache) {
		printf("from the memory to the cache\n");
	} else if (type == cacheToMemory) {
		printf("from the cache to the memory\n");
	} else if (type == cacheToNowhere) {
		printf("from the cache to nowhere\n");
	}
} // printAction
int convertNum(int num) {
        if(num &(1<<15)) {
            num-=(1<<16);
        }
    return num;
}//convertNum

void print_stats(int n_instrs) {
    printf("INSTRUCTIONS: %d\n", n_instrs);
}//print_stats

