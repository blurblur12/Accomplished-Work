# Accomplished-Work
C++ programs demonstrating applications of OOP, data abstraction, and various data structure including priority queue AVL tree, and skip lists, and LZW compression

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
