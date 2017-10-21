# Simulator

Using C to model a simulator of UST3400 assembly code  
Elena Ryan and Jenna Olson  
Computer Architecture (CISC340)  
Project 1 Part 2 10/20/2017  

## Procedure

The program takes as input assembled machine code.  
After loading each line into memory, the program begins executing instructions.  
The main method determines each opcode and sends the instruction to the appropriate function  

## Additional Files

* multiply.txt  
    This is the text file holding our UST-3400 machine language to multiply two integers

* mult
    this is the assembled file given as input to our simulator

## Running Instructions
* Run the following commands in order to run the multiplication program  
	  make  
	  ./simulator mult  
* Run the following commands on the test files  
      make  
      ./simulator ASSEMBLED_TEST_FILE  
* Run the following optional command to clean up all .o and executables  
      make clean

## Test Suite
*

*

*
