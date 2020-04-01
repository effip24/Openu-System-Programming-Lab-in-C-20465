/*
mmn14 - two pass assembler model based on imaginary computer
and imaginary assembly language.
assembler.c - this file has the main function.
@author - Ephraim Phil 205390156.
@version - 10/03/2020. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "data_storage.h"
#include "file_parser.h"

int main(int argc, char *argv[])
{
  int i;

  /* checking if the user didn't enter a file */
  if(argc <= 1)
  {
    puts("please specify file name as parameter.");
    exit(0);
  }

  /* running a loop that goes through argv
  (array that contains all the files that were given by the user) */
  for(i = 1; i < argc; i++)
  {
    /* passing the file name to the fileParser */
    fileParser(argv[i]);
  }
  return 0;
}
