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
#include <math.h>
#include <diskio.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <f3d_user_btn.h>




FATFS Fatfs;		/* File system object */
FIL fid;		/* File object */
BYTE Buff[128];		/* File read buffer */

int picnum = 0;

typedef struct {
  unsigned char magic [2];
} bmpfile_magic;

typedef struct {
  uint32_t filesz ;
  uint16_t creator1 ;
  uint16_t creator2 ;
  uint32_t bmp_offset ;
} bmpfile_header;

typedef struct {
  uint32_t header_sz ;
  int32_t width ;
  int32_t height ;
  uint16_t nplanes ;
  uint16_t bitspp ;
  uint32_t compress_type ;
  uint32_t bmp_bytesz ;
  int32_t hres;
  int32_t vres;
  uint32_t ncolors ;
  uint32_t nimpcolors ;
} BITMAPINFOHEADER ;

typedef struct { // little endian byte order
  uint8_t b;
  uint8_t g;
  uint8_t r;
} bmppixel;

uint16_t convertColor(bmppixel* color);
void truncateImageBottom();
void readHeaders();

static const uint16_t height_width_difference = ST7735_height - ST7735_width;

void die (FRESULT rc) {
  printf("Failed with rc=%u.n", rc);
  while (1) ;
}

uint16_t convertColor(bmppixel *color) {  
  uint16_t r = color->r >> 3;
  uint16_t g = color->g >> 2;
  uint16_t b = color->b >> 3;

  uint16_t pixel = r;
  pixel |= (g << 5);
  pixel |= (b << 11);
  return pixel;
}

void truncateImageBottom() {
  UINT br;
  bmppixel trashPixelBuffer[ST7735_width];
  int i;
  for (i = 0; i < height_width_difference; i++) {
    f_read(&fid, (void *) trashPixelBuffer, sizeof(bmppixel) * ST7735_width, &br);
  }
}

bmpfile_magic magic;
bmpfile_header header;
BITMAPINFOHEADER info;

void readHeaders() {
  UINT br;
  
  f_read(&fid, (void *) &magic, 2, &br);
  f_read(&fid, (void *) &header, sizeof(header), &br);
  f_read(&fid, (void *) &info, sizeof(info), &br);
  
  // @hack - we have to throw away a bunch of bytes here
  //         for the image to line up correctly
  const int TRASH_BYTES = sizeof(bmppixel) * 28;
  bmppixel bmpTrash[TRASH_BYTES];
  //f_read(&Fil, (void *) bmpTrash, TRASH_BYTES, &br);
}


void init() {
  printf("Initialize starting");
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  f3d_uart_init();
  f3d_lcd_init();
  f3d_delay_init();
  f3d_rtc_init();
  f3d_led_init();
  f3d_led_all_off();
  f3d_i2c1_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f3d_mag_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);
  f3d_rtc_init();
  delay(10);
  f3d_user_btn_init();
  delay(10);
  printf("All initialized\n");
}

void drawpicture(){
  char footer[20];
  int count=0;
  int i;
  FRESULT rc;         
  DIR dir;            
  FILINFO fno;          
  UINT bw, br;
  unsigned int retval;


  f_mount(0, &Fatfs); 
  char fname[10];
  if(picnum ==0){
  strcpy(fname,"player.bmp");
  }
  else{
  strcpy(fname,"player.bmp");
  }

  rc = f_open(&fid, fname, FA_READ);
  if (rc) die(rc);
  readHeaders(); 
  //printf("Read Headers");
  int row, col;
  bmppixel bmpBuffer[sizeof(bmppixel) * ST7735_width];
  uint16_t rowBuffer[ST7735_width];
  
  int border = 0;
  for (row = border; row <= ST7735_height - 1 - border; row++) {
    // read in one row of the image
    // printf("row is %d", row);
    f_read(&fid, (void *) bmpBuffer, sizeof(bmppixel) * ST7735_width, &br);
      
    for (col = ST7735_width - 1; col >= 0; col--) {
      // convert bmp pixels to ST7735 pixels
      rowBuffer[col] = convertColor(&bmpBuffer[col]);
    }
    //printf("%x",rowBuffer);
    // write to screen
    f3d_lcd_setAddrWindow(0, row, ST7735_width - 1, row + 1, 0x2);
    f3d_lcd_pushColor(rowBuffer, ST7735_width);
  }   
}
    
    

void drawpicture2(){
  char footer[20];
  int count=0;
  int i;
  FRESULT rc;         
  DIR dir;            
  FILINFO fno;          
  UINT bw, br;
  unsigned int retval;


  f_mount(0, &Fatfs); 
  char fname[10];
  if(picnum ==0){
      strcpy(fname,"puppy.bmp");
    }
  else{
    strcpy(fname,"kitten.bmp");
  }
  rc = f_open(&fid, fname, FA_READ);
  if (rc) die(rc);
  readHeaders(); 
  //printf("Read Headers");
  int row, col;
  bmppixel bmpBuffer[sizeof(bmppixel) * ST7735_width];
  uint16_t rowBuffer[ST7735_width];
  
  int border = 0;
  for (row = border; row <= ST7735_height - 1 - border; row++) {
    // read in one row of the image
    // printf("row is %d", row);
    f_read(&fid, (void *) bmpBuffer, sizeof(bmppixel) * ST7735_width, &br);
      
    for (col = ST7735_width - 1; col >= 0; col--) {
      // convert bmp pixels to ST7735 pixels
      rowBuffer[col] = convertColor(&bmpBuffer[col]);
    }
    //printf("%x",rowBuffer);
    // write to screen
    f3d_lcd_setAddrWindow(0, row, ST7735_width - 1, row + 1, 0x3);
    f3d_lcd_pushColor(rowBuffer, ST7735_width);
  }   
}

int main(void) {
  init();
  f3d_gyro_init();
  f3d_lcd_fillScreen(WHITE);
 
  int a =1;
  float array[3] = {1.1,2.2,3.3};
 
  
  drawpicture();

  int c,z,num,numy;
  nunchuk_t nun;

  while(1){
    printf("while loop");

    f3d_nunchuk_read(&nun);
    c = nun.c;
    z = nun.z;
    num = nun.jx;
    numy = nun.jy;

    printf("%d",c);
    f3d_gyro_getdata(array);
    if(z==1||num==0||c==1|| num ==255||user_btn_read()){
      picnum = picnum +1;
      picnum = picnum %2;
      a = -1;
      array[1] = -10;
      printf("changing pic num");
    }
    printf("a is %f\n",array[1]);

    if(array[1] < -5 && (a!=0)) {
      printf("one");
      drawpicture();
      a = 0;
    }
    if(array[1] >=5 && (a!=1)){
      printf("two %d",a);
      drawpicture2();
      a=1;
      }
  }
  printf("ending program");
}


#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
