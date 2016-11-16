#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**********************************************************
 * rpn.c
 *
 * Author: Jarod England
 * Date Created: 9/3
 * Last Modified by: Jarod England
 * Date Last Modified: 9/5
 * Assignment: hw0
 * Part of: rpn.c
 */

 typedef struct CELL *LIST;
 
 struct CELL { //this cell holds the linked list of the numbers
    int val;
    LIST next;
    };


//extern <-was having problem using extern here
LIST stack;

void push(int val) { //pushes an element to the top of the stack
    LIST c = (LIST) malloc(sizeof(struct CELL));
    if (c) {
        c->val = val;
        c->next = stack;
        stack = c;
    } else {
        printf("error");
    }
}

int pop(){ //releases the value at the top of the stack
    
    if (stack->val =='\0'){
        printf("rpn: stack empty");
    }
    int value = stack->val;
    //printf("POP! : %d\n", value);
    LIST *spot = stack;
    stack = stack->next;
    free(spot);
    return value;
}

void clearstack(){ //clears the stack
    if(stack->next != '\0'){
        pop();
        clearstack();
    }
}

void printValues(LIST a){ //prints all the values in the current stack
    if (a->next!='\0'){
        printf("%d\n",a->val);
        printValues(a->next);
    }
}


int main(){
    char str [1000];
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
            clearstack();
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
            stack->val = stack->val * a;
            }
        }
        else if (strncmp(str, "/", 1) == 0 ){ //if divide is input
            if(stack->val == '\0' || stack->next =='\0'){
                printf("rpn: stack empty\n");
            }
            else{
            int a = pop();
            stack->val = stack->val / a;
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
