#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strmap.h"
#include "strmap.c"

/*
 * Elena Ryan and Jenna Olson
 * Computer Architecture
 * Assembler
 * Project Part I | 10/15/2017
*/

int CheckRegisters(char*);

int main(int argc, char **argv)
{

    StrMap *sm;
    char buf[300];
    sm = sm_new(10);
    int result;

    if(argc == 1) {
        printf("No input given.\n");
	return 0;
    } else if (argc > 1 && argc <= 3 ) {
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
	    const char *special_characters = "`~@#$%^&*()+=<>,.?/{}[]|";
	    while (token) {
                if(mark != markold) {
                    //Check if first token is a label
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
			// Checks for invalid opcodes -- 
			char* token2 = token;
			token = strtok(NULL, " \t\n");
			if(strcmp(token, "beq") != 0 && strcmp(token, "add") != 0 && strcmp(token, "nand") != 0 && strcmp(token, "lw") != 0 && strcmp(token, "sw") != 0 && strcmp(token, "jalr") != 0 && strcmp(token, "halt") != 0 && strcmp(token, "noop") != 0 && strcmp(token, ".fill") != 0)
			{
				fprintf(stderr, "Opcode invalid.\n");
				return 0;
			}
                        char str[128];
                        sprintf(str, "%d", mark);
                        sm_put(sm, token2, str);
                     }//inner statement
                    markold = mark; //so mark and markold only diverge for first token in a line
                }
                //printf("%s\n", token);
                token = strtok(NULL, " \t\n");
            }
            mark++;
        }//while
        fseek(f, 0, SEEK_SET);

        char parse[2048];
	int instructions[mark]; //integer array of instructions
        int m = 0;//counts lines
        int inst;
        while(fgets(parse, 2048, f) != NULL) {
            char* tok = strtok(parse, " \t\n");
            if(sm_exists(sm, tok)!=0 || tok == NULL){
                tok = strtok(NULL, " \t\n");
            }// if first tok is a label, move on


            if(strcmp(tok, "add")==0) {
		inst = 0 << 22;
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = atoi(tok) | inst;
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = atoi(tok)<<19 | inst;
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = atoi(tok)<<16 | inst;
		instructions[m] = inst;
            }

            if(strcmp(tok, "nand")==0) {
                inst = 1 << 22;
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = atoi(tok) | inst;
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = atoi(tok)<<19 | inst;
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = atoi(tok)<<16 | inst;
                instructions[m] = inst;
            }

            if(strcmp(tok, "lw")==0) {
                inst = 2 << 22;
                char buf[6];
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = (atoi(tok) <<19) | inst;
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
		inst = atoi(tok)<<16 | inst;
		tok = strtok(NULL, " \t\n");
		if(sm_get(sm, tok, buf, sizeof(buf)) != 0) {
			inst = (atoi(buf) & 0xFFFF) | inst;
		} else if(CheckRegisters(tok) == 0) {return 0;}
		else {
	        	inst = atoi(tok) | inst;
                }
		instructions[m] = inst;
            }

            if(strcmp(tok, "sw")==0) {
                //printf("INSW\n");
                inst = 3 << 22;
		char buf[6];
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = (atoi(tok) <<19) | inst;
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
		inst = (atoi(tok) <<16) | inst;
		tok = strtok(NULL, " \t\n");
		if(sm_get(sm, tok, buf, sizeof(buf)) != 0) {
            //printf("INSIDE label if\n");
			inst = (atoi(buf) & 0xFFFF)  | inst;
		}else if(CheckRegisters(tok) == 0) {return 0;}
                else {
                    //tok = strtok(NULL, " \t\n");
                    //printf("before setting inst\n");
                	inst = atoi(tok) | inst;
                    //printf("after setting inst\n");
                }
            //printf("trying to access inst[%d] and put in%d\n", m, inst);
		    instructions[m] = inst;
            //printf("SW DONE");
            }
            if(strcmp(tok, "beq")==0) {
                inst = 4 << 22;
		char buf[6];
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = (atoi(tok) <<19) | inst;
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
		inst = (atoi(tok) << 16)| inst;
		tok = strtok(NULL, " \t\n");
		if(sm_get(sm, tok, buf, sizeof(buf)) != 0) {
			int a = atoi(buf);
			a = a - (m+1);
			a = a & 0xFFFF;
			inst = a | inst;
		}else {
	         	inst = atoi(tok) | inst;
                }
                instructions[m] = inst;
            }

            if(strcmp(tok, "jalr")==0) {
                inst = 5 << 22;
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = atoi(tok)<<19 | inst;
                tok = strtok(NULL, " \t\n");
		if(CheckRegisters(tok) == 0) {return 0;}
                inst = atoi(tok)<<16 | inst;
                instructions[m] = inst;
            }

            if(strcmp(tok, "halt")==0) {
                inst = 6 << 22;
		instructions[m] = inst;
            }

            if(strcmp(tok, "noop")==0) {
                inst = 7 << 22;
                instructions[m] = inst;
            }

            if(strcmp(tok, ".fill")==0) {
		tok = strtok(NULL," \t\n");
		char buf[6];
		if(sm_get(sm, tok, buf, sizeof(buf)) != 0) {
			inst = atoi(buf);
		} else {
			inst = atoi(tok);
		}
                instructions[m] = inst;
             }


            while(tok) {
                tok = strtok(NULL," \t\n");
            }
            m++;

        }
        fclose(f);

	if(argc == 3) {
		//write out results to input file
		FILE *fp;
		fp = fopen(argv[2], "w+");
		int i = 0;
		while(i < m) {
			fprintf(fp, "%d\n", instructions[i]);
			i++;
		}
		fclose(fp);
	} else {
		int i = 0;
		while(i < m) {
		printf("%d\n", instructions[i]);
		i++;
		}
	}//print to file

    } else {
        printf("Only enter two file inputs.\n");
    }
	sm_delete(sm);
    return 0;

}//main

// Function checks if Reg A and Reg B are numbers
int CheckRegisters(char* tok)
{
	const char *badchars = "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM~!@#$%^&*()_+=?<>{}[]|/";
	int tok_num = atoi(tok);
	if(strpbrk(tok, badchars) != NULL || tok_num >= 8 || tok_num < 0)
	{   
        printf("reg is %s\n", tok);
		fprintf(stderr, "Reg A and Reg B must be valid registers.\n");
		return 0;
	}
	return 1;
}
