/*
mmn14 - two pass assembler model based on imaginary computer
and imaginary assembly language.
input_parser.h - header file, contains the functions prototypes of input_parser.c
@author - Ephraim Phil 205390156.
@version - 10/03/2020. */


#ifndef INPUT_PARSER


/*-------CHECKERs---------------------------------------------*/

/* this function gets a line(input) and checks if the first field in the line
is a symbol (also checks for erros).
return true if a symbol was found.
*/
bool checkForSymbol(char [], char *, bool,char[],int,bool*);

/* this function gets a line(input) and checks if it is .data or .string directive.
(also checks for erros).
return true if .data or .string directives was found.
*/
bool checkForDataOrString(char [],char *,bool,char[],int,bool*);

/* this function gets a line(input) and checks if it is .extern or .entry directive.
(also checks for erros).
return true if .extern or .entry directives was found.
*/
bool checkForExternOrEntry(char [],char *,bool,char[],int,bool *);

/* this function gets a line(input) and checks if the sorting method of the source and the destination operands
are valid to the instruction.
return true if the line is valid.
*/
bool checkSortingMethodValidity(int ,int ,int,char[],char[],char[],int);


/*--------GETERs ---------------------------------------------*/

/* this function gets a line(input) and returns the opCode of the instruction name
and copies the opcode in binary to a variable.
return the opCode of the instruction name.
*/
int getOpCode(char [],char * );

/* this function gets a line(input) and extracts the operands(source and destination).
return true if no errors were found.
*/
bool getOperands(ptrSYMBOLS *,char [],int,char *,char *,int *,int*,int,bool,char[],int);

/* this function gets a source operand, checks for errors and sets sourceOperand to the binary representation
of its sorting method.
*/
void getSource(ptrSYMBOLS *,char[],char *,int *,bool,char[],int);

/* this function gets a destination operand, checks for errors and sets destinationOperand
to the binary representation of its sorting method.
*/
void getDestination(ptrSYMBOLS *,char[],char *,int *,bool,char[],int);

/* this function gets a line(input), and extracts the operand of extern or entry directive, it saves it in operand.
*/
void getExternOrEntryOperand(char [], char *);


/*----------REMOVERs ------------------------------------------*/

/* this function removes a symbol from a line(input).
*/
void removeSymbol(char []);

/* this function removes the instruction name from a line(input).
*/
void removeOpCode(char []);

/* this function removes an empty line(if the line contains only spaces or tabs).
*/
void removeEmptyLine(char []);

/* this function removes the '\n' char from a line.
*/
void removeNewLine(char []);

/* this function removes comments from a line.
*/
void removeComments(char []);

/* this funtions removes white space or tabs from a line (input).
when the funtion encounter a charachter which is not white space or tab,
the function stops and return the index of that charachter. */
int removeSpace(int i,char []);

#endif
