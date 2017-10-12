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
	    char* token = strtok(write," \t");
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
	    char *checkerspecialchar1 = strstr(token, "!");
            char *checkerspecialchar2 = strstr(token, "@");
  	    char *checkerspecialchar3 = strstr(token, "#");
  	    char *checkerspecialchar4 = strstr(token, "$");
  	    char *checkerspecialchar5 = strstr(token, "%");
  	    char *checkerspecialchar6 = strstr(token, "^");
  	    char *checkerspecialchar7 = strstr(token, "&");
  	    char *checkerspecialchar8 = strstr(token, "*");
  	    char *checkerspecialchar9 = strstr(token, "?");
  	    char *checkerspecialchar10 = strstr(token, "<");
  	    char *checkerspecialchar11 = strstr(token, ">");	    

	    while (token) {
                if(mark != markold) {
                    //check if first token is a label
                    if(strcmp(token, "beq") != 0 && strcmp(token, "add") != 0 && strcmp(token, "nand") && strcmp(token, "lw")!= 0 && strcmp(token, "sw") != 0 && strcmp(token, "jalr")!=0 && strcmp(token, "halt") != 0 && strcmp(token, "noop")!= 0 && strcmp(token, ".fill") != 0) {
                        // Checks for invalid labels
			if(checker0 == token || checker1 == token || checker2 == token || checker3 == token || checker4 == token || checker5 == token || checker6 == token || checker7 == token || checker8 == token || checker9 == token|| checkerspecialchar1 == token || checkerspecialchar2 == token || checkerspecialchar3 == token || checkerspecialchar4 == token || checkerspecialchar5 == token || checkerspecialchar6 == token || checkerspecialchar7 == token || checkerspecialchar8 == token || checkerspecialchar9 == token || checkerspecialchar10 == token || checkerspecialchar11 == token)
			{
				fprintf(stderr, "Label cannot begin with a number, or contain symbols.\n");
				return 0;
			}
			// Checks for duplicates
			if(sm_exists(sm, token) == 1)
			{
				fprintf(stderr, "Duplicate labels cannot be entered.\n");
				return 0;
			}
			printf("label is %s*** at line %d\n", token, mark);
                        char str[128];
                        sprintf(str, "%d", mark);
                        sm_put(sm, token, str);
                     }//inner absurd statement                    
                    markold = mark; //so mark and markold only diverge for first token in a line
                }
                token = strtok(NULL, " \t");
            }
            mark++;
        }//while 
        fseek(f, 0, SEEK_SET);

        char parse[2048];//be sure to change these numbers to max handlers of the assembler
        

        
        //HERE the file is open, but outside of the first pass

        














        fclose(f);
    } else {
        printf("Only Enter one input file.\n");
    }









    sm_delete(sm);
    return 0;



}//main

