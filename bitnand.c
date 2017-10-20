/* nand test */

#include <string.h>
#include <stdio.h>

int main(int argc, char **argv)
{   
    int mult = 5;
    int b = 1;
    int c = 2;

    int this = ~(~(5 & 1) & ~(1 & 0));
    int that = ~(~(5 & 2) & ~(2&0));

    printf("nand 5 and 1: %d\n nand 5 and 2: %d\n", this, that);
    return 0;



}//main
