#ifndef HEADER_CELLS_H
#define HEADER_CELLS_H
#endif

#include<stdio.h>
#include<stdlib.h>

#define ALIVE 'M' // ASCII 219 for black box 
#define DEAD  '-' //These can be change for something more visible later on.

//Might not need this.
typedef enum {false, true} bool;

typedef struct ruleDef{
  int left;
  int parent;
  int right;
  int use;
} ruleDef;

//The main bit
int cellularAutomaton(int rule, int* ruleBin, int length, int yRows, ruleDef* ruleDefs);
int printRow(int* row, int length);
int checkCell(int* parent, int* child, int i, ruleDef* rDs, int length);
int readFile();

//Cell Rules
int ruleSet(int* bin, ruleDef* ruleDefs);
int detDefs(int* bin, int* defs);

//Display arrays
int printBin(int* bin);
int printBinWithGuide(int* bin);

//Value conversion
int convertDec(int dec, int* bin); //Converts decimal to binary.
int convertBin(int* bin, int* dec);//Converts binary to decimal.

//Change variables
int changeVarRuleDec(int* rule);
int changeVarRuleBin(int* ruleBin);
int changeVarLength(int* length);

//Testing
int testValues();
int testRuleSet(ruleDef* rulesAsDef);

//Error checking
int checkValidNum(); //Checks that the user has input a valid int.
