/*
mmn22 - interactive program to perform Algebra set calculations.
myset.c - this file has the main function and all the operations
on the user input such as checking for errors, checking for
commands, sets and parameters from user input.
@author - Ephraim Phil 205390156.
@version - 10/1/2020. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "myset.h"
#include "set.h"

int main()
{
  /* user input */
  static char input[MAX_INPUT];
  /* holds the current command that was entered by the user. */
  int state;
  int i;
  /* holds the scanf return value to check if EOF was reached. */
  int scan;

  /* setting 6 sets from set type. */
  set SETA, SETB, SETC, SETD, SETE, SETF;
  /* array of pointers to hold a pointer to each SET. */
  char ** setsArray;
  /* memory allocation for setsArray. */
  setsArray = (char **)malloc(sizeof(set) *6);

  /* SETS INITIALIZING   ---------------------------------------------------*/
  for(i = 0; i < 7 ; i++)
  {
    calibrateSet(SETA);
    calibrateSet(SETB);
    calibrateSet(SETC);
    calibrateSet(SETD);
    calibrateSet(SETE);
    calibrateSet(SETF);
  }
  /* END OF INITIALIZING  --------------------------------------------------*/

  /* initializing setsArray ----------------------------------------------- */

  /* setsArray[0] will hold a pointer to SETA */
  setsArray[SET_A] = SETA;
  /* setsArray[1] will hold a pointer to SETB */
  setsArray[SET_B] = SETB;
  /* setsArray[2] will hold a pointer to SETC */
  setsArray[SET_C] = SETC;
  /* setsArray[3] will hold a pointer to SETD */
  setsArray[SET_D] = SETD;
  /* setsArray[4] will hold a pointer to SETE */
  setsArray[SET_E] = SETE;
  /* setsArray[5] will hold a pointer to SETF */
  setsArray[SET_F] = SETF;

  /*EOF INITIALIZING        ------------------------------------------------*/

  printMssg();
  printf("command> " );

  /* scaning for user input until user enter stop command */
  while( (scan = scanf(" %[^\n]s",input)) && (strcmp(input,"stop") != 0) )
  {
    printf("command> %s\n",input );
    if(scan != EOF)
    {
      /* detecting the current command from user input and send it to the right
      function. */
      state = currState(input);
      switch(state)
      {
        case READ_SET:
        {
          checkReadSetInput(input,setsArray);
          break;
        }
        case PRINT_SET:
        {
          checkPrintSetInput(input,setsArray);
          break;
        }
        case UNION_SET:
        {
          checkUnionIntersectSubSymdiffINput(UNION_SET,input,setsArray);
          break;
        }
        case INTERSECT_SET:
        {
          checkUnionIntersectSubSymdiffINput(INTERSECT_SET,input,setsArray);
          break;
        }
        case SUB_SET:
        {
          checkUnionIntersectSubSymdiffINput(SUB_SET,input,setsArray);
          break;
        }
        case SYMDIFF_SET:
        {
          checkUnionIntersectSubSymdiffINput(SYMDIFF_SET,input,setsArray);
          break;
        }
        default:
        {
          break;
        }
      }
      puts("" );
      printf("command> " );
    }
    else
    {
      puts("");
      puts(" Reached EOF stopping the program");
      break;
    }
    fflush(stdin);
  }
  /* before the program ends we free the array of pointers of SETs. */
  free(setsArray);
  return 0;
}

/* detects command name from user input.
after the function detects the command name it substring the user input
so the input doesnt contain the command name any more.
@param input - the user input.
@return -1 if it did not detect a command and returns value from 0-5 if
a command detected(any value represents a command). */
int currState(char input[])
{
  int i = 0;
  int j = 0;
  char help[MAX_INPUT];
  char s[MAX_INPUT];
  char tempInput[MAX_INPUT];

  /* this if stateme checks if the user inserted --help for instructions */
  i = removeSpace(i,input);
  if(input[i] =='-' && input[i+1] == '-')
  {

    help[i] = input[i];
    help[i+1] = input[i+1];
    i = i+2;
    j = i;

    while(i < strlen(input) && isalpha(input[i]))
    {
      help[j] = input[i];
      i++;
      j++;
    }
    help[j] = '\0';

    if(strcmp("--help",help) == 0)
    {
      printManual();
      return -1;
    }
  }

  i=0;
  /* this loops go thrugh the user input to detect a command name */
  while (i < strlen(input) && (input[i] != '_' ))
  {
    s[i] = input[i];
    i++;
  }

  s[i] = input[i];
  i++;

  while ( i < strlen(input) && isalpha(input[i]) )
  {
    s[i] = input[i];
    i++;
  }
  s[i] = '\0';

  /* after a command was detected we delete it from user input */
  strcpy(tempInput,input+i);
  strcpy(input,"");
  strcpy(input,tempInput);

  i = 0;
  i = removeSpace(i,input);
  if(input[i] == ',')
  {
    puts(" Illegal comma");
    return -1;
  }
  if (strcmp(s,"read_set") == 0)
  {
    return READ_SET;
  }
  else if(strcmp(s,"print_set") == 0)
  {
    return PRINT_SET;
  }
  else if(strcmp(s,"union_set") == 0)
  {
    return UNION_SET;
  }
  else if(strcmp(s,"intersect_set") == 0)
  {
    return INTERSECT_SET;
  }
  else if(strcmp(s,"sub_set") == 0)
  {
    return SUB_SET;
  }
  else if(strcmp(s,"symdiff_set") == 0)
  {
    return SYMDIFF_SET;
  }
  else
  {
    puts(" Undefined command name");
    return -1;
  }
}

/* detects a set from user input.
after the function detects a set it substring the user input so the input
does not contain the set name any more.
@param input - the user input.
@return -1 if did not detect a set name and a value from 0-5 if a set
was detected (each value represents a set name). */
int detectSet(int cmd,char input[])
{
  int i = 0;
  int j =0;
  char setName[MAX_INPUT];
  char tempInput[MAX_INPUT];

  if(strlen(input) == 0 )
  {
    if(cmd == UNION_SET)
    {
      return -1;
    }
  }

  /* this loop goes throu the user input to detect the set name */
  while(i < strlen(input))
  {

    if(isalpha(input[i]))
    {
      j=0;
      /* runing and saving current char from user input to temp char array (setName)
      as long as the current char is a letter. */
      while(i < strlen(input) && isalpha(input[i]) )
      {
        setName[j] = input[i];
        i++;
        j++;
      }
      setName[j] = '\0';

      /* checking if the temp char array(setName) is equals to one of the possible SETs
      if a SET name was found we substring user input that the SET name gets
      removed from it. */
      if(input[i] != ' ' && input[i] != ',' && input[i] != '\t' && input[i] != 0)
      {
        puts(" Undefined set name ");
        return -1;
      }
      else if (strcmp(setName,"SETA") == 0 )
      {
        strcpy(tempInput,input+i);
        strcpy(input,"");
        strcpy(input,tempInput);
        return SET_A;
      }

      else if (strcmp(setName,"SETB") == 0)
      {
        strcpy(tempInput,input+i);
        strcpy(input,"");
        strcpy(input,tempInput);
        return SET_B;
      }

      else if (strcmp(setName,"SETC") == 0)
      {
        strcpy(tempInput,input+i);
        strcpy(input,"");
        strcpy(input,tempInput);
        return SET_C;
      }

      else if (strcmp(setName,"SETD") == 0)
      {
        strcpy(tempInput,input+i);
        strcpy(input,"");
        strcpy(input,tempInput);
        return SET_D;
      }

      else if (strcmp(setName,"SETE") == 0)
      {
        strcpy(tempInput,input+i);
        strcpy(input,"");
        strcpy(input,tempInput);
        return SET_E;
      }
      else if (strcmp(setName,"SETF") == 0)
      {
        strcpy(tempInput,input+i);
        strcpy(input,"");
        strcpy(input,tempInput);
        return SET_F;
      }
      else
      {
        puts(" Undefined set name ");
        return -1;
      }
    }
    i++;
  }
  puts(" Undefined set name ");
  return -1;
}

/* this function checks for errors in user input when read_set command
was inserted by user.
if no error detected the function calls readSet() function from set.c
@param input - the user input.
@param setsArray - array of pointers to each set. */
void checkReadSetInput(char input[] ,char** setsArray)
{
  /*contain an INteger that represents a set name to be sent ro readSet() function. */
  int set = detectSet(READ_SET,input);
  /* temp array that will hold all the numbers from user input until */
  int * numbersArray = (int *)malloc(1*sizeof(int));
  int * q;
  int numOfNUmbers = 0;
  int i = 0;
  int j =0;

  /* a temporary array that contain digits from user input untill it gets converted to integer. */
  char num[MAX_INPUT];
  char tempInput[MAX_INPUT];
  i = removeSpace(i,input);

  if(!numbersArray)
  {
    puts("falied to allocate memory ");
    return;
  }

  /* checking if no set was given by the user */
  if(set == -1)
  {
    return;
  }

  /* checking for missing comma because the set name should be seperate by comma */
  if(input[i] != ',')
  {
    puts(" Missing comma (type --help for instrauctions).");
    return;
  }
  /*if no comma was missing we delete the current comma from the input */
  strcpy(tempInput,input+i+1);
  strcpy(input,"");
  strcpy(input,tempInput);

  /* running through the input to check for errors and for a given numbers to
  insert to the given set. */
  for(i = 0; i <strlen(input); i++)
  {
    /* first we remove any white space or tabs */
    if(input[i] == ' ' || input[i] =='\t')
    {
      i = removeSpace(i,input);
    }

    /* we already checked for the comma so if a comma was found again its an error. */
    if(input[i] == ',')
    {
      puts(" Multiple consecutive commas (type --help for instrauctions).");
      return;
    }
    /* if the char '-' was found it means we need to check for the ending sign -1 */
    else if(input[i] == '-')
    {

      /* checking for the ending sign -1 */
      if((i+1) <= strlen(input) && isdigit(input[i+1]))
      {
        if((i+2) <= strlen(input) && (input[i+1] - '0') == 1 && ( input[i+2] == 0 || input[i+2] == ' ' || input[i+2] == '\t') )
        {
          i = removeSpace(i+2,input);
          if(input[i] == 0 )
          {
            /* after the ending sign -1 was found we calibrate the SET and insert
            all the numbers from the temp array to the SET using readSet() */
            calibrateSet(setsArray[set]);
            for(i = 0; i < numOfNUmbers; i++)
            {
              readSet(numbersArray[i],setsArray[set]);
            }
            free(numbersArray);
            return;
          }
        }
      }

      /*if the ending sign -1 was not found we check for two errors, out of range
      or not an integer.*/
      i++;
      if( isdigit(input[i]) )
      {
        while(isdigit(input[i]))
        {
          i++;
        }

        i = removeSpace(i,input);
        if(input[i] == 0 || input[i] == ',')
        {
          puts(" Invalid set member – value out of range (type --help for instrauctions).");
          return ;
        }
        else
        {
          puts(" Invalid set member – not an integer (type --help for instrauctions).");
          return;
        }
      }
    }
    /* now we check if the current char is a digint from 0-9 */
    else if(isdigit(input[i]))
    {
      j = 0;
      /* running through a loop as long as the current char is a digit and save the chars
      to a temp array which represent an integer. */
      while(isdigit(input[i]))
      {
        num[j] = input[i];
        i++;
        j++;
      }
      num[j] = '\0';

      /* now we convert the temp array to integer using toInt() function and check if the value
      is within the range of 0-127 and then we insert the number to a tempArray */
      if(toInt(num) >= 0 && toInt(num) <= 127)
      {
        if(notInSet(toInt(num),setsArray[set]));
        {
          numbersArray[numOfNUmbers++] = toInt(num);

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
          strcpy(num,"");
        }
      }
      /* if the converted number is out of range an Error is given. */
      else
      {
        puts(" Invalid set member – value out of range (type --help for instrauctions).");
        return;
      }

      /* now we check for a white space or tabs and remove them using removeSpace() function. */
      if(input[i] == ' ' || input[i] =='\t')
      {
        i = removeSpace(i,input);
      }

      /* every number needs to be seperated by a comma, so after we inserted a number to a SET
      we need to check that the number is sepereated by a comma before we move on with the loop. */
      if(input[i] != ',' )
      {
        /* so if the current char was not a comma there might be two Errors
        1. the current char is a digit so the user gets a missing comma Error.
        2. the number is not an integer. */
        if(isdigit(input[i]) || (input[i] == '-'  && (((input[i+1] - '0') == 1) && input[removeSpace(i+2,input)] == 0) ))
        {

          puts(" Missing comma (type --help for instrauctions).");
          return;
        }
        if(input[i] != 0)
        {
          puts(" Invalid set member – not an integer (type --help for instrauctions).");
          return;
        }
        else if(isalpha(input[i]))
        {
          puts(" Invalid set member – not an integer (type --help for instrauctions).");
          return;
        }
      }
    }
    else
    {
      puts(" Invalid set member – not an integer (type --help for instrauctions).");
      return;
    }
  }
  /*if the loop was terminated without any errors it means that the ending sign -1 was not
  inserted by the user and an Eror is given. */
  puts(" List of set members is not terminated correctly (type --help for instrauctions).");
  return;
}

/* this function checks for errors in user input when print_set command
was inserted by user.
if no error detected the function calls printSet() function from set.c
@param input - the user input.
@param setsArray - array of pointers to each set.
*/
void checkPrintSetInput(char input[], char** setsArray)
{
  /* contain an Integer that represents a set name to be sent ro readSet() function.  */
  int set = detectSet(PRINT_SET,input);
  int i = 0;

  if(set == -1)
  {
    return;
  }
  /* after detectSet function the length of the user input should be 0.
  if its not it means that there is extra text after the set name. */
  i = removeSpace(i,input);
  if(input[i] != 0)
  {
    puts(" Extraneous text after end of command (type --help for instrauctions).");
    return;
  }
  printSet(setsArray[set]);
}

/* this function checks for errors in user input when one of union_set,
intersect_set, sub_set, symdiff_set command was inserted by user.
@param cmd - contains an integer represents a command name.
@param input - the user input.
@param setsArray - array of pointers to each set.
*/
void checkUnionIntersectSubSymdiffINput(int cmd,char input[], char** setsArray)
{
  /* contain an integer that represents the first SET name to be sent */
  int firstSet;
  /* contain an integer that represents the first SET name to be sent */
  int secondSet;
  /* contain an integer that represents the first SET name to be sent */
  int thirdSet;
  int i =0;

  firstSet = detectSet(UNION_SET,input);
  if(firstSet == -1)
  {
    /* if detectSet() return -1 it means that no set was detected then we stop. */
    return;
  }

  /* after we found that there is atleast two SETs in user input we check if the
  current length is greater than 8 if its not it means that the user entered
  just one SET. */
  if(strlen(input) < 8)
  {
    puts(" Missing parameters (type --help for instrauctions).");
    return;
  }

  /* all the given SETs parameters should be seperated by comma.
  this if statement check if the first parameter is seperated by comma. */
  i = removeSpace(i, input);
  if(input[i] != ',')
  {
    puts(" Missing comma (type --help for instrauctions).");
    return;
  }

  i = removeSpace(i+1,input);
  i = removeSpace(i, input);

  /*after removing white spaces and tabs we check if ther is multiple commas. */
  if(input[i] == ',')
  {
    puts(" Multiple consecutive commas (type --help for instrauctions).");
    return;
  }

  secondSet = detectSet(UNION_SET,input);
  if(secondSet == -1)
  {
    /* if detectSet return -1 it means that no set was detected then we stop. */
    return;
  }

  /* after we found that there is atleast two SETs in user input we check if the
  current length is greater than 4 if its not it means that the user entered
  just two SETs. */
  if(strlen(input) < 4)
  {
    puts(" Missing parameter (type --help for instrauctions).");
    return;
  }

  /* all the given SETs parameters should be seperated by comma.
  this if statement check if the second parameter is seperated by comma. */
  i=0;
  i = removeSpace(i, input);

  if(input[i] != ',')
  {
    puts(" Missing comma (type --help for instrauctions).");
    return;
  }

  i = removeSpace(i+1,input);
  i = removeSpace(i, input);

  /*after removing white spaces and tabs we check if ther is multiple commas. */
  if(input[i] == ',')
  {
    puts(" Multiple consecutive commas (type --help for instrauctions).");
    return;
  }


  thirdSet = detectSet(UNION_SET,input);
  if(thirdSet == -1)
  {
    /* if detectSet return -1 it means that no set was detected  then we stop.*/
    return;
  }

  i = removeSpace(i,input);

  /* after we remove white spaces and tabs we check if there is extra text after the last parameter
  if there is an error is given to the user.*/
  if(strlen(input) > 0)
  {
    puts(" Extraneous text after end of command (type --help for instrauctions).");
    return;
  }

  /* after we checked the input and no error was given we call the right function
  from set.c via a switch that checks the cmd parameter and call one of the functions:
  unionSet(), intersectSet(), subSet(), symdiffSet(). */
  switch(cmd)
  {
    case UNION_SET:
    {
      unionSet(setsArray[firstSet],setsArray[secondSet],setsArray[thirdSet]);
      break;
    }
    case INTERSECT_SET:
    {
      intersetctSet(setsArray[firstSet],setsArray[secondSet],setsArray[thirdSet]);
      break;
    }
    case SUB_SET:
    {
      subSet(setsArray[firstSet],setsArray[secondSet],setsArray[thirdSet]);
      break;
    }
    case SYMDIFF_SET:
    {
      symDiff(setsArray[firstSet],setsArray[secondSet],setsArray[thirdSet]);
      break;
    }
  }
}

/* this funtions removes white space or tabs from user input.
@param i - the current index of white space or tab int user input.
@param input - the user input.
when the funtion encounter a charachter which is not white space or tab,
the function stop and return the index of that charachter. */
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

/* this function converts an array of chars
(if the chars represent numbers from 0-9) to integer.
@param input - the array of chars to be converted to int.
@return - return an integers. */
int toInt(char input[])
{
  int value;
  int i = 0;
  char s[MAX_INPUT];

  while(i < strlen(input))
  {
    s[i] = input[i];
    i++;
  }
  s[i] = '\0';
  value = atoi(s);

  return value;
}

/* this function prints a messeage to user when the program starts */
void printMssg()
{
  puts("*------------------------------------------------------------------------------*");
  puts("| \t\tSet Algebra Calculator program.                                |");
  puts("|                                                                              |");
  puts("|  * There are 7 stored sets(SETA, SETB, SETC, SETD, SETE, SETF).              |");
  puts("|    Each set can store integers from 0 - 127.                                 |");
  puts("|                                                                              |");
  puts("|  * You can perform set algebra calculation such as:                          |");
  puts("|       Union, Intersect, Subtract and Symmetric difference.                   |");
  puts("|                                                                              |");
  puts("|  * For the program's commands and manual please type --help                  |");
  puts("*------------------------------------------------------------------------------*");
  puts("");
}

/* this function prints instructions to user when the user inserts --help */
void printManual()
{
  puts("");
  puts("          \t\t*USER MANUAL*                               ");
  puts("");
  puts("  **** PLEASE NOTICE, ALL THE COMMANDS ARE CASE SENSETIVE. **** ");
  puts("   *** EACH GIVEN PARAMETER MUST BE SEPERATED BY COMMA.    ***");
  puts("    ** WHITE SPACES AND TAB SPACES WILL BE AUTO IGNORED.    **");
  puts("");
  puts("");
  puts(" * read_set:  This command inserts given list of values to a given set. ");
  puts("              A given value can show up more than one time in a list.");
  puts("              (repeated values will not be inserted to set).");
  puts("              To end a list of value type -1.");
  puts("");
  puts("");
  puts("          ATTENTION: This commands creates new SET and replaces the former one.");
  puts("                     If the list doesn't contain a value besides -1, (the end sign)");
  puts("                     the new set will be an Empty set.");
  puts("                     -1 must be the value that ends the set list otherwise error ");
  puts("                     will be given.");
  puts("                     Please pay attention, each set can store integers from 0-127.");
  puts("                     If an integer out of this range entered, an error will be given.");
  puts("                     Any given number and parameter MUST be seperated by comma.");
  puts("");
  puts("          * Syntax Example: read_set SETA, 5, 6, 5, 76, 44, 23, 23, 98, 23,-1 *");
  puts("");
  puts("");
  puts("");
  puts(" * print_set:  This command prints the values of a given SETA in ascending order.");
  puts("");
  puts("          * Syntax Example: print_set SETA *");
  puts("");
  puts("");
  puts("");
  puts(" * union_set:  The command unites given SETA with given SETB and stores the result ");
  puts("                in SETC.");
  puts("");
  puts("          * Syntax Example: union_set SETA, SETB, SETD *");
  puts("");
  puts("");
  puts("");
  puts(" * intersect_set:  This command intersects given SETA and given SETB then stores ");
  puts("                   the result in SETC. ");
  puts("");
  puts("         * Syntax Example: intersect_set SETA, SETB, SETD *");
  puts("");
  puts("");
  puts("");
  puts(" * sub_set:  This command subtract given SETB from given SETA then stores the result");
  puts("              in SETC.");
  puts("");
  puts("         * Syntax Example: sub_set SETA, SETB, SETC *");
  puts("");
  puts("");
  puts(" * symdiff_set:  This command calculate the symmetric difference of SETA and SETB,");
  puts("                 and stores the result in given SETC.");
  puts("");
  puts("         * Syntax Example: symdiff_set SETA, SETB, SETC  *");
}
