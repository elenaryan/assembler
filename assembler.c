#include <stdio.h>

/* 
 * Elena Ryan and Jenna Olson
 * Computer Architecture
 * Assembler
 * Project Part I | 10/13/2017
*/



int main(int argc, char **argv)
{
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

        while(fgets(write, 2048, f) != NULL) {
            printf ("%s", write);
            /* This will probably be the first step through of the program where labels are located and stored*/

        }//while 
        fclose(f);
    } else {
        printf("Only Enter one input file.\n");
    }


    return 0;



}//main
