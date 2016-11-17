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

#define TIMER 20000
#define AUDIOBUFSIZE 128

FATFS Fatfs;        /* File system object */
FIL fid;        /* File object */
BYTE Buff[512];     /* File read buffer */
int ret;

const char *b[1];
int mode = 0;

extern uint8_t Audiobuf[AUDIOBUFSIZE];
extern int audioplayerHalf;
extern int audioplayerWhole;

FATFS Fatfs;        /* File system object */
FIL fid;            /* File object */
BYTE Buff[512];     /* File read buffer */
int ret;

struct ckhd {
    uint32_t ckID;
    uint32_t cksize;
};

struct fmtck {
    uint16_t wFormatTag;
    uint16_t nChannels;
    uint32_t nSamplesPerSec;
    uint32_t nAvgBytesPerSec;
    uint16_t nBlockAlign;
    uint16_t wBitsPerSample;
};

void readckhd(FIL *fid, struct ckhd *hd, uint32_t ckID) {
    f_read(fid, hd, sizeof(struct ckhd), &ret);
    if (ret != sizeof(struct ckhd))
    exit(-1);
    if (ckID && (ckID != hd->ckID))
    exit(-1);
}

void die (FRESULT rc) {
    printf("Failed with rc=%u.n", rc);
    while (1);
}



int main(void) {
  /*
    init();
    // Draw some fancy stuff to select audio files
    while(1) {
      // Read nunchuk
      // if c or z is pressed it will play the selected file
        if (DO NUNCHUK STUFF) {
        play();
      }
      // if up on the nunchuck is hit, it will go to the previous file
        if (DO NUNCHUK STUFF) {
        mode = mode - 1;
        previous();
      }
      // if down on the nunchuck is hit, it will go to the next file
        if (DO NUNCHUK STUFF)  {
        mode = mode + 1;
        next();
      }
      // if user button is pressed also go to the next song AND play it
      if (DO USER BUTTON STUFF) {
        while(DO USER BUTTON STUFF);
        mode = mode + 1;
        next();
          delay(10);
        play();
      }
    }
*/
}



#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif


/* main.c ends here */
