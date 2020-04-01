#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define NUM_OF_NAMES 30
#define NAME_SIZE 21

char * get_name();
int icmp(char *s1, char *s2);
bool checkIfNameIn(int,char[],char mat[][NAME_SIZE]);

char names[NUM_OF_NAMES][NAME_SIZE];

int main()
{
  int i;
  char s[NAME_SIZE];
  int numOfNames = 0;
  bool alreadyIn = false;
  srand(clock());

  puts("Please enter 30 names: ");
  while( numOfNames < NUM_OF_NAMES )
  {
    if ( (scanf("%s",s)) != EOF )
    {
      alreadyIn = false;
      /* checks if the user input is already in the 2dArray */
      if(checkIfNameIn(numOfNames,s,names))
      {
        alreadyIn = true;
      }
      if(!alreadyIn)
      {

        strcpy(names[numOfNames++],s);

      }
      else
      {
        printf("%s already entered \n",s);
        exit(0);
      }
    }
    else
    {
      break;
    }
  }

  if(numOfNames == NUM_OF_NAMES )
  {
    puts("you have reached the maximum number of names");
    puts("");
    puts("Printing entered names:");

    for(i = 0; i < numOfNames; i++)
    {
      printf(" %s,",names[i]);
    }

    puts("");
    puts("\nPrinting 10 random names:");
    for(i = 0; i < 10; i++)
    {
      char * name = get_name();
      printf(" %s,",name);
      free(name);
    }
    puts("");
  }
  else
  {
    puts("you didn't enter 30 names ");
  }

  return 0;
}

/* this function return a name from a global array of 30 names */
char * get_name()
{
  char * name;
  name = malloc(sizeof(char)*21);
  strcpy(name,names[rand()%30]);

  return name;
}

/* gets two strings and check if they are equal without case sensitive
if equal the function return 0.
*/
int icmp(char *s1, char *s2)
{
  for (; *s1 && *s2 && (toupper(*s1) == toupper(*s2)); ++s1, ++s2);

  return *s1 - *s2;
}

/* gets a string and 2dArray of strings.
the function check if the given string is already in given 2dArray of strings.
return true if the given name is already in the given 2dArray.
*/
bool checkIfNameIn(int numOfName,char s[],char mat[][NAME_SIZE])
{
  int i;
  int j;

  for(i = 0; i < numOfName; i++)
  {
    j = icmp(s,mat[i]);
    if(j == 0)
    {
      return true;
    }
  }
  return false;
}
