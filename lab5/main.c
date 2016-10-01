//main.c for lab5
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

  while(1){
    f3d_gyro_getdata(posns);
    
    printf("%f %f %f\n", posns[0], posns[1], posns[2]);
  }
}

void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
