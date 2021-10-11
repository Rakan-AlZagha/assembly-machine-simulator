/*
 * File name: script.c
 * Project: Project on Interpreter
 * CPSC 275 F 2020
 * Author: @Rakan AlZagha
 */

#include <stdio.h>
#include <string.h>        //for string comparision (strcmp)
#include <stdbool.h>       //boolean functionality for flags and state of while loop
#include <ctype.h>         //library to use toUpper function
#define MAX 127            //maximum number contraint
#define MIN -128           //minimum number contraint
#define ARRAY_SIZE 15      //Array size for memory and registers
#define COMM_ARRAY_SIZE 11 //Array size for number of commands

typedef struct checkCommand{ //struct for returning location and destination (r0, m1)
  int location, destination; //int variables to compare position relative to array locations
} Check; //name of struct type

//command functions
void read(int data, char* memoryLocation, int storedData[], const char* possibilities[]);           //read prototype
void write(char* memoryLocation, int storedData[], const char* possibilities[]);                    //write prototype
void printState(int s[], const char* b[]);                                                          //print prototype
void move(char* memoryLocation, char* destination, int storedData[], const char* possibilities[]);  //move prototype
void add(char* memoryLocation, char* destination, int storedData[], const char* possibilities[]);   //add prototype
void sub(char* memoryLocation, char* destination, int storedData[], const char* possibilities[]);   //sub prototype
void mult(char* memoryLocation, char* destination, int storedData[], const char* possibilities[]);  //mult prototype
void div(char* memoryLocation, char* destination, int storedData[], const char* possibilities[]);   //div prototype
void mod(char* memoryLocation, char* destination, int storedData[], const char* possibilities[]);   //mod prototype
void comp(char* memoryLocation, char* destination, int storedData[], const char* possibilities[]);  //comp prototype
bool quit(bool isValidState);                                                                       //quit prototype

//supporting functions to limit redundant code
int getInput(char* command, const char* possibilities[]);                                           //getInput prototype
char oneUpperCase(char* command);                                                                   //one word uppercase prototype
void twoUpperCase(char* memoryLocation, char* destination);                                         //two words uppercase prototype
int commandCheck(char* command, const char* possibilities[]);                                       //checking if command is valid function prototype
Check validMemoryAndRegister(char* memoryLocation, char* destination, const char* possibilities[]); //checking if user inputs valid m | r -> r (or opposite) prototype
Check repeatFuncCalls(char* memoryLocation, char* destination, const char* possibilities[]);        //condensing function calls into one function for readability prototype
int locationErrorChecking(Check result);                                                            //if memory to memory or non-valid input prototype
void overflowCheck(int data, int storedData[]);                                                     //checking if there is overflow prototype

/*
 *   Function: main()
 *   ----------------------------
 *   Purpose: reading input from the terminal and passing the command to
 *   the correct function and if needed, display error case for unidentifiable command
 *
 */

void main(void) {
  int R0 = 128, R1 = 128, R2 = 128, R3 = 128; //registers declaration
  int M0 = 128, M1 = 128, M2 = 128, M3 = 128, M4 = 128, M5 = 128, M6 = 128, M7 = 128; //memory declaration
  int data, validCommand; //variable to hold data being passed in and to hold which command is being executed
  char command[ARRAY_SIZE], memoryLocation[ARRAY_SIZE], destination[ARRAY_SIZE]; //
  bool ZF = 0, SF = 0, OF = 0, isValidState = 1; //boolean flags and while loop condition
  const char* names[ARRAY_SIZE] = {"R0", "R1", "R2", "R3", "M0", "M1", "M2", "M3", "M4", "M5", "M6", "M7", "ZF", "SF", "OF"}; //array holding valid registers and memory
  const char* possibleCommands[COMM_ARRAY_SIZE] = {"READ", "WRITE", "PRINTS", "MOVE", "ADD", "SUB", "MULT", "DIV", "MOD", "COMP", "QUIT"}; //array holding valid commands
  int storedData[ARRAY_SIZE] = {R0, R1, R2, R3, M0, M1, M2, M3, M4, M5, M6, M7, ZF, SF, OF}; //array holding the values of all registers and memory locations

  //welcoming message and instructions
  printf("-----------------------------------------------------------------------------------------------------\n");
  printf("\t\t\tWelcome to the SM-275 simulator/interpreter!\n\t\t\tPlease input any assembly contruction commands:\n\n");
  printf("\t\t-READ\t\t\t-WRITE\t\t\t-PRINTS\n\t\t-MOVE\t\t\t-ADD\t\t\t-SUB\t\t\t-MULT\n\t\t-DIV\t\t\t-MOD\t\t\t-COMP\t\t\t-QUIT\n\n");
  printf("\t\t\tAccepted registers and memory locations are:\n-R0\t-R1\t-R2\t-R3\tR4\t-M0\t-M1\t-M2\t-M3\t-M4\t-M5\t-M6\t-M7\n");
  printf("-----------------------------------------------------------------------------------------------------\n\n\n");
  printf("\t\t\t\tCurrent state of the interpreter\n");
  printf("\t\t\t\t--------------------------------\n\n\n");
  printState(storedData, names);
  printf("\n\n\n");

  while(isValidState && scanf("%s", command) != EOF) {//while loop based on the state, remains true until user calls quit command or file terminates
    validCommand = getInput(command, possibleCommands); //convert to upper and check for validity
    if(validCommand == -1) { //if command is not recognizable from possible commands array
      printf("???\n"); //error message
    }
    else { //if command is recognized, pass it into the switch cases to call correct function
      switch(validCommand) { //based on possibleCommands array
        case 0: //READ
          scanf("%d, %s", &data, memoryLocation);
          read(data, memoryLocation, storedData, names);
          break;
        case 1: //WRITE
          scanf("%s", memoryLocation);
          write(memoryLocation, storedData, names);
          break;
        case 2: //PRINTS
          printState(storedData, names);
          break;
        case 3: //MOVE
          move(memoryLocation, destination, storedData, names);
          break;
        case 4: //ADD
          add(memoryLocation, destination, storedData, names);
          break;
        case 5: //SUB
          sub(memoryLocation, destination, storedData, names);
          break;
        case 6: //MULT
          mult(memoryLocation, destination, storedData, names);
          break;
        case 7: //DIV
          div(memoryLocation, destination, storedData, names);
          break;
        case 8: //MOD
          mod(memoryLocation, destination, storedData, names);
          break;
        case 9: //COMP
          comp(memoryLocation, destination, storedData, names);
          break;
        case 10: //QUIT
          isValidState = quit(isValidState); //making state false, terminating the for loop
          break;
        default: //DEFAULT Statement (n/a)
          break;
      }
    }
  }
}

/*
 *   Function: read()
 *   ----------------------------
 *   Purpose: read data into registers and/or memory locations
 *
 *   int data
 *   char* memoryLocation
 *   int storedData[]
 *   const char* possibilities[]
 *
 */

void read(int data, char* memoryLocation, int storedData[], const char* possibilities[]) {
  int location; //what location to store data in
  overflowCheck(data, storedData); //checking if data exceeds limitations and setting OF accordingly
  oneUpperCase(memoryLocation); //make the location passed in uppercase for comparison
  location = commandCheck(memoryLocation, possibilities); //check if the location passed is valid and which register/memory
  if(location == -1) { //the case where the location passed in does not exist
    printf("???\n"); //print error case
  }
  else { //if valid, store the data in the specified location
    storedData[location] = data;
  }
}

/*
 *   Function: write()
 *   ----------------------------
 *   Purpose: to print out specific places in memory and/or registers
 *
 *   char* memoryLocation
 *   int storedData[]
 *   const char* possibilities[]
 *
 */

void write(char* memoryLocation, int storedData[], const char* possibilities[]) {
  int location; //what location to print
  oneUpperCase(memoryLocation); //make the location passed in uppercase for comparison
  location = commandCheck(memoryLocation, possibilities); //check if the location passed is valid and which register/memory
  if(location == -1) { //the case where the location passed in does not exist
    printf("???\n"); //print error case
  }
  else { //if valid, store the data in the specified location
    printf("%d\n", storedData[location]); //print out data stored at location
  }
}

/*
 *   Function: move()
 *   ----------------------------
 *   Purpose: move (copy) value of memory or register into another
 *
 *   char* memoryLocation
 *   char* destination
 *   int storedData[]
 *   const char* possibilities[]
 *
 */

void move(char* memoryLocation, char* destination, int storedData[], const char* possibilities[]) {
  Check result = repeatFuncCalls(memoryLocation, destination, possibilities); //scan in locations, convert to uppercase, and check if valid
  if(locationErrorChecking(result)) { //if no errors found (not memory to memory or invalid location) then move data accordingly
    storedData[result.destination] = storedData[result.location]; //move data to destination
  }
}

/*
 *   Function: printState()
 *   ----------------------------
 *   Purpose: print out the data located at register and memory locations
 *
 *   int storedData[]
 *   const char* names[]
 *
 */

void printState(int storedData[], const char* names[]) {
  int i; //incrementer
  for(i = 0; i < ARRAY_SIZE; i++) { //looping through data
    if(storedData[i] > 127 || storedData[i] < -128) { //check if any values are out of bounds
      printf("?\t"); //replace with the ? symbol
    }
    else {
      printf("%d\t", storedData[i]); //if in bounds, then print out data
    }
  }
  printf("\n");
  for(i = 0; i < ARRAY_SIZE; i++) { //dashed line
    printf("----\t");
  }
  printf("\n");
  for(i = 0; i < ARRAY_SIZE; i++) { //print out the names of registers and memory locations
    printf("%s\t", names[i]); //names contains the names of all registers and memory locations
  }
  printf("\n");
}

/*
 *   Function: add()
 *   ----------------------------
 *   Purpose: perform addition on (register and memory) or (register and register)
 *   then store value in R0
 *
 *   char* memoryLocation
 *   char* destination
 *   int storedData[]
 *   const char* possibilities[]
 *
 */

void add(char* memoryLocation, char* destination, int storedData[], const char* possibilities[]) {
  Check result = repeatFuncCalls(memoryLocation, destination, possibilities); //scan in locations, convert to uppercase, and check if valid
  if(locationErrorChecking(result)) { //if no errors found (not memory to memory or invalid location) then move data accordingly
    storedData[0] = storedData[result.destination] + storedData[result.location]; //add the two locations and store value in R0
    overflowCheck(storedData[0], storedData); //check if the addition resulted in overflow and set OF accordingly
  }
}

/*
 *   Function: sub()
 *   ----------------------------
 *   Purpose: perform subtraction on (register and memory) or (register and register)
 *   then store value in R0
 *
 *   char* memoryLocation
 *   char* destination
 *   int storedData[]
 *   const char* possibilities[]
 *
 */

void sub(char* memoryLocation, char* destination, int storedData[], const char* possibilities[]) {
  Check result = repeatFuncCalls(memoryLocation, destination, possibilities); //scan in locations, convert to uppercase, and check if valid
  if(locationErrorChecking(result)) { //if no errors found (not memory to memory or invalid location) then move data accordingly
    storedData[0] = storedData[result.destination] - storedData[result.location]; //subtract the two locations and store value in R0
    overflowCheck(storedData[0], storedData); //check if the subtraction resulted in overflow and set OF accordingly
  }
}

/*
 *   Function: mult()
 *   ----------------------------
 *   Purpose: perform multiplication on (register and memory) or (register and register)
 *   then store value in R0
 *
 *   char* memoryLocation
 *   char* destination
 *   int storedData[]
 *   const char* possibilities[]
 *
 */

void mult(char* memoryLocation, char* destination, int storedData[], const char* possibilities[]) {
  Check result = repeatFuncCalls(memoryLocation, destination, possibilities); //scan in locations, convert to uppercase, and check if valid
  if(locationErrorChecking(result)) { //if no errors found (not memory to memory or invalid location) then move data accordingly
    storedData[0] = storedData[result.destination] * storedData[result.location]; //multiply the two locations and store value in R0
    overflowCheck(storedData[0], storedData); //check if the multiplication resulted in overflow and set OF accordingly
  }
}

/*
 *   Function: div()
 *   ----------------------------
 *   Purpose: perform division on (register and memory) or (register and register)
 *   then store value in R0
 *
 *   char* memoryLocation
 *   char* destination
 *   int storedData[]
 *   const char* possibilities[]
 *
 */

void div(char* memoryLocation, char* destination, int storedData[], const char* possibilities[]) {
  Check result = repeatFuncCalls(memoryLocation, destination, possibilities); //scan in locations, convert to uppercase, and check if valid
  if(locationErrorChecking(result)) { //if no errors found (not memory to memory or invalid location) then move data accordingly
    storedData[0] = storedData[result.destination] / storedData[result.location]; //divide the two locations and store value in R0
    overflowCheck(storedData[0], storedData); //check if the division resulted in overflow and set OF accordingly
  }
}

/*
 *   Function: mod()
 *   ----------------------------
 *   Purpose: perform remainder division on (register and memory) or (register and register)
 *   then store value in R0
 *
 *   char* memoryLocation
 *   char* destination
 *   int storedData[]
 *   const char* possibilities[]
 *
 */

void mod(char* memoryLocation, char* destination, int storedData[], const char* possibilities[]) {
  Check result = repeatFuncCalls(memoryLocation, destination, possibilities); //scan in locations, convert to uppercase, and check if valid
  if(locationErrorChecking(result)) { //if no errors found (not memory to memory or invalid location) then move data accordingly
    storedData[0] = storedData[result.destination] % storedData[result.location]; //take the remainder of two locations and store value in R0
    overflowCheck(storedData[0], storedData); //check if the mod resulted in overflow and set OF accordingly
  }
}

/*
 *   Function: comp()
 *   ----------------------------
 *   Purpose: compare the values of two locations and set flags accordingly
 *
 *   char* memoryLocation
 *   char* destination
 *   int storedData[]
 *   const char* possibilities[]
 *
 */

void comp(char* memoryLocation, char* destination, int storedData[], const char* possibilities[]) {
  Check result = repeatFuncCalls(memoryLocation, destination, possibilities); //scan in locations, convert to uppercase, and check if valid
  if(locationErrorChecking(result)) { //if no errors found (not memory to memory or invalid location) then move data accordingly
    if(storedData[result.location] > storedData[result.destination]) { //Rj > Rk case
      storedData[12] = 0; //ZF = 0
      storedData[13] = 1; //SF = 1
    }
    else if(storedData[result.location] < storedData[result.destination]) { //Rj < Rk
      storedData[12] = 0; //ZF = 0
      storedData[13] = 0; //SF = 0
    }
    else if(result.location == result.destination) { //Rj == Rk
      storedData[12] = 1; //ZF = 1
      storedData[13] = 0; //SF = 0
    }
  }
}

/*
 *   Function: quit()
 *   ----------------------------
 *   Purpose: terminate the interpreter
 *
 *   bool isValidState
 *
 */

bool quit(bool isValidState) {
  isValidState = 0; //set boolean state value to FALSE (0)
  return isValidState; //return the state to while loop
}

/*
 *   Function: getInput()
 *   ----------------------------
 *   Purpose: receive the command from the program, convert it to uppercase,
 *   then check if is valid and return correct command
 *
 *   char* command
 *   const char* possibilities[]
 *
 */

int getInput(char* command, const char* possibilities[]) { //
  int validCommand; //position in array will be associated with this int variable
  oneUpperCase(command); //convert command to uppercase
  validCommand = commandCheck(command, possibilities); //check if it is a valid command by cross-referencing possibilities
  return validCommand; //return correct place in the array (correct command)
}

/*
 *   Function: oneUpperCase()
 *   ----------------------------
 *   Purpose: convert a command into uppercase (or a location)
 *
 *   char* command
 *
 */

char oneUpperCase(char* command) {
  int i;
  for(i = 0; command[i] != '\0'; i++) { //loop until reach null character
    command[i] = toupper(command[i]); //use to upper function to convert from lowercase ASCII to uppercase
  }
}

/*
 *   Function: twoUpperCase()
 *   ----------------------------
 *   Purpose: convert two commands (locations) into uppercase
 *
 *   char* memoryLocation
 *   char* destination
 *
 */

void twoUpperCase(char* memoryLocation, char* destination) {
  int i;
  for(i = 0; memoryLocation[i] != '\0'; i++) { //loop until reach null character
    memoryLocation[i] = toupper(memoryLocation[i]); //use to upper function to convert from lowercase ASCII to uppercase
    destination[i] = toupper(destination[i]); //use to upper function to convert from lowercase ASCII to uppercase
  }
}

/*
 *   Function: commandCheck()
 *   ----------------------------
 *   Purpose: check if the command is within the realm of possibilities
 *
 *   char* command
 *   const char* possibilities[]
 *
 */

int commandCheck(char* command, const char* possibilities[]) {
  int i, correctCommand = -1; //starts at -1 for error handling case when command DNE
  for(i = 0; i < COMM_ARRAY_SIZE; i++) { //loop through array of commands
    int dataCommand = strcmp(command, possibilities[i]); //check every possibility and store which command is valid
    if(dataCommand == 0) { //if our comparison is successful save position in array
      correctCommand = i;
    }
  }
  if(correctCommand == -1) { //if our loop terminated without changing correctCommand, then command is INVALID
    correctCommand = -1;
  }
  return correctCommand; //return command
}

/*
 *   Function: validMemoryAndRegister()
 *   ----------------------------
 *   Purpose: check to see if memory and register arguments are passed in correctly
 *
 *   char* memoryLocation
 *   char* destination
 *   const char* possibilities[]
 *
 */

Check validMemoryAndRegister(char* memoryLocation, char* destination, const char* possibilities[]) {
  int i, j;
  Check result; //struct type to return both location and destination positions in array
  result.location = -1; //if no match
  result.destination = -1; //if no match
  for(i = 0; i < ARRAY_SIZE; i++) { //loop through array possibilities
    int dataLocation = strcmp(memoryLocation, possibilities[i]); //check if there is a match
    for(j = 0; j < ARRAY_SIZE; j++) { //loop through array possibilities
      int dataDestination = strcmp(destination, possibilities[j]); //check if there is a match
      if(dataLocation == 0 && dataDestination == 0) { //in order for command to run correctly, we need a match for both
        result.location = i;
        result.destination = j;
        return result;
      }
    }
  }
   if(result.location == -1 || result.destination == -1) { //case where we have no match for either argument (invalid entry)
     return result;//return position of correspoining register and/or memory location
   }
}

/*
 *   Function: repeatFuncCalls()
 *   ----------------------------
 *   Purpose: condense other functions by performing all operations in one function,
 *   scan for input, convert to uppercase, and checking for validity all happens here
 *
 *   char* memoryLocation
 *   char* destination
 *   const char* possibilities[]
 *
 */

Check repeatFuncCalls(char* memoryLocation, char* destination, const char* possibilities[]) {
  Check result;
  scanf("%2s, %2s", memoryLocation, destination); //scan for 2 character input
  twoUpperCase(memoryLocation, destination); //convert to uppercase
  result = validMemoryAndRegister(memoryLocation, destination, possibilities); //check for validity
  return result; //return result (struct)
}

/*
 *   Function: locationErrorChecking()
 *   ----------------------------
 *   Purpose: checking for the case where we have 2 memory locations passed in
 *   or we get invalid location input
 *
 *   Check result
 *
 */

int locationErrorChecking(Check result) {
  if(result.location > 3 && result.destination > 3) { //if both are memory locations (based off of array position)
    printf("???\n"); //error message
    return 0; //return false
  }
  if(result.location == -1 || result.destination == -1) { //if we get invalid input (LOGIC OFF)
    printf("???\n"); //error message
    return 0; //return false
  }
  else {
    return 1; //return true
  }
}

/*
 *   Function: overflowCheck()
 *   ----------------------------
 *   Purpose: check if there is overflow when data is being passed in (out of bounds)
 *
 *   int data
 *   int storedData[]
 *
 */

void overflowCheck(int data, int storedData[]) {
  if(data > MAX || data < MIN) { //if data is larger than 127 or smaller than -128
    storedData[14] = 1; //if true, set OF to true
  }
  else { //if false, set OF to false
    storedData[14] = 0;
  }
}
