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

static char files[10][30];
int number_of_files = 0;
int current_file = 0;

const char *b[1];

extern uint8_t Audiobuf[AUDIOBUFSIZE];
extern int audioplayerHalf;
extern int audioplayerWhole;

FATFS Fatfs;        /* File system object */
FIL fid;            /* File object */
BYTE Buff[512];     /* File read buffer */
int ret;
nunchuk_t nunchuk;

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

//Display the current files 
void display_and_store() {
	int x = 20;
	int y = 10;
	int i = 0;
	rc = f_opendir(&dir, "");
	if (rc) die(rc);
	for (;;) {
		delay(30);
		rc = f_readdir(&dir, &fno);		/* Read a directory item */
		if (rc || !fno.fname[0]) break;	/* Error or end of dir */
		if (fno.fattrib & AM_DIR);
		else {
			strcpy(files[i], fno.fname);
			f3d_lcd_drawString(x, y, files[i], BLACK, WHITE);
			y += 10;
			i++;
			number_of_files++;
		}
	}
}

//Displays the selector for the file to play from the SD card based on the offset you pass it, which should be the current file to play
void selector_display(int offset) {
	f3d_lcd_fillScreen(WHITE);
	int i;
	int x = 20;
	int y = 10;
	for(i = 0; i < number_of_files; i++) {
		f3d_lcd_drawString(x, y, files[i], BLACK, WHITE);
		y += 10;
	}
	int newx;
	int newy;
	int y_offset = ((offset + 1) * 10) + 5;
	for(newx = 10; newx < 15; newx++) {
		for(newy = (offset + 1) * 10; newy < y_offset; newy++) {
			f3d_lcd_drawPixel(newx, newy, RED);
		}
		f3d_lcd_drawPixel(newx, newy, RED);
	}
}

//checks if the current file has a .wav extension
int check_extension() {
	int i;
	for(i = 0; i < sizeof(files[current_file]); i++) {
		if(files[current_file][i] == '.') {
			if(files[current_file][i+1] == 'W' && files[current_file][i+2] == 'A' && files[current_file][i+3] == 'V')
				return 1;
		}
	}
	return 0;
}


int main(void) { 
  printf("starting program");
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
  
	//inits
	f3d_delay_init();
	f3d_uart_init();
	delay(10);
	f3d_lcd_init();
	delay(10);
	f3d_i2c1_init();
	delay(10);
	f3d_rtc_init();
	delay(10);
	f3d_nunchuk_init();
	delay(10);
	f3d_timer2_init();
	delay(10);
	f3d_dac_init();
	delay(10);
	f3d_systick_init();


	f3d_lcd_fillScreen(WHITE);
  
	f_mount(0, &Fatfs);/* Register volume work area */

	char *thermo = "thermo.wav";
	play(thermo);

	display_and_store();
	selector_display(current_file);
	
	//Based on nunchuk input, play the file, change the file to be played, and display the new selector for the file to be played
	while(1) {
		f3d_nunchuk_read(&nunchuk);
		if(nunchuk.z == 1) {
			if(check_extension())
				play(files[current_file]);
		}
		if(nunchuk.jx == 0x00) {
			if(current_file == 0)
				current_file = number_of_files - 1;
			else
				current_file--;
			selector_display(current_file);
		}
		if(nunchuk.jx == 0xff) {
			if(current_file == number_of_files - 1)
				current_file = 0;
			else
				current_file++;
			selector_display(current_file);
		}
		delay(50);
	}
  
  while (1);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif


/* main.c ends here */
