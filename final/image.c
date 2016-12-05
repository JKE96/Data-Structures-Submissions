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

#include <stdint.h>

struct bmpfile_magic magic;
struct bmpfile_header header;
BITMAPINFOHEADER info;


void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}

FATFS Fatfs;		/* File system object */
FIL Fil;		/* File object */
BYTE HeaderBuff[54];		/* File read buffer */

typedef struct {
    uint8_t b;
    uint8_t g;
    uint8_t r;
} bmppixel;
// instance of ImgBuff
bmppixel ImgBuff;

uint16_t colorChanger(bmppixel *ImgBuff) {
    uint16_t blue = ImgBuff->b;
    uint16_t red = ImgBuff->r;
    uint16_t green = ImgBuff->g;
    uint16_t pix;
    pix |= (red >> 3) << 11;
    pix |= (green >> 2) << 5;
    pix |= (blue >> 3);
    return pix;
}

void draw(int img, int orientation) {
  FRESULT rc;			/* Result code */
  DIR dir;			/* Directory object */
  FILINFO fno;			/* File information object */
  UINT bw, br;
  unsigned int retval;init();
  f_mount(0, &Fatfs);
  rc = f_open(&Fil, "player.bmp", FA_READ);
  if (rc) die(rc);
  rc = f_read(&Fil, &magic, 2, &br);
  rc = f_read(&Fil, &header, sizeof(header), &br);
  rc = f_read(&Fil, &info, sizeof(info), &br);



  // do something with orientation (probably here)
  int c=40;
  int r=40;
  for(;;) {
    rc = f_read(&Fil, &ImgBuff, sizeof(ImgBuff), &br);
    if (rc || !br) break;

    // do orientation stuff

      // set r and c values
    
      // drawPixel implementation
      // how could you do this with pushcolor instead?
      // (to get full credit you need to use pushcolor)
      f3d_lcd_drawPixel(c,r,colorChanger(&ImgBuff));
      if(c==100){c=0; r++;}
      else{++c;}
  }
  rc = f_close(&Fil);
  if (rc) die(rc);
}


/* main.c ends here */
