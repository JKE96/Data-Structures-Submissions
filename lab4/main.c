/* main.c - contains the main functions for the lab program
 * Author: Jarod England username:jkenglan and Steven Russell    
#* Date Created: 9/23/16              
#* Last Modified by: Jarod England          
#* Date Last Modified: 9/23/16        
#* Assignment: lab04                      
#* Part of: lab04 
/* Code: */

#include <f3d_uart.h>
#include <stdio.h>
#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <stdio.h>
// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void) {
  f3d_uart_init();

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  
  while (1){
    int c, newl, newc, neww, word;
   

    newl = newc = neww = 0;
    word = 1;
    while((c = getchar()) != 0x1b) {
      ++newc;
      if(c == '\n')
        ++newl; 
      if(c == ' ' || c == '\n' || c == '\t')
	++neww; 
    } 
    printf("%d %d %d\n",newl, neww, newc);
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
