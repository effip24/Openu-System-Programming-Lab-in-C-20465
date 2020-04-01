/* mmn22 - interactive program to perform Algebra set calculations.
set.c - this file has the functions that perform bitwise operations on SETS.
@author - Ephraim Phil 205390156.
@version - 10/1/2020. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "set.h"

/* this function turn on a specific bit which was given by the user
on a specific SET which was given by the user.
@param num - the bit to turn on.
@param s - a given SET to turn the bit on.
*/
void readSet(int num, char s[])
{
  /* each SET is an array of chars in size of 16,
  each char in the array has 8 bits and each bit represents a number.
  if we want to turn on a specific bit on a specific SET
  we need to go to the right char in the SET.
  for instance if we want to turn on bit number 15 in a SET,
  we need to devide 15/8 (because every char has 8 bits)
  and  to create a mask that shift
  1 << sizeof(char)* ( 7(0-7 bits in a char) - (num - multiply of num 16))
  */

  /* the index to get to specific char in SET */
  int i = (num / 8);
  int mask;

  /* this if's statements check the range of the given number
  to determine the mask */
  if(num >=0 && num <= 7)
  {
    mask = 1 << (sizeof(char) *(7-num));
    s[i] =  s[i] | mask;
  }

  else if(num >=8 && num <= 15)
  {
    mask = 1 << (sizeof(char) *(7-(num-8)));
    s[i] =  s[i] | mask;
  }

  else if(num >=16 && num <= 23)
  {
    mask = 1 << (sizeof(char) *(7-(num-16)));
    s[i] =  s[i] | mask;
  }

  else if(num >=24 && num <= 31)
  {
    mask = 1 << (sizeof(char) *(7-(num-24)));
    s[i] =  s[i] | mask;
  }

  else if(num >=32 && num <= 39)
  {
    mask = 1 << (sizeof(char) *(7-(num-32)));
    s[i] =  s[i] | mask;
  }

  else if(num >=40 && num <= 47)
  {
    mask = 1 << (sizeof(char) *(7-(num-40)));
    s[i] =  s[i] | mask;
  }

  else if(num >=48 && num <= 55)
  {
    mask = 1 << (sizeof(char) *(7-(num-48)));
    s[i] =  s[i] | mask;
  }

  else if(num >=56 && num <= 63)
  {
    mask = 1 << (sizeof(char) *(7-(num - 56)));
    s[i] =  s[i] | mask;
  }

  else if(num >=64 && num <= 71)
  {
    mask = 1 << (sizeof(char) *(7-(num-64)));
    s[i] =  s[i] | mask;
  }

  else if(num >=72 && num <= 79)
  {
    mask = 1 << (sizeof(char) *(7-(num-72)));
    s[i] =  s[i] | mask;
  }

  else if(num >=80 && num <= 87)
  {
    mask = 1 << (sizeof(char) *(7-(num-80)));
    s[i] =  s[i] | mask;
  }

  else if(num >=88 && num <= 95)
  {
    mask = 1 << (sizeof(char) *(7-(num-88)));
    s[i] =  s[i] | mask;
  }

  else if(num >=96 && num <= 103)
  {
    mask = 1 << (sizeof(char) *(7-(num-96)));
    s[i] =  s[i] | mask;
  }

  else if(num >=104 && num <= 111)
  {
    mask = 1 << (sizeof(char) *(7-(num-104)));
    s[i] =  s[i] | mask;
  }

  else if(num >=112 && num <= 119)
  {
    mask = 1 << (sizeof(char) *(7-(num-112)));
    s[i] =  s[i] | mask;
  }

  else if(num >=120 && num <= 127)
  {
    mask = 1 << (sizeof(char) *(7-(num-120)));
    s[i] =  s[i] | mask;
  }
}

/* this function prints to the screen the turned on bits from a given SET.
the function prints the numbers in line of 16.
@param set - the set to print the turned on bits.
*/
void printSet( char set[])
{
  /* counts how much bits are turned on (each bit represents a number ) */
  int cnt = 0;
  int i;
  /* count how much numbers were printed to screen if it printed 16
  we need to print new line. */
  int line = 0;


  for(i = 0; i < 128; i++)
  {
    /* checks if the current i, i represents a bit that represents a number,
    is turned on in the given set, notInSet() return true if the bit is
    not turned on so we need to check if !notInSet() if its true the
    current i is printed to screen. */
    if(!notInSet(i,set))
    {
      if(line == 16)
      {
        puts("");
        line = 0;
      }
      cnt++;
      line++;
      printf("%d ",i );
    }
  }
  /* if cnt is zero it means that there is not a turned on bit in the given SET
  so the SET is empty. */
  if (cnt == 0)
  {
    puts("this set is empty");
  }
  puts(" ");
}

/* this function prints to the screen the turned on bits from a given SET.
the function prints the numbers in line of 16.
@param set - the set to print the turned on bits.
*/
void unionSet(char setA[], char setB[], char setC[])
{
  int i;
  /* this is a temporary array that will save the turned on bits(number)
  in SETA and turned on bits(number) in SETB. */
  int * numbersArray = (int *)malloc(1*sizeof(int));
  int * q;
  /* number of saved numbers in the temp array. */
  int numOfNUmbers = 0;

  /* if we could not allocate memory to the temp array we print an error. */
  if(!numbersArray)
  {
    puts("falied to allocate memory ");
    return;
  }

  for(i = 0; i < 128; i++)
  {
    /* we check if the current i(current bit) is on in SETA OR SETB then we save
    the current i(as a number) in the temp array. */
    if(!notInSet(i,setA) || !notInSet(i,setB))
    {
      numbersArray[numOfNUmbers++] = i;

      /* after we saved a number we need to dynamically re allocate memory space
      to the next number. */
      q  = (int *)realloc(numbersArray,(numOfNUmbers+1)*sizeof(int));
      if(!q)
      {
        puts("memory allocation falied ");
      }
      else
      {
        numbersArray = q;
      }
    }
  }
  /* now the temp array holds all the numbers of SETA and SETB we need to calibrate
  SETC so we can isert all the numbers in the temp array to SETC. */
  calibrateSet(setC);

  /* a loop that runs and send the currnt number in the temp array to readSet()
  that inserts it to SETC. */
  for(i = 0; i < numOfNUmbers;i++)
  {
    readSet(numbersArray[i],setC);
  }
  /* after we finished to insert all the number to SETC we free the temp array. */
  free(numbersArray);
}

/* this function saves to a temporray array of integers the turned on bits from
SETA, SETB only if the bit is turned on on both SETS
and then calls readSet() function to turn on the saved bits on th third SET.
@param setA - the first set to save the turned on bit to a temp array.
@param setB - the second set to save the turned on bits to a temp array.
@param setC - the third set to turn on all the saved bits from setA and setB
on.
*/
void intersetctSet(char setA[], char setB[], char setC[])
{
  int i;
  /* a temp array that will hold the numbers of SETA and SETC. */
  int * numbersArray = (int *)malloc(1*sizeof(int));
  int * q;
  int numOfNUmbers = 0;

  if(!numbersArray)
  {
    puts("falied to allocate memory ");
    return;
  }

  for(i = 0; i < 128; i++)
  {
    /* checks if the current i(current bit) is turned on on both SETA
    and SETB. */
    if(!notInSet(i,setA) && !notInSet(i,setB))
    {
      numbersArray[numOfNUmbers++] = i;

      /* after we saved a number we need to dynamically re allocate memory space
      to the next number. */
      q  = (int *)realloc(numbersArray,(numOfNUmbers+1)*sizeof(int));
      if(!q)
      {
        puts("memory allocation falied ");
        return;
      }
      else
      {
        numbersArray = q;
      }
    }
  }
  /* before we insert the numbers from themp array to SETC we need to calibrate
  SETC. */
  calibrateSet(setC);

  /* sending all the numbers from the temp array to readSet() that insert
  them to SETC then we free the temp array.*/
  for(i = 0; i < numOfNUmbers;i++)
  {
    readSet(numbersArray[i],setC);
  }
  free(numbersArray);
}

/* this function saves to a temporray array of integers the turned on bits from
SETA, only if the bit is not turned on on the second given SET.
and then calls readSet() function to turn on the saved bits on th third SET.
@param setA - the first set to save the turned on bit to a temp array.
@param setB - the second set to save the turned on bits to a temp array.
@param setC - the third set to turn on all the saved bits from setA and setB
on.
*/
void subSet(char setA[], char setB[], char setC[])
{
  /* a temp array that will hold the numbers of SETA and SETB. */
  int i;
  int * numbersArray = (int *)malloc(1*sizeof(int));
  int * q;
  int numOfNUmbers = 0;
  if(!numbersArray)
  {
    puts("falied to allocate memory ");
    return;
  }

  for(i = 0; i < 128; i++)
  {
    /* checks if the current i(current bit) is on in SETA */
    if(!notInSet(i,setA))
    {
      /* check if the current i(current bit) is not on in SETB then the current i
      (as a number) inserted to the temp array. */
      if(notInSet(i,setB))
      {
        numbersArray[numOfNUmbers++] = i;
        q  = (int *)realloc(numbersArray,(numOfNUmbers+1)*sizeof(int));
        if(!q)
        {
          puts("memory allocation falied ");
          return;
        }
        else
        {
          numbersArray = q;
        }
      }
    }
  }

  /* before we insert the numbers from themp array to SETC we need to calibrate
  SETC. */
  calibrateSet(setC);

  /* sending all the numbers from the temp array to readSet() that insert
  them to SETC then we free the temp array.*/
  for(i = 0; i < numOfNUmbers;i++)
  {
    readSet(numbersArray[i],setC);
  }
  free(numbersArray);
}

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
void symDiff(char setA[], char setB[], char setC[])
{
  int i;
  /* a temp array that will hold the numbers of SETA and SETB. */
  int * numbersArray = (int *)malloc(1*sizeof(int));
  int * q;
  int numOfNUmbers = 0;
  if(!numbersArray)
  {
    puts("falied to allocate memory ");
    return;
  }


  for(i = 0; i < 128; i++)
  {
    /* checks if the current i(current bit) is on in SETA */
    if(!notInSet(i,setA))
    {
      /* check if the current i(current bit) is not on in SETB then the current i
      (as a number) inserted to the temp array. */
      if(notInSet(i,setB))
      {
        numbersArray[numOfNUmbers++] = i;
        q  = (int *)realloc(numbersArray,(numOfNUmbers+1)*sizeof(int));
        if(!q)
        {
          puts("memory allocation falied ");
          return;
        }
        else
        {
          numbersArray = q;
        }
      }
    }

    /* checks if the current i(current bit) is on in SETB */
    else if(!notInSet(i,setB))
    {
      /* check if the current i(current bit) is not on in SETA then the current i
      (as a number) inserted to the temp array. */
      if(notInSet(i,setA))
      {
        numbersArray[numOfNUmbers++] = i;
        q  = (int *)realloc(numbersArray,(numOfNUmbers+1)*sizeof(int));
        if(!q)
        {
          puts("memory allocation falied ");
          return;
        }
        else
        {
          numbersArray = q;
        }
      }
    }
  }

  /* before we insert the numbers from themp array to SETC we need to calibrate
  SETC. */
  calibrateSet(setC);

  /* sending all the numbers from the temp array to readSet() that insert
  them to SETC then we free the temp array.*/
  for(i = 0; i < numOfNUmbers;i++)
  {
    readSet(numbersArray[i],setC);
  }
  free(numbersArray);
}

/* this function checks if a specific bit is turned on from a specific SET.
@param num - the specific bit to check if turned on
@param set - the specific SET to check if the bit is turned on.
@return true if the bit is not on and false if the bit is on.
*/
bool notInSet(int num,char set[])
{
  int i = 0;
  int j = 0;
  unsigned int mask;

  while(i < 16)
  {
    mask = 1 << ((sizeof(char)*8)-1);

    while(mask)
    {
      if  ((set[i]&mask) == 0)
      {
      }
      else
      {
        if(j == num)
        {
          return false;
        }
      }
      mask = mask >> 1;
      j++;
    }
    i++;
  }

  return true;
}

/* this functions turns all the bit of a given array of char in size 16 to 0
@param set - the SET to calibrate.
*/
void calibrateSet(char set[])
{
  int i ;
  unsigned int mask;
  for(i = 0;i < 16; i++)
  {
    mask = 1 << ((sizeof(char)*8)-1);

    while(mask)
    {
      set[i] = set[i]&~(mask);
      mask = mask >> 1;
    }
  }
}

/* this function checks if a specific bit is turned on from a specific SET.
@param num - the specific bit to check if turned on
@param set - the specific SET to check if the bit is turned on.
@return true if the bit is not on and false if the bit is on.
*/
int setLeng(char set[])
{
  int i = 0;
  int cnt = 0;
  unsigned int mask;

  while(i < 16)
  {
    mask = 1 << ((sizeof(char)*8)-1);

    while(mask)
    {
      if  ((set[i]&mask) == 0)
      {

      }
      else
      {
        cnt++;
      }
      mask = mask >> 1;
    }
    i++;
  }
  return cnt;
}
