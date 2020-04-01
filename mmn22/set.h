/*
mmn22 - interactive program to perform Algebra set calculations.
set.h - this file contains functions declaration of set.c, defines, includes and
the typedef of set.
@author - Ephraim Phil 205390156.
@version - 10/1/2020. */

typedef char set[128 / (sizeof(char))];


/*----------set.c functions -------------------------------------------------*/

/* this function turn on a specific bit which was given by the user
on a specific SET which was given by the user.
@param num - the bit to turn on.
@param s - a given SET to turn the bit on.
*/
void readSet(int,char []);

/* this function saves to a temporray array of integers the turned on bits from
SETA, SETB and then calls readSet() function to turn on the saved bits on
SETC.
@param setA - the first set to save the turned on bit to a temp array.
@param setB - the second set to save the turned on bits to a temp array.
@param setC - the third set to turn on all the saved bits from setA and setB
on.
*/
void unionSet(char setA[], char setB[], char setC[]);

/* this function prints to the screen the turned on bits from a given SET.
the function prints the numbers in line of 16.
@param set - the set to print the turned on bits.
*/
void printSet( char []);

/* this function saves to a temporray array of integers the turned on bits from
SETA, SETB only if the bit is turned on on both SETS
and then calls readSet() function to turn on the saved bits on th third SET.
@param setA - the first set to save the turned on bit to a temp array.
@param setB - the second set to save the turned on bits to a temp array.
@param setC - the third set to turn on all the saved bits from setA and setB
on.
*/
void intersetctSet(char setA[], char setB[], char setC[]);

/* this function saves to a temporray array of integers the turned on bits from
SETA, only if the bit is not turned on on the second given SET.
and then calls readSet() function to turn on the saved bits on th third SET.
@param setA - the first set to save the turned on bit to a temp array.
@param setB - the second set to save the turned on bits to a temp array.
@param setC - the third set to turn on all the saved bits from setA and setB
on.
*/
void subSet(char [], char [], char []);

/* this function saves to a temporray array of integers the turned on bits from
first SET only if the bits are not turned on on second SET
and the turned on bits from second set only if the bits are not turned on
on first set.
and then calls readSet() function to turn on the saved bits on the third SET.
@param setA - the first set to save the turned on bit to a temp array.
@param setB - the second set to save the turned on bits to a temp array.
@param setC - the third set to turn on all the saved bits from setA and setB
on.
*/
void symDiff(char [], char [], char []);

/* this functions turns all the bit of a given array of char in size 16 to 0*/
void calibrateSet(char []);

/* this function calculates the number of turned on bits on a given SET
@param set - the set to calcualte the number of turned on bits.
@return the number of turned on bits.
*/
int setLeng(char []);

/* this function checks if a specific bit is turned on from a specific SET.
@param num - the specific bit to check if turned on
@param set - the specific SET to check if the bit is turned on.
@return true if the bit is not on and false if the bit is on.
*/
bool notInSet(int,char []);
/*--------------------GLOBAL declarations -----------------------------------*/
