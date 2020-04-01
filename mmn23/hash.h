/* mmn23
hash.h - this file contains functions declaration of hash.c, defines, includes and
the typedef of nodes.

@author - Ephraim Phil 205390156.
@version - 25/1/2020. */

/* max charachter of a file name */
#define MAX_FILENAME 128
/* maximun node in array of nodes */
#define MAX_NODES 29
/*this macro checks if x in bigger then 1 or not.
if it is bigger, then it prints times, else prints time.
the given x is a number of time a given number is in a file. */
#define leng(x)\
{\
  if(x > 1)\
  {\
    printf("times");\
  }\
  else\
  {\
    printf("time");\
  }\
}
/* this creates a data type of a pointer to a list */
typedef struct node * ptr;

/*creates a linked list*/
typedef struct node{
  /*each node represents a filename*/
  char fileName[MAX_FILENAME];
  /* contains the number of time a number is in a file */
  int data;
  /* contains an address of the next node in the list */
  ptr next;
}list;

/* this function checks how many times a given number is in a given array
@param j - the number to be checked how much time is in an array
@param numof - the length of a given array
@param tempArray the array to be checked
@return integer that represents how much times a number is in an array */
int checkHowManyNums(int j,int numof,int tempArray[]);

/* this function adds a new node to a linked list each node contain fileName
and data.
@param ptr - the linked list to add a new node to
@param num - will be inserted to the new node data, the data contains how much
time a given number is in a file.
@param fileName - will be inserted to the new node fileName, filename contains
the name of a file */
void addToList(ptr *, int,char []);

/* this function prints a given linked list from array of linked lists.
@param ptr - the linked list to be printed
@param num - the linked list to be printed from the array of linked lists. */
void printlist(ptr,int num);

/* this function frees a given linked list */
void freelist(ptr *);
