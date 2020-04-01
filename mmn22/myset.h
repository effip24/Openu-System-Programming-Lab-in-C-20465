/*
mmn22 - interactive program to perform Algebra set calculations.
myset.h - this file contains functions declaration of myset.c, and defines.
@author - Ephraim Phil 205390156.
@version - 10/1/2020. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define READ_SET 0
#define PRINT_SET 1
#define UNION_SET 2
#define INTERSECT_SET 3
#define SUB_SET 4
#define SYMDIFF_SET 5
#define MAX_INPUT_NUMBERS 127

#define SET_A 0
#define SET_B 1
#define SET_C 2
#define SET_D 3
#define SET_E 4
#define SET_F 5
#define MAX_INPUT 1000
/*-----------myset.c functions ----------------------------------------------*/

/* detects command name from user input.
after the function detects the command name it substring the user input
so the input doesnt contain the command name any more.
@param input - the user input.
@return -1 if it did not detect a command and returns value from 0-5 if
a command detected(any value represents a command).
*/
int currState(char []);

/* detects a set from user input.
after the function detects a set it substring the user input so the input
does not contain the set name any more.
@param input - the user input.
@return -1 if did not detect a set name and a value from 0-5 if a set
was detected (each value represents a set name).
*/
int detectSet(int,char []);

/* this function checks for errors in user input when read_set command
was inserted by user.
if no error detected the function calls readSet() function from set.c
@param input - the user input.
@param setsArray - array of pointers to each set.
*/
void checkReadSetInput(char [], char**);

/* this function checks for errors in user input when print_set command
was inserted by user.
if no error detected the function calls printSet() function from set.c
@param input - the user input.
@param setsArray - array of pointers to each set.
*/
void checkPrintSetInput(char [], char**);

/* this function checks for errors in user input when one of union_set,
intersect_set, sub_set, symdiff_set command was inserted by user.
@param input - the user input.
@param setsArray - array of pointers to each set.
*/
void checkUnionIntersectSubSymdiffINput(int,char [], char**);

/* this funtions removes white space or tabs from user input.
@param i - the current index of white space or tab int user input.
@param input - the user input.
when the funtion encounter a charachter which is not white space or tab,
the function stop and return the index of that charachter.
*/
int removeSpace(int ,char []);

/* this function converts an array of chars
(if the chars represent numbers from 0-9) to integer.
@param input - the array of chars to be converted to int.
@return - return an integers.
*/
int toInt(char[]);

/* this function prints a messeage to user when the program starts*/
void printMssg();

/* this function prints instructions to user when the user inserts --help*/
void printManual();
