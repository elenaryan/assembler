
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Elena Ryan and Jenna Olson
 * Computer Architecture
 * Pipelined Simulator
 * Project 3 | 11/20/2017
 *
 * Takes as input a file of assembled machine code
 * In the form of decimal numbers
 * Parses out opcodes and executes instructions
 *
 * The main loop executes one clock cycle. The body of the loop 
 * determines the new state of the machine (memory, registers,
 * pipeline registers) at the end of the cycle. This simulator is
 * pipelined, meaning that instructions will be carried out in different
 * stages of the pipeline, and multiple instructions will be overlapped.
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
                //printf("state.instrMem[j] = %d\n", i);
                state.dataMem[j] = i;
                fscanf(f, "%d", &i);
                j++;
            }
            fclose(f);
        } else {
            printf("Too many inputs given");

        }//end file reading and memory initialization


        /*** THIS IS WHERE LOOP 2 STARTS ***/

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
                printf("total of %d instructions retired\n", state.retired);
                printf("total of %d branches executed\n", state.branches);
                printf("total of %d branch mispredictions\n", state.mispreds);
                exit(0);
 
            }
            state.cycles++;
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
        

            state = newState; 
        }//end while 
        
       
    print_stats(c);
    return 0;

}//main


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
    newState->pc = state->pc+1;
    newState->IFID.pcPlus1 = state->IFID.pcPlus1+1;
    newState->fetched = state->fetched+1;
    printf("AT cycle: %d instruction is %d\n", newState->fetched, newState->IFID.instr);
    if(opcode(state->IFID.instr) == LW) {
        if(field0(state->instrMem[state->pc]) == field0(state->IFID.instr) || field1(state->instrMem[state->pc]) == field0(state->IFID.instr)) {
            newState->IFID.instr = NOOPINSTRUCTION;
            newState->pc = state->pc;
            newState->IFID.pcPlus1 = state->IFID.pcPlus1;
            newState->fetched = state->fetched;
            //printf(" LOAD STALL NOOP SENT THROUGH CYCLE\n");//can remove this later
        }//interior if
    } //LOAD STALL
}//IFID
void IDEX(stateType *state, stateType *newState) {
    newState->IDEX.instr = state->IFID.instr;
    newState->IDEX.readRegA = state->reg[field0(state->IFID.instr)];//actually you have to find what is here
    newState->IDEX.readRegB = state->reg[field1(state->IFID.instr)];//what is in this register
    //printf("****\n*****\n****\nIT IS: %d", state->reg[field2(state->IFID.instr)]);
    if (opcode(state->IFID.instr) == BEQ) {    
        newState->IDEX.offset = signExtend(field2(state->IFID.instr));//should be offset
        //printf("FOR INST %d offset is %d\n", state->IFID.instr, signExtend(field2(state->IFID.instr)));
        //printf("PCPLUS1 is %d\n ", state->IFID.pcPlus1);
    } else if (opcode(state->IFID.instr) == LW){
        newState->IDEX.offset = signExtend(field2(state->IFID.instr));//should be offset
        //printf("****\n*****\n****\nIT IS: %d", state->reg[field2(state->IFID.instr)]);
    }//lw 
    newState->IDEX.pcPlus1 = state->IFID.pcPlus1;
   
}//IDEX
void EXMEM(stateType *state, stateType *newState) {
    int t = 1;
    newState->EXMEM.instr = state->IDEX.instr;
    newState->EXMEM.branchTarget = state->IDEX.offset+state->IDEX.pcPlus1;
    //newState->EXMEM.readReg = state->IDEX.readRegA;//IF REG A IS LOADED, THIS CHANGES TOO
    int regA = field0(state->IDEX.instr);//helpful for nand and add data forwarding
    int regB = field1(state->IDEX.instr);//helpful for nand and add data forwarding
    int regAcont = state->IDEX.readRegA;
    int regBcont = state->IDEX.readRegB;
    int takenA = 0;
    int takenB = 0;
    
    if (opcode(state->EXMEM.instr) == ADD || opcode(state->EXMEM.instr) == NAND && !(t==takenA)) {
        if(regA == (state->EXMEM.instr & 7)) {
            //printf("inside A data forward\n");
            regAcont = state->EXMEM.aluResult;
            takenA = 1;
        }
    }
    if(opcode(state->MEMWB.instr) == ADD || opcode(state->MEMWB.instr) == NAND && !(t==takenA)) {
        if (regA == (state->MEMWB.instr & 7)) {
            //printf("inside A data forward\n");
            regAcont = state->MEMWB.writeData;//might have to check that this corresponds
            takenA = 1;
        }
    }
    if(opcode(state->MEMWB.instr) == LW && !(t==takenA)) {
        if (regA == field0(state->MEMWB.instr)) {//i think you load it with regB*???
            printf("inside A data forward for that one LW MEMWB\n");
            regAcont = state->MEMWB.writeData;
            takenA = 1;
        }
    }
    if(opcode(state->WBEND.instr) == ADD || opcode(state->WBEND.instr) == NAND && !(t==takenA)) {
        if (regA == (state->WBEND.instr & 7)) {
            //printf("inside A data forward\n");
            regAcont = state->WBEND.writeData;//might have to check that this corresponds
            takenA = 1;
        }
    }
    if(opcode(state->WBEND.instr) == LW && !(t==takenA)) {
        if (regA == field0(state->WBEND.instr)) {
            //printf("inside A data forward\n");
            regAcont = state->WBEND.writeData;
            takenA = 1;
        }
        
    }//make sure regA is not a dest register

    if (opcode(state->EXMEM.instr) == ADD || opcode(state->EXMEM.instr) == NAND && !(t==takenB)) {
        if(regB == (state->EXMEM.instr & 7)) {
            //printf("inside B data forward\n");
            regBcont = state->EXMEM.aluResult;
            takenB = 1;
        }
    }
    if(opcode(state->MEMWB.instr) == ADD || opcode(state->MEMWB.instr) == NAND && !(t==takenB)) {
        if (regB == (state->MEMWB.instr & 7)) {
            //printf("inside B data forward\n");
            regBcont = state->MEMWB.writeData;//might have to check that this corresponds
            takenB = 1;
        }
    }
    if(opcode(state->MEMWB.instr) == LW && !(t==takenB)) {
        if (regB == field0(state->MEMWB.instr)) {
            //printf("inside B data forward\n");
            regBcont = state->MEMWB.writeData;
            takenB = 1;
        }
    }
    if(opcode(state->WBEND.instr) == ADD || opcode(state->WBEND.instr) == NAND && !(t==takenB)) {
        if (regB == (state->WBEND.instr & 7)) {
            //printf("inside B data forward\n");
            regBcont = state->WBEND.writeData;//might have to check that this corresponds
            takenB = 1;
        }
    }
   if(opcode(state->WBEND.instr) == LW && !(t==takenB)) {
        if (regB == field0(state->WBEND.instr)) {
            printf("inside CORRECT data forward\n");
            regBcont = state->WBEND.writeData;
            takenB = 1;
        }
    } 


    //}//make sure regB is not a dest register

    
    newState->EXMEM.readReg = regAcont;//make sure sw is set up for success


    //actual ALU ops
    if (opcode(state->IDEX.instr) == ADD) {
        newState->EXMEM.aluResult = regAcont + regBcont;
    } else if (opcode(state->IDEX.instr) == NAND) {
        newState->EXMEM.aluResult = !(regAcont & regBcont);
    } else if (opcode(state->IDEX.instr) == SW || opcode(state->IDEX.instr)== LW) {
        newState->EXMEM.aluResult = regBcont + state->IDEX.offset;//I think something here might be wrong
        //printf("THINGS ARE NOT WELL %d %d\n", regBcont, state->IDEX.offset);
    } else if(opcode(state->IDEX.instr) == BEQ) {
        if(regAcont == regBcont) {
            newState->EXMEM.aluResult = 1;
        } else {
            newState->EXMEM.aluResult = 0;
        }
    }//lw/sw/beq


}//EXMEM

void MEMWB(stateType *state, stateType *newState) {
    newState->MEMWB.instr = state->EXMEM.instr;
    if(opcode(state->EXMEM.instr) == LW) {
        newState->MEMWB.writeData = state->dataMem[state->EXMEM.aluResult];
    } else if(opcode(state->EXMEM.instr) == SW) {
        newState->dataMem[state->EXMEM.aluResult] = state->EXMEM.readReg;
    } else if(opcode(state->EXMEM.instr) == ADD || opcode(state->EXMEM.instr) == NAND) {
        newState->MEMWB.writeData = state->EXMEM.aluResult;
    } else if(opcode(state->EXMEM.instr) == BEQ && state->EXMEM.aluResult == 1) {
            newState->pc = state->EXMEM.branchTarget;//BEQ
            //newState->            
            printf("newState->pc is %d\n and instrMem[that num] is %d\n", newState->pc, state->instrMem[newState->pc]);
            newState->EXMEM.instr = NOOPINSTRUCTION;
            newState->IDEX.instr = NOOPINSTRUCTION;
            newState->IFID.instr = NOOPINSTRUCTION;
            newState->branches++;
            newState->mispreds++;
            newState->fetched = state->fetched-3;//let's not count the accidents        
    }

}//MEMWB

void WBEND(stateType *state, stateType *newState) {
    newState->WBEND.instr = state->MEMWB.instr;
    if (opcode(state->MEMWB.instr) == ADD || opcode(state->MEMWB.instr) == NAND) {
        newState->reg[state->MEMWB.instr & 7] = state->MEMWB.writeData;
    } else if (opcode(state->MEMWB.instr) == LW) {
        newState->reg[field0(state->MEMWB.instr)] = state->MEMWB.writeData;
    }//write back loads
    if(state->MEMWB.instr != 0) {
        newState->retired++;
    }//inst retired
    newState->WBEND.writeData = state->MEMWB.writeData;

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

