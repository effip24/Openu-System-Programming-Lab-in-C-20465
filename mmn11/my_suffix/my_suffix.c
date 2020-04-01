#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING 128

/* functions prototype */
int suffix(char str[MAX_STRING], char c);
char * removeSpace(char []);

int main()
{
  char s[MAX_STRING];
  int c;
  char ch;

  printf("Please enter a charachter then enter a string:\n" );
  /* getting user input and send it to suffix function
      until EOF */
  while (  scanf("%c\n",&ch ) != EOF && scanf("%[^\n]",s ) != EOF)
  {
      printf("\ncharachter: %c\n",ch);
      printf("string: %s\n",s);
      printf("result:\n" );
      printf("number of suffix: %d\n",suffix( removeSpace(s), ch) );
      /* clearing keyboard cache */
      while ((c = getchar()) != '\n' && c != EOF );
      puts("");
  }
  return 0;
}

/* returns the number of substrings of a given string that
    start with a given charachter
*/
int suffix(char str[MAX_STRING], char c)
{
    int count = 0;
    int i;

    for( i = 0; i < strlen(str); i++)
    {
        if(str[i] == c)
        {
            printf("\t%s\n",str+i);
            count++;
        }
    }
    return count;
}
/* removes white space from a string */
char * removeSpace(char s [MAX_STRING])
{
    int i;
    char * str = malloc(sizeof(char)*MAX_STRING);
    strcpy(str,s);

    for(i=0; i<strlen(str); i++)
    {
        if(str[i] == ' ')
        {
            break;
        }
    }

    for(i=i; i<strlen(str); i++)
    {
        if(str[i] == strlen(str)-1)
        {
            str[i] =0;
            break;
        }
        str[i] = str[i+1];
    }
    return str;
}
