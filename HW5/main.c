#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "complex.h"
/**********************                  
 * main.c                                       
 *                                              
 * Author: Jarod England                   
 * Date Created: 11/15                               
 * Last Modified by: Jarod England                 
 * Date Last Modified: 9/5                 
 * Assignment: hw5
 * main.c                        
 */
typedef struct CELL *LIST;

struct CELL {
  int val;
  LIST next;
};

LIST stack;

extern int push(int val);
extern int pop();
extern int empty();
extern int add(int a, int b);
extern int subtract(int a, int b);
extern int monsub(int a, int b);
extern int bitand(int a, int b);
extern int bitor(int a, int b);

extern int multiply(int a, int b);
extern int divide(int a, int b);
extern void printvalues(LIST a);
extern void clearstack(LIST *a);


int main(){
  char str[1000];
  stack = malloc(sizeof(LIST));
  stack->next ='\0';
  stack->val = '\0';
  while(scanf("%s", str) != EOF && strncmp(str, "q", 1) != 0){ //continues while scanf does not equal EOF or and str != q 
      if (strncmp(str, "p", 1) == 0 && stack->val=='\0'){ //if p is inputted             
	printf("rpn: stack empty\n");
      }
      else if (strncmp(str, "p", 1) == 0 && stack->val!='\0' ){
	printf("%d",stack->val);
      }
      else if (strncmp(str, "f", 1) == 0 ){ //if f is inputted    
	printValues(stack);
      }
      else if (strncmp(str, "c", 1) == 0 ){ //if c is input                 
	clearstack(&stack);
      }
      else if (isdigit(str[0])){ //if a number is input  
	push(atoi(str));
      }
      else if (strncmp(str, "*", 1) == 0 ){ //if multiplication is input                                                                         
	if(stack->val == '\0' || stack->next =='\0'){
	  printf("rpn: stack empty\n");
	}
	else{
	  int a = pop();
	  stack->val = muliply(stack->val,a);
	}
      }
      else if (strncmp(str, "/", 1) == 0 ){ //if divide is input                                                                                 
	if(stack->val == '\0' || stack->next =='\0'){
	  printf("rpn: stack empty\n");
	}
	else{
	  int a = pop();
	  stack->val = divide(stack->val, a);
	}
      }
      else if (strncmp(str, "+", 1) == 0 ){ //if plus is input                                                                                   
	if(stack->val == '\0' || stack->next =='\0'){
	  printf("rpn: stack empty\n");
	}
	else{
	  int a = pop();
	  stack->val = stack->val +a;
	}
      }

      else if (strncmp(str, "bitor" , 5) ==0){
	if(stack->val == '\0' || stack->next =='\0'){
          printf("rpn: stack empty\n");
        }
	else{
	  int a = pop();
	  stack->val = bitor(stack->val, a); 
	}
      }
      else if (strncmp(str, "bitand", 6) ==0){
	if(stack->val =='\0' || stack->next == '\0'){
	  printf("stack empty"); 
	}
	else{
	  int a = pop();
	  stack->val = bitand(stack->val, a);
	}
      }


      else if (strncmp(str, "-", 1) == 0 ){ //if minus is input                                                                                  
	if(stack->val == '\0' || stack->next =='\0'){
	  printf("rpn: stack empty\n");
	}
	else{
	  int a = pop();
	  stack ->val = stack->val - a;
	}
      }

  }
    free(stack); //frees the stack                                                                                                                 
    return 0;
}
