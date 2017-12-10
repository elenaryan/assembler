#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Elena Ryan and Jenna Olson
 * Computer Architecture
 * CPU Cache Simulator
 * Project IV | 12/6/2017
 *
 * Takes as input a file of assembled machine code in the form of decimal numbers
 * Optional input: Block Size, Number of Sets, and Associativity
 * If not given, user will be prompted for these values
 * The instructions and data are accessed, these accesses will be serviced by the cache
 * Cache will transfer data to/from memory as needed
 *
 * See Overview Document for an in-depth summary.
 *
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

int powerOfTwo(int num);


int main(int argc, char **argv)
{

    stateType stat;

    // giving these the scope of the whole main function (for the user input conditionals)
    int b_size; //block size
    int num_s; //number of sets in cache
    int c_assoc; //associativity of the cache
    char file[255];
    FILE *f;

    int i = 0;
    int j = 0;

    while(i<NUMREGS){
        stat.reg[i] = 0;
        i++;
    }

    if(argc <= 1) {

        printf("Enter the machine code program to simulate: ");
        scanf("%s", file);
	f = fopen(file, "r");

        printf("Please enter cache settings\n Note that total # blocks must be <= 256, and that all entries must be powers of 2\n");

        printf("Enter the block size of the cache in words: ");
        scanf("%d", &b_size);

	printf("Enter the number of sets in the cache: ");
        scanf("%d", &num_s);

        printf("Enter the associativity of the cache: ");
        scanf("%d", &c_assoc);
    } // if no arguments are entered (all or none are required)

    else if (argc == 5){

	// grabbing the file
	f = fopen(argv[1], "r");

        // grabbing b_size
	b_size = atoi(argv[2]);

	// grabbing num_s
	num_s = atoi(argv[3]);

	// grabbing c_assoc
        c_assoc = atoi(argv[4]);

    } // if correct arguments are entered

    else {
        printf("Invalid amount of arguments given, please give all or none");
	return 0;
    } // if an invalid number of arguments are entered


    // Error checking for arguments
    if(powerOfTwo(b_size) == 0 || b_size > 256 || powerOfTwo(num_s) == 0 || powerOfTwo(c_assoc) == 0) {
	printf("Block size, number of sets, and associativity must be powers of 2\n");
	return 0;
    }

    // Reading the file
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

    // Setting up the cache, lru, nunMemory, pc, and inst counter
    int cache[num_s][c_assoc][b_size+3]; //three d array, set x associativity x blocksize+ room for valid/dirty/tag
    int lru[num_s][c_assoc]; //2d array keeps track of last pc each block was accessed
    stat.numMemory = j; //actual necessary mem size
    stat.pc = 0; //initialize program counter
    int c = 0; //inst counter

    while(stat.pc < stat.numMemory) {

       if(stat.mem[stat.pc] > 32767) {

          int curri = stat.mem[stat.pc];
/* ----- Implementing Cache Instruction Fetch ------ */
          int addr = stat.pc;
          int block= addr/b_size; //tag of block in which the address is
          int set  = block % num_s;
          int val;
          int incache = 0;
          int pullmem = 1;

          for(int i = 0; i<c_assoc; i++) {
            if(cache[set][i][2] == block) {
                curri = cache[set][i][3+(addr % b_size)];
                lru[set][i] = stat.pc;//update lru value for block
                incache = 1;
                printAction(addr, 1, cacheToProcessor);
                break;
            }
          }//for

          if (incache == 0) {
                for(int i = 0; i<c_assoc; i++) {
                    if(cache[set][i][0] != 1) {
                        cache[set][i][0] = 1;//sets valid bit
                        cache[set][i][1] = 0; //dirty bit
                        cache[set][i][2] = block;//sets tag
                        lru[set][i] = stat.pc;
                        for(int j = 0; j<b_size; j++) {
                            cache[set][i][j+3] = stat.mem[(block*b_size)+j];//pulls block from memory
                        }
                        printAction(block*b_size, b_size, memoryToCache);
                        printAction(addr, 1, cacheToProcessor);
                        curri = cache[set][i][3+(addr % b_size)]; //set current inst
                        pullmem = 0;
                        break;
                    }//if
                }
           }
           if (incache == 0 && pullmem == 1) {
                int min = lru[set][0];//last recently accessed
                int blk = 0; //last recently used block
                for (int i = 0; i<c_assoc; i++) {
                    if (lru[set][i] < min) {
                            min = lru[set][i];
                            blk = i;
                    }
                }

                int tag = cache[set][blk][2];
                if(cache[set][blk][1] == 1) {
                    for (int i = 0; i<b_size; i++) {
                       stat.mem[(tag*b_size) +i] = cache[set][blk][i+3];
                    }
                    printAction(tag*b_size, b_size, cacheToMemory);
                } else {
                    printAction(tag*b_size, b_size, cacheToNowhere);
                    //if the block isn't dirty, throw it out
                }

                cache[set][blk][0] = 1;
                cache[set][blk][1] = 0; //dirty bit
                cache[set][blk][2] = block;//current block
                lru[set][blk] = stat.pc; //update lru value
                for (int i=0; i<b_size; i++) {
                    cache[set][blk][i+3] = stat.mem[(block*b_size)+i];//block in from mem
                }
                printAction(block*b_size, b_size, memoryToCache);
                printAction(addr, 1, cacheToProcessor);
                curri = cache[set][i][3+(addr % b_size)];//set instruction

           }// if the value is not already in the cache and pullmem has not been set to 0;


          /** ----  INSTRUCTION FETCH FROM CACHE ----**/

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


/** ------ CACHE ACCESS FOR LW --------**/

            addr = regB+immed;
            block= addr/b_size; //tag of block in which the address is
            set  = block % num_s;
            int val;
            incache = 0;
            pullmem = 1;

          for(int i = 0; i<c_assoc; i++) {
            if(cache[set][i][2] == block) {
                val = cache[set][i][3+(addr % b_size)];
                lru[set][i] = stat.pc;
                incache = 1;
                printAction(addr, 1, cacheToProcessor);
                break;
            }
          }//for

          if (incache == 0) {
                for(int i = 0; i<c_assoc; i++) {
                    if(cache[set][i][0] != 1) {
                        cache[set][i][0] = 1;//sets valid bit
                        cache[set][i][1] = 0; //set dirty bit
                        cache[set][i][2] = block;//sets tag
                        lru[set][i] = stat.pc;
                        for(int j = 0; j<b_size; j++) {
                            cache[set][i][j+3] = stat.mem[(block*b_size)+j];//pulls block from memory
                        }
                        val = cache[set][i][3+(addr % b_size)]; //set current inst
                        printAction(block*b_size, b_size, memoryToCache);
                        printAction(addr, 1, cacheToProcessor);
                        pullmem = 0;
                        break;
                    }//if
                }
           }//If Cache set is full with lru and writeback done below

           if (incache == 0 && pullmem == 1) {
                int min = lru[set][0];//last recently accessed
                int blk = 0; //last recently used block
                for (int i = 0; i<c_assoc; i++) {
                    if (lru[set][i] < min) {
                            min = lru[set][i];
                            blk = i;
                    }
                }//locate LRU block

                int tag = cache[set][blk][2];
                if(cache[set][blk][1] == 1) {

                    for (int i = 0; i<b_size; i++) {
                       stat.mem[(tag*b_size) +i] = cache[set][blk][i+3];
                    }
                    printAction(tag*b_size, b_size, cacheToMemory);

                } else {
                    printAction(tag*b_size, b_size, cacheToNowhere);
                }//if block dirty, write back to mem, else, knock it out

                cache[set][blk][0] = 1;
                cache[set][blk][1] = 0; //set dirty bit
                cache[set][blk][2] = block;//current block
                lru[set][blk] = stat.pc; //update lru value
                for (int i=0; i<b_size; i++) {
                    cache[set][blk][i+3] = stat.mem[(block*b_size)+i];//block in from mem
                }
                printAction(block*b_size, b_size, memoryToCache);
                printAction(addr, 1, cacheToProcessor);
                val = cache[set][blk][3+(addr % b_size)];// THROWS SEG FAULT

           }// if the value is not already in the cache and pullmem has not been set to 0

             stat.reg[regA] = val;//pass value from cache to regA

/** ------ END CACHE STUFF -----------**/

             stat.pc++;//lw
          } else if(op == 3) {


/** ---------  STORE WORD CACHE ACCESS-------- **/

            addr = regB+immed;
            block= addr/b_size; //tag of block in which the address is
            set  = block % num_s;
            int swval = stat.reg[regA];
            incache = 0;
            pullmem = 1;

          for(int i = 0; i<c_assoc; i++) {
            if(cache[set][i][2] == block) {
                cache[set][i][3+(addr % b_size)] = swval;
                cache[set][i][1] = 1; //set dirty bit
                lru[set][i] = stat.pc;
                incache = 1;
                printAction(addr, 1, processorToCache);
                break;
            }
          }//for

          if (incache == 0) {
                for(int i = 0; i<c_assoc; i++) {
                    if(cache[set][i][0] != 1) {
                        cache[set][i][0] = 1;//sets valid bit
                        cache[set][i][1] = 0; //initialized dirty bit
                        cache[set][i][2] = block;//sets tag
                        lru[set][i] = stat.pc;
                        for(int j = 0; j<b_size; j++) {
                            cache[set][i][j+3] = stat.mem[(block*b_size)+j];//pulls block from memory
                        }
                        printAction(block*b_size, b_size, memoryToCache);
                        printAction(addr, 1, processorToCache);
                        cache[set][i][3+(addr % b_size)] = swval; //set new value
                        cache[set][i][2] = 1; //dirty bit
                        pullmem = 0;
                        break;
                    }//if
                }
           }

           if (incache == 0 && pullmem == 1) {
                int min = lru[set][0];//last recently accessed
                int blk = 0; //last recently used block
                for (int i = 0; i<c_assoc; i++) {
                    if (lru[set][i] < min) {
                            min = lru[set][i];
                            blk = i;
                    }
                }

                int tag = cache[set][blk][2];
                if(cache[set][blk][1] == 1) {

                    for (int i = 0; i<b_size; i++) {
                       stat.mem[(tag*b_size) +i] = cache[set][blk][i+3];
                    }
                    printAction(tag*b_size, b_size, cacheToMemory);
                } else {
                    printAction(tag*b_size, b_size, cacheToNowhere);
                }//evict LRU to memory or nowhere

                cache[set][blk][0] = 1;
                cache[set][blk][1] = 0; //init dirty bit
                cache[set][blk][2] = block;//current block tag
                lru[set][blk] = stat.pc; //update lru value
                for (int i=0; i<b_size; i++) {
                    cache[set][blk][i+3] = stat.mem[(block*b_size)+i];//block in from mem
                }
                //do the necessary store word ish right here
                printAction(block*b_size, b_size, memoryToCache);
                printAction(addr, 1, cacheToProcessor);
                cache[set][i][3+(addr % b_size)] = swval; //set new value
                cache[set][i][2] = 1; //dirty bit
           }// if the value is not already in the cache and pullmem has not been set to 0;






/** ------- STORE WORD CACHE ACCESS ------ **/

             //stat.mem[stat.reg[regB] + immed] = stat.reg[regA];
             stat.pc++;//sw
          } else if(op == 4) {
             beq(curri, &stat);
          } else if(op == 5) {

             jalr(curri, &stat);
          } else if(op == 6) {
             printf("Halt\n");
             int wbtag;
               for (int i = 0; i<num_s; i++) {
                    for (int j = 0; j<c_assoc; j++) {
                        if(cache[i][j][1] == 1){
                            //if dirty, write back to mem
                            printAction(wbtag*b_size, b_size, cacheToMemory);
                            printf("WB IN HALT\n");
                            wbtag = cache[i][j][2];
                            for(int k = 0; k<b_size; k++) {
                                stat.mem[(wbtag*b_size) +k] = cache[i][j][k+3];
                            }//write back in HALT

                        }//if
                    }//interior for
            }//exterior for



             //LOOP THROUGH ALL DIRTY CACHE BLOCKS
             break;
             stat.pc++;
          } else if(op == 7) {
             stat.pc++;
          }
        } else {
            stat.pc++;
        }//takes .fill into account
        //printState(&stat);
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

int powerOfTwo(int num) {
	while((num % 2) == 0 && num > 1) {
		num = num / 2;
	}
	if (num == 1) return 1;
	else return 0;
}//powerOfTwo
