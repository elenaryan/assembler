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

* givenexample.txt
	provides the given UST-3400 assembly instructions for testing

* invalidlabel1.txt

* invalidlabel2.txt

* invalidlabel3.txt

* invalidopcode.txt
	this tests two commands with invalid opcodes but proper registers and formatting

* invalidregister.txt
	attempts to use an invalid register name for the destination field of add

* text.txt
	tests lw, add, beq, noop, nand, halt, .fill with and without labels, with all valid fields


