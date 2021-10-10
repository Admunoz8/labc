#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // malloc & free
#include <stdint.h> // use guaranteed 64-bit integers
#include "tokenizer.h" // Create header file and reference that
#include "memory.h" // built-in functions to read and write to a specific file

int32_t* reg; // Array of 32 32-bit registers

void init_regs();
bool interpret(char* instr);
void write_read_demo();
bool areSame(char* str1, char* str2);
int caseHandler(char* str);
int findRegNum(char* str);
void print_regs();

void print_regs(){
  int col_size = 10;
  for (int i = 0; i < 8; i++){
    printf("X%02i:%.*lld", i, col_size, (long long int) reg[i]);
    printf("X%02i:%.*lld", i+8, col_size, (long long int) reg[i+8]);
    printf("X%02i:%.*lld", i+16, col_size, (long long int) reg[i+16]);
    printf("X%02i:%.*lld", i+24, col_size, (long long int) reg[i+24]);
  }
}

/*Receives register format strings and return the int value of the register.
For example, "X13" -> 13*/
int findRegNum(char* str){ 
  int count = 0;
  char num[2];
  while(*(str+count)!='\0'){
    count++;
  }
  if (count >= 3){ //accounting for the last token to have an extra NULL at the end.
    num[0] = *(str+1); num[1] = *(str+2);
  }
  else
    num[0] = *(str+1);

  return  atoi(num);
}
//this will be able to handle the strings as a int so we can use them  in our swithc stateement 
int caseH(char* str){
  if (sameC(str, "LW")) {return 0;}
  else if (sameC(str, "SW")) {return 1;}
  else if (sameC(str, "ADD")) {return 2;}
  else if (sameC(str, "ADDI")) {return 3;}
  else
    return -1;
}
//this meethod helps my case handler help by letting me compare th given string
bool sameC(char* str1, char* str2){
  char* tmp1 = str1;
  char* tmp2 = str2;
  int count = 0;
  int count2 = 0;

  while (*(tmp1+count) != '\0'){
    count++;
  }
  while (*(tmp2+count2) != '\0'){
    count2++;
  }
  if (count != count2){return false;}
  
  int i = 0;
  while (*(str1+i) != '\0' && *(str2+i) != '\0'){
    if (*(str1+i) != *(str2+i))
      return false;
    i++;
  }
  return true;
}

/**
 * Initialize register array for usage.
 * Malloc space for each 32-bit register then initializes each register to 0.
 * Do not alter this function!
 */
void init_regs(){
	int reg_amount = 32;
	reg = malloc(reg_amount * sizeof(int32_t)); // 32 * 4 bytes
	for(int i = 0; i < 32; i++)
		reg[i] = i;
}



/**
 * Fill out this function and use it to read interpret user input to execute RV64 instructions.
 * You may expect that a single, properly formatted RISC-V instruction string will be passed
 * as a parameter to this function.
 */
 //this method is use so that we can properly interpert, it will use pointers 
bool interpret(char* instr){
  char* text = "mem.txt";
  char **pointerS;
  pointerS = tokenize(instr, ' ');
  //print_all_tokens(pointerStorage); use for debugging purposes.
  int caseN = caseH(pointerS[0]); // decides what case the instruction is.
  switch(caseN){
    //this case is for LW,The lw instruction loads a word into a register from memory.
  case 0:
    printf("LW done");
    char* saveLW = *(pointerS+1);
    int saveLWint = findRegNum(saveLW);
    char** memTokLW = tokenize(*(pointerS+2), '('); // splits the main 3rd token in 2.
    char** cleanMemLW = tokenize(*(memTokLW+1),')'); // cleans the mem token to only reg.

    char* memRegLW = *cleanMemLW;
    int memRegLWint = findRegNum(memRegLW);

    int32_t addressLW = atoi(*memTokLW) + memRegLWint;
    int32_t readLW = read_address(addressLW, text);
    reg[saveLWint] = readLW; // register is given whatever was read in addressLW
    break;
    //this case is for SW,it will copy the data from a register to memory. The register is not changed. 

  case 1:
    printf("SW done");
    char* saveSW = *(pointerS+1);
    int saveSWint = findRegNum(saveSW);
    char** memTokSW = tokenize(*(pointerS+2), '(');
    char** cleanMemSW = tokenize(*(memTokSW+1), ')');
    char* memRegSW = *cleanMemSW;
    int memRegSWint = findRegNum(memRegSW);
    
    int32_t datawriteSW = reg[saveSWint];
    int32_t addressSW = atoi(*memTokSW) + memRegSWint; 
    int32_t write = write_address(datawriteSW, addressSW, text);
    
    break;
    //Adds the first operand (destination operand) and the second operand (source operand) and stores the result in the destination operand.
  case 2:
    printf("ADD done");
    char* saveTo = *(pointerS+1);
    char* firstOperand = *(pointerS+2);
    char* secondOperand = *(pointerS+3);

    int saveToReg = findRegNum(saveTo);
    int firstReg = findRegNum(firstOperand);
    int secReg = findRegNum(secondOperand);

    reg[saveToReg] = reg[firstReg] + reg[secReg];
    printf("%d", reg[saveToReg]);
    break;
    //This case is for ADDI instruction. Addi is used to  adds the immediate specified in the instruction to a value in a register
  case 3:
    printf("ADDI done");
    char* saveToADDI = *(pointerS+1);
    char* addTo = *(pointerS+2);
    int add = atoi(*(pointerS+3));

    int saveToADDIint = findRegNum(saveToADDI);
    int addToint = findRegNum(addTo);

    reg[saveToADDIint] = reg[addToint] + add;
    break;
  default:
    printf("ERROR");
    return false;
  }
  return true;
}

/**
 * Your code goes in the main
 *
 */
int main(){
	// Do not write any code between init_regs
	init_regs(); // DO NOT REMOVE THIS LINE

	char input[1000]; // User input can only be 100 chars long.
	printf("Enter 'ctrl+D to EOF' to quit.\n");
  //while loop so it can do the functions over and oveer until user press x, therefore it breaks
	while(1){
	  printf("Input: ");
	  fgets(input, sizeof(input), stdin);
	  if (*input == 'x'){break;}
	  interpret(input);
	  printf("\n");
	}
	print_regs();

	return 0;
}

