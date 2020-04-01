/*
mmn14 - two pass assembler model based on imaginary computer
and imaginary assembly language.
file_parser.h - header file, contains the functions prototypes of file_parser.c
@author - Ephraim Phil.
@version - 10/03/2020. */

#ifndef FILE_PARSER


/* this function read line by line from a file that were given by the user then
it calls the runFirstScan function, then it calls the runSecondScan function and
at the end calls the functions that write all the data to files.
*/
void fileParser(char []);

/* first pass - this function disects a given line and builds the instructions table, the data table
and symbols table.
*/
bool runFirstScan(ptrCODE *, ptrDATA *, ptrSYMBOLS *, char [], int *, int *,char[],int);

/* second pass -  this function disects a given line and updates the instructions table, the data table
and symbols table.
*/
bool runSecondScan(ptrCODE *, ptrDATA *, ptrSYMBOLS *, char [], int *, int *,char[],int);

/* this function disects an instruction line, build the words
and adds it to the instructions table.
*/
void addInstructionWord(ptrCODE *, int *,char *,char *,char *,int,int,int);

/* this function disects .string or .data directives line, build the words
and adds it to the data table.
*/
bool addData(ptrDATA *,int *,char *,char [],char[],int);

#endif
