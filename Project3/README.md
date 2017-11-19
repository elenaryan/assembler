
# Pipeline Simulator

Using C to model a pipelined implementation of UST-3400 
Elena Ryan and Jenna Olson  
Computer Architecture (CISC340)  
Project 3 11/20/2017  


## Procedure
This c code is a pipeline simulator.  Using a series of pipeline registers, the simulator processes assembled machine code.  The first step takes the instruction from memory, the next step reads the appropriate registers, the step after that is the alu, the next step accesses memory, and the final step writes back to memory or the appropriate register.

## Running Instructions

Upon unzipping the .zip:  
* Run the following commands  
	  make  
	  ./pipeline_simulator assembled_file  
* You can also run  
	   make clean  
to remove the .o and executable files

## Test Suite
	
* loadhaz  
	This checks for load stall capability as well as data forwarding hazards.  This ensures that if both registers inputted to the ALU are being updated by a prior instruction later in the cycle, the correct values will be sent to the ALU.  In this example, the data is forwarded from two different steps to both registers.

* pip1  
	This uses adds, loads, and beq to demonstrate handling of the branching hazard




