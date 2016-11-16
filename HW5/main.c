#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

