/*Jarod England jkenglan complex.c for hw5*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int multiply(int a, int b){
  return a*b;
}

int divide(int a, int b){
  return a/b;
}

void clearstack(LIST *a){ //clears the stack                                                                                       
  if(a->next != '\0'){
    pop();
    clearstack(a);
  }
}

void printValues(LIST a){ //prints all the values in the current stack                                                      
  if (a->next!='\0'){
    printf("%d\n",a->val);
    printValues(a->next);
  }
}
