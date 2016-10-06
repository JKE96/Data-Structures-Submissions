/* main.c ---
*
* Filename: main.c
* Description:
* Author:
* Maintainer:
* Created: Thu Jan 10 11:23:43 2013
/* Code: */

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_led.h> // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <stdio.h>

#define TIMER 20000


int main(void) {
 
  f3d_gyro_init();
  f3d_uart_init();
  f3d_lcd_init();
  
  //turns the screen black
  f3d_lcd_fillScreen(BLACK);
  int AXIS=75;
  float a[3] = {1.1, 2.2, 3.3};
  int i = 0;
  int j = 0;


  drawRect(0, 1000, AXIS, AXIS, BLUE);
 while(1) {

    f3d_gyro_getdata(a);
    
    int xValue = a[0];
    int yValue = a[1];
    int zValue = a[2];

    //Drawing the bar for X-value
    drawRect(10,40, 75, AXIS+40, RED);
 }//close while
}//close main

void drawRect(int x1, int x2, int y1, int y2, uint16_t color){
  int i;
  int j;
  if(x1 > x2){
    int temp = x1;
    x1 = x2;
    x2 = temp;
  }
  if(y1>y2){
    int tem = y1;
    y1 = y2;
    y2 = tem;
  }

  for(i= x1; i<= x2; i++){
    for (j = y1; j<=y2; j++){
      f3d_lcd_drawPixel(i,j,color);
    }
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
