# Cache Simulator

Using C to model a cache and processor  
Elena Ryan and Jenna Olson  
Computer Architecture (CISC340)  
12/5/2017  


## Procedure

Parses and executes machine code instructions one by one (single-cycle).  
Simultaneously, rather than accessing instructions or values directly from memory, the cache simulation, a 3D array, acts as the interface through which the procession interacts with memory.  
The cache is structured by user input determining block size, set number, and associativity.  The cache pulls in blocks from memory, adhering to a write back policy when evicting dirty blocks, or halting the program.


## Running Instructions
Unzip folder project4\_ryan0263\_olso7916 to access the directory of the same name
Within the directory run the command:  
make  
and then run either: ./simulator (to be prompted for input) or  
./simulator -f <filename> -bsize <block size in words> -nsets <number of sets> -a <associativity>  



## Test Suite

* givenexample.4.2.1 
	Tests lw and store word through the example given in the instruction packet

* testWB.4.2.1 
	Tests the functionality of writing back to memory at halt

* LRUtest.4.2.1 
	Tests the functionality of the eviction using LRU

* difArgs.8.4.2 
	Tests a different block size, number of sets, and associativity


