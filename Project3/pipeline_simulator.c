
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ******************* WORKING OFF OF THE ORIGINAL SIMULATOR ****************//

/*
 * Elena Ryan and Jenna Olson
 * Computer Architecture
 * Pipelined simulator
 * Project 3 | 11/20/2017
 *
 * // May need to change this description
 *
 * Takes as input a file of assembled machine code
 * In the form of decimal numbers
 * Parses out opcodes and executes instructions
 *
 * The main method takes the file input and loads
 * to memory.  As it steps through, the program identifies each
 * opcode and executes the instruction by parsing out registers
 * and immediates and updating the registers and memory accordingly
 */


#define NUMMEMORY 65536 /* maximum number of data words in memory */
#define NUMREGS 8 /* number of machine registers */
#define ADD 0
#define NAND 1
#define LW 2
#define SW 3
#define BEQ 4
#define JALR 5 /*not implemented*/
#define HALT 6
#define NOOP 7

#define NOOPINSTRUCTION 0x1c00000

typedef struct IFIDstruct{
	int instr;
	int pcPlus1;
} IFIDType;

typedef struct IDEXstruct{
	int instr;
	int pcPlus1;
	int readRegA;
	int readRegB;
	int offset;
} IDEXType;

typedef struct EXMEMstruct{
	int instr;
	int branchTarget;
	int aluResult;
	int readReg;
} EXMEMType;

typedef struct MEMWBstruct{
	int instr;
	int writeData;
} MEMWBType;

typedef struct WBENDstruct{
	int instr;
	int writeData;
} WBENDType;

typedef struct statestruct{
	int pc;
	int instrMem[NUMMEMORY];
	int dataMem[NUMMEMORY];
	int reg[NUMREGS];
	int numMemory;
	IFIDType IFID;
	IDEXType IDEX;
	EXMEMType EXMEM;
	MEMWBType MEMWB;
	WBENDType WBEND;
	int cycles; /* Number of cycles run so far */
	int fetched; /* Total number of instructions fetched */
	int retired; /* Total number of completed instructions */
	int branches; /* Total number of branches executed */
	int mispreds; /* Number of branch mispredictions*/
} stateType;

// Will probably need to take this struct out, replace with the one directly above from the instruction packet
// Myre uses dataMem instead of mem, we'll need to choose one to use consistently
/*typedef struct stateStruct {
       int pc;
       int mem[NUMMEMORY];
       int reg[NUMREGS];
       int numMemory;
    } stateType;
*/
void printState(stateType *statePTr);

int convertNum(int num);

void add(int inst, stateType *statePtr);

void nand(int inst, stateType *statePtr);

void lw(int inst, stateType *statePtr);

void sw(int inst, stateType *statePtr);

void beq(int inst, stateType *statePtr);

int opcode(int instruction);

void print_stats(int n_instrs);

int signExtend(int num);

int field0(int instruction);

int field1(int instruction);

int field2(int instruction);

void printInstruction(int instr);
void IFID(stateType *state, stateType *newState);
void IDEX(stateType *state, stateType *newState);
void EXMEM(stateType *state, stateType *newState);
void MEMWB(stateType *state, stateType *newState);
void WBEND(stateType *state, stateType *newState);

int main(int argc, char **argv)
{

    stateType state;
    stateType newState;
   /** IFIDType ifid;
    IDEXType ides;
    EXMEMType exmem;
    MEMWBType memwb;
    WBENDType wbend;

**/

    int i = 0;
    int j = 0;
    /***initialize everything to NOOP in here***/
    state.IFID.instr = NOOPINSTRUCTION;
    state.IDEX.instr = NOOPINSTRUCTION;
    state.EXMEM.instr = NOOPINSTRUCTION;
    state.MEMWB.instr = NOOPINSTRUCTION;
    state.WBEND.instr = NOOPINSTRUCTION;


    while(i<NUMREGS){
        state.reg[i] = 0;
        i++;
    }
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
            while (!feof(f) && j< NUMMEMORY) {
                state.instrMem[j] = i;
                printf("state.instrMem[j] = %d\n", i);
                state.dataMem[j] = i;
                fscanf(f, "%d", &i);
                j++;
            }
            fclose(f);
        } else {
            printf("Too many inputs given");

        }//end file reading and memory initialization


        /***THIS IS WHERE LOOP 2 STARTS***/

        state.numMemory = j;//actual necessary mem size
        state.pc = 0;//initialize program counter
        int c = 0;//inst counter

        //i = 0;
        while(1) {
            //i++;
            printState(&state);
            /*checking for halt*/

            if(HALT == opcode(state.MEMWB.instr)) {
                printf("machine halted\n");
                printf("total of %d cycles executed\n", state.cycles);
                printf("total of %d instructions fetched\n", state.fetched);
                printf("total of %d instructions retured\n", state.retired);
                printf("total of %d branches executed\n", state.branches);
                printf("total of %d branch mispredictions\n", state.mispreds);
                exit(0);
 
            }
            newState = state;
/*------------------ IF stage ----------------- */
            IFID(&state, &newState);
            



/*------------------ ID stage ----------------- */
            IDEX(&state, &newState);
            

/*------------------ EX stage ----------------- */
            EXMEM(&state, &newState);



/*------------------ MEM stage ----------------- */
            MEMWB(&state, &newState);


/*------------------ WB stage ----------------- */
            WBEND(&state, &newState);
        
            state = newState; /* this is the last statement before the end of the loop.
It marks the end of the cycle and updates the current
state with the values calculated in this cycle
– AKA “Clock Tick”. */





        }//end while 
        
    /**while(stat.pc < stat.numMemory) {

       if(stat.mem[stat.pc] > 32767) {
          int op = stat.mem[stat.pc]>>22;
          c++;
          if(op == 0) {
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
    }//end while **/
    print_stats(c);
    return 0;
}//main

/*
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
    if(statePtr->reg[regA] == statePtr->reg[regB]) {
        statePtr->pc = statePtr->pc + 1 + immed;
    } else {
        statePtr->pc = statePtr->pc +1;
    }
}//beq

*/
void printState(stateType *statePtr) {
        int i;
        printf("\n@@@\nstate before cycle %d starts\n", statePtr->cycles);
        printf("\tpc %d\n", statePtr->pc);

        printf("tmemory:\n");
        for(i=0; i<statePtr->numMemory; i++) {
            printf("\t\tdataMem[%d]%d\n", i, statePtr->dataMem[i]);
        }

        printf("\tregisters:\n");
        for(i=0; i<NUMREGS;i++) {
            printf("\t\treg[%d]%d\n", i, statePtr->reg[i]);
        }

	printf("\tIFID:\n");
		printf("\t\tinstruction ");
		printInstruction(statePtr->IFID.instr);
		printf("\t\tpcPlus1 %d\n", statePtr->IFID.pcPlus1);

	printf("\tIDEX:\n");
		printf("\t\tinstruction");
		printInstruction(statePtr->IDEX.instr);
		printf("\t\tpcPlus1 %d\n", statePtr->IDEX.pcPlus1);
		printf("\t\treadRegA %d\n", statePtr->IDEX.readRegA);
		printf("\t\treadRegB %d\n", statePtr->IDEX.readRegB);
		printf("\t\toffset %d\n", statePtr->IDEX.offset);
   printf("\tEXMEM:\n");
        printf("\t\tinstruction ");
        printInstruction(statePtr->EXMEM.instr);
        printf("\t\tbranchTarget %d\n", statePtr->EXMEM.branchTarget);
        printf("\t\taluResult %d\n", statePtr->EXMEM.aluResult);
        printf("\t\treadRegB %d\n", statePtr->EXMEM.readReg);
    printf("\tMEMWB:\n");
		printf("\t\tinstruction ");
		printInstruction(statePtr->MEMWB.instr);
		printf("\t\twriteData %d\n", statePtr->MEMWB.writeData);
	printf("\tWBEND:\n");
		printf("\t\tinstruction ");
		printInstruction(statePtr->WBEND.instr);
		printf("\t\twriteData %d\n", statePtr->WBEND.writeData);

        printf("end state\n");
}//printState


void printInstruction(int instr) {
	char opcodeString[10];
	if (opcode(instr) == ADD) {
		strcpy(opcodeString, "add");
	} else if (opcode(instr) == NAND) {
		strcpy(opcodeString, "nand");
	} else if (opcode(instr) == LW) {
		strcpy(opcodeString, "lw");
	} else if (opcode(instr) == SW) {
		strcpy(opcodeString, "sw");
	} else if (opcode(instr) == BEQ) {
		strcpy(opcodeString, "beq");
	} else if (opcode(instr) == JALR) {
		strcpy(opcodeString, "jalr");
	} else if (opcode(instr) == HALT) {
		strcpy(opcodeString, "halt");
	} else if (opcode(instr) == NOOP) {
		strcpy(opcodeString, "noop");
	} else {
		strcpy(opcodeString, "data");
	}

	if(opcode(instr) == ADD || opcode(instr) == NAND){
		printf("%s %d %d %d\n", opcodeString, field2(instr), field0(instr), field1(instr));
	}else if(0 == strcmp(opcodeString, "data")){
		printf("%s %d\n", opcodeString, signExtend(field2(instr)));
	}else{
		printf("%s %d %d %d\n", opcodeString, field0(instr), field1(instr),
			signExtend(field2(instr)));
    }
}//printInstruction


int field0(int instruction){
	return( (instruction>>19) & 0x7);
}

int field1(int instruction){
	return( (instruction>>16) & 0x7);
}

int field2(int instruction){
	return(instruction & 0xFFFF);
}

int opcode(int instruction){
	return(instruction>>22);
}
void IFID(stateType *state, stateType *newState) {
    
    newState->IFID.instr = state->instrMem[state->pc];
    newState->pc = state->IFID.pcPlus1;
    newState->IFID.pcPlus1 = state->IFID.pcPlus1+1;
    newState->fetched = state->fetched+1;
    if(state->IFID.instr == LW && ((field0(state->instrMem[state->pc]) == field1(state->IFID.instr)) || (field1(state->instrMem[state->pc]) == field1(state->IFID.instr)))) {
        newState->IFID.instr = NOOPINSTRUCTION;
        newState->pc = state->pc;
        newState->IFID.pcPlus1 = state->IFID.pcPlus1;
        newState->fetched = state->fetched;
        printf(" LOAD STALL NOOP SENT THROUGH CYCLE\n");//can remove this later
    } //LOAD STALL
//printf("state.pc is %d\n", state->pc);
//printf("state->instrMem[state->pc] %d\n", state->instrMem[state->pc]);

}//IFID
void IDEX(stateType *state, stateType *newState) {
    newState->IDEX.instr = state->IFID.instr;
    newState->IDEX.readRegA = state->reg[field0(state->IFID.instr)];//actually you have to find what is here
    newState->IDEX.readRegB = state->reg[field1(state->IFID.instr)];//what is in this register
    newState->IDEX.offset = signExtend(field2(state->IFID.instr));
    newState->IDEX.pcPlus1 = state->IFID.pcPlus1;
   
}//IDEX
void EXMEM(stateType *state, stateType *newState) {
    newState->EXMEM.instr = state->IDEX.instr;
    newState->EXMEM.branchTarget = state->IDEX.offset+state->IDEX.pcPlus1;
    newState->EXMEM.readReg = state->IDEX.readRegA;//this is the contents of reg b

    //ALL ALU DATA FORWARDING GOES HERE
    




    if (opcode(state->IDEX.instr) == ADD) {
        //if(field0(state->IDEX.instr) == 
            newState->EXMEM.aluResult = state->IDEX.readRegA + state->IDEX.readRegB;
    } else if (opcode(state->IDEX.instr) == NAND) {
        newState->EXMEM.aluResult = !(state->IDEX.readRegA & state->IDEX.readRegB);
    } else if (opcode(state->IDEX.instr) == SW || opcode(state->IDEX.instr)== LW) {
        newState->EXMEM.aluResult = state->IDEX.readRegB + state->IDEX.offset;
    } //lw/sw


}//EXMEM

void MEMWB(stateType *state, stateType *newState) {
    newState->MEMWB.instr = state->EXMEM.instr;
    if(opcode(state->EXMEM.instr) == LW) {
        newState->MEMWB.writeData = state->dataMem[state->EXMEM.aluResult];
    } else if(opcode(state->EXMEM.instr) == SW) {
        newState->dataMem[state->EXMEM.aluResult] = state->EXMEM.readReg;
    } else if(opcode(state->EXMEM.instr) == ADD || opcode(state->EXMEM.instr) == NAND) {
        newState->MEMWB.writeData = state->EXMEM.aluResult;
    } else if(opcode(state->EXMEM.instr) == BEQ) {
        if (field0(state->EXMEM.instr) == field1(state->EXMEM.instr)) {
            newState->pc = state->EXMEM.branchTarget;
        }
    }//alu

}//MEMWB

void WBEND(stateType *state, stateType *newState) {
    newState->WBEND.instr = state->MEMWB.instr;
    if (opcode(state->MEMWB.instr) == ADD || opcode(state->MEMWB.instr) == NAND) {
        newState->reg[state->MEMWB.instr & 7] = state->MEMWB.writeData;
    } else if (opcode(state->MEMWB.instr) == LW) {
        newState->reg[field0(state->MEMWB.instr)] = state->MEMWB.writeData;
    }//write back loads
    //WRITE THE DATA to dest reg for and and nand
    // write the data to regA if load
    

}//WBEND



int convertNum(int num) {
        if(num &(1<<15)) {
            num-=(1<<16);
        }
    return num;
}//convertNum


int signExtend(int num){
    // convert a 16-bit number into a 32-bit integer
    if (num & (1<<15) ) {
        num -= (1<<16);
    }
    return num;
}//signExtend




void print_stats(int n_instrs) {
    printf("INSTRUCTIONS: %d\n", n_instrs);
}//print_stats*/

