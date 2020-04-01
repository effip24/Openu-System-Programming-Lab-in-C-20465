#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING 128

/* prototype declaration */
void translate_dec_hex(char [MAX_STRING]);

int main()
{
    char str[MAX_STRING];
    puts("Please enter a number to convert to binary: ");

    while( (scanf("%s",str )) != EOF)
    {

        translate_dec_hex(str);
    }
    return 0;
}

/*this function gets a string and converts it to unsigned int then it prints
it in binary
*/

void translate_dec_hex(char str[MAX_STRING])
{
    int c;
    unsigned int num = 0;
    int i;
    unsigned int mask = 1 << ((sizeof(num)*8)-1);

    /* converting user input(string) to unsigned int */
    for(i = 0; i < strlen(str); i++)
    {
        c = str[i];
        num = (num*10) + c - '0';
    }
    printf("%u in binary is: ",num);

    /* this while loop shifts the mask right so it will not print resundant
        redundant zero  */
    while(mask)
    {
        if  ((num&mask) != 0)
        {
            break;
        }
        mask = mask >> 1;
    }
    /* this while loop prints 0 if the current bit is zero and 1 if the
        current bit is 1
      */
    while(mask)
    {
        if  ((num&mask) == 0)
        {
            printf("0");
        }
        else
        {
            printf("1");
        }
        mask = mask >> 1;
    }
    putchar('\n');

}
