/* main.c --- 
 * 
 * Filename: main.c
 * Description: main file for lab9
 * Author: Patrick Gibbons and Clayton Tow
 * Maintainer: PG and CT
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 11/1/16
 *           By: Patrick Gibbons
 *     Update #: 5
 * Keywords: lab9
 * Compatibility: lab9
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */
/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <ff.h>
#include <diskio.h>
#include <stdio.h>
#include <math.h>

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}

FRESULT rc;			/* Result code */
DIR dir;			/* Directory object */
FILINFO fno;			/* File information object */
UINT bw, br;

struct bmppixel pixel_data[128];
uint16_t pixel_color[128];
BYTE HeaderBuff[54];
BYTE RGBBuff[3];
FATFS Fatfs;		/* File system object */
FIL Fil;		/* File object */


//drawPicture - draws a picture on the LCD screen
//accepts an orientation and the file name of the .BMP to be drawn
//orient = 1 is portrait, orient = 0 is landscape
void drawPicture(int orient, char *name) {

  int row, i;

  // opens file designated by name
  rc = f_open(&Fil, name, FA_READ);
  if (rc) die(rc);
  
  // reads from file
  rc = f_read(&Fil, HeaderBuff, sizeof HeaderBuff, &br);
  if (orient == 1) {
    for (row = 160; row >= 0; row--) {
      rc = f_read(&Fil, &pixel_data, sizeof pixel_data, &br);
      if (rc || !br) break;

      for(i = 128; i >= 0; i--) {
	RGBBuff[0] = ((uint16_t) (pixel_data[i].b)) >> 3;
	RGBBuff[1] = ((uint16_t) (pixel_data[i].g)) >> 2;
	RGBBuff[2] = ((uint16_t) (pixel_data[i].r)) >> 3;
	pixel_color[i] = (RGBBuff[0] << 11) | (RGBBuff[1] << 5) | RGBBuff[2];
      }

      f3d_lcd_setAddrWindow(0, row, 128, row, MADCTLGRAPHICS);
      f3d_lcd_pushColor(pixel_color, 128);
    }
  } else { 
    for (row = 128; row >= 0; row--) {
      rc = f_read(&Fil, &pixel_data, sizeof pixel_data, &br);
      if (rc || !br) break;

      for(i = 160; i >= 0; i--) {
	RGBBuff[0] = ((uint16_t) (pixel_data[i].b)) >> 3;
	RGBBuff[1] = ((uint16_t) (pixel_data[i].g)) >> 2;
	RGBBuff[2] = ((uint16_t) (pixel_data[i].r)) >> 3;
	pixel_color[i] = (RGBBuff[0] << 11) | (RGBBuff[1] << 5) | RGBBuff[2];
      }

      f3d_lcd_setAddrWindow(row, 0, row, 160, MADCTLGRAPHICS);
      f3d_lcd_pushColor(pixel_color, 160);
    }   
  }
}

int main(void) { 

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  //inits
  f3d_uart_init();
  delay(10);
  f3d_lcd_init();
  delay(10);
  f3d_delay_init();
  delay(10);
  f3d_lcd_sd_interface_init();
  delay(10);
  f3d_i2c1_init();
  delay(10);
  f3d_rtc_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);

  f_mount(0, &Fatfs);		/* Register volume work area (never fails) */

  int task = 0;
  nunchuk_t nunchuk;
  float fl[3];

  while (1) {
    
    f3d_nunchuk_read(&nunchuk);
    f3d_accel_read(fl);

    //reads nunchuk for joystick/button input and changes value of task accordingly
    if(nunchuk.z == 1 || nunchuk.c == 1 || nunchuk.jx == 0x00 || nunchuk.jx == 0xff) {
      //z button pressed - moves forward
      if(nunchuk.z == 1) {
	switch(task) {
	case 0: task = 1; break;
	case 1: task = 2; break;
	case 2: task = 0; break;
	}
      }
      //c button pressed - moves backward
      if(nunchuk.c == 1) {
	switch(task) {
	case 0: task = 2; break;
	case 1: task = 0; break;
	case 2: task = 1; break;
	}
      }
      //joystick left - moves backward
      if(nunchuk.jx == 0x00) {
	switch(task) {
	case 0: task = 2; break;
	case 1: task = 0; break;
	case 2: task = 1; break;
	}
      }
      //joystick right - moves forward
      if(nunchuk.jx == 0xff) {
	switch(task) {
	case 0: task = 1; break;
	case 1: task = 2; break;
	case 2: task = 0; break;
	}
      }
    }
    
    //draws pictures depending on value of task, prints value of task to serialT
    if(task == 0) {
      if (fl[1] < 0.05 && fl[1] > -0.05) //Logic to determine orientation
	drawPicture(1, "DOG.BMP");
      else 
	drawPicture(0, "DOG.BMP");
    }
    if(task == 1) {
      if (fl[1] < 0.05 && fl[1] > -0.05)
	drawPicture(1, "CAT.BMP");
      else
	drawPicture(0, "CAT.BMP");
    }
    if(task == 2) {
      if (fl[1] < 0.05 && fl[1] > -0.05)
	drawPicture(1, "SMILEY.BMP");
      else
	drawPicture(0, "SMILEY.BMP");
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
