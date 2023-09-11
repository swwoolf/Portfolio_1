Steven Woolf
Auburn University
COMP 4300: Computer Architecture 

Project 1 README

This text file will explain the compilation and use of the Stack-based machine simulator and Accumulator-based machine simulator pieces of software. 
Both of these simulators are intended to run assembly code programs that employ one of two simple instruction set architectures with custom hexadecimal
encodings. In other words, I created my own stack machine assembly language and accumulator machine assembly language for the purpose of this project.
Both of these "languages" were based off of the MIPS assembly language. 

The stack assembly program and accumulator assembly program both perform the function of finding the result of the mathematical equation AX^2 + BX + C,
whereas X = 3, A = 7, B = 5, and C = 4. The result is printed in the format of "Answer = [some number]". Note that memory locations of integers 
and characters are hard coded into the programs. Since the simulators do not have the ability to automatically "know" where to store and retrieve data from memory, 
they must perform these actions as I manually directed them to in the assembly programs. 

The source code for the stack based simulator is "stackSim.cpp" and the source code for the accumulator based simulator is "accumSim.cpp". 
The assembly code for the quadratic_eval program intended to run on the stack machine is called "stackCode.txt" and the quadratic_eval 
program for the accumulator machine is called "accumCode.txt". stackCode can only run on the stack simulator and accumCode can only run on 
the accumulator simulator.

Both programs are coded with C++. They use file I/O functions from the C++ library to gather the text from the assembly files. Subsequently, the text 
from the assembly files is chagned into 8-bit and 32-bit integers for the purpose of being readable to the machines. These integers are stored in
array data structures, which serve much the same purpose as physical memory hardware on a computer. The stack in the stack-based machine simulator
is also implemented as an array of 32-bit integers. However, the accumulator in the accumulator-based machine simulator is modeled using a generic integer
primitive value (without the actual size specified). 

Memory address 0 is the start of the kernel text segment. Address 100 is the start of the kernel data segment. Address 200 is the beginning of the
user text segment, while address 500 is the base of the user data segment. 

The kernel text segment contains opcodes that perform the operations of the machine. These are comparable to functions or methods that are included with the language
in a high-level programming language. The kernel data segment contains data relevant to the internal functions of the machine. This includes the base addresses of 
the user data and text segments. The user data segment contains all of the values in the ".data" section of the program. The addresses in which these values are 
stored is listed to the left of the value to be stored. The user text segment contains all opcodes and memory addresses that show up in the the ".text" segment of 
the assembly code. All memory addresses in the ".text" code are addresses pointing to locations in the user data segment.   

Instructions for compiling the simple machine source codes (C++ files), along with a few technical jargon definitions, directions for use of the machines, and 
explanations of my custom instruction set architectures (custom assembly languges) are provided below.

TECHNICAL BACKGROUND AND DEFINITIONS:
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
Stack-Based Machine- a simple computer whereas a special region of memory known as the "stack" performs the functions that would be performed by registers in
		     a normal computer system. All values used from data memory for mathematical calculations or printing to a console must be "pushed" onto the stack
		     before the necessary operations are performed.

Accumulator-Based Machine- a simple computer based off of a single register. This register is called the "accumulator". All values from data used for mathematical 
			   operations and printing to a console must pass through the "accumulator".

MIPS Architecture- a RISC-based instruction set architecture invented by MIPS Technologies in 1985. Used for the CPUs of many servers and supercomputers in the 1980s
		   and 1990s. Also employed in the Sony Playstation and Playstation 2 video game consoles. Today, the MIPS architecture is often the system of choice 
		   for university computer architecture courses. 

COMPILATION:
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
If compiling in a linux terminal using gcc, use the command "g++ -std=c++11 stackSim.cpp -o stackSim" for the stack based simulator and use 
"g++ -std=c++11 accumSim.cpp -o accumSim" for the accumulator based simulator. The resulting executable files must be placed in the same directory 
as the stack machine and accumulator machine assembly programs.

USE:
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
Note that the stack and accumulator machine assembly files must be in the same directory as the compiled simulator source code files in order
for the programs to run.

After following the above instructions for compiling, the simulators can both be run with the usual ./[executable file name] command. 
The resulting terminal output is the output and result of the mathematical operations in the program. Both assembly programs 
should have the same output because they both perform the same general function, though with different machines.

INSTRUCTION ENCODINGS AND DESCRIPTION:
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
8-bit Hexadecimal Opcodes for Stack Machine Instructions:

PUSH = 81 (hex) = 129 (decimal)
POP = 82 (hex) = 130 (decimal)
ADD = 83 (hex) = 131 (decimal)
MULT = 84 (hex) = 132 (decimal)
print string = 85 (hex) = 133 (decimal)
print from stack = 86 (hex) = 134 (decimal)
Load immediate = 90 (hex) = 144 (decimal)
END = 87 (hex) = 134 (decimal)

PUSH- takes a data segment memory address as an operand and places the value at that address at the top of the stack
POP- takes a data segment memory address as an operand and places the value on top of the stack into that location
ADD- pops two values off of the stack, adds the two values, and places the result on the stack
MULT- pops two values off of the stack, multiplies them, and places the result on the stack
print string- prints a string that begins at the specified memory location to the console
print from stack- prints the value on top of the stack to the console
PUSH immediate- takes a numerical value as an operand and pushes it onto the stack
END- Ends the program  

8-bit Hexadecimal Opcodes for Accumulator Machine Instructions:

LOAD = 81 (hex) = 129 (decimal)
STO = 82 (hex) = 130 (decimal)
ADD = 83 (hex) = 131 (decimal)
MULT = 84 (hex) = 132 (decimal)
print string = 85 (hex) = 133 (decimal)
print value in accumulator = 86 (hex) = 134 (decimal)
Load immediate = 90 (hex) = 144 (decimal)
END = 87 (hex) = 134 (decimal)

LOAD- takes a memory address as a parameter and loads a data item from that address into the accumulator
STO- takes a memory address as a parameter and loads the data item currently occupying the accumulator into the specified memory address
ADD- takes a memory address as a parameter and adds the value at that address to the value in the accumulator. The result is stored in the accumulator.
MULT takes a memory address as a parameter and multiplies the value from that address times the value in the accumulator. The result is stored in the accumulator.
print string- prints a string that begins at the specified memory location to the console
print value in accumulator- prints the value in the accumulator to the console
LOAD immediate- takes a numerical value as an operand and loads it into the accumulator
END- Ends the program 

An instruction is 40 bits long, consisting of an 8-bit opcode and
a 32-bit memory address. 

Data values are loaded into the data segment (the data segment array) 
as 32-bit integers. All addresses equal to or greater than 0x000001f4
are part of the data segment. Strings are denoted by a '/' character
at the beginning and a '\' character at the end. All characters are
stored as 32-bit integers in the form of their ASCII numerical values. 

An instruction is loaded into memory (that is, the text segment
array) in the order of opcode, operand. Thus, if an opcode is in 
address 0x00000019, then the memory location or immediate value 
operand will be located in address 0x0000001A. The text segment
encompasses all memory addresses less than 0x000001f4 or greater 
than 0x000000c7.

All numbers in the Stack Machine and Accumulator Machine assembly 
programs are hexadecimal numbers.
