/*
mmn14 - two pass assembler model based on imaginary computer
and imaginary assembly language.
data_storage.h - header file, contains the functions prototypes of data_storage.c
@author - Ephraim Phil 205390156.
@version - 10/03/2020. */

#ifndef DATA_STORAGE

#define EXTERNAL 8
#define ADDRESS 16
#define SYMBOL 33
#define TYPE 8

/* linked list, the instructions table */
typedef struct CODE * ptrCODE;
typedef struct CODE
{
  /* the decimal address of each word */
  unsigned short address;
  /* external flag, incase the word has a lable address, the flag indicates if the lable is external*/
  char external[EXTERNAL];
  /* the binary address of each word */
  char binaryAddress[ADDRESS];
  /* points at the next node */
  ptrCODE next;
}CODE;

/* linked list, the data table */
typedef struct DATA * ptrDATA;
typedef struct DATA
{
  /* the decimal address of each word */
  unsigned short address;
  /* the binary address of each word in the data table */
  char binaryAddress[ADDRESS];
  /* points at the next node */
  ptrDATA next;
}DATA;

/* linked list, the symbols table */
typedef struct SYMBOLS * ptrSYMBOLS;
typedef struct SYMBOLS
{
  /* the decimal address of each word */
  int address;
  /* the name of the symbols */
  char symbol[SYMBOL];
  /* flag, indicates if the symbol is external/entry */
  char type[TYPE];
  /* points at the next node */
  ptrSYMBOLS next;
}SYMBOLS;

/* -----------------------ADDers ---------------------------------------------*/
/*
this function creates a new node and adds it to the symbols table(linked list).
*/
void addToSymbols(ptrSYMBOLS *, int , char[],char[]);

/*
this function creates a new node and adds it to the instructions table(linked list).
*/
void addToInstructions(ptrCODE *,int *,char[]);

/*
this function creates a new node and adds it to the data table(linked list).
*/
void addToData(ptrDATA *, int *, char[]);


/* ---------------------UPDATErs ---------------------------------------------*/
/*
this function updates the decimal address of the symbols that classified as data
*/
void updateSymbolsTable(ptrSYMBOLS *,int *);

/*
this function updates the type flag of a symbol at the second pass to .entry.
*/
void updateSymbolType(ptrSYMBOLS *, char []);

/*
this function updates the binary adresses of all the lables in the instructions table.
the function gets called at the end of the second pass.
*/
void updateInstructions(ptrCODE *,ptrSYMBOLS *);

/*
this function updates the decimal addresses of the data table.
the function gets called at the end of the second pass.
*/
void updateData(ptrDATA *, int *);


/*----------------------WRITErs ----------------------------------------------*/
/*
this function writes the instructions table to a given file.
*/
void writeInstructions(ptrCODE *,FILE * );

/*
this function writes the data table to a given file.
*/
void writeData(ptrDATA *,FILE * );

/*
this function writes the entries to .ent file and externals to .ext file.
*/
void writeEntrysOrExternals(ptrSYMBOLS *,ptrCODE *,char[], FILE * );


/*------------------CHECKERs -------------------------------------------------*/
/*
this function checks if a lable is not ignored.
*/
bool checkIfSymbolValid(ptrSYMBOLS *,char[]);

/*
this function checks if a lable is local.
*/
bool checkIfSymbolLocal(ptrSYMBOLS *,char[]);

/*
this function checks if there is .extern/.entry lables in the symbols table.
*/
bool checkExternOrEntry(ptrSYMBOLS *,char[]);

/*
this function checks if a lable was declared as external in the first pass.
*/
bool checkIfDesignatedExternal(ptrSYMBOLS *,char[]);


/*------------------GETERs---------------------------------------------------*/
/*
this function return the decimal address of a lable.
*/
int getSymbolAddress(char[],ptrSYMBOLS *);



/*
this function checks if a lable is in the symbols table.
*/
bool notInSymbols(ptrSYMBOLS *,char *);

/*
this function frees the instructions table(linked list).
*/
void freeInstrcutions(ptrCODE code);

/*
this function frees the symbols table(linked list).
*/
void freeSymbols(ptrSYMBOLS symbols);

/*
this function frees the data table(linked list).
*/
void freeData(ptrDATA data);

#endif
