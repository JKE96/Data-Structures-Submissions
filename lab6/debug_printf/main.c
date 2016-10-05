/**********************************************************
 * main.c
 *
 * These definitions create the main program that connects to the STM32
 *
 * Author: Jarod England username:jkenglan and
 * Date Created: 9/12/16
 * Last Modified by: Jarod England
 * Date Last Modified: 9/12/16
 * Assignment: C on Discovery
 * Part of: lab03
 */



#include <stm32f30x.h>  // Pull in include files for F30x standard drivers

#include <f3d_led.h>     // Pull in include file for the local drivers

#include <f3d_user_btn.h>

#include <stdio.h>

#include <f3d_uart.h>

// Simple looping delay function
void delayy(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop");
  }
}


int main(void) {
  f3d_led_init();
  f3d_user_btn_init();
  f3d_uart_init();
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  int light =8;
  while(1){
    printf("turn on");
    f3d_led_on(light);
    printf("delayy");
    delayy();
    printf("turn off");
    f3d_led_off(light);
    printf("second delayy");
    delayy();
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
