#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strmap.h"
#include "strmap.c"

/* 
 * Elena Ryan and Jenna Olson
 * Computer Architecture
 * Assembler
 * Project Part I | 10/13/2017
*/



int main(int argc, char **argv)
{

    StrMap *sm;
    char buf[255];
    sm = sm_new(10);
    int result;

    if(argc == 1) {
        printf("No input given.\n");
    } else if (argc == 2) {
        FILE *f;
        f = fopen(argv[1], "r");

        if (f == NULL)   {
            printf("Invalid file or path.\n");
            exit(0);
        }

        char write[2048]; //char array line by line
        int mark = 0;
        int markold = 1;//to denote first line char

        while(fgets(write, 2048, f) != NULL) {
            // Checking if label is invalid
	    char* token = strtok(write," \t\n");
	    char *checker0 = strstr(token, "0");
	    char *checker1 = strstr(token, "1");
            char *checker2 = strstr(token, "2");
	    char *checker3 = strstr(token, "3");
	    char *checker4 = strstr(token, "4");
	    char *checker5 = strstr(token, "5");
	    char *checker6 = strstr(token, "6");
	    char *checker7 = strstr(token, "7");
	    char *checker8 = strstr(token, "8");
	    char *checker9 = strstr(token, "9");
	    const char *special_characters = "`~@#$%^&*()+=<>,.?/\{}[]|";
	    while (token) {
                if(mark != markold) {
                    //check if first token is a label
                    if(strcmp(token, "beq") != 0 && strcmp(token, "add") != 0 && strcmp(token, "nand") && strcmp(token, "lw")!= 0 && strcmp(token, "sw") != 0 && strcmp(token, "jalr")!=0 && strcmp(token, "halt") != 0 && strcmp(token, "noop")!= 0 && strcmp(token, ".fill") != 0) {
                        // Checking for invalid labels -- Beginning with a number
			if(checker0 == token || checker1 == token || checker2 == token || checker3 == token || checker4 == token || checker5 == token || checker6 == token || checker7 == token || checker8 == token || checker9 == token)
			{
				fprintf(stderr, "Label cannot begin with a number.\n");
				return 0;
			}
			// Checking for invalid labels -- Containing special symbols
			if(strpbrk(token, special_characters) != 0)
			{
				fprintf(stderr, "Label cannot contain special symbols.\n");
				return 0;
			}
			// Checking for invalid labels -- Duplicates
			if(sm_exists(sm, token) == 1)
			{
				fprintf(stderr, "Duplicate labels cannot be entered.\n");
				return 0;
			}
			// Checking for invalid labels -- More than 6 characters
			if(strnlen(token, 32) > 6)
			{
				fprintf(stderr, "Label cannot be more than 6 characters.\n");
				return 0;
			}
			// Checks for invalid opcodes -- not sure why this breaks your test2.txt
			/*char* token2 = strtok(NULL, " \t");
			if(strcmp(token2, "beq") != 0 && strcmp(token2, "add") != 0 && strcmp(token2, "nand") != 0 && strcmp(token2, "lw") != 0 && strcmp(token2, "sw") != 0 && strcmp(token2, "jalr") != 0 && strcmp(token2, "halt") != 0 && strcmp(token2, "noop") != 0 && strcmp(token2, ".fill") != 0)
			{
				fprintf(stderr, "Opcode invalid.\n");
				return 0;
			}*/
			printf("label is %s*** at line %d\n", token, mark);
                        char str[128];
                        sprintf(str, "%d", mark);
                        sm_put(sm, token, str);
                     }//inner absurd statement
                    markold = mark; //so mark and markold only diverge for first token in a line
                }
                token = strtok(NULL, " \t\n");
            }
            mark++;
        }//while
        fseek(f, 0, SEEK_SET);

        char parse[2048];//be sure to change these numbers to max handlers of the assembler

        int m = 0;//counts lines
        int inst;
        while(fgets(parse, 2048, f) != NULL) {
            char* tok = strtok(parse, " \t\n");
            printf("in second pass %s\n", tok);
            if(sm_exists(sm, tok)!=0 || tok == NULL){
                tok = strtok(NULL, " \t\n");
            }// if first tok is a label, move on

            //printf("HERE WITH toke %s\n", tok);

            if(strcmp(tok, "add")==0) {
		inst = 0 << 22;
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = atoi(tok) | inst;//just throw an e
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = atoi(tok)<<19 | inst;//just throw an e
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = atoi(tok)<<16 | inst;//just throw an e
                printf("line is %d and inst is %d\n", m, inst);
            }

            if(strcmp(tok, "nand")==0) {
                inst = 1 << 22;
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = atoi(tok) | inst;//just throw an e
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = atoi(tok)<<19 | inst;//just throw an e
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = atoi(tok)<<16 | inst;//just throw an e
                printf("line is %d and inst is %d\n", m, inst);
            }

            if(strcmp(tok, "lw")==0) {
                inst = 2 << 22;
                //printf("stream
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = (atoi(tok) <<19) | inst;//just throw an e
                //printf("int is%d\n",inst);
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = (atoi(tok)<<16) | inst;//just throw an e
                //works exceps for labels
                tok = strtok(NULL, " \t\n");
                inst = atoi(tok) | inst;//just throw an e
                printf("line is %d and inst is %d\n", m, inst);
            }

            if(strcmp(tok, "sw")==0) {
                inst = 3 << 22;
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = (atoi(tok) <<19) | inst;//just throw an e
                //printf("int is%d\n",inst);
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = (atoi(tok)<<16) | inst;//just throw an e
                //works exceps for labels
                tok = strtok(NULL, " \t\n");
                inst = atoi(tok) | inst;//just throw an e
                printf("line is %d and inst is %d\n", m, inst);
            }

            if(strcmp(tok, "beq")==0) {
                inst = 4 << 22;
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = (atoi(tok) <<19) | inst;//just throw an e
                //printf("int is%d\n",inst);
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = (atoi(tok)<<16) | inst;//just throw an e
                //works exceps for labels
                tok = strtok(NULL, " \t\n");
                inst = atoi(tok) | inst;//just throw an e
                //printf("line is %d and inst is %d\n", m, inst);
                printf("line is %d and inst is %d\n", m, inst);
            }

            if(strcmp(tok, "jalr")==0) {
                inst = 5 << 22;
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = atoi(tok)<<19 | inst;//just throw an e
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = atoi(tok)<<16 | inst;//just throw an e
                //printf("line is %d and inst is %d\n", m, inst);
                printf("line is %d and inst is %d\n", m, inst);
            }

            if(strcmp(tok, "halt")==0) {
                inst = 6 << 22;
                printf("line is %d and inst is %d\n", m, inst);
            }

            if(strcmp(tok, "noop")==0) {
                inst = 7 << 22;
                printf("line is %d and inst is %d\n", m, inst);
            }
            if(strcmp(tok, ".fill")==0) {
		tok = strtok(NULL," \t\n");
                inst = atoi(tok);//just throw an e
                printf("line is %d and inst is %d\n", m, inst);
             }


            while(tok) {
                //printf("%s ", tok);
                tok = strtok(NULL," \t\n");
                //tm++;
            }
            m++;

        }
        fclose(f);
    //}

    } else {
        printf("Only Enter one input file.\n");
    }









    sm_delete(sm);
    return 0;



}//main

// Function checks if Reg A and Reg B are numbers
int CheckRegisters(char* tok)
{
	const char *letters = "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM";
	if(strpbrk(tok, letters) != 0)
	{
		fprintf(stderr, "Reg A and Reg B must be numbers\n");
		return 0;
	}
	return 1;
}
