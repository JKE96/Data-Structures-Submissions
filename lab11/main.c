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

int init(){

    return 0;
}

int play(int a){

  FRESULT rc;			/* Result code */
  DIR dir;			/* Directory object */
  FILINFO fno;			/* File information object */
  UINT bw, br;
  unsigned int retval;
  int bytesread;


  f_mount(0, &Fatfs);/* Register volume work area */
  printf("\nOpen thermo.wav\n");
  rc = f_open(&fid, "thermo.wav", FA_READ);
  
  if (!rc) {
    struct ckhd hd;
    uint32_t  waveid;
    struct fmtck fck;
    
    readckhd(&fid, &hd, 'FFIR');
    
    f_read(&fid, &waveid, sizeof(waveid), &ret);
    if ((ret != sizeof(waveid)) || (waveid != 'EVAW'))
      return -1;
    
    readckhd(&fid, &hd, ' tmf');
    
    f_read(&fid, &fck, sizeof(fck), &ret);
    
    // skip over extra info
    
    if (hd.cksize != 16) {
      printf("extra header info %d\n", hd.cksize - 16);
      f_lseek(&fid, hd.cksize - 16);
    }
    
    printf("audio format 0x%x\n", fck.wFormatTag);
    printf("channels %d\n", fck.nChannels);
    printf("sample rate %d\n", fck.nSamplesPerSec);
    printf("data rate %d\n", fck.nAvgBytesPerSec);
    printf("block alignment %d\n", fck.nBlockAlign);
    printf("bits per sample %d\n", fck.wBitsPerSample);
    
    // now skip all non-data chunks !
    
    while(1){
      readckhd(&fid, &hd, 0);
      if (hd.ckID == 'atad')
	break;
      f_lseek(&fid, hd.cksize);
    }
    
    printf("Samples %d\n", hd.cksize);
    
    // Play it !
    
    //      audioplayerInit(fck.nSamplesPerSec);
    
    f_read(&fid, Audiobuf, AUDIOBUFSIZE, &ret);
    hd.cksize -= ret;
    audioplayerStart();
    while (hd.cksize) {
      int next = hd.cksize > AUDIOBUFSIZE/2 ? AUDIOBUFSIZE/2 : hd.cksize;
      if (audioplayerHalf) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(Audiobuf, AUDIOBUFSIZE/2);
	f_read(&fid, Audiobuf, next, &ret);
	hd.cksize -= ret;
	audioplayerHalf = 0;
      }
      if (audioplayerWhole) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(&Audiobuf[AUDIOBUFSIZE/2], AUDIOBUFSIZE/2);
	f_read(&fid, &Audiobuf[AUDIOBUFSIZE/2], next, &ret);
	hd.cksize -= ret;
	audioplayerWhole = 0;
      }
    }
    audioplayerStop();
  }
  
  printf("\nClose the file.\n"); 
  rc = f_close(&fid);
  if (rc) die(rc);
}



int main(void) { 
  

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_uart_init();
  f3d_timer2_init();
  f3d_dac_init();
  f3d_delay_init();
  f3d_rtc_init();
  f3d_systick_init();

  f3d_lcd_init();
  delay(10);
  f3d_i2c1_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);

  printf("Reset\n");
  
  play(1);

  while (1);
}
    /*
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



#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif


/* main.c ends here */
