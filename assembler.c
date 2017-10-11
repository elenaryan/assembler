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
            char* token = strtok(write," \t");
            while (token) {
                if(mark != markold) {
                    //check if first token is a label
                    if(strcmp(token, "beq") != 0 && strcmp(token, "add") != 0 && strcmp(token, "nand") && strcmp(token, "lw")!= 0 && strcmp(token, "sw") != 0 && strcmp(token, "jalr")!=0 && strcmp(token, "halt") != 0 && strcmp(token, "noop")!= 0 && strcmp(token, ".fill") != 0) {
                        printf("label is %s*** at line %d\n", token, mark);
                        char str[128];
                        sprintf(str, "%d", mark);
                        sm_put(sm, token, str);
                     }//inner absurd statement
                    
                    markold = mark; //so mark and markold only diverge for first token in a line
                }
                token = strtok(NULL, " \t");
            }
            /* This will probably be the first step through of the program where labels are located and stored*/
            mark++;
        }//while 
        fclose(f);
    } else {
        printf("Only Enter one input file.\n");
    }


    result = sm_get(sm, "hur", buf, sizeof(buf));
    if (result == 0) {
        /* Handle value not found... */
    }
    printf("value: %s\n", buf);


    sm_delete(sm);
    return 0;



}//main
