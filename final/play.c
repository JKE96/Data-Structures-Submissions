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

FRESULT rc;			/* Result code */
DIR dir;			/* Directory object */
FILINFO fno;			/* File information object */
UINT bw, br;
unsigned int retval;
int bytesread;

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

int play(char *name) {
  	rc = f_open(&fid, name, FA_READ);

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
			
		// now skip all non-data chunks !
		
	while(1) {
		readckhd(&fid, &hd, 0);
		if (hd.ckID == 'atad')
		break;
		 f_lseek(&fid, hd.cksize);
	}
		
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
  
  rc = f_close(&fid);
	  
  if (rc) die(rc);
  return 0;
}




/* main.c ends here */
