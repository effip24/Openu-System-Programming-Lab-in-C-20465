/*
mmn14 - two pass assembler model based on imaginary computer
and imaginary assembly language.
input_parser.c - this file has all the functions that resposibles
to disect the lines of a file and check for erros in input.
@author - Ephraim Phil 205390156.
@version - 10/03/2020. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "data_storage.h"
#include "utilities.h"
#include "input_parser.h"


/*---------CHECKERs ----------------------------------------------------------*/
/*
this function gets a line(input) and checks if the first field in the line
is a symbol (also checks for erros).
@param input - the line to be cheked.
@param symbol - pointer to the symbols table.
@param fpass - boolean flag, indicates if it is the first parse.
@param fileName - the name of the current open file.
@param line - the current line number that was read from the file.
@param errors - pointer to erros(a flag that indicates if errors was found).
return true if a symbol was found.
*/
bool checkForSymbol(char input[],char *symbol,bool fpass,char fileName[],int line,bool *errors)
{
  /* indexes */
  int i;
  int j;
  /* will contain the lable name */
  char temp[128];
  /* array of strings, represents opCodes table */
  char opCodes[][5] = {"mov","cmp","add","sub","lea","clr","not", "inc",
  "dec","jmp","bne","red","prn","jsr","rts","stop"};

  /* array of strings, represents the registers of the cpu */
  char registers[][3] = {"r0","r1","r2","r3","r4","r5","r6","r7"};

  /* initializing temp and removing any white spaces and tabs from input */
  strcpy(temp,"");
  i = removeSpace(0,input);
  j=0;
  /* running through a loop until we get to ':' char, indicates we found a label*/
  while(i < strlen(input) &&input[i] != ':' && input[i] != ' ' && input[i] != '\t')
  {
    temp[j] = input[i];
    i++;
    j++;
  }
  temp[j] = '\0';

  /* if input[i] is ':' it means that there is a label */
  if(input[i] == ':')
  {
    /* we only want to print  errors during the first pass so we dont print
    the same error twice */
    /* checking for errors */

    /* error, a lable should be seperated by white space */
    if(input[i+1] != ' ' && input[i+1] != '\t')
    {

      /* checking if it is the first pass */
      if(fpass)
      printf("%s:%d:\033[1;31merror:\033[0m invalid label\n",fileName,line);
      *errors = 1;
      removeSymbol(input);
      return false;
    }
    /* error, a lable cannot be  cannot be an instruction name */
    for(i = 0; i < 16; i++)
    {
      if(!strcmp(temp,opCodes[i]))
      {
        if(fpass)
        printf("%s:%d:\033[1;31merror:\033[0m invalid label (cannot be an instruction name)\n",fileName,line);
        *errors = 1;
        removeSymbol(input);
        return false;
      }
      /* error, a lable cannot be  cannot be a register name */
      else if(i < 8 && !strcmp(temp,registers[i]))
      {
        if(fpass)
        printf("%s:%d:\033[1;31merror:\033[0m invalid label (cannot be a register name)\n",fileName,line);
        *errors = 1;
        removeSymbol(input);
        return false;
      }
    }
    /* error, empty line */
    if(temp[removeSpace(0,temp)] == 0)
    {
      if(fpass)
      printf("%s:%d:\033[1;31merror:\033[0m invalid label\n",fileName,line);
      *errors = 1;
      removeSymbol(input);
      return false;
    }
    /* error, non-alphabetic first character */
    if(isdigit(temp[0]))
    {
      if(fpass)
      printf("%s:%d:\033[1;31merror:\033[0m invalid label (non-alphabetic first character)\n",fileName,line);
      *errors = 1;
      removeSymbol(input);
      return false;
    }
    /* error, invalid label (too long) */
    else if(strlen(temp) > 31)
    {
      if(fpass)
      printf("%s:%d:\033[1;31merror:\033[0m invalid label (too long)\n",fileName,line);
      *errors = 1;
      removeSymbol(input);
      return false;
    }
    strcpy(symbol,temp);
    removeSymbol(input);
    return true;
  }
  else
  {
    /* running through a loop to check if the lable name is an instruction name of directive */
    for(i = 0; i < 16; i++)
    {
      if(!strcmp(temp,opCodes[i]) || !strcmp(temp,".entry") ||
      !strcmp(temp,".extern") || !strcmp(temp,".data") ||
      !strcmp(temp,".string"))return false;
    }
    if(fpass)
    {
      printf("%s:%d:\033[1;31merror:\033[0m invalid symbol (%s is not an instruction name and not a valid label)\n",fileName,line,temp);
      *errors = 1;
    }
  }
  return false;
}

/* this function gets a line(input) and checks if it is .data or .string directive.
(also checks for erros).
@param input - the line to be cheked.
@param dataOrString - a flag, will contain string if it is .string directive or
data if it is .data directive.
@param fpass - boolean flag, indicates if it is the first parse.
@param fileName - the name of the current open file.
@param line - the current line number that was read from the file.
@param errors - pointer to erros(a flag that indicates if errors was found).
return true if .data or .string directives was found.
*/
bool checkForDataOrString(char input[], char *dataOrString,bool fpass,char fileName[],int line,bool *errors)
{
  /* indexes */
  int i;
  int j;

  /* temp string will hold .string or .data */
  char temp[128];

  /* initializing temp and removing any white spaces and tabs from input */
  strcpy(temp,"");
  /* running through a loop until we get to '.' */
  for(i = 0; i < strlen(input); i++)
  {
    /* possibility of .string or .data directive */
    if(input[i] == '.')
    {
      i++;
      j=0;
      /* copies the directive name to temp */
      while(isalpha(input[i]))
      {
        temp[j] = input[i];
        i++;
        j++;
      }
      temp[j] = '\0';
      break;
    }
  }

  /* checking if is .string or .data directives */
  if(!strcmp(temp,"string") || !strcmp(temp,"data"))
  {
    /* copies temp to dataOrString flag */
    strcpy(dataOrString,temp);
    /* return true directive was found */
    return true;
  }
  /* case sensitivity of directive is an error */
  else if(!strcmp(temp,"DATA") || !strcmp(temp,"STRING"))
  {
    /* print the error messege only during the first pass */
    if(fpass)
    {
      printf("%s:%d:\033[1;31merror:\033[0m undefined directive (case sensitivity of directive name)\n",fileName,line );
    }
    *errors = 1;
    return false;
  }
  return false;
}

/* this function gets a line(input) and checks if it is .extern or .entry directive.
(also checks for erros).
@param input - the line to be cheked.
@param externOrEntry - a flag, will contain extern if it is .extern directive or
entry if it is .entry directive.
@param fpass - boolean flag, indicates if it is the first parse.
@param fileName - the name of the current open file.
@param line - the current line number that was read from the file.
@param errors - pointer to erros(a flag that indicates if errors was found).
return true if .extern or .entry directives was found.
*/
bool checkForExternOrEntry(char input[],char *externOrEntry,bool fpass,char fileName[],int line,bool *errors)
{
  /* indexes */
  int i;
  int j;
  /* temp string will hold .extern or .entry */
  char temp[128];

  strcpy(temp,"");
  /* running through a loop until we get to '.' */
  for(i = 0; i < strlen(input); i++)
  {
    /* possibility of .string or .data directive */
    if(input[i] == '.')
    {
      i++;
      j = 0;
      /* copies the directive name to temp */
      while(i < strlen(input) && input[i] != ' ' && input[i] != '\t')
      {
        temp[j] = input[i];
        i++;
        j++;
      }
      temp[j] = '\0';
      break;
    }
  }

  /* checks if the directive name is extern or entry and checks for erros */
  if(!strcmp(temp,"extern") || !strcmp(temp,"entry"))
  {
    /* copies the directive name to externOrEntry flag */
    strcpy(externOrEntry,temp);
    /* clearing temp */
    strcpy(temp,"");
    /* copies input to temp without the directive name to check for erros */
    strcpy(temp,input+removeSpace(i,input));
    /* checking the length of tem */
    if(strlen(temp) < 1)
    {
      /* print the error only during the first pass */
      if(fpass)
      {
        printf("%s:%d:\033[1;31merror:\033[0m missing argument in directive\n",fileName,line );
      }
      *errors = 1;
    }
    return true;
  }
  /* checking for case sensitivity error */
  else if(!strcmp(temp,"EXTERN") || !strcmp(temp,"ENTRY"))
  {
    if(fpass)
    {
      printf("%s:%d:\033[1;31merror:\033[0m undefined directive (case sensitivity of directive name)\n",fileName,line );
    }
    *errors = 1;
    return false;
  }

  return false;
}

/* this function gets a line(input) and checks if the sorting method of the source and the destination operands
are valid to the instruction.
@param opCode - number - represents the instruction name.
@param sourceCode - number - represents the sort method of the source operand.
@param source - the source operand.
@param dest - the destination operand.
@param fileName - the name of the current open file.
@param line - the current line number that was read from the file.
return true if the line is valid.
*/
bool checkSortingMethodValidity(int opCode,int sourceCode,int destinationCode,char source[],char dest[],char fileName[],int line)
{
  switch(opCode)
  {
    /* cmp instruction */
    /* all of the sorting method are valid for both the source and the destination operand */
    case CMP:
    {
      /* the instruction requiquires two operands, if one is missing it is an erros */
      if(sourceCode == UNDEFINED || destinationCode == UNDEFINED)
      {
        return false;
      }
      return true;
    }

    /* mov, add, sub instructions */
    /* these instructions requiquire two operands, if one is missing it is an erros */
    /* valid sorting method for source operand: 0,1,2,3 */
    /* valid sorting method for destination operand: 1,2,3 */
    case MOV:
    case ADD:
    case SUB:
    {
      /* checks if there was no source operand */
      if(sourceCode == MISSING)
      {
        printf("source: %d\n",sourceCode );
        printf("%s:%d:\033[1;31merror:\033[0m Missing operand \n",fileName,line );
        return false;
      }
      /* checks if there was no destination operand */
      else if(destinationCode == MISSING)
      {
        printf("%s:%d:\033[1;31merror:\033[0m Missing operand\n",fileName,line );
        return false;
      }
      /* immediate sort of the destination operand is not allowed in these instructions */
      if(destinationCode == IMMEDIATE_SORT)
      {
        printf("%s:%d:\033[1;31merror:\033[0m invalid target operand (immediate)\n",fileName,line );
        return false;
      }
      /* unkown destination operand */
      if(destinationCode == UNDEFINED)return false;
      break;
    }

    /* lea instruction */
    /* valid sorting method for source operand: 1 */
    /* valid sorting method for destination operand: 1,2,3 */
    case LEA:
    {
      /* checks if there was no source operand */
      if(sourceCode == MISSING)
      {
        printf("%s:%d:\033[1;31merror:\033[0m Missing operand\n",fileName,line );
        return false;
      }
      /* checks if there was no destination operand */
      else if(destinationCode == MISSING)
      {
        printf("%s:%d:\033[1;31merror:\033[0m Missing operand\n",fileName,line );
        return false;
      }
      if(sourceCode != DIRECT_SORT)
      {
        /* indirect register sort of the source operand is not allowed in these instructions */
        if(sourceCode == INDIRECT_REGISTER_SORT)
        {
          printf("%s:%d:\033[1;31merror:\033[0m invalid source operand (register indirect)\n",fileName,line);
          return false;
        }
        /* direct register sort of the source operand is not allowed in these instructions */
        else if(sourceCode == DIRECT_REGISTER_SORT)
        {
          printf("%s:%d:\033[1;31merror:\033[0m invalid source operand (register direct)\n",fileName,line);
          return false;
        }
        /* immediater sort of the source operand is not allowed in these instructions */
        else if(sourceCode == IMMEDIATE_SORT)
        {
          printf("%s:%d:\033[1;31merror:\033[0m invalid source operand (immediate)\n",fileName,line);
          return false;
        }
      }
      else
      {
        /* immediater sort of the destination operand is not allowed in these instructions */
        if(destinationCode == IMMEDIATE_SORT)
        {
          printf("%s:%d:\033[1;31merror:\033[0m invalid target operand (immediate)\n",fileName,line );
          return false;
        }
        if(destinationCode == UNDEFINED)return false;
      }
      break;
    }

    /* clr, not, inc, dec, red instructions */
    /* these instructions requiquire one operands, there is only destination operand */
    /* (source operand is the destination operand) */
    /* valid sorting method for source operand: no operand */
    /* valid sorting method for destination operand: 1,2,3 */
    case CLR:
    case NOT:
    case INC:
    case DEC:
    case RED:
    {
      /* if there is destination operand it means there is two operands, its an error */
      if(destinationCode != MISSING)
      {
        printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,dest );
        return false;
      }
      /* immediate  sort of the source(destination) operand is not allowed in these instructions */
      if(sourceCode == IMMEDIATE_SORT)
      {
        printf("%s:%d:\033[1;31merror:\033[0m invalid target operand (immediate)\n",fileName,line );
        return false;
      }
      /* if there is no operands */
      if(sourceCode == MISSING)
      {
        printf("%s:%d:\033[1;31merror:\033[0m Missing operand\n",fileName,line );
        return false;
      }
      if(sourceCode == UNDEFINED)return false;
      break;
    }

    /* jmp, bne, jsr instructions */
    /* these instructions requiquire one operands, there is only destination operand */
    /* (source operand is the destination operand) */
    /* valid sorting method for source operand: no operand */
    /* valid sorting method for destination operand: 1,2 */
    case JMP:
    case BNE:
    case JSR:
    {
      /* if there is destination operand it means there is two operands, its an error */
      if(destinationCode != MISSING)
      {
        printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,dest );
        return false;
      }
      /* immediate  sort of the source(destination) operand is not allowed in these instructions */
      /* direct register sort of the source(destination) operand is not allowed in these instructions */
      if(sourceCode == IMMEDIATE_SORT || sourceCode == DIRECT_REGISTER_SORT)
      {
        printf("%s:%d:\033[1;31merror:\033[0m invalid target operand (immediate)\n",fileName,line );
        return false;
      }
      if(sourceCode == UNDEFINED)return false;
    }

    /* prn instructions */
    /* these instruction requiquires one operands, there is only destination operand */
    /* (source operand is the destination operand) */
    /* valid sorting method for source operand: no operand */
    /* valid sorting method for destination operand: 0,1,2,3 */
    case PRN:
    {
      /* if there is destination operand it means there is two operands, its an error */
      if(destinationCode != MISSING)
      {
        printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,dest );
        return false;
      }
      break;
    }

    /* rts,stop instructions */
    /* these instructions has no operand */
    /* valid sorting method for source operand: no operand */
    /* valid sorting method for destination operand: no operand */
    case RTS:
    case STOP:
    {
      /* check if there is source operand */
      if(sourceCode != MISSING)
      {
        printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,source );
        return false;
      }
      /* check if there is destination operand */
      if(destinationCode != MISSING)
      {
        printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,dest );
        return false;
      }
      break;
    }
  }
  return true;
}


/*---------GETERs ------------------------------------------------------------*/
/*
this function gets a line(input) and returns the opCode of the instruction name
and copies the opcode in binary to a variable.
@param input - the line to be cheked.
@param binaryOpCode - binary number - saves the opCode in binary.
return the opCode of the instruction name.
*/
int getOpCode(char input[],char *binaryOpCode)
{
  /* indexes */
  int i;
  int j;
  char * binaryOp;
  char temp[128];
  /* array of strings, represents opCodes table */
  char opCodes[][5] = {"mov","cmp","add","sub","lea","clr","not", "inc",
  "dec","jmp","bne","red","prn","jsr","rts","stop"};

  strcpy(temp,"");
  i = removeSpace(0,input);
  j=0;

  /* running a loop that copies the instruction name to temp */
  while(i < strlen(input) && input[i] != ' ' && input[i] != '\t')
  {
    temp[j] = input[i];
    i++;
    j++;
  }
  temp[j] = '\0';

  /* running a loop through the opCode array. i represent the opCode number */
  for(i = 0; i < 16; i++)
  {
    if(strcmp(temp,opCodes[i]) == 0)
    {
      /* removing the instruction name from input */
      removeOpCode(input);
      /* converting the opCode to binary */
      binaryOp = toBInary(i);
      strcpy(binaryOpCode,binaryOp);
      free(binaryOp);
      return i;
    }
  }
  /* if theere is no instruction */
  removeOpCode(input);
  return -1;
}

/* this function gets a line(input) and extracts the operands(source and destination).
@param symbols - pointer to the symbols table.
@param input - the line to be cheked.
@param L - contains the number of words to be coded(indicated if we need one operands or two).
@param sourceOperand - the name of the source operand will be saved in this variable.
@param destinationOperand - the name of the destination operand will be saved in this variable.
@param sourceCode - contains a number that represents the sorting method of the source operand.
@param destinationCode - contains a number that represents the sorting method of the destination operand.
@param opCode - contains a number that represents an instruction name.
@param fpass - boolean flag, indicates if it is the first parse.
@param fileName - the name of the current open file.
@param line - the current line number that was read from the file.
return true if no errors were found.
*/
bool getOperands(ptrSYMBOLS *symbols,char input[],int L,char *sourceOperand,char *destinationOperand, int *sourceCode, int* destinationCode,int opCode,bool fpass,char fileName[],int line)
{
  /* indexes */
  int i;
  int j;

  /* contains the source and the destination names */
  char source[128];
  char dest[128];
  /* error flag */
  bool errors;

  i = removeSpace(0,input);
  j = 0;
  /* this loop extracts the source name */
  while(i < strlen(input) && (input[i] != ',' && input[i] != ' ' &&
  input[i] != '\t'))
  {
    source[j] = input[i];
    i++;
    j++;
  }
  source[j] = '\0';

  i = removeSpace(i,input);

  i = removeSpace(i+1,input);
  j = 0;

  /* if L is 3 it means we need two operands */
  if( L == 3)
  {
    /* extracts the destination name */
    while(i < strlen(input))
    {
      dest[j] = input[i];
      i++;
      j++;
    }
    dest[j] = '\0';
  }
  else
  {
    dest[0] = '\0';
  }

  /*calling getSource function, this function checks for errors */
  getSource(symbols,source,sourceOperand,sourceCode,fpass,fileName,line);
  /*calling getDestination function, this function checks for errors */
  getDestination(symbols,dest,destinationOperand,destinationCode,fpass,fileName,line);

  /* calling checkSortingMethodValidity function to check for errors */
  errors = checkSortingMethodValidity(opCode,*sourceCode,*destinationCode,source,dest,fileName,line);

  return errors;
}

/* this function gets a source operand, checks for errors and sets sourceOperand to the binary representation of its sorting method.
@param symbols - pointer to the symbols table.
@param source - the name of the source operand.
@param sourceOperand - the name of the source operand will be saved in this variable(in binary).
@param sourceCode - contains a number that represents the sorting method of the source operand.
@param fpass - boolean flag, indicates if it is the first parse.
@param fileName - the name of the current open file.
@param line - the current line number that was read from the file.
*/
void getSource(ptrSYMBOLS *symbols,char source[],char *sourceOperand,int *sourceCode,bool fpass,char fileName[],int line)
{
  /* indexes */
  int i;
  int j;
  /* will hold the number of the immediate sort */
  char number[128];
  /* will contain the binary representation of each sort  */
  char * tobinary;

  switch(source[0])
  {
    /* if the source operand is immediate sort */
    case '#':
    {
      /* indexes */
      i=1;
      j=0;
      /* extracts the number */
      while(i < strlen(source))
      {
        if(isdigit(source[i]) || (source[i] =='-' && isdigit(source[i+1])))
        {
          number[j] = source[i];
          i++;
          j++;
          while(isdigit(source[i]))
          {
            number[j] = source[i];
            i++;
            j++;
          }
          number[j] = '\0';
          break;
        }
        else
        {
          /* error, the value afte '#' is not a number, print error only during the first pass */
          if(fpass)
          {
            printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,source+1);
          }
          return;
        }
        i++;
      }
      /*checks  if the extracted number has a charachter or it is not a number */
      if(!isnumber(number))
      {
        /* print error only during the first pass */
        if(fpass)
        printf("%s:%d:\033[1;31merror:\033[0m invalid operand (incomplete immediate #)\n",fileName,line);
        *sourceCode = UNDEFINED;
        break;
      }
      /* cheks the range of the number */
      if(atoi(number) <= 2047 && atoi(number) >= -2047)
      {
        /* converts the number to binary */
        tobinary = toBInary(atoi(number));
        /* copies the binary number to sourceOperand */
        strcpy(sourceOperand,tobinary);
        free(tobinary);
        *sourceCode = IMMEDIATE_SORT;
        break;
      }
      else
      {
        /* the number is out of range,  print error only during the first pass */
        if(fpass)
        {
          /* negetive number */
          if(atoi(number) > 0)
          {
            printf("%s:%d:\033[1;31merror:\033[0m invalid first operand (positive immediate too high to fit in 12 bits)\n",fileName,line);
          }
          else
          {
            printf("%s:%d:\033[1;31merror:\033[0m invalid first operand (negetive immediate too high to fit in 12 bits)\n",fileName,line);
          }
          *sourceCode = UNDEFINED;
          break;
        }
      }

    }
    /* if the source operand is indirect register sort */
    case '*':
    {
      /* the length of the operand has to be 3, if not its not valid*/
      if(strlen(source) > 3)
      {
        /* print error in the first pass only */
        if(fpass)
        printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,source+1);
        *sourceCode = UNDEFINED;
        break;
      }
      /* checks if the operand has space or tab in the name */
      else if(source[1] == ' ' || source[1] == '\t' || source[1] == 0)
      {
        /* print error in the first pass only */
        if(fpass)
        printf("%s:%d:\033[1;31merror:\033[0m invalid operand (incomplete indirect *)\n",fileName,line);
        *sourceCode = UNDEFINED;
        break;
      }
      /* the name of the operand has to be *r[0-9] */
      else if(source[1] != 'r')
      {
        /* print error in the first pass only */
        if(fpass)
        printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,source+1);
        *sourceCode = UNDEFINED;
        break;
      }
      /* invalid register number */
      else if(!isdigit(source[2]))
      {
        /* print error in the first pass only */
        if(fpass)
        printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,source+1);
        *sourceCode = UNDEFINED;
        break;
      }
      /* if the register number is out of range, only allowe 0-7 */
      else if( (source[2] - '0') < 0 || (source[2] -'0') > 7)
      {
        /* print error in the first pass only */
        if(fpass)
        printf("%s:%d:\033[1;31merror:\033[0m register number is out of range (%s)\n",fileName,line,source);
        *sourceCode = UNDEFINED;
        break;
      }
      /*no errors were found, converts the register number to binary */
      tobinary = toBInary(source[2]-'0');
      strcpy(sourceOperand,tobinary);
      free(tobinary);
      *sourceCode = INDIRECT_REGISTER_SORT;
      break;
    }
    /* if the source operand is direct register sort */
    case 'r':
    {
      /* the length of the operand has to be 2, if not its not valid */
      if(strlen(source) > 2)
      {
        /* print error in the first pass only */
        if(fpass)
        printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,source);
        *sourceCode = UNDEFINED;
        break;
      }

      /* invalid source name */
      if(!isdigit(source[1]))
      {
        /* print error in the first pass only */
        if(fpass)
        printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,source);
        *sourceCode = UNDEFINED;
        break;
      }

      /* if the register number is out of range, only allowe 0-7 */
      else if((source[1]-'0') < 0 || (source[1]-'0') > 7 )
      {
        /* print error in the first pass only */
        if(fpass)
        printf("%s:%d:\033[1;31merror:\033[0m register number is out of range (%s)\n",fileName,line,source);
        *sourceCode = UNDEFINED;
        break;
      }

      /* no errors were found converting the sort method to binary */
      tobinary = toBInary(source[1]-'0');
      strcpy(sourceOperand,tobinary);
      free(tobinary);
      *sourceCode = DIRECT_REGISTER_SORT;
      break;
    }
    /* checking for erro or it is direct sort */
    default:
    {
      /* if it the second pass */
      if(!fpass)
      {
        /* checking if the symbol is not ignored */
        if(isalpha(source[0]) && !checkIfSymbolValid(symbols,source))
        {
          printf("%s:%d:\033[1;31merror:\033[0m undefined label as first operand (label %s is ignored)\n",fileName,line,source);
          *sourceCode = MISSING;
          break;
        }
        /* if the symbol is not in the symbols table */
        else if(isalpha(source[0]) && notInSymbols(symbols,source))
        {
          printf("%s:%d:\033[1;31merror:\033[0m undefiend lable as first operand\n",fileName,line);
          *sourceCode = MISSING;
          break;
        }
        /* it is not a lable or it is NULL */
        else if(!isalpha(source[0]) && source[0] != 0)
        {
          printf("%s:%d:\033[1;31merror:\033[0m invalid operand (undefined addressing mode)\n",fileName,line);
          *sourceCode = MISSING;
          break;
        }
        /* no errors were foun in the second pass, setting sourceCode to positive number (10 is random)*/
        *sourceCode = 10;
        break;
      }

      /*sourceOperand will hold the lable name (if it is a lable)*/
      strcpy(sourceOperand,source);
      /* there is no operands */
      if(strlen(source) == 0)
      {
        *sourceCode = MISSING;
        break;
      }
      /* valid lable */
      else if(isalpha(source[0]))
      {
        *sourceCode = DIRECT_SORT;
        break;
      }
      /* undefined operand */
      *sourceCode = UNDEFINED;
      break;
    }
  }
}

/* this function gets a destination operand, checks for errors and sets destinationOperand to the binary representation of its sorting method.
@param symbols - pointer to the symbols table.
@param dest - the name of the destination operand.
@param destinationOperand - the name of the destination operand will be saved in this variable(in binary).
@param destinationCode - contains a number that represents the sorting method of the destinationCode operand.
@param fpass - boolean flag, indicates if it is the first parse.
@param fileName - the name of the current open file.
@param line - the current line number that was read from the file.
*/
void getDestination(ptrSYMBOLS *symbols,char dest[],char *destinationOperand,int *destinationCode,bool fpass,char fileName[],int line)
{
  /* indexes */
  int j;
  int i;
  /* will hold the number of the immediate sort */
  char number[128];
  /* will contain the binary representation of each sort  method */
  char * tobinary;

  switch(dest[0])
  {
    /* if the destination operand is immediate sort */
    case '#':
    {
      /* indexes */
      i=1;
      j=0;
      /* extracts the number */
      while(i < strlen(dest))
      {
        if(isdigit(dest[i]) || (dest[i] =='-' && isdigit(dest[i+1])))
        {
          number[j] = dest[i];
          i++;
          j++;
          while(isdigit(dest[i]))
          {
            number[j] = dest[i];
            i++;
            j++;
          }
          number[j] = '\0';
          break;
        }
        else
        {
          /* error, the value afte '#' is not a number, print error only during the first pass */
          if(fpass)
          {
            printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,dest+1);
          }
          return;
        }
        i++;
      }
      /*checks  if the extracted number has a charachter or it is not a number */
      if(!isnumber(number))
      {
        /* print error only during the first pass */
        if(fpass)
        printf("%s:%d:\033[1;31merror:\033[0m invalid operand (incomplete immediate #)\n",fileName,line);
        *destinationCode = UNDEFINED;
        break;
      }
      /* cheks the range of the number */
      if(atoi(number) <= 2047 && atoi(number) >= -2047)
      {
        /* converts the number to binary */
        tobinary = toBInary(atoi(number));
        /* copies the binary number to destinationOperand */
        strcpy(destinationOperand,tobinary);
        free(tobinary);
        *destinationCode = IMMEDIATE_SORT;
        break;
      }
      else
      {
        /* the number is out of range,  print error only during the first pass */
        if(fpass)
        {
          /* negetive number */
          if(atoi(number) > 0)
          {
            printf("%s:%d:\033[1;31merror:\033[0m invalid second operand (positive immediate too high to fit in 12 bits)\n",fileName,line);
          }
          else
          {
            printf("%s:%d:\033[1;31merror:\033[0m invalid second operand (negetive immediate too high to fit in 12 bits)\n",fileName,line);
          }
          *destinationCode = UNDEFINED;
          break;
        }
      }
    }
    case '*':
    {
      /* the length of the operand has to be 3, if not its not valid*/
      if(strlen(dest) > 3)
      {
        /* print error in the first pass only */
        if(fpass)
        printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,dest+1);
        *destinationCode = UNDEFINED;
        break;
      }
      /* checks if the operand has space or tab in the name */
      else if(dest[1] == ' ' || dest[1] == '\t' || dest[1] == 0)
      {
        /* print error in the first pass only */
        if(fpass)
        printf("%s:%d:\033[1;31merror:\033[0m invalid operand (incomplete indirect *)\n",fileName,line);
        *destinationCode = UNDEFINED;
        break;
      }
      /* the name of the operand has to be *r[0-9] */
      else if(dest[1] != 'r')
      {
        /* print error in the first pass only */
        if(fpass)
        printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,dest+1);
        *destinationCode = UNDEFINED;
        break;
      }
      /* invalid register number */
      else if(!isdigit(dest[2]))
      {
        /* print error in the first pass only */
        if(fpass)
        printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,dest+1);
        *destinationCode = UNDEFINED;
        break;
      }
      /* if the register number is out of range, only allowe 0-7 */
      else if( (dest[2] - '0') < 0 || (dest[2] -'0') > 7)
      {
        /* print error in the first pass only */
        if(fpass)
        printf("%s:%d:\033[1;31merror:\033[0m register number is out of range (%s)\n",fileName,line,dest);
        *destinationCode = UNDEFINED;
        break;
      }
      /*no errors were found, converts the register number to binary */
      tobinary = toBInary(dest[2]-'0');
      strcpy(destinationOperand,tobinary);
      free(tobinary);
      *destinationCode = INDIRECT_REGISTER_SORT;
      break;
    }
    /* if the destination operand is direct register sort */
    case 'r':
    {
      /* the length of the operand has to be 2, if not its not valid */
      if(strlen(dest) > 2)
      {
        if(fpass)
        printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,dest);
        *destinationCode = UNDEFINED;
        break;
      }

      /* invalid source name */
      if(!isdigit(dest[1]))
      {
        /* print error in the first pass only */
        if(fpass)
        printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,dest);
        *destinationCode = UNDEFINED;
        break;
      }

      /* if the register number is out of range, only allowe 0-7 */
      else if((dest[1]-'0') < 0 || (dest[1]-'0') > 7 )
      {
        /* print error in the first pass only */
        if(fpass)
        printf("%s:%d:\033[1;31merror:\033[0m register number is out of range (%s)\n",fileName,line,dest);
        *destinationCode = UNDEFINED;
        break;
      }

      /* no errors were found converting the sort method to binary */
      tobinary = toBInary(dest[1]-'0');
      strcpy(destinationOperand,tobinary);
      free(tobinary);
      *destinationCode = DIRECT_REGISTER_SORT;
      break;
    }

    /* checking for erro or it is direct sort */
    default:
    {
      /* if it the second pass */
      if(!fpass)
      {
        /* checking if the symbol is ignored */
        if(isalpha(dest[0]) && !checkIfSymbolValid(symbols,dest))
        {
          printf("%s:%d:\033[1;31merror:\033[0m undefined label as first operand (label %s is ignored)\n",fileName,line,dest);
          *destinationCode = MISSING;
          break;
        }
        /* if the symbol is not in the symbols table */
        else if(isalpha(dest[0]) && notInSymbols(symbols,dest))
        {
          printf("%s:%d:\033[1;31merror:\033[0m undefiend lable as second operand\n",fileName,line);
          *destinationCode = MISSING;
          break;
        }
        /* it is not a lable or it is NULL */
        else if(!isalpha(dest[0]) && dest[0] != 0)
        {
          printf("%s:%d:\033[1;31merror:\033[0m invalid operand (undefined addressing mode)\n",fileName,line);
          *destinationCode = MISSING;
          break;
        }
        /* no errors were foun in the second pass, setting destinationCode to positive number (10 is random)*/
        *destinationCode = 10;
        break;
      }

      /*destinationOperand will hold the lable name (if it is a lable)*/
      strcpy(destinationOperand,dest);

      /* there is no destination operand */
      if(strlen(dest) == 0)
      {
        *destinationCode = MISSING;
        break;
      }
      /* valid lable */
      else if(isalpha(dest[0]))
      {
        *destinationCode = DIRECT_SORT;
        break;
      }
      /* undefined operand */
      *destinationCode = UNDEFINED;
      if(fpass)
      printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line, dest);
      break;
    }
  }
}

/* this function gets a line(input), and extracts the operand of extern or entry directive, it saves it in operand.
@param input - the line to be cheked.
@param operand - dynamic allocated variable contains the operand of .extern/.entry directives.
*/
void getExternOrEntryOperand(char input[], char *operand)
{
  /* index */
  int i;

  i=0;
  i = removeSpace(i,input);
  /* extracts the operand name */
  while(i < strlen(input))
  {
    if(input[i] == ' ' || input[i] == '\t')
    {
      break;
    }
    i++;
  }
  i = removeSpace(i,input);

  strcpy(operand,input+i);
}


/*--------REMOVERs -----------------------------------------------------------*/
/*
this function removes a symbol from a line(input).
@param input - the line to be cheked.
*/
void removeSymbol(char input[])
{
  /* index */
  int i;
  /* temp will hold input value */
  char temp[128];

  i = 0;
  /* at the end of the loop i index will point at the end of the lable */
  while(i < strlen(input))
  {
    if(input[i] == ':')
    {
      break;
    }
    i++;
  }

  i++;
  /* empty input */
  strcpy(temp,input);
  strcpy(input,"");
  /* copies temp to input without the lable name */
  strcpy(input,temp+i);

}

/* this function removes the instruction name from a line(input).
@param input - the line to be cheked.
*/
void removeOpCode(char input[])
{
  /* index */
  int i;
  /* temp will hold input value */
  char temp[128];

  strcpy(temp,"");
  /* removing spaces and tabs */
  i=removeSpace(0,input);
  /* at the end of the loop i will point at the source operand */
  while(i < strlen(input) && input[i] != ' ')
  {
    i++;
  }
  i = removeSpace(i,input);

  /* copies input to temp */
  strcpy(temp,input);
  /* empty input */
  strcpy(input,"");
  /* copies temp to input without the instruction name */
  strcpy(input,temp+i);
}

/* this function removes an empty line(if the line contains only spaces or tabs).
@param input - the line to be cheked.
*/
void removeEmptyLine(char input[])
{
  /* indes */
  int i;
  /* flag indicates if the line is empty */
  bool empty = true;

  /* running in a loop to check if the line is empty or not */
  for(i = 0; i < strlen(input); i++)
  {
    /* a charachter was found, the line is not empty, seting the empty flag to false */
    if(input[i] != ' ' && input[i] != '\t')
    {
      empty = false;
    }
  }

  /* checks if the flag is true, so the line is empty */
  if(empty)
  {
    strcpy(input,"");
  }
}

/* this function removes the '\n' char from a line.
@param input - the line to be cheked.
*/
void removeNewLine(char input[])
{
  /* index */
  int i;

  for(i = 0; i < strlen(input); i++)
  {
    /* if '\n' was found */
    if(input[i] == '\n')
    {
      input[i] = '\0';
      break;
    }
  }
}

/* this function removes comments from a line.
@param input - the line to be cheked.
*/
void removeComments(char input[])
{
  /* index */
  int i;
  /* flag, indicate if the line is .string directive */
  char *dataOrString;
  bool errors = false;

  /* memory allocation of the flag */
  dataOrString = (char *)malloc(sizeof(char)*8);
  if(!dataOrString)
  {
    puts("memory allocation failed");
    exit(1);
  }

  /* first we check if the line has .string directive, we need to check for few things:
  1. if the comment char ';' is in the string we dont delete it from the line.
  2. if the comment char ';' is at the end of the string, we remove it so addData function will not report an error
  */
  /* checks if the line has .string directive */
  if(checkForDataOrString(input,dataOrString,false,"",0,&errors))
  {

    if(!strcmp(dataOrString,"string"))
    {
      /* removing white spaces and tabs and forwarding i to point at the string of the directive */
      i = removeSpace(0,input);
      while(i < strlen(input) && input[i] != ' ' && input[i] != '\t')i++;
      i = removeSpace(i,input);
      while(i < strlen(input) && input[i] != ' ' && input[i] != '\t')i++;

      if(input[i] ==' '|| input[i] == '\t')i++;

      /* if the string doesnt start with " char we stop her, addData function will handle .string directive errors "*/
      if(input[i] != '\"')return;

      i++;

      /* running to the closing " char of the string */
      while(i < strlen(input))
      {
        if(input[i] == '\"')break;
        i++;
      }

      /* if the string doesnt ends with " char we stop her, addData function will handle .string directive errors "*/
      if(input[i] != '\"')return;

      /* if the line ends after the string end it means there is no comment to remove */
      if( i == strlen(input))return;

      i++;
      /* searching for the comment char after the string ends */
      while(i < strlen(input))
      {
        if(input[i] == ';')
        {
          input[i] = '\0';
          break;
        }
        i++;
      }

    }
    free(dataOrString);
    return;
  }
  /* there is string to check, freeing dataOrString flag */
  else
  {
    free(dataOrString);
  }

  for(i= removeSpace(0,input); i < strlen(input); i++)
  {
    /* the ';' indicated that a comment is open */
    if(input[i] == ';')
    {
      /* removing the comment by setting '\0' insted of ';' char */
      input[i] = '\0';
      break;
    }
  }
}

/* this funtions removes white space or tabs from a line (input).
@param i - the current index of white space or tab in the line.
@param input - the line to be cheked.
when the funtion encounter a charachter which is not white space or tab,
the function stops and return the index of that charachter. */
int removeSpace(int i,char input[])
{
  /* this loop runs as long as the current char is white space or tab. */
  while(input[i] == ' ' || input[i] == '\t')
  {
    i++;
  }
  /* the index of the first char which is not tab or a space. */
  return i;
}
