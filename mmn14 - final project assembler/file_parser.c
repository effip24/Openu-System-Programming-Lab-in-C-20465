/*
mmn14 - two pass assembler model based on imaginary computer
and imaginary assembly language.
file_parser.c - this file has all the functions that resposibles
for the first and the second pass.
@author - Ephraim Phil.
@version - 10/03/2020. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

#include "utilities.h"
#include "data_storage.h"
#include "input_parser.h"
#include "file_parser.h"

/* this function read line by line from a file that were given by the user then
it calls the firstParse function, then it calls the secondParse function and
at the end calls the functions that write all the data to files.
@param fileName - the file to parse.
*/
void fileParser(char fileName[])
{
  /*i,j are index */
  int i;
  int j;
  /* the current line from the given file */
  int line = 1;

  /* instruction counter */
  int ic = 0;
  /* data counter */
  int dc = 0;

  /* flag, turns on if errors were detected through the first parse */
  bool firstPassErrors = false;
  /* flag, turns on if errors were detected through the second parse */
  bool secondPassErrors = false;

  /*linked list, contains all the symbols */
  ptrSYMBOLS symbols;
  /*linked list, contains all the instructions words */
  ptrCODE code;
  /*linked list, contains all the data */
  ptrDATA data;

  /* contains a full line from fileName */
  static char input[128];
  char obFile[128];
  char extFile[128];
  char entFile[128];


  FILE *fp;

  /* initializing the lionked lists */
  symbols = NULL;
  code = NULL;
  data = NULL;

  /* openning the current file */
  fp = fopen(fileName, "r");

  /* if we could not open the given file we return back to main
  to get the next file*/
  if(fp == NULL)
  {
    printf(" Unable to to open %s\n",fileName );
    return;
  }

  /* checking valadity of the file name */
  for(i = 0; i < strlen(fileName); i++)
  {
    if(fileName[i] == '.')
    {
      /* checking if the given file is not .as file */
      if(strcmp(fileName+i,".as"))
      {
        puts(" Unkows file type. please insert .as files");
        return;
      }
    }
  }

  /*  ---------parsing for the first time --------------------*/

  /* the loop reads line by line from the file and pass it to
  firstParse function*/
  while(fgets(input, sizeof(input), fp) != NULL)
  {
    /* this function check if the current line is a comment or has a comment
    and removs it from input */
    removeComments(input);
    /* this function removes '\n' charachter from the end of input */
    removeNewLine(input);
    /* this function checks if input only has space and tabs which means that
    it is an empty string and sets input[0] to be '\0' */
    removeEmptyLine(input);

    /* starting the first pass, the if statement checks if errors detected
    during the first pass */
    if(runFirstScan(&code, &data, &symbols, input, &ic, &dc,fileName,line))
    {
      firstPassErrors = true;
    }
    /* increasing the lines counter by 1 after reading a line */
    line++;
    /* empty input and make it ready for the next read */
    strcpy(input,"");
  }
  /* after we parsed through the whole file we close the file */
  fclose(fp);

  /* --------------parsing for the second time----------------- */

  /* calibrate the lines counter */
  line = 1;
  fp = NULL;
  fp = fopen(fileName, "r");
  if(fp == NULL)
  {
    printf(" Unable to to open %s\n",fileName );
    return;
  }

  /* checking if no errors were detected during the first pass */
  if(!firstPassErrors)
  {
    /* the loop reads line by line from the file and pass it to
    firstParse function*/
    while(fgets(input, sizeof(input), fp) != NULL)
    {
      removeComments(input);
      removeNewLine(input);
      removeEmptyLine(input);

      if(runSecondScan(&code, &data, &symbols, input, &ic, &dc,fileName,line))
      {
        secondPassErrors = true;
      }
      line++;
      input[0] = '\0';
    }
    runSecondScan(&code, &data, &symbols, "NULL", &ic, &dc,fileName,line);
    fclose(fp);
  }

  /* if no errors were detected during the first and the second pass we write
  the data structures to files */
  if(!firstPassErrors && !secondPassErrors)
  {
    /* writing .ob file */

    /* copies the fileName to the .ob, .ext, .ent files we create */
    for(i = 0,j=0; i < strlen(fileName); i++)
    {
      if(fileName[i] == '.')
      {
        break;
      }
      obFile[j] = fileName[i];
      extFile[j] = fileName[i];
      entFile[j] = fileName[i];
      j++;
    }
    obFile[j] = '\0';
    extFile[j] = '\0';
    entFile[j] = '\0';

    strcat(obFile,".ob");
    strcat(extFile,".ext");
    strcat(entFile,".ent");

    /* creates the .ob file */
    fp = NULL;
    fp = fopen(obFile, "w");
    if(fp == NULL)
    {
      printf(" Unable to to open %s\n",fileName );
      return;
    }
    /* writing ic and dc to the file */
    fprintf(fp,"   %d %d\n",ic,dc );
    /* writing the instructions and data structures to the created file*/
    writeInstructions(&code,fp);
    writeData(&data,fp);
    /* closing the file after we finished to write data to it */
    fclose(fp);


    /* checking if we need to create .ext file */
    if(checkExternOrEntry(&symbols,"extern"))
    {
      /* creating fileName.ext */
      fp = NULL;
      fp = fopen(extFile, "w");
      if(fp == NULL)
      {
        printf(" Unable to to open %s\n",fileName );
        return;
      }
      /* writing data to the .ext file and close it */
      writeEntrysOrExternals(&symbols,&code,"extern",fp);
      fclose(fp);
    }

    /* checking if we need to create .ent file */
    if(checkExternOrEntry(&symbols,"entry"))
    {
      /* creating fileName.ent */
      fp = NULL;
      fp = fopen(entFile, "w");
      if(fp == NULL)
      {
        printf(" Unable to to open %s\n",fileName );
        return;
      }
      /* writing data to the .ent file and close it */
      writeEntrysOrExternals(&symbols,&code,"entry",fp);
      fclose(fp);
    }
  }

  /* EOF second parse */

  /* after we finshed to write all the files we free the linked lists*/
  freeSymbols(symbols);
  freeInstrcutions(code);
  freeData(data);
}

/* first pass - this function disects a given line and builds the instructions table, the data table
and symbols table.
@param code - pointer to the instructions table.
@param data - pointer to the data table.
@param symbols - pointer to the symbols table.
@param ic - instructions counter.
@param dc - data counter.
@param fileName - dthe current open assembly file.
@param line - line counter, contains the current line value.
*/
bool runFirstScan(ptrCODE *code, ptrDATA *data, ptrSYMBOLS *symbols, char input[], int *ic, int *dc,char fileName[], int line)
{
  /* contains a number that represents an instruction name */
  int opCode;
  /* contains a number that represents the sorting method of the source operand */
  int sourceCode;
  /* contains a number that represents the sorting method of the destination operand */
  int destinationCode;
  /* contains the number of word that needs to be added */
  int L;

  /* saves symbol name from input */
  char * symbol;
  /* saves .data or .string from input */
  char * dataOrString;
  /* saves .extern or .entry from input */
  char * externOrEntry;
  /* saves .extern operand from input */
  char * externOperand;
  /* has the converted opCode in binary */
  char * binaryOpCode;
  /* saves source operand from input */
  char * sourceOperand;
  /* saves destination operand from input */
  char * destinationOperand;

  /* symbol flag, if a symbol was found in input, the flag turns on */
  bool isSymbol = false;
  /* errors flag, if a errors were found in input, the flag turns on */
  bool errors = false;

  /* memory allocation */
  symbol = (char *)malloc(sizeof(char)*SYMBOL);
  dataOrString = (char *)malloc(sizeof(char)*DATA_OR_STRING);
  externOrEntry = (char *)malloc(sizeof(char)*EXTERN_OR_ENTRY);
  externOperand = (char *)malloc(sizeof(char)*EXTERN_OPERAND);
  binaryOpCode = (char *)malloc(sizeof(char)*ADDRESS);
  sourceOperand = (char *)malloc(sizeof(char)*SOURCE_OPERAND);
  destinationOperand = (char *)malloc(sizeof(char)*DESTINATION_OPERAND);

  /* checking if the memory allocation was successful */
  if(!symbol || !dataOrString || !externOrEntry || !externOperand || !binaryOpCode || !sourceOperand || !destinationOperand)
  {
    puts("memory allocation failed!");
    exit(1);
  }

  /* 2.  if input is an empty line we go back and read the next line. */
  if(strlen(input) <= 1)
  {
    free(symbol);
    free(dataOrString);
    free(externOrEntry);
    free(externOperand);
    free(binaryOpCode);
    free(sourceOperand);
    free(destinationOperand);
    return errors;
  }

  /* 3. checking if the first field of the line is a symbol */
  if(checkForSymbol(input,symbol,true,fileName,line,&errors))
  {
    /* 4.  turning on the symbol flag. */
    isSymbol = true;
  }

  /* 5.  checking if it is .data or .string directives  */
  if(checkForDataOrString(input,dataOrString,true,fileName,line,&errors))
  {
    /* 6.  checking if the symbol flag is on  */
    if(isSymbol)
    {
      /* checking if the symbol is not already in the symbols table */
      if(notInSymbols(symbols,symbol))
      {
        /* if no errors were detected we add the symbol to the symbols table */
        if(!errors)
        addToSymbols(symbols,*dc,symbol,".data");
      }
      else
      {
        printf("%s:%d:\033[1;31merror:\033[0m label was already declared earlier\n",fileName,line);
        errors = true;
      }
    }
    /* 7.   calling addData function to code the data and insert it to the data table */
    /* we check if the data were added succesfuly, if not we turn on errors flag */
    if(!addData(data,dc,dataOrString,input,fileName,line))errors = true;
    /* we free the allocated memory and return to read the next line */
    free(symbol);
    free(dataOrString);
    free(externOrEntry);
    free(externOperand);
    free(binaryOpCode);
    free(sourceOperand);
    free(destinationOperand);
    return errors;
  }

  /* 8.  checking if it is .extern or .entry directives */
  else if(checkForExternOrEntry(input,externOrEntry,true,fileName,line,&errors))
  {
    /*  9.  we check if the symbol flag is on */
    if(isSymbol)
    {
      /* ignoring this symbol*/
      printf("%s:%d:\033[1;35mWarning:\033[0m lable %s is ignored\n",fileName,line,symbol );
      addToSymbols(symbols,-1,symbol,"ignored");
    }
    /* if it is .entry we go back to read the next line (.entries will be taken care in second pass)*/
    if(!strcmp(externOrEntry,"entry"))
    {
      free(symbol);
      free(dataOrString);
      free(externOrEntry);
      free(externOperand);
      free(binaryOpCode);
      free(sourceOperand);
      free(destinationOperand);
      return errors;
    }

    /*10.  if it is .extern directive */
    else if(!strcmp(externOrEntry,"extern"))
    {
      /* extracting the operand from input */
      getExternOrEntryOperand(input, externOperand);
      /* checking if the operand is not local */
      if(!checkIfSymbolLocal(symbols,externOperand))
      {
        /* checking if the symbol is not already in the symbol table */
        if(notInSymbols(symbols,externOperand))
        {
          /*if no errors were detected so far we insert the symbol to symbols table */
          if(!errors)
          addToSymbols(symbols,-1,externOperand,".extern");
        }
        /* return to read the next line */
        free(symbol);
        free(dataOrString);
        free(externOrEntry);
        free(externOperand);
        free(binaryOpCode);
        free(sourceOperand);
        free(destinationOperand);
        return errors;
      }
      else
      {
        printf("%s:%d:\033[1;31merror:\033[0m local label cannot be declared as external\n",fileName,line );
        errors = true;
      }
      free(symbol);
      free(dataOrString);
      free(externOrEntry);
      free(externOperand);
      free(binaryOpCode);
      free(sourceOperand);
      free(destinationOperand);
      return errors;
    }
  }
  else
  {
    /* 11. this is an instruction line, checking if symbol flag is on */
    if(isSymbol)
    {
      if(notInSymbols(symbols,symbol))
      {
        if(!errors)
        addToSymbols(symbols,*ic+100,symbol,".code");
      }
      else
      {
        printf("%s:%d:\033[1;31merror:\033[0m label was already declared earlier\n",fileName,line );
        errors = true;
      }
    }

    /* 12. getting the opcode from input */
    if((opCode = getOpCode(input,binaryOpCode)) == UNDEFINED)
    {
      printf("%s:%d:\033[1;31merror:\033[0m undefined instruction name\n",fileName,line );
      errors = true;
    }

    /*13  calculating L, the number of words */
    L = calculateL(opCode);

    /* getting the operands  */
    if(!getOperands(symbols,input,L,sourceOperand,destinationOperand,&sourceCode,&destinationCode,opCode,true,fileName,line))
    {
      errors = true;
    }
    else
    {
      /*encoding the instruction line to the instructions TABLE */
      if(!errors)
      addInstructionWord(code,ic,binaryOpCode,sourceOperand,destinationOperand,L,sourceCode,destinationCode);
    }
  }
  free(symbol);
  free(dataOrString);
  free(externOrEntry);
  free(externOperand);
  free(binaryOpCode);
  free(sourceOperand);
  free(destinationOperand);
  return errors;
}

/* second pass -  this function disects a given line and updates the instructions table, the data table
and symbols table.
@param code - pointer to the instructions table.
@param data - pointer to the data table.
@param symbols - linked list, the symbols table.
@param ic - instructions counter.
@param dc - data counter.
@param fileName - dthe current open assembly file.
@param line - line counter, contains the current line value.
*/
bool runSecondScan(ptrCODE *code,ptrDATA *data,ptrSYMBOLS *symbols,char input[],int *ic,int *dc,char fileName[],int line)
{
  /* contains a number that represents an instruction name */
  int opCode;
  /* contains a number that represents the sorting method of the source operand */
  int sourceCode;
  /* contains a number that represents the sorting method of the destination operand */
  int destinationCode;
  /* contains the number of word that needs to be added */
  int L;

  /* saves symbol name from input */
  char * symbol;
  /* saves .data or .string from input */
  char * dataOrString;
  /* has the converted opCode in binary */
  char * binaryOpCode;
  /* saves .extern or .entry from input */
  char * externOrEntry;
  /* saves .extern operand from input */
  char * externOperand;
  /* saves .entry operand from input */
  char * entryOperand;
  /* saves source operand from input */
  char * sourceOperand;
  /* saves destination operand from input */
  char * destinationOperand;

  bool errors = false;

  /* memory allocation */
  symbol = (char *)malloc(sizeof(char)*SYMBOL);
  dataOrString = (char *)malloc(sizeof(char)*DATA_OR_STRING);
  externOrEntry = (char *)malloc(sizeof(char)*EXTERN_OR_ENTRY);
  binaryOpCode = (char *)malloc(sizeof(char)*ADDRESS);
  externOperand = (char *)malloc(sizeof(char)*EXTERN_OPERAND);
  entryOperand = (char *)malloc(sizeof(char)*ENTRY_OPERAND);
  sourceOperand = (char *)malloc(sizeof(char)*SOURCE_OPERAND);
  destinationOperand = (char *)malloc(sizeof(char)*DESTINATION_OPERAND);

  /* checking if the memory allocation was successful */
  if(!symbol || !dataOrString || !externOrEntry || !externOperand || !sourceOperand || !destinationOperand || !entryOperand || !binaryOpCode)
  {
    puts("memory allocation failed!");
    exit(0);
  }

  /* 2.   checking if we reached to the end of the file */
  if(!strcmp(input,"NULL") || strlen(input) <= 1 )
  {
    /* in case the line is an empty line we return to read the next line */
    if(strlen(input) <= 1)
    {
      free(symbol);
      free(dataOrString);
      free(externOrEntry);
      free(binaryOpCode);
      free(externOperand);
      free(entryOperand);
      free(sourceOperand);
      free(destinationOperand);
      return errors;
    }
    /* this is when we get to the end of the second pass, we update the tables */
    updateSymbolsTable(symbols,ic);
    updateInstructions(code,symbols);
    updateData(data,ic);
    free(symbol);
    free(dataOrString);
    free(externOrEntry);
    free(binaryOpCode);
    free(externOperand);
    free(entryOperand);
    free(sourceOperand);
    free(destinationOperand);
    return errors;
  }
  /* 3.  checking if the first field of the line is a symbol*/
  checkForSymbol(input,symbol,false,fileName,line,&errors);

  /* 5.   checking if it is .data or .string directives, if it is, we return
  to read the next line because we already codes this line during the first pass */
  if(checkForDataOrString(input,dataOrString,false,fileName,line,&errors))
  {
    free(symbol);
    free(dataOrString);
    free(externOrEntry);
    free(binaryOpCode);
    free(externOperand);
    free(entryOperand);
    free(sourceOperand);
    free(destinationOperand);
    return errors;
  }

  /* 8.  checking if it is .extern or .entry directives */
  else if(checkForExternOrEntry(input,externOrEntry,false,fileName,line,&errors))
  {
    /* checking if it is .entry and extracting .entry operand */
    getExternOrEntryOperand(input, entryOperand);
    if(!strcmp(externOrEntry,"entry"))
    {
      /* .entries did not handle during the first pass only at the second pass */
      /* if the operand is not already in the symbols table */
      if(notInSymbols(symbols,entryOperand))
      {
        printf("%s:%d:\033[1;31merror:\033[0m undefined lable \n",fileName,line);
        errors = true;
      }
      /* checking if the symbol was not ignored during the first pass */
      else if(!checkIfSymbolValid(symbols,entryOperand))
      {
        printf("%s:%d:\033[1;31merror:\033[0m undefined label (label %s is ignored)\n",fileName,line,entryOperand );
        errors = true;
      }
      else
      {
        /* checking if the operand is not designated as .extern */
        if(!checkIfDesignatedExternal(symbols,entryOperand))
        {
          /* update the symbol type flag in symbols table */
          if(!errors)
          updateSymbolType(symbols,entryOperand);
        }
        else
        {
          printf("%s:%d:\033[1;31merror:\033[0m label already designated as external (cannot be both entry and exteral)\n",fileName,line );
          errors = true;
        }
      }
    }
    else if(!strcmp(externOrEntry,".extern"))
    {
      if(!checkIfSymbolValid(symbols,entryOperand))
      {
        printf("%s:%d:\033[1;31merror:\033[0m undefined label (label %s is ignored)\n",fileName,line,entryOperand);
        errors = true;
      }
    }
    free(symbol);
    free(dataOrString);
    free(externOrEntry);
    free(binaryOpCode);
    free(externOperand);
    free(entryOperand);
    free(sourceOperand);
    free(destinationOperand);
    return errors;
  }
  else
  {
    /* we alraedy encoded the instruction in the first pass so now we only  check for errors */

    /* 12. getting the opcode from input */
    if((opCode = getOpCode(input,binaryOpCode)) == UNDEFINED)
    {
      printf("%s:%d:\033[1;31merror:\033[0m undefined instruction name\n",fileName,line );
      errors = true;
    }

    /*13  calculating L, the number of words*/
    L = calculateL(opCode);

    /* getting the source and destination operand on the way we check again for erros
    (we repeat the proccess from the first pass intentionally) */
    getOperands(symbols,input,L,sourceOperand,destinationOperand,&sourceCode,&destinationCode,-4,false,fileName,line);

    if(sourceCode < 0 || destinationCode < 0)
    {
      errors = true;
    }
    free(symbol);
    free(dataOrString);
    free(externOrEntry);
    free(binaryOpCode);
    free(externOperand);
    free(entryOperand);
    free(sourceOperand);
    free(destinationOperand);
    return errors;
  }
}

/* this function disects an instruction line, build the words
and adds it to the instructions table.
@param code - pointer to the instructions table.
@param ic - instructions counter.
@param binaryOpCode - the opCode converted to binary(in binary).
@param sourceOperand - the source operand.
@param destinationOperand - the destination operand.
@param L - the number of words to encode and add to the Instrcutions table.
@param sourceCode - the sorting method number of the source operand.
@param destinationCode - the sorting method number of the destination operand.
*/
void addInstructionWord(ptrCODE *code,int *ic, char *binaryOpCode,char *sourceOperand,char *destinationOperand,int L,int sourceCode, int destinationCode)
{
  /* indexes */
  int i;
  int j;

  /* the maximum number that need to be build is 3, so we create 3 temporary string that
  represent each word (in binary) */
  char firstWord[16];
  char secondWord[16];
  char thirdWord[16];

  /*initializing each word to 0 (as binary) */
  strcpy(firstWord,"000000000000000");
  strcpy(secondWord,"000000000000000");
  strcpy(thirdWord,"000000000000000");

  /*copies binaryOpCode to the first word, indexes 0,1,2,3 as 14,13,12,11 bits in a word */
  firstWord[0] = binaryOpCode[11];
  firstWord[1] = binaryOpCode[12];
  firstWord[2] = binaryOpCode[13];
  firstWord[3] = binaryOpCode[14];

  /* if we need to build 3 words */
  if(L==3)
  {
    /* detecting the sorting method to code of the source operand */
    switch(sourceCode)
    {
      /* if the source operand is IMMEDIATE_SORT EX: '#'*/
      case IMMEDIATE_SORT:
      {
        /* coding 0001 in indexes 4,5,6,7 as 7,8,9,10 bits in a word */
        firstWord[4] = '0';
        firstWord[5] = '0';
        firstWord[6] = '0';
        firstWord[7] = '1';

        /* coding the second word in bit 3-12 */
        j=3;
        for(i = 0; i < 12; i++)
        {
          secondWord[i] = sourceOperand[j];
          j++;
        }
        /* coding A,R,E field 100 */
        secondWord[12] = '1';
        secondWord[13] = '0';
        secondWord[14] = '0';
        break;
      }
      /* if the source operand is DIRECT_SORT ex: LABLE */
      case DIRECT_SORT:
      {
        /* coding 0010 in indexes 4,5,6,7 as 7,8,9,10 bits in a word */
        firstWord[4] = '0';
        firstWord[5] = '0';
        firstWord[6] = '1';
        firstWord[7] = '0';

        /* sourceOperand contains the binary word of the source operand */
        strcpy(secondWord,sourceOperand);
        break;
      }
      /* if the source operand is INDIRECT_REGISTER_SORT EX: '*r0'*/
      case INDIRECT_REGISTER_SORT:
      {
        /* coding 0100 in indexes 4,5,6,7 as 7,8,9,10 bits in a word */
        firstWord[4] = '0';
        firstWord[5] = '1';
        firstWord[6] = '0';
        firstWord[7] = '0';

        /* coding the second word in bit 6-8 */
        secondWord[6] = sourceOperand[12];
        secondWord[7] = sourceOperand[13];
        secondWord[8] = sourceOperand[14];

        /* if both source operand and destination operand are direct_register_sort
        or indirect_register sort, they share a mutual word, bits 3-5 of the word
        represents the number of the distination rigister and bits 6-8
        represents the number of the source register */
        if(destinationCode == INDIRECT_REGISTER_SORT || destinationCode == DIRECT_REGISTER_SORT)
        {
          secondWord[9] = destinationOperand[12];
          secondWord[10] = destinationOperand[13];
          secondWord[11] = destinationOperand[14];
        }
        /* coding A,R,E field 100 */
        secondWord[12] = '1';
        secondWord[13] = '0';
        secondWord[14] = '0';
        break;
      }
      /* if the source operand is DIRECT_REGISTER_SORT ex: r0 */
      case DIRECT_REGISTER_SORT:
      {
        /* coding 1000 in indexes 4,5,6,7 as 7,8,9,10 bits in a word */
        firstWord[4] = '1';
        firstWord[5] = '0';
        firstWord[6] = '0';
        firstWord[7] = '0';

        /* coding the second word in bit 6-8 */
        secondWord[6] = sourceOperand[12];
        secondWord[7] = sourceOperand[13];
        secondWord[8] = sourceOperand[14];

        /* if both source operand and destination operand are direct_register_sort
        or indirect_register sort, they share a mutual word, bits 3-5 of the word
        represents the number of the distination rigister and bits 6-8
        represents the number of the source register */
        if(destinationCode == INDIRECT_REGISTER_SORT || destinationCode == DIRECT_REGISTER_SORT)
        {
          secondWord[9] = destinationOperand[12];
          secondWord[10] = destinationOperand[13];
          secondWord[11] = destinationOperand[14];
        }
        /* coding A,R,E field 100 */
        secondWord[12] = '1';
        secondWord[13] = '0';
        secondWord[14] = '0';
        break;
      }
      default:
      {
        break;
      }
    }

    /* detecting the sorting method to code of the destination operand */
    switch(destinationCode)
    {
      /* if the destination operand is IMMEDIATE_SORT EX: '#'*/
      case IMMEDIATE_SORT:
      {
        /* coding 0001 in indexes 4,5,6,7 as 3,4,5,6 bits in a word */
        firstWord[8] = '0';
        firstWord[9] = '0';
        firstWord[10] = '0';
        firstWord[11] = '1';

        /* coding the third word in bit 3-12 */
        j=3;
        for(i = 0; i < 12; i++)
        {
          thirdWord[i] = destinationOperand[j];
          j++;
        }
        /* coding A,R,E field 100 */
        thirdWord[12] = '1';
        thirdWord[13] = '0';
        thirdWord[14] = '0';
        break;
      }
      /* if the destination operand is DIRECT_SORT ex: LABLE */
      case DIRECT_SORT:
      {
        /* coding 0010 in indexes 4,5,6,7 as 3,4,5,6 bits in a word */
        firstWord[8] = '0';
        firstWord[9] = '0';
        firstWord[10] = '1';
        firstWord[11] = '0';

        /* destinationOperand contains the binary word of the destination operand */
        strcpy(thirdWord,destinationOperand);
        break;
      }
      /* if the destination operand is INDIRECT_REGISTER_SORT EX: '*r0'*/
      case INDIRECT_REGISTER_SORT:
      {
        /* coding 0100 in indexes 4,5,6,7 as 3,4,5,6 bits in a word */
        firstWord[8] = '0';
        firstWord[9] = '1';
        firstWord[10] = '0';
        firstWord[11] = '0';

        /* coding the third word in bit 3-5 */
        thirdWord[9] = destinationOperand[12];
        thirdWord[10] = destinationOperand[13];
        thirdWord[11] = destinationOperand[14];

        /* coding A,R,E field 100 */
        thirdWord[12] = '1';
        thirdWord[13] = '0';
        thirdWord[14] = '0';
        break;
      }
      /* if the source operand is DIRECT_REGISTER_SORT ex: r0 */
      case DIRECT_REGISTER_SORT:
      {
        /* coding 1000 in indexes 4,5,6,7 as 3,4,5,6 bits in a word */
        firstWord[8] = '1';
        firstWord[9] = '0';
        firstWord[10] = '0';
        firstWord[11] = '0';

        /* coding the third word in bit 3-5 */
        thirdWord[9] = destinationOperand[12];
        thirdWord[10] = destinationOperand[13];
        thirdWord[11] = destinationOperand[14];

        /* coding A,R,E field 100 */
        thirdWord[12] = '1';
        thirdWord[13] = '0';
        thirdWord[14] = '0';
        break;
      }
      default:
      {
        break;
      }

    }
    /* coding A,R,E field 100 of the first word*/
    firstWord[12] = '1';
    firstWord[13] = '0';
    firstWord[14] = '0';

    /* adding the first two words to the instruction table */
    addToInstructions(code,ic,firstWord);
    addToInstructions(code,ic,secondWord);
    /* checking if both source operand and destination operand are
    indirect register sort or direct register sort, if not, we add the third word
    to the instructions table, otherwise we dont need the third word because
    both shares a mutual word (the second word) */
    if(!((sourceCode == INDIRECT_REGISTER_SORT || sourceCode == DIRECT_REGISTER_SORT) && (destinationCode == INDIRECT_REGISTER_SORT || destinationCode == DIRECT_REGISTER_SORT)))
    {
      addToInstructions(code,ic,thirdWord);
    }
  }
  /* if we need to build 2 words */
  else if(L==2)
  {
    /* detecting the sorting method to code of the source operand */
    switch(sourceCode)
    {
      /* if the source operand is IMMEDIATE_SORT EX: '#'*/
      case IMMEDIATE_SORT:
      {
        /* coding 0001 in indexes 4,5,6,7 as 7,8,9,10 bits in a word */
        firstWord[8] = '0';
        firstWord[9] = '0';
        firstWord[10] = '0';
        firstWord[11] = '1';

        /* coding the second word in bit 3-12 */
        j=3;
        for(i = 0; i < 12; i++)
        {
          secondWord[i] = sourceOperand[j];
          j++;
        }
        /* coding A,R,E field 100 */
        secondWord[12] = '1';
        secondWord[13] = '0';
        secondWord[14] = '0';
        break;
      }
      /* if the source operand is DIRECT_SORT ex: LABLE */
      case DIRECT_SORT:
      {
        /* sourceOperand contains the binary word of the source operand */
        strcpy(secondWord,sourceOperand);
        /* coding 0010 in indexes 4,5,6,7 as 7,8,9,10 bits in a word */
        firstWord[8] = '0';
        firstWord[9] = '0';
        firstWord[10] = '1';
        firstWord[11] = '0';
        break;
      }
      /* if the source operand is INDIRECT_REGISTER_SORT EX: '*r0'*/
      case INDIRECT_REGISTER_SORT:
      {
        /* coding 0100 in indexes 4,5,6,7 as 7,8,9,10 bits in a word */
        firstWord[8] = '0';
        firstWord[9] = '1';
        firstWord[10] = '0';
        firstWord[11] = '0';

        /* coding the second word in bit 3-5 */
        secondWord[9] = sourceOperand[12];
        secondWord[10] = sourceOperand[13];
        secondWord[11] = sourceOperand[14];

        /* coding A,R,E field 100 */
        secondWord[12] = '1';
        secondWord[13] = '0';
        secondWord[14] = '0';
        break;
      }
      /* if the source operand is DIRECT_REGISTER_SORT ex: r0 */
      case DIRECT_REGISTER_SORT:
      {
        /* coding 1000 in indexes 4,5,6,7 as 3,4,5,6 bits in a word */
        firstWord[8] = '1';
        firstWord[9] = '0';
        firstWord[10] = '0';
        firstWord[11] = '0';

        /* coding the second word in bit 3-5 */
        secondWord[9] = sourceOperand[12];
        secondWord[10] = sourceOperand[13];
        secondWord[11] = sourceOperand[14];

        /* coding A,R,E field 100 */
        secondWord[12] = '1';
        secondWord[13] = '0';
        secondWord[14] = '0';
        break;
      }
      default:
      {
        break;
      }
    }
    /* coding A,R,E field 100 of the first word */
    firstWord[12] = '1';
    firstWord[13] = '0';
    firstWord[14] = '0';
    /* adding the first two words to the instruction table */
    addToInstructions(code,ic,firstWord);
    addToInstructions(code,ic,secondWord);
  }
  /* we only need to build one word */
  else
  {
    firstWord[12] = '1';
    firstWord[13] = '0';
    firstWord[14] = '0';

    addToInstructions(code,ic,firstWord);
  }
}

/* this function disects .string or .data directives line, build the words
and adds it to the data table.
@param data - pointer to the data table.
@param dc - data counter.
@param dataOrString - flag,contains string of .string or .data.
@param input - a line from the open file.
@param fileName - the name of the current open file.
@param line - line counter, contains the current line value.
*/
bool addData(ptrDATA *data,int *dc,char *dataOrString,char input[],char fileName[],int line)
{
  /* indexes */
  int i;
  int j;
  char temp[128];
  /* holds a number to be converted to binery and added to the data table from input */
  char num[128];
  /* the binary representation of num */
  char * number;
  /* the binary representation of a charachter in a string from input */
  char *string;

  /* checking if dataOrString flag is .data */
  if(!strcmp(dataOrString,"data"))
  {
    /* removing white spaces and tabs from the begining of input */
    i = removeSpace(0,input);
    /* */
    while(i < strlen(input) && input[i] != ' ' && input[i] != '\t')i++;
    /* removes .data from input */
    strcpy(temp,input+removeSpace(i,input));
    /* means no arguments  */
    if(strlen(temp) <= 0)
    {
      printf("%s:%d:\033[1;31merror:\033[0m missing argument in directive\n",fileName,line );
      return false;
    }

    /* running through input to identify errors and to extract arguments */
    for(; i <strlen(input); i++)
    {
      /* first we remove any white space or tabs */
      if(input[i] == ' ' || input[i] =='\t')
      {
        i = removeSpace(i,input);
      }
      /* we already checked for the comma so if a comma was found again its an error. */
      if(input[i] == ',')
      {
        printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,input+i );
        return false;
      }
      /* now we check if the current char is a digint from 0-9 or its '+' or '-' chars */
      else if(isdigit(input[i]) || ((input[i] == '+' || input[i] == '-') && isdigit(input[i+1])))
      {
        j = 0;
        if(input[i] == '-' || input[i] == '+')
        {
          /* means it is a negetive number we add '-' to num */
          if(input[i] == '-')
          {
            num[j] = input[i];
            i++;
            j++;
          }
          else
          {
            i++;
          }
        }
        /* as long as the current char is a digit we run the loop */
        while(isdigit(input[i]))
        {
          num[j] = input[i];
          i++;
          j++;
        }
        num[j] = '\0';
        /* if it is not a number */
        if(!isnumber(num))
        {
          printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,input+i );
          return false;
        }
        /* checking if the number is in range */
        if(atoi(num) <= 2047 && atoi(num) >= -2047)
        {
          number = toBInary(atoi(num));
          addToData(data,dc,number);
          free(number);
        }
        /* means that the number is out of range */
        else
        {
          /* if it is a negetive number */
          if(atoi(num) < 0)
          {
            printf("%s:%d:\033[1;31merror:\033[0m negative number too low to fit in 12 bits\n",fileName,line);
            return false;
          }
          else
          {
            printf("%s:%d:\033[1;31merror:\033[0m positive number too low to fit in 12 bits\n",fileName,line);
            return false;
          }
        }
        /* removing white spaces and tabs */
        if(input[i] == ' ' || input[i] =='\t')
        {
          i = removeSpace(i,input);
        }
        /* checking for errors after we extracted a number (each number should be seperated by comma)*/
        if(input[i] != ',' && input[i+1] != 0)
        {
          if(isdigit(input[i]))
          {
            printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,input+i );
            return false;
          }
          if(input[i] != 0)
          {
            printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,input+i );
            return false;
          }
          else if(isalpha(input[i]))
          {
            printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,input+i );
            return false;
          }
        }
        else if(input[i] == ',' && input[removeSpace(i+1,input)] == 0)
        {
          printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,input+i );
          return false;
        }
      }
      else
      {
        printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s)\n",fileName,line,input+i );
        return false;
      }

    }
    return true;
  }
  /* checking if dataOrString flag is .string */
  else if(!strcmp(dataOrString,"string"))
  {
    /* removing .string from input and copy to temp */
    i = removeSpace(0,input);
    while(i < strlen(input) && input[i] != ' ' && input[i] != '\t')i++;
    i = removeSpace(i,input);

    strcpy(temp,input+removeSpace(i,input));

    /* means no arguments were given */
    if(strlen(temp) <= 0)
    {
      printf(" %s:%d:\033[1;31merror:\033[0m missing argument in directive\n",fileName,line );
      return false;
    }

    /* each string should be open and close with '\"' */
    if(temp[0] != '\"')
    {
      printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s is not a string)\n",fileName,line,temp);
      return false;
    }

    /* removing white spaces and tabs from the end of the string */
    i = 1;
    while(i < strlen(temp))
    {
      if(temp[i] == '\"')break;
      i++;
    }
    /* saving i index in j */
    j = i;

    /* if the string does not ends with " char or */
    if(temp[i] != '\"')
    {
      printf("%s:%d:\033[1;31merror:\033[0m invalid characters (%s is not a string)\n",fileName,line,temp+i);
      return false;
    }

    i = removeSpace(i+1,temp);
    /* if there is extra charachter after the end of the string */
    if(temp[i] != ' ' && temp[i] != '\t' && temp[i] != 0)
    {
      printf("%s:%d:\033[1;31merror:\033[0m extra charachters at the end of the string (%s is not a string)\n",fileName,line,temp+i);
      return false;
    }
    temp[j+1] = '\0';


    /* running in a loop to extract the string and add it to data table */
    for(i = 1; i < strlen(temp)-1; i++)
    {
      /* checking if the ascii code of the current char is in range */
      if((temp[i] >= 32 && temp[i] <= 128) || temp[i] == '\t')
      {
        /* converting the asccii code to binary */
        string = toBInary(temp[i]);
        /* adding to data table */
        addToData(data,dc,string);
        free(string);
      }
      else
      {
        printf("%s:%d:\033[1;31merror:\033[0m invalid characters (out of range)\n",fileName,line);
        return false;
      }
    }

    /* adding '\0' to data table */
    string = toBInary('\0');
    addToData(data,dc,string);
    free(string);
    return true;
  }
  return false;
}
