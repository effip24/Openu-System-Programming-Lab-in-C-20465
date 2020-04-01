/* mmn23 - this program gets a list of files each file is an arguments in the terminal
each file contains integers from 0-28, each number is seperated by
(one or more)comma.
the program reads all the numbers from all the given files, then prints
to the screen a summary that details all the names of the files which
contain a number and how much times its in each file.
ex:
dat1.in: 11 2 4 21 23 21
dat2.in: 21 7 4 23 21 13 13
dat3.in: 5 23 5 23 6 23 23

./hash dat1.in dat2.in dat3.in

output:
2 appears in file dat1.in - 1 time
4 appears in file dat1.in - 1 time, file dat2.in - 1 time
5 appears in file dat3.in - 2 times
6 appears in file dat3.in - 1 time
7 appears in file dat2.in - 1 time
11 appears in file dat1.in - 1 time
13 appears in file dat2.in - 2 times
21 appears in file dat1.in - 2 times, file dat2.in - 2 times
23 appears in file dat1.in - 1 time, file dat2.in - 1 time, file dat3.in - 4 times

hash.c - this file  contains  all the functions that handle the program.

@author - Ephraim Phil 205390156.
@version - 25/1/2020. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "hash.h"

/*main */
int main(int argc, char *argv[])
{
  int i;
  int j;
  /* array of linked lists */
  ptr h[MAX_NODES];

  FILE *file;

  /* a temp array that will hold all the numbers in a file */
  int *tempArray;
  /* using q to realloc the thempArray */
  int *q;
  /*holds the length of the tempArray */
  int numof = 0;
  /* holds a number in a file */
  int num;

  /* checking if the user did not give an argument(name of a file) */
  if(argc < 2)
  {
    puts("missing arguments.");
    exit(0);
  }

  /* initializing all the nodes to NULL */
  for(i = 0; i < 29; i++)
  {
    h[i] = NULL;
  }

  /* this loop runs until there is no more arguments in argv. */
  for(i = 1; i < argc; i++)
  {
    /* 1. open the current file (argv[i]). */
    file = fopen(argv[i],"r");
    if(!file)
    {
      puts("could not open the file. ");
      exit(0);
    }

    tempArray = (int *)malloc(sizeof(int)*1);
    if(!tempArray)
    {
      puts("failed to allocate memory to temporary array");
      return 0;
    }
    /* 2. saves all the number from the file into tempArray. */
    while(fscanf(file,"%d", &num) == 1)
    {
      if(num >= 0 && num <= 28)
      {
        tempArray[numof++] = num;
        q  = (int *)realloc(tempArray,(numof+1)*sizeof(int));
      }

      if(!q)
      {
        puts("failed to allocate memory to temporary array");
        return 0;
      }
      else
      {
        tempArray = q;
      }
    }
    /* 3. close the file. */
    fclose(file);

    /* 4. running a loop using j index to check how many time all the values
    0-28 are in the tempArray. */
    for(j = 0; j < 29; j++)
    {
      /* using checkHowManyNums() function the returned value will be saved in
      tempNUm */
      int tempNUm = checkHowManyNums(j,numof,tempArray);
      if(tempNUm > 0)
      {
        /* inserted to a linked list usint addToList() function, the added node
        will contain the file name(argv[i]) to filneName and numer of time the
        current number(the index j) is in tempArray to data. */
        addToList(&h[j],tempNUm,argv[i]);
      }
    }
    /* 5. free tempArray and calibrate numOf for the next file(argv[i]). */
    free(tempArray);
    numof = 0;
  }

  /* prints all the linked list from h array as long h[i] not NULL */
  for(i = 0; i < 29;i++)
  {
    if(h[i] != NULL)
    {
      printlist(h[i],i);
    }
    /* after the linked list was printed to screen we free it */
    freelist(&h[i]);
  }

  return 0;
}

/* this function adds a new node to a linked list each node contain fileName
and data.
@param ptr - the linked list to add a new node to
@param num - will be inserted to the new node data, the data contains how much
time a given number is in a file.
@param fileName - will be inserted to the new node fileName, filename contains
the name of a file */
void addToList(ptr *hptr, int num,char fileName[])
{
  ptr p1,p2,t;

  /* initializeng t which is a temp pointer to the linked list*/
  t = (ptr)malloc(sizeof(list));

  /*chekcs if we could not allocate memory to t */
  if(!t)
  {
    puts("could not allocate memory;");
    exit(0);
  }

  /* fileName of t gets the given filename and the data gets num which is the
  number of time a number is in a file(fileName) */
  strcpy(t->fileName,fileName);
  t->data = num;

  /* if the list is empty */
  if(!(*hptr))
  {
    t->next = NULL;
    *hptr = t;
    return;
  }

  /* searching for the new place to put the new node.
  the loop runs until p1 == NULL and then the new node will be added last */
  p1 = *hptr;
  while((p1))
  {
    p2 =p1;
    p1 = p1->next;
  }

  t->next = p1;
  p2->next = t;

}

/* this function prints a given linked list from array of linked lists.
@param ptr - the linked list to be printed
@param num - the linked list to be printed from the array of linked lists. */
void printlist(ptr h, int num)
{
  printf("%d appears in ",num );
  /* running through the given list */
  while(h)
  {
    /* this if statement check if there is another node in the list so we print
    ',' to the screen */
    if((h->next) != NULL)
    {
      printf("file %s - %d ",h->fileName,h->data);
      /* this macro prints time in singal or plural depends on the value of data */
      leng(h->data);
      printf(", ");

    }
    /* we reached to the last node so ',' does not need to be printed */
    else
    {
      printf("file %s - %d ",h->fileName,h->data);
      leng(h->data);
    }
    h = h->next;
  }
  puts("");
}

/* this function frees a given linked list */
void freelist(ptr *hptr)
{
  /* a temp pointer that hold the given list */
  ptr p;
  /* as long as the current node is not NULL the loop keep going*/
  while(*hptr)
  {
    /* giving p the current node so we dont lose it after we move to the next node */
    p = *hptr;
    /* moving to the next node */
    *hptr = p->next;
    free(p);
  }
}

/* this function checks how many times a given number is in a given array
@param j - the number to be checked how much time is in an array
@param numof - the length of a given array
@param tempArray the array to be checked
@return integer that represents how much times a number is in an array */
int checkHowManyNums(int num,int numof,int tempArray[])
{

  int i;
  /* will hold how much numbers in a given array equlas to the given num */
  int cnt = 0;

  for(i = 0; i < numof; i++)
  {
    if(tempArray[i] == num)
    {
      cnt++;
    }
  }
  return cnt;
}
