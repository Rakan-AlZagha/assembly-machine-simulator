# assembly_machine_simulator
SM-275 Assembly Language instruction interpreter / script compiler

This project is part of a programming assignment for the Computer Systems course taught at Trinity College-Hartford.

## Project Instructions / Brief

"The SM-275 is a new machine that is in the design phase right now. Your assignment is to write
a simulator for the machine, which will interpret instructions to test its design. The simulator
will be written in C, and it should accept instructions in the assembly language of the SM-275,
SMA."

Machine also detects input errors by outputting "???" for invalid instructions or the wrong number of arguments. 

The machine is case insensitive when referencing memory or register locations. 

## Commands
The instruction set is as follows:  
o READ data, Mk. Input data is stored at Mk. OF set if data out of range.  
o WRITE Rk|Mk. Value stored at register or memory location is output.  
o PRINTS. Print the current state of the machine (see sample output below).  
o MOVE Rj|Mj, Rk|Mk. Value stored at first operand copied to second, except
both cannot be memory locations. Example: MOVE M2, R1  
o ADD, SUB, MULT, DIV, MOD. These are all two-operand instructions.  
Each operand can be either a register or a memory location, except both cannot  
be memory locations. The result is always stored in R0. The order of operands  
for subtraction is “subtract arg1 from arg2”. The order of both division  
operations is “arg1 div/mod arg2”. 
Examples:  
▪ ADD R2, M1  
▪ MOD M3, R1  
o COMP Rj,Rk. sets flags according the table below. (This instruction is only  
required for the third program machine.c.)  
o QUIT. Quit the simulator.  

**interp.c:
**

In this file, the simulator acts as interpretor at they keyboard level. Here, the user enters an instruction in the assembly language and the simulated machine will perform the commands and update its state. 

Example at COMMAND LINE:
read 11, m0
read 17, m1
write m0
11
move m1, r1
sub r1, m0
prints (print value of memory and registers)


**script.c:
**

In this file, rather than keyboard level commands, the machine simulator reads a sequence of instructiosn from a file and performs them just as interp.c does. 

Example in file:
read 11, m0
read 17, m1
write m0
11
move m1, r1
sub r1, m0
prints (print value of memory and registers)

