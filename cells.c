#include<stdio.h>
#include<stdlib.h>
#include"cells.h"   //All cells.c functions are prototyped in this header

//Main menu, where the magic happens.
int main(){

  //Initialize to the infamous Rule 30
  int rule = 30;
  //Declare a binary array and convert the rule to it.
  //This could be done by direct declaration but we already have the funky function for it soooooo.
  int ruleInBin[8] = {0};
  //Immediately convert the rule.
  convertDec(rule, ruleInBin);

  //Initialize definitions of the rule.
  ruleDef ruleDefs[8];
  ruleSet(ruleInBin, ruleDefs);
  
  //Initialize definition count to be shown.
  int defs = 0;
  detDefs(ruleInBin, &defs);

  //How many cells on the X axis
  int length = 31; // For compatibility with older code this needs to keep the length name.
  //How many rows to print
  int yLength = 30;

  //Initialize to a ridiculous value that isn't 0.
  int choice=99;


  while(choice != 0){
    printf("\nWelcome to the Cellular Automata machine.\n");
    
    //Current user variables.
    printf("Rule: %d\n", rule);
    printf("Rule in binary: ");
    printBin(ruleInBin);
    printf("\nLegnth: %d\n", length);
    printf("\nRows: %d\n", yLength);
    printf("This rule has %d definitions.\n", defs);
    printf("This rule will print when the parent cells equal:");
    testRuleSet(ruleDefs);

    //Menu contents
    printf("\n----------------------------\n");
    printf("1. Run with current variables.\n");
    printf("2. Change rule (decimal).\n");
    printf("3. Change rule (binary).\n");
    printf("4. Change length.\n");
    printf("5. Change rows.\n");
    printf("6. Print saved file.\n");
    printf("7. Verify rule.\n");    
    printf("8. Test data functions.\n");
    printf("0. Exit Program\n");
    printf("----------------------------\n");   

    //Check for integers.
    choice = checkValidNum();    
    switch(choice){
    case 0:
      printf("Goodbye.\n");
      return 0;
      break;
    case 1: 
      cellularAutomaton(rule, ruleInBin, length, yLength, ruleDefs);
      break;
    case 2:
      changeVarRuleDec(&rule);
      convertDec(rule, ruleInBin);
      ruleSet(ruleInBin, ruleDefs);
      detDefs(ruleInBin, &defs);
      break;
    case 3: 
      changeVarRuleBin(ruleInBin);
      convertBin(ruleInBin, &rule);
      ruleSet(ruleInBin, ruleDefs);
      detDefs(ruleInBin, &defs);
      break;
    case 4:
      printf("How long do you want each row to be? ");
      length = checkValidNum();
      break;
    case 5:
      printf("How many rows do you want there to be? ");
      yLength = checkValidNum();
      break;
    case 6:
      readFile();
      break;
    case 7:
      printBinWithGuide(ruleInBin);
      break;
    case 8:
      testValues();
      break;
    default:
      printf("Banks gone bust");
      break;
    }
  }
  
  return 0;
}

int cellularAutomaton(int rule, int* ruleBin, int length, int yRows, ruleDef* ruleDefs){
  //How long the left to right will be. There seems to be no way to define this dynamically.
  int xLength = length;
  int yLength = yRows;

  //Generations
  int parent[xLength];
  int child[xLength];

  //Initialize to 0.
  for(int i=0; i<xLength; i++){
    parent[i] = 0;
    child[i] = 0;
  }

  //Define the starting point in the middle.
  parent[(xLength/2)] = 1;
  printRow(parent, length);
  
  //This overwrites the previous file.
  FILE* pFile;
  pFile = fopen("cell.txt", "w");
  fprintf(pFile,"This is the saved cellular automaton printout.\n");
  fclose(pFile);

  for(int i = 0; i < yLength; i++){
    //New line before printing a row of cells.
    for(int j = 0; j < xLength; j++){
      checkCell(parent, child, j, ruleDefs, xLength);
    }
    //Prints the child row.
    printRow(child, length);
    
    //Makes child the new parent.
    for(int j = 0; j < xLength; j++){
      parent[j] = child[j];
    }
    //Resets child since it will carry over 1s and break the rules if you dont.
    for(int j=0; j<xLength; j++){
      child[j] = 0;
    }
  }
  printf("\n\nThis has been saved to the file \"cells.txt\" for you.\n");
  return 0;
}

int checkCell(int* parent, int* child, int i, ruleDef* rDs, int length){
  //Left most case
  if( i == 0 ){ 
    for(int r = 0; r < 8; r++){
        //Checks if there is a valid rule / if the rule is valid.
        if(rDs[r].left == 0 && rDs[r].parent == parent[i] && rDs[r].right == parent[(i+1)]){
          child[i] = 1;
          return 0;
        }
    }
  }
  //Right most case
  else if( i == (length - 1)){
    for(int r = 0; r < 8; r++){
      //Checks if there is a valid rule / if the rule is valid.
      if(rDs[r].left == parent[(i-1)] && rDs[r].parent == parent[i] && rDs[r].right == 0){
        child[i] = 1;
        return 0;
      }
    }
  }
  //Every other case
  else{
    for(int r = 0; r < 8; r++){
      //Checks if there is a valid rule / if the rule is valid.
      if(rDs[r].left == parent[(i-1)] && rDs[r].parent == parent[i] && rDs[r].right == parent[(i+1)]){
        child[i] = 1;
        return 0;
      }
    }
  }
  return 0;
}

int printRow(int* row, int length){
  //File saving.
  FILE* pFile;
  pFile = fopen("cell.txt", "a");

  //New Line to seperate starting position from last line printed.
  printf("\n");
  for(int i = 0; i < length; i++){
    if(row[i] == 0){
      printf("%c",DEAD);
      fprintf(pFile, "%c" ,DEAD);
    }
    else{
      printf("%c",ALIVE);
      fprintf(pFile, "%c" ,ALIVE);
    }
  }
  fprintf(pFile, "\n");
  fclose(pFile);
  return 0;
}

int readFile(){
  //Declare file and read intention.
  FILE *pFile;
  pFile = fopen("cell.txt", "r");
  
  int i;
  while ((i = getc(pFile)) != EOF){
    putchar(i);
  }
  fclose(pFile);
  return 0;
}

int ruleSet(int* bin, ruleDef* ruleDefs){
  //Create new temporary ruleDef struct to store values.
  int j = 7;

  for(int i = 0; i < 8; i++){
    int threeBits[8];
    convertDec(i,threeBits);

    if(bin[j] == 1){
      ruleDefs[i].left = threeBits[5];
      ruleDefs[i].parent = threeBits[6];
      ruleDefs[i].right = threeBits[7];
      ruleDefs[i].use = 1;
    }
    else{
      ruleDefs[i].left = 2;
      ruleDefs[i].parent = 2;
      ruleDefs[i].right = 2;
      ruleDefs[i].use = 0;
    }
    //Decrement j
    j--;
  }

  return 0;
}

int detDefs(int* bin, int* defs){
  int nDefs = 0;

  for(int i = 0; i < 8; i++){
    if(bin[i]==1){
      nDefs++;
    }
  }

  *defs = nDefs;
  return 0;
}

int printBin(int* bin){
  //Check for null pointer.
  if(bin == NULL){
    printf("NULL POINTER");
    return 0;
  }

  for(int i = 0; i < 8; i++){
    printf("%d",bin[i]);
  }
  return 0;
}

int printBinWithGuide(int* bin){
  //Check for null pointer.
  if(bin == NULL){
    printf("NULL POINTER");
    return 0;
  }

  int nn =128;

  for(int i = 0; i < 8; i++){
    printf("%d\t",nn);
    nn = nn /2;
  }

  //New line so it doesnt look ridiculous.
  printf("\n");

  for(int i = 0; i < 8; i++){
    printf("%d\t",bin[i]);
  }
  return 0;
}

int convertDec(int dec, int* bin){
  if(bin == NULL){
    printf("NULL POINTER");
    return 0;
  }
  //8 bits, anything higher than 8 255 will break it.

  //128 is the 8th bit
  int divver = 128;
  //String to store binary value
  int binString[8] = {0};

  //Operate 8 times for 8bit value
  for(int i = 0; i < 8; i++){
    int isone = dec / divver;
    int decr = dec%divver;
    if(isone != 1){
      binString[i] = 0;
    }
    else{
      binString[i] = 1;
      dec=decr;
    }
    bin[i] = binString[i];
    divver = divver / 2;
  }

  return 0;
}

int convertBin(int* bin, int* dec){
  //Check if a null pointers weres given.
  if(bin == NULL){
    printf("NULL POINTER");
    return 0;
  }
  if(dec == NULL){
    printf("NULL POINTER");
    return 0;
  }

  //Assign the value of the 8th bit (array is left to right)
  int bit = 128;
  //Declare value for passback
  int conv = 0;

  //Loops until eighth bit, adding when bit is valid, bit is halved on each loop
  for(int i = 0; i < 8; i++){
    if(bin[i] == 1){
      conv = conv+bit;
    }
    else{
      conv = conv;
    }
    bit = bit /2;
  }
  //Feed the pointer the newly converted value.
  *dec = conv;

  //Return 0, I have no idea what return codes to use / if to use?
  return 0;
}

int changeVarRuleDec(int* ruleDec){
  printf("\nWhat rule (decimal) do you want to use?\n");
  *ruleDec = checkValidNum();
  if(*ruleDec <0 || *ruleDec > 255){
    printf("Invalid option please try again.");
    changeVarRuleDec(ruleDec);
  }
  printf("\n");
  return 0;
}

int changeVarRuleBin(int* ruleBin){
  printf("\nWhat rule (binary) do you want to use?\n");
  char binaryIn[100]={0};
  int binBack[8];

  //Input string (this is an error check as well as a method of input.
  scanf("%s",binaryIn);

  //Check each character in the array.
  for(int i = 0; i < 8; i++){
      if(binaryIn[i] == '1'|| binaryIn[i] == 1 + '0'){
        binBack[i] = 1;
      }
      else if(binaryIn[i] == '0'|| binaryIn[i] == 0 + '0'){
        binBack[i] = 0;
      }
      else{
        printf("Character %d is invalid: %c\n", (i+1), binaryIn[i]);
        return 0;
      }
  }
  
  for(int i = 0; i < 8; i++){
    ruleBin[i] = binBack[i];
  }
  return 0;
}

int changeVarLength(int* length){
  return 0;
}

int testValues(){

  //Declare test variables for pre-test testing.
  int testBin[8]={0};
  int testDec = 30;
  int testDef = 0;
  ruleDef testRuleDef[8];

  printf("\n\n---TESTING OUTPUT---\n");

  //Inform the user of the testing variable being used.
  printf("For testing purposes we will be using the integer value: %d\n\n", testDec);

  //Test initialization was fulfilled correctly.
  printf("This is a newly initialized binary array, it should be all 0s.\nBinary: ");
  printBin(testBin);

  //Convert the testing decimal value to binary and store it in the testing Binary.
  printf("\n\nThis is the testing integer converted to binary.\nBinary: ");
  convertDec(testDec, testBin);
  printBin(testBin);

  //Print the Binary value with an 8 bit guide.
  printf("\nThis is the same value but with a guide to help verify it is correct.\n");
  printBinWithGuide(testBin);

  //Test the binary to decimal conversion.
  printf("\n\nThe integer value has been converted from binary and will be printed below.");
  printf("\nIt should equal %d",testDec);
  convertBin(testBin, &testDec);
  printf("\nInteger: %d\n",testDec);

  //Explain what definition are.
  printf("\nDefinitions are the number of combinations that can produce children.");

  //Test initialization was corrrect.
  printf("\nThis is to test that testDef was initialized to 0: %d\n",testDef);

  //Determine the number of definitions and display based on the rule.
  detDefs(testBin, &testDef);
  printf("\nHow many definitions does this rule have: %d.",testDef);

  //These are the definitions of the rule.
  printf("\n\nThe following are definitions (combinations that produce a child) of the rule:");
  ruleSet(testBin, testRuleDef);
  testRuleSet(testRuleDef);
  printf("\nIf there are any values other than a 1 or a 0 in these definitions then something is wrong.");

  printf("\n---TESTING OUTPUT DONE---\n\n");

  return 0;
}

int testRuleSet(ruleDef* rulesAsDef){
  printf("\n");
  for(int i = 0; i < 8; i++){
    if(rulesAsDef[i].use == 1){
      printf("%d%d%d\n",rulesAsDef[i].left,rulesAsDef[i].parent,rulesAsDef[i].right);
    }
  }
  return 0;
}


int checkValidNum(){
  int checkMeOut = 0;
  while(!scanf("%d", &checkMeOut)){
    while((checkMeOut=getchar()) != '\n'){}
    printf("Invalid input. Please enter a numeric value.\n");
  }
  return checkMeOut;
}
