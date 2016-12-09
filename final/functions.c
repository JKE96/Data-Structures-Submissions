#include <stm32f30x.h>  // Pull in include files for F30x standard drivers
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <f3d_systick.h>
#include <ff.h>
#include <diskio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <f3d_user_btn.h>


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
  if (y1 > 75 && y2 > 75) {
    for(j= y2; j>= y1; j--){ 
      for (i = x1; i<=x2; i++){
	f3d_lcd_drawPixel(i,j,color);
      }
    }
  }
  for(j= y1; j<= y2; j++){ 
    for (i = x1; i<=x2; i++){
      f3d_lcd_drawPixel(i,j,color);
    }
  }
}


unsigned short lfsr = 0xACE1u;
unsigned bit;
unsigned rando(){ //the max number will be 65536 because it is 16 bit
  bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
  return lfsr =  (lfsr >> 1) | (bit << 15);
}

int randomy(int one, int two){
  int a =(int)rando();
  double stuff = a/65536.0;
  //printf("stuff:%f\n",stuff);

  two= two-one+1;

  double b = (stuff*(double)two) +(double)one;
  //printf("b: %f\n",b);
  int c = (int)b;
  //printf("c:%d\n",c);
  return c;
}