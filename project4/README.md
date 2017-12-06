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




## Test Suite

