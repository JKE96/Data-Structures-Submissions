/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */
//Jarod England
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
#include <fcntl.h>
#include <stdio.h>

#include <stdint.h>
struct bmpfile_magic {
  unsigned char magic[2];
};
struct bmpfile_header {
  uint32_t filesz;
  uint16_t creator1;
  uint16_t creator2;
  uint32_t bmp_offset;
};
typedef struct
{
  uint32_t header_sz;
  int32_t   width;
  int32_t   height;
  uint16_t nplanes;
  uint16_t bitspp;
  uint32_t compress_type;
  uint32_t bmp_bytesz;
  int32_t   hres;
  int32_t   vres;
  uint32_t ncolors;
  uint32_t nimpcolors;
} BITMAPINFOHEADER;
/*struct bmppixel {
  // little endian byte order
  uint8_t b;
  uint8_t g;
  uint8_t r;
};*/

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

void init() {
    f3d_uart_init();
    f3d_lcd_init();
    f3d_delay_init();
    f3d_rtc_init();
    f3d_led_init();
    f3d_i2c1_init();
    delay(10);
    f3d_accel_init();
    delay(10);
    f3d_mag_init();
    delay(10);
    f3d_nunchuk_init();
    delay(10);
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

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
  rc = f_open(&Fil, "puppy.bmp", FA_READ);
  if (rc) die(rc);
  rc = f_read(&Fil, &magic, 2, &br);
  rc = f_read(&Fil, &header, sizeof(header), &br);
  rc = f_read(&Fil, &info, sizeof(info), &br);



  // do something with orientation (probably here)
  int c,r=0;
  for(;;) {
    rc = f_read(&Fil, &ImgBuff, sizeof(ImgBuff), &br);
    if (rc || !br) break;

    // do orientation stuff

      // set r and c values
    
      // drawPixel implementation
      // how could you do this with pushcolor instead?
      // (to get full credit you need to use pushcolor)
      f3d_lcd_drawPixel(c,r,colorChanger(&ImgBuff));
      if(c==128){c=0; r++;}
      else{++c;}
  }
  rc = f_close(&Fil);
  if (rc) die(rc);
}




int main(void){ 
 
  //f_mount(0, &Fatfs);		// Register volume work area (never fails) 

  printf("hello world");
  init();
  draw(0,0);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
