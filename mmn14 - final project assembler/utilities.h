/*
mmn14 - two pass assembler model based on imaginary computer
and imaginary assembly language.
utilities.h - header file, contains the functions prototypes of utilities.c
@author - Ephraim Phil.
@version - 10/03/2020. */

#ifndef UTILITIES

#define DATA_OR_STRING 8
#define EXTERN_OR_ENTRY 8
#define EXTERN_OPERAND 32
#define ENTRY_OPERAND 32
#define SOURCE_OPERAND 128
#define DESTINATION_OPERAND 128

#define UNDEFINED -1
#define MISSING -2

#define MOV 0
#define CMP 1
#define ADD 2
#define SUB 3
#define LEA 4
#define CLR 5
#define NOT 6
#define INC 7
#define DEC 8
#define JMP 9
#define BNE 10
#define RED 11
#define PRN 12
#define JSR 13
#define RTS 14
#define STOP 15

#define IMMEDIATE_SORT 0
#define DIRECT_SORT 1
#define INDIRECT_REGISTER_SORT 2
#define DIRECT_REGISTER_SORT 3

/* calculateL - gets an instruction opCode and calculated the number of words
that need to be coded to the instructions table.
return the number of words that need to be coded to the instructions table.
*/
int calculateL(int opCode);

/* toBInary - converts decimal number to binary.
return the given number in binary.
*/
char * toBInary(int num);

/* convertToOctal - converts binary number to base8.
return the given number in base8.
*/
int convertToOctal(char binary[]);

/* isnumber - checking if a string is a number.
return true if the string is a number.
*/
bool isnumber(char number[]);

#endif
