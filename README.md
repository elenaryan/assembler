# Assembler

Using C to model an assembler of UST3400 machine language  
Elena Ryan and Jenna Olson  
Computer Architecture (CISC340)  
Project 1 Part 1 10/15/2017  


## Procedure
Steps through the input code once, calculates and stores the addresses of all symbolic labels

On second pass, construct UST3400 machine code, and if there is a destination file, write to it.  If not, print machine code.

## Source and Includes

The hashmap used in this implementation is a string hashmap from  is from : http://pokristensson.com/strmap.html

## Running Instructions

Upon unzipping the .zip including strmap.h, strmap.c, assembler.c, test_suite folder, README.md, and Makefile:
* Run the following commands  
	  make  
	  ./assembler input_file_with_assembly_code_for_UST-3400 Instructions_Destination_File  
* You can also run  
	   make  
	   ./assembler input_file  
to print the instructions rather than writing them into a destination file

## Test Suite
*	alpharegisters
	
* duplicates.txt  
	checks for duplicate labels

* givenexample.txt  
	provides the given UST-3400 assembly instructions for testing (tests lw, add, beq, noop, halt, and .fill)

* otheroperations.txt  
	tests for operations not used in any other tests (nand, sw, jalr)

* invalidlabel1.txt  
	checks for labels that begin with numbers

* invalidlabel2.txt  
	checks for labels that contain special characters

* invalidlabel3.txt  
	checks for labels greater than 6 characters

* invalidopcode.txt  
	tests a command with an invalid opcode but proper registers and formatting

* invalidregister.txt  
	attempts to use an invalid register (a register that does not exist)

* alpharegisters  
	attempts to use an invalid register (one that is not a number)


