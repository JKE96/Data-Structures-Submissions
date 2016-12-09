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
#include "game.h"

nunchuk_t nunchuk;


void displayMenu() {
  f3d_lcd_fillScreen(WHITE);
  f3d_lcd_drawString(25, 10, "Super Dave TM", RED, WHITE);
  
  f3d_lcd_drawString(50,65,"Dave",BLUE,WHITE);

  drawRect(58,66,75,82,BLACK);
  drawRect(59,65,76,81, WHITE);

  f3d_lcd_drawString(10, 100, "Play Game", RED, WHITE);
  f3d_lcd_drawString(10, 125, "Quit :( ",RED,WHITE);

}

void runMenu() {
  char *sound = "pause.wav";
  int starting = 0; //boolean for checking if it is starting
  int curMenu = 0; //this will hold the curMenu information that the user is on

  while(!starting) {
    nunchuk_t nun;
    f3d_nunchuk_read(&nun);
    int num = nun.jy;
    int c = nun.c;
    int z = nun.z;

    printf("%d|",num);

    drawRect(5,120,35,randomy(36,37),BLUE);


    if(num>240 ){
      curMenu=0;
      drawRect(3,8,125,133,WHITE);//erase old
      drawRect(3,8,100,108,BLUE); //draw new
    }
    else if (num<15){
      curMenu=1;
      drawRect(3,8,100,108,WHITE);
      drawRect(3,8,125,133,BLUE);
    }

    if (c||z){
      starting=1;
    }
  }
 
  if(curMenu==0){
    delay(10);
    play(sound);
    delay(10);
    f3d_lcd_drawString(10, 125, "Starting the game!",RED,WHITE);
    printf("tstart");
    transition();
    printf("gstart");
    startGame(); //THIS IS THE FUNCTION THAT STARTS THE GAME!!
  }
  else{
    play(sound);
    f3d_lcd_drawString(10, 125, "Quitting game!",RED,WHITE);
    delay(200);
    f3d_lcd_fillScreen(WHITE);
    f3d_lcd_drawString(25, 10, "You Quit the game reset the board to restart",BLUE,WHITE);
  }
  
}
    
 

