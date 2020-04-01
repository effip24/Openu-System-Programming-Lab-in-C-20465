/*
mmn14 - two pass assembler model based on imaginary computer
and imaginary assembly language.
utilities.c - this file contains utilities functions and additions functions.
@author - Ephraim Phil 205390156.
@version - 10/03/2020. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

#include "utilities.h"

/* calculateL - gets an instruction opCode and calculated the number of words
that need to be coded to the instructions table.
@param opCode - an instruction opCode.
return the number of words that need to be coded to the instructions table.
*/
int calculateL(int opCode)
{
  switch(opCode)
  {
    /* mov, cmp, add, sub, lea instructions return 3 words */
    case MOV:
    case CMP:
    case ADD:
    case SUB:
    case LEA:
    {
      return 3;
    }
    /* clr, not, inc, dec, jmp, bne, red, prn, jsr instructions return 2 words */
    case CLR:
    case NOT:
    case INC:
    case DEC:
    case JMP:
    case BNE:
    case RED:
    case PRN:
    case JSR:
    {
      return 2;
    }
    /* rts, stop,instructions return 1 words */
    case RTS:
    case STOP:
    {
      return 1;
    }
  }
  return -1;
}

/* toBInary - converts decimal number to binary.
@param num - the number to be converted to binary.
return the given number in binary.
*/
char *toBInary(int num)
{
  /* index */
  int i = 0;
  /* mask */
  unsigned int mask = 1 << 14;
  /* the return number, converted to binary */
  char * number = (char *)malloc((sizeof(char)) * 16);
  char temp[16];


  if(!number)
  {
    puts("memory allocation failed!");
    exit(0);
  }

  /* running throught the number bits */
  while(mask)
  {
    /* if the current bit is zero */
    if((num&mask) == 0)
    {
      temp[i] = '0';
    }
    /* the bit is 1 */
    else
    {
      temp[i] = '1';
    }
    mask = mask >> 1;
    i++;
  }
  temp[i] = '\0';
  /* copies the binary to number */
  strcpy(number,temp);

  return number;
}

/* convertToOctal - converts binary number to base8.
@param binary - the number to be converted to base8.
return the given number in base8.
*/
int convertToOctal(char binary[])
{
  /* the converted number */
  int oct;
  /* indexes*/
  int i;
  int j;
  /* temporary */
  char temp[128];

  char firstNum[128];
  char secondNum[128];
  char thirdNum[128];
  char fourthNum[128];
  char fifthNumber[128];

  /* the map between binary number and its equivalent octal */
  char groups[][5] = {"000","001","010","011","100","101","110","111"};

  /* firstNum hold the 12-14 bits of the given num */
  firstNum[0] = binary[0];
  firstNum[1] = binary[1];
  firstNum[2] = binary[2];
  firstNum[3] = '\0';

  /* secondNum hold the 9-11 bits of the given num */
  secondNum[0] = binary[3];
  secondNum[1] = binary[4];
  secondNum[2] = binary[5];
  secondNum[3] = '\0';

  /* thirdNum hold the 6-8 bits of the given num */
  thirdNum[0] = binary[6];
  thirdNum[1] = binary[7];
  thirdNum[2] = binary[8];
  thirdNum[3] = '\0';

  /* fourthNum hold the 3-5 bits of the given num */
  fourthNum[0] = binary[9];
  fourthNum[1] = binary[10];
  fourthNum[2] = binary[11];
  fourthNum[3] = '\0';

  /* fifthNum hold the 0-2 bits of the given num */
  fifthNumber[0] = binary[12];
  fifthNumber[1] = binary[13];
  fifthNumber[2] = binary[14];
  fifthNumber[3] = '\0';

  /* add min 0's in the end to make right substring length divisible by 3 to temp of each segment
  (firstNum, secondNum, thirdNum, fourthNum, fifthNumber) */

  j=0;
  for(i = 0; i < 8;i++)
  {
    if(!strcmp(firstNum,groups[i]))
    {
      temp[j] = i +'0';
      j++;
      break;
    }
  }

  for(i = 0; i < 8;i++)
  {
    if(!strcmp(secondNum,groups[i]))
    {
      temp[j] = i +'0';
      j++;
      break;
    }
  }

  for(i = 0; i < 8;i++)
  {
    if(!strcmp(thirdNum,groups[i]))
    {
      temp[j] = i +'0';
      j++;
      break;
    }
  }

  for(i = 0; i < 8;i++)
  {
    if(!strcmp(fourthNum,groups[i]))
    {
      temp[j] = i +'0';
      j++;
      break;
    }
  }

  for(i = 0; i < 8;i++)
  {
    if(!strcmp(fifthNumber,groups[i]))
    {
      temp[j] = i +'0';
      j++;
      break;
    }
  }
  temp[j] = '\0';
  oct = atoi(temp);

  return oct;
}

/* isnumber - checking if a string is a number.
@param number - the string to be checked.
return true if the string is a number.
*/
bool isnumber(char number[])
{
  int i;
  if(number[0] == '-')i=1;
  else i=0;
  for(; i < strlen(number); i++)
  {
    if(!isdigit(number[i]))return false;
  }
  return true;
}
