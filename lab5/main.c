
//main.c for lab5, runs the main function for the lab.
//Author: Jarod England
// Date Created: Sept. 2016
// Last Modified by: Jarod England
// Date Last Modified: Oct 5. 2016
// Assignment: Lab5
// Part of: lab5
//

#include <stm32f30x.h>
#include <f3d_uart.h>
#include <stdio.h>
#include <f3d_gyro.h>
#include <f3d_led.h>
#include <f3d_user_btn.h>

int main(void){
  f3d_led_init();
  f3d_user_btn_init();
  f3d_uart_init();
  f3d_gyro_init();
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  float posns[3] =  {0};

  int button = 0;
  while(1){
    f3d_gyro_getdata(posns);
    //printf("%f %f %f\n", posns[0], posns[1], posns[2]);
    if (user_btn_read()){
      button++;
      button = button%3;
    }
    if (button==0){
      f3d_led_on(8);
      f3d_led_off(9);
      f3d_led_off(10);
      printf("%f\n", posns[0]);
    }
    else if(button ==1){
      f3d_led_off(8);
      f3d_led_on(9);
      f3d_led_off(10);
      printf("%f\n", posns[1]);
    }
    else{
      f3d_led_off(8);
      f3d_led_off(9);
      f3d_led_on(10);
      printf("%f\n", posns[2]);
    }
  }
}

void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
