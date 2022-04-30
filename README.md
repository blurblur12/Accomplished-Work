# Accomplished-Work
C++ programs demonstrating applications of OOP, data abstraction, various data structures, and computer architecture

Straights:
- a simplified straights card game implemented using object oriented programming and data abstraction, review the demo.pdf file to see functions and working features of the program

Priority queue:
- included three different implementations of the priority queue data stracture
- the first implementation used linked list, the second used max-heapd, and the third used a vector of queues
- included runtime and space analysis for each implementation
- check the description.txt file to see commands for the program

AVLSkip:
- implements two realizations of dicionary using AVL-tree and skip list
- check description.txt file to see commands for the program

LZW Compression:
- implements a variant of LZE compression
- the source alphabet is the lower case English alphabet, coded alphabet is binary
- two modeifications to the LZW:
1. the initial dictionary consists of ’a’: 0, ’b’: 1, ’c’: 2,..., ’z’: 25 and ’aa’: 26, ’ab’: 27, ...., ’az’: 51,
’ba’: 52, ’bb’: 53, ..., ’bz’: 77,
...........................
’za’: 676 , ’zb’: 677,..., ’zz’: 701

2. at every iteration adds two new dictionary entries instead of one

- check description.txt to see details about acceptable inputs

MIPS Assembler:
- implements an assembler for MIPS assembly language
- reads from standard input a MIPS assembly language program
- if input file is not a valid assembly language program, produce an error message in standard error and return
- if the input contains a correct MIPS assembly language program, the assembler output the equivalent MIPS machine    language to standard output
- check description.txt to see details about files in the directory

WLP4 Scanner:
- implements a scanner for the WLP4 programming language, which is a simplified C++ programming language
- reads a WLP4 program from standard input and identify the tokens in the order they appear in the input
- outputs the token and the lexeme (the string of characters making up the token) to standard output
- WLP4 is a language developed by University of Waterloo for educational purpose:
https://student.cs.uwaterloo.ca/~cs241/wlp4/WLP4tutorial.html
- check description.txt to see details about files in the directory

WLP4 Parser:
- implements a parser for the WLP4 programming language
- reads the output of WLP4 scanner as input, which should be a list of tokens and lexemes in the order of appearance
- outputs a parse tree using preorder traversal
- if the input is not recognized, error is delcared to standard error and the program terminates
- check description.txt to see details about files in the directory

WLP4 Compiler:
- implements a compiler for a WLP4 program and compiles it to to assembly language program
- reads the output of WLP4 parser as input, which should be the context-free parse of a WLP4 program
- outputs the equivalent MIPS assembly code

Allocator:
- includes two programs demonstrating memory allocation using free list algorithm
- cons.cc implements the functionality of a10p2 using a10p1
- malloc.cc: includes two functions namely mymalloc and myfree
  - mymalloc: takes an argument called size and allocates a block of memory of at least size words, where a word is 32 bits, and returns the address of that memory or 0 (NULL) if memory could not be allocated
  - myfree: takes an argument called address and deallocates the memory stored at address, assuming that address contains either an address allocated by mymalloc, in which case it deallocates that memory, or the value 0 (NULL), in which case myfree does nothing.
- the programs a10p1 and a10p2 are both provided by the University of Waterloo
