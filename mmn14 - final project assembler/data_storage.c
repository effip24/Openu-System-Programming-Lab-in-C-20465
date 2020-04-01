/*
mmn14 - two pass assembler model based on imaginary computer
and imaginary assembly language.
data_storage.c - this file has all the functions that resposibles
to build and perform types on the data structures.
(linked lists, the instruction, data, symbols tables).
@author - Ephraim Phil.
@version - 10/03/2020. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "utilities.h"
#include "data_storage.h"

/* -----------------------ADDers ---------------------------------------------*/
/*
this function creates a new node and adds it to the instructions table(linked list).
@param code - pointer to the instructions table, a linked list.
@param ic - pointer to the instruction counter.
@param binaryAddress - the address of the new word to be added.
*/
void addToInstructions(ptrCODE *code,int *ic,char binaryAddress[])
{
  /* p1,p2 are temporary pointers, newWord is the new node to be added */
  ptrCODE p1,p2,newWord;

  /* memory allocation for the new word */
  newWord = (ptrCODE)malloc(sizeof(CODE));

  if(!newWord)
  {
    puts("memory allocation failed!");
    exit(1);
  }

  /* the decimal address of the new word will be ic+100 */
  newWord->address = *ic+100;
  /* increasing ic+1, because we added new word */
  *ic+=1;
  /* copies the binary address of the new word */
  strcpy(newWord->binaryAddress,binaryAddress);
  /* external is a flag, setting to NULL because the new word is not external */
  strcpy(newWord->external,"NULL");

  /* checks if the instructions table is empty */
  if(!(*code))
  {
    newWord->next = NULL;
    /* setting the new node to be the head of the list(the first node) */
    *code = newWord;
    return;
  }

  /* if the list is not empty we place the new node at the end of the list */
  /* setting p1 to be the head, p1 is temporary pointer so we dont lose the head pointer */
  p1 = *code;
  /* running till the end of the list, the new node will be added last */
  while((p1))
  {
    p2 =p1;
    p1 = p1->next;
  }
  /* placing the new node at the end of the list */
  newWord->next = p1;
  p2->next = newWord;
}

/*
this function creates a new node and adds it to the data table(linked list).
@param data - pointer to the data table, a linked list.
@param dc - pointer to the data counter.
@param binaryAddress - the address of the new data to be added.
*/
void addToData(ptrDATA *data,int *dc,char binaryAddress[])
{
  /* p1,p2 are temporary pointers, newData is the new node to be added */
  ptrDATA p1,p2,newData;

  newData = (ptrDATA)malloc(sizeof(DATA));

  /* if the memory allocation failed */
  if(!newData)
  {
    puts("memory allocation failed!");
    exit(1);
  }

  /* setting the decimal address of the new data */
  newData->address = *dc;
  /* copies the binary address of the new data */
  strcpy(newData->binaryAddress,binaryAddress);
  /* increasing data counter by 1, we added new data */
  *dc+=1;

  /* if the data table is empty */
  if(!(*data))
  {
    /* placing the new node at the first */
    newData->next = NULL;
    *data = newData;
    return;
  }

  /* if the list is not empty we place the new node at the end of the list */
  /* setting p1 to be the head, p1 is temporary pointer so we dont lose the head pointer */
  p1 = *data;
  /* running till the end of the list, the new node will be added last */
  while((p1))
  {
    p2 =p1;
    p1 = p1->next;
  }
  /* placing the new node at the end of the list */
  newData->next = p1;
  p2->next = newData;
}

/*
this function creates a new node and adds it to the symbols table(linked list).
@param symbols - pointer to the symbols table, a linked list.
@param address - the decimal address of each symbol.
@param symbol - the symbol name.
@param type - flag, indicates if the symbol is .extern or .entry.
*/
void addToSymbols(ptrSYMBOLS *symbols,int address,char symbol[],char type[])
{
  /* p1,p2 are temporary pointers, newSymbol is the new node to be added */
  ptrSYMBOLS p1,p2,newSymbol;

  /* memory allocation for the new symbol node */
  newSymbol = (ptrSYMBOLS)malloc(sizeof(SYMBOLS));

  /*if the memory allocation failed */
  if(!newSymbol)
  {
    puts("memory allocation failed!");
    exit(1);
  }

  /* the decimal address of the new symbol */
  newSymbol->address = address;
  /* the symbol name */
  strcpy(newSymbol->symbol,symbol);
  /* flag, the type of the symbol, local/extern/entry */
  strcpy(newSymbol->type,type);

  /* if the list is empty */
  if(!(*symbols))
  {
    /* placing the new node at the first place */
    newSymbol->next = NULL;
    *symbols = newSymbol;
    return;
  }

  /* if the list is not empty we place the new node at the end of the list */
  /* setting p1 to be the head, p1 is temporary pointer so we dont lose the head pointer */
  p1 = *symbols;
  /* running till the end of the list, the new node will be added last */
  while((p1) )
  {
    p2 =p1;
    p1 = p1->next;
  }
  /* placing the new node at the end of the list */
  newSymbol->next = p1;
  p2->next = newSymbol;
}



/* ---------------------UPDATErs ---------------------------------------------*/
/*
this function updates the decimal address of the symbols that classified as data
at the second pass.
@param symbols - pointer to the symbols table, a linked list.
@param ic - pointer to the instruction counter.
*/
void updateSymbolsTable(ptrSYMBOLS *symbols,int *ic)
{
  /* temp pointer to the head of the list */
  ptrSYMBOLS ptr;
  ptr = *symbols;

  /* running through the list */
  while((ptr))
  {
    /* checking if the type flag is data, we update all the symbols the has not been updated yet
    we know that by checking if their decimal address is from 0-100.
    */
    if(!strcmp((ptr)->type,".data") || ((ptr)->address < 100 && (ptr)->address >= 0))
    {
      ptr->address += *ic+100;
    }
    (ptr) = (ptr)->next;
  }
}

/*
this function updates the type flag of a symbol at the second pass to .entry.
@param symbols - pointer to the symbols table, a linked list.
@param symbol - the symbol to update its flag.
*/
void updateSymbolType(ptrSYMBOLS *symbols,char symbol[])
{
  /* temp pointer to symbols head */
  ptrSYMBOLS ptr;
  ptr = *symbols;

  /* running through the list */
  while((ptr))
  {
    /* if the current symbol is the symbol to be updated */
    if(!strcmp((ptr)->symbol,symbol))
    {
      /* changing the symbols types flag to .entry */
      strcpy(ptr->type,"");
      strcpy(ptr->type,".entry");
    }
    (ptr) = (ptr)->next;
  }
}

/*
this function updates the binary adresses of all the lables in the instructions table.
the function gets called at the end of the second pass.
@param code - pointer to the instructions table, a linked list.
@param symbols - pointer to the symbols table, a linked list.
*/
void updateInstructions(ptrCODE *code,ptrSYMBOLS *symbols)
{
  /* the decimal address of the lable */
  int symbolAddress;
  /* the binary address of the lable */
  char word[16];
  char * binary;

  /* temp pointer to the head of the list */
  ptrCODE ptr;
  ptr = *code;

  /* setting the binary address to be 0 */
  strcpy(word,"000000000000000");

  /* running through the list, search for lable, update its binary address and decimal address */
  while((ptr))
  {
    /* indicated we found a lable in the list */
    if(isalpha(ptr->binaryAddress[0]))
    {
      /* getting the decimal address of the lable from the symbols table */
      symbolAddress = getSymbolAddress(ptr->binaryAddress,symbols);
      /* checks if tha lable is external */
      if(symbolAddress == -1)
      {
        /* coding the A,R,E */
        word[14] = '1';
        word[13] = '0';
        word[12] = '0';
        strcpy(ptr->external,"");
        /* hold the binary address of external lables */
        strcpy(ptr->external,ptr->binaryAddress);
        strcpy(ptr->binaryAddress,"");
        /* copies the binary address of the lable */
        strcpy(ptr->binaryAddress,word);
        strcpy(word,"000000000000000");
      }
      else
      {
        /* converting the decimal address of the lable to binary */
        binary = toBInary(symbolAddress);
        /* coding the A,R,E field */
        word[14] = '0';
        word[13] = '1';
        word[12] = '0';

        /* coding the rest of the binary address of the lable  */
        word[11] = binary[14];
        word[10] = binary[13];
        word[9] = binary[12];
        word[8] = binary[11];

        word[7] = binary[10];
        word[6] = binary[9];
        word[5] = binary[8];
        word[4] = binary[7];

        word[3] = binary[6];
        word[2] = binary[5];
        word[1] = binary[4];
        word[0] = binary[3];

        /* copies the binary address of the lable */
        strcpy(ptr->binaryAddress,"");
        strcpy(ptr->binaryAddress,word);
        strcpy(word,"000000000000000");
        free(binary);
      }
    }
    ptr = ptr->next;
  }
}

/*
this function updates the decimal addresses of the data table.
the function gets called at the end of the second pass.
@param data - pointer to the data table, a linked list.
@param ic - pointer to the instruction counter.
*/
void updateData(ptrDATA *data,int *ic)
{
  /* temp pointer to the head of the list */
  ptrDATA ptr;
  ptr = *data;

  /* running till the end of the list */
  while((ptr))
  {
    /* update each address to ic+100 */
    ptr->address += *ic+100;
    (ptr) = (ptr)->next;
  }
}



/*----------------------WRITErs ----------------------------------------------*/
/*
this function writes the instructions table to a given file.
@param code - pointer to the instructions table, a linked list.
@param file - the file to write all the data to.
*/
void writeInstructions(ptrCODE *code,FILE *file)
{
  /* will hold the converted number from binary to base 8*/
  int oct;
  /* temp pointer to the head of the list */
  ptrCODE ptr;
  ptr = *code;

  /* running till the end of the list */
  while((ptr))
  {
    /* convert the current address from binary to base8 */
    oct = convertToOctal(ptr->binaryAddress);

    /* completing the decimal address by adding 0's */
    if(ptr->address < 1000)
    {
      fprintf(file,"0");
    }

    fprintf(file,"%d  ", ptr->address);

    if(oct >= 1000 && oct < 10000)
    {
      fprintf(file,"0");
    }
    else if(oct >= 100 && oct < 1000)
    {
      fprintf(file,"00");
    }
    else if(oct >= 10 && oct < 100)
    {
      fprintf(file,"000");
    }
    else if(oct < 10)
    {
      fprintf(file,"0000");
    }
    fprintf(file,"%d\n",oct);

    ptr = ptr->next;
  }
}

/*
this function writes the data table to a given file.
@param data - pointer to the data table, a linked list.
@param file - the file to write all the data to.
*/
void writeData(ptrDATA *data,FILE *file)
{
  /* will hold the converted number from binary to base 8*/
  int oct;
  /* temp pointer to the head of the list */
  ptrDATA ptr;
  ptr = *data;

  while((ptr))
  {
    /* convert the current address from binary to base8 */
    oct = convertToOctal(ptr->binaryAddress);

    /* completing the decimal address by adding 0's */
    if(ptr->address < 1000)
    {
      fprintf(file,"0");
    }

    fprintf(file,"%d  ", ptr->address);

    if(oct >= 1000 && oct < 10000)
    {
      fprintf(file,"0");
    }
    else if(oct >= 100 && oct < 1000)
    {
      fprintf(file,"00");
    }
    else if(oct >= 10 && oct < 100)
    {
      fprintf(file,"000");
    }
    else if(oct < 10)
    {
      fprintf(file,"0000");
    }
    fprintf(file,"%d\n",oct);

    ptr = ptr->next;
  }
}

/*
this function writes the entries to .ent file and externals to .ext file.
@param symbols - pointer to the symbols table, a linked list.
@param code - pointer to the instructions table, a linked list.
@param flag - indicated which file to write .ext/.ent.
@param file - the file to write all the data to.
*/
void writeEntrysOrExternals(ptrSYMBOLS *symbols,ptrCODE *code,char flag[], FILE *file)
{
  /* temp pointer to the head of the symbols list */
  ptrSYMBOLS ptr1;
  /* temp pointer to the head of the instructions list */
  ptrCODE ptr2;

  ptr1 = *symbols;
  ptr2 = *code;

  /* if the flag is entry we write to the .ent file */
  if(!strcmp(flag,"entry"))
  {
    /* running through the end of the list */
    while((ptr1))
    {
      /* if the current lable type is entry */
      if(!strcmp(ptr1->type,".entry"))
      {
        fprintf(file,"%s ",ptr1->symbol);
        /* completing 0 to the decimal address */
        if(ptr1->address < 1000)
        {
          fprintf(file,"0");
        }
        fprintf(file,"%d\n",ptr1->address );
      }

      ptr1 = ptr1->next;
    }
  }

  /* if the flag is extern we write to the .ext file */
  else if(!strcmp(flag,"extern"))
  {
    /* running through the end of the list */
    while((ptr2))
    {

      /* if the word external flag is not null it means that it is external */
      if(strcmp(ptr2->external,"NULL"))
      {
        fprintf(file,"%s ", ptr2->external);

        /* completing 0 to the decimal address */
        if(ptr2->address < 1000)
        {
          fprintf(file,"0");
        }
        fprintf(file,"%d\n",ptr2->address );
      }

      ptr2 = ptr2->next;
    }
  }
}



/*------------------CHECKERs -------------------------------------------------*/
/*
this function checks if a lable is not ignored.
@param symbols - pointer to the symbols table, a linked list.
@param symbol - the lable name.
return true if it is not ignored.
*/
bool checkIfSymbolValid(ptrSYMBOLS *symbols,char symbol[])
{
  /* temp pointer to the head of the list */
  ptrSYMBOLS p = *symbols;

  /* running till the end of the list */
  while((p))
  {
    /* if the current symbol is the symbol to be checked */
    if(!strcmp(p->symbol,symbol))
    {
      /* if it is ignored return false (not valid) */
      if(!strcmp(p->type,"ignored"))return false;
    }
    p = p->next;
  }
  return true;
}

/*
this function checks if a lable is local.
@param symbols - pointer to the symbols table, a linked list.
@param symbol - the lable name.
return true if the lable is local.
*/
bool checkIfSymbolLocal(ptrSYMBOLS *symbols,char symbol[])
{
  /* temp pointer to the head of the list */
  ptrSYMBOLS p = *symbols;
  while((p))
  {
    /* if it is the symbol to be checked */
    if(!strcmp(p->symbol,symbol))
    {
      /* checks if the symbol is local */
      if(!strcmp(p->type,".code"))return true;
    }
    p = p->next;
  }
  return false;
}

/*
this function checks if there is .extern/.entry lables in the symbols table.
@param symbols - pointer to the symbols table, a linked list.
@param flag - the type flag.
return true if there is .extern/.entry lables in the symbols table.
*/
bool checkExternOrEntry(ptrSYMBOLS *symbol,char flag[])
{
  /* pointer to the head of the list */
  ptrSYMBOLS ptr;
  ptr = *symbol;

  /* checks if the flag is extern */
  if(!strcmp(flag,"extern"))
  {
    /* running till the end of the list */
    while((ptr))
    {
      /* if it is external lable*/
      if(!strcmp(ptr->type,".extern"))
      {
        return true;
      }
      ptr = ptr->next;
    }
  }
  /* checks if the flag is entry */
  else if(!strcmp(flag,"entry"))
  {
    /* running till the end of the list */
    while((ptr))
    {
      /* if it is entry lable*/
      if(!strcmp(ptr->type,".entry"))
      {
        return true;
      }
      ptr = ptr->next;
    }
  }
  return false;
}

/*
this function checks if a lable was declared as external in the first pass.
@param symbols - pointer to the symbols table, a linked list.
@param symbol - the lable to be checked.
return true if it is external.
*/
bool checkIfDesignatedExternal(ptrSYMBOLS *symbols,char symbol[])
{
  /* pointer to the head of the list */
  ptrSYMBOLS p = *symbols;

  /* running through the end of the list */
  while((p))
  {
    /* checks if it the lable to be checked */
    if(!strcmp(p->symbol,symbol))
    {
      /* if the lable is external */
      if(!strcmp(p->type,".extern"))return true;
    }
    p = p->next;
  }
  return false;
}



/*------------------GETER-----------------------------------------------------*/
/*
this function return the decimal address of a lable.
@param symbols - pointer to the symbols table, a linked list.
@param symbol - the lable to be checked.
return the decimal address of a lable.
*/
int getSymbolAddress(char symbol[],ptrSYMBOLS *symbols)
{

  ptrSYMBOLS ptr;
  ptr = *symbols;


  while((ptr))
  {
    if(!strcmp(ptr->symbol,symbol))
    {
      return ptr->address;
    }
    ptr = ptr->next;
  }
  return -500;
}




/* this function checks if a lable is in the symbols table.
@param symbols - pointer to the symbols table, a linked list.
@param symbol - the lable to be checked.
return true if a lable is in the symbols table.
*/
bool notInSymbols(ptrSYMBOLS *symbols,char *symbol)
{
  /* temp pointer to the symbol table */
  ptrSYMBOLS sym;

  sym = *symbols;
  /* running through the list */
  while(sym)
  {
    /* if the symbol name is in the simbols table */
    if(strcmp(sym->symbol,symbol) == 0)
    {
      return false;
    }
    sym = sym->next;
  }
  return true;
}


/* this function frees the symbols table(linked list).
@param symbols - the symbols table, a linked list.
*/
void freeSymbols(ptrSYMBOLS symbols)
{
  ptrSYMBOLS t;
  while(symbols)
  {
    t = symbols;
    symbols = symbols->next;
    free(t);
  }
}

/* this function frees the instructions table(linked list).
@param symbols - the symbols table, a linked list.
*/
void freeInstrcutions(ptrCODE code)
{
  ptrCODE t;
  while(code)
  {
    t = code;
    code = code->next;
    free(t);
  }
}

/* this function frees the data table(linked list).
@param symbols - the symbols table, a linked list.
*/
void freeData(ptrDATA data)
{
  ptrDATA t;
  while(data)
  {
    t = data;
    data = data->next;
    free(t);
  }
}
