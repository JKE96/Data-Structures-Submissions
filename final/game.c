
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


#define TIMER 20000
#define AUDIOBUFSIZE 128

FATFS Fatfs;        /* File system object */
FIL fid;        /* File object */
BYTE Buff[512];     /* File read buffer */
int ret;

const char *b[1];

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
  f_mount(0, &Fatfs);
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


int prevY =0;

int level=0;

int time=0;

struct Player {
  int x1;
  int x2;
  int y1;
  int y2;
  int xvel;
  int yvel;
  int life;
  int alive;
  int points;
  // char name[20];
};

struct Monster {
  int x1;
  int x2;
  int y1;
  int y2;
  int xvel;
  int yvel;
  int alive;
};

struct Coin {
  int x;
  int y;
  int alive;
};
    
struct Obstacle{
  int x1;
  int x2;
  int y1;
  int y2;
};

struct Door{  //might want the door same place everytime so may delete this
  int x1;
  int x2;
  int y1;
  int y2;
};

nunchuk_t nunchuk;

struct Player plyr;
struct Coin zOne;
struct Coin zTwo;
struct Coin zThree;

struct Obstacle obOne;
struct Monster monOne;

struct Door door;

void levelDisplay(){
  char str[10];
  sprintf(str, "%d", level);
  f3d_lcd_drawString(0,110, "You are on level: ", BLUE, WHITE);
  f3d_lcd_drawString(0,120, str, BLUE, WHITE);

  char strr[10];
  sprintf(strr,"%d",time);
  f3d_lcd_drawString(0,130, "Your time is: ", BLUE, WHITE);
  f3d_lcd_drawString(0,140, strr, BLUE, WHITE);
}

void coinDisplay(){
  if(!zOne.alive){
    f3d_led_on(0);
    f3d_led_on(1);
  }
  if(!zTwo.alive){
    f3d_led_on(2);
    f3d_led_on(3);
  }
  if(!zThree.alive){
    f3d_led_on(4);
    f3d_led_on(5);
  }
}



void transition(){
  int a=0;
  while(a<=160){
    drawRect(0,128,a,a+20,BLUE);
    a= a+20;
  }
  f3d_lcd_fillScreen(BLACK);
}

void initMapOne(){
  plyr.x1 = 0;
  plyr.x2 = 5;
  plyr.y1 = 0;
  plyr.y2 = 5;
  plyr.xvel = 0;
  plyr.yvel=0;
  
  zOne.x = 10;
  zOne.y = 10;
  zOne.alive=1;

  zTwo.x=20;
  zTwo.y=20;
  zTwo.alive=1;

  zThree.x=30;
  zThree.y=30;
  zThree.alive=1;

  obOne.x1 = 40;
  obOne.x2 = 100;
  obOne.y1= 20;
  obOne.y2=30;

  monOne.x1=randomy(80, 145);
  monOne.x2=monOne.x1+10;
  monOne.y1=0;
  monOne.y2=10;
  monOne.alive=1;
}

void initMapTwo(){
  plyr.x1 = 0;
  plyr.x2 = 5;
  plyr.y1 = 0;
  plyr.y2 = 5;
  plyr.xvel = 0;
  plyr.yvel=0;

  obOne.x1 = randomy(20,70);
  obOne.x2 = randomy(obOne.x1+20,100);
  obOne.y1= randomy(0,20);
  obOne.y2=randomy(obOne.y1+5,30);

  zOne.x = randomy(obOne.x1-10,obOne.x1-6);
  zOne.y = randomy(10,30);
  zOne.alive=1;

  zTwo.x=randomy(obOne.x1+1,obOne.x1+10);
  zTwo.y=randomy(obOne.y2+5,50);
  zTwo.alive=1;

  zThree.x=randomy(obOne.x2,obOne.x2+5);
  zThree.y=randomy(obOne.y2+5,50);
  zThree.alive=1;

  int size = randomy(5,15);
  monOne.x1=randomy(80,140);
  monOne.x2=monOne.x1+ size;
  monOne.y1=100;
  monOne.y2=monOne.y1+size;
  monOne.alive=1;
}




void createMapOne(){
  initMapOne();

  drawRect(obOne.y1,obOne.y2,obOne.x1,obOne.x2,GREEN);

  drawCoins();
  drawMonsterOne();
  drawDoor();
  drawPlayer();
  drawLife();
}

void createMapTwo(){
  initMapTwo();

  drawRect(obOne.y1,obOne.y2,obOne.x1,obOne.x2,GREEN);

  drawCoins();
  drawMonsterOne();
  drawDoor();
  drawPlayer();
  drawLife();
}

void jump(){
  if(!plyr.yvel && prevY!=1){
    char *jump ="jump.wav";
    play(jump);
    plyr.yvel= plyr.yvel+9;
  }
}

void jumpMon() {
  if(!monOne.yvel) {
    monOne.yvel = monOne.yvel+9;
  }
}

void drawMonsterOne() {
  if(monOne.alive){
  drawRect(monOne.y1,monOne.y2,monOne.x1,monOne.x2,RED);
  }
}
void drawMonsterWhite() {
  if(monOne.alive) {
    drawRect(monOne.y1, monOne.y2, monOne.x1, monOne.x2, BLACK);
  }
}
void drawDoor(){
  drawRect(door.y1,door.y2, door.x1, door.x2, BLUE);
}

void drawCoins(){
  if(zOne.alive){
    drawRect(zOne.y,zOne.y+5, zOne.x,zOne.x+5, YELLOW);

  }
  if(zTwo.alive){
    drawRect(zTwo.y,zTwo.y+5, zTwo.x,zTwo.x+5, YELLOW);
  }
  if(zThree.alive){
    drawRect(zThree.y,zThree.y+5, zThree.x,zThree.x+5, YELLOW);
  }
  if(!zOne.alive){
    drawRect(zOne.y,zOne.y+5, zOne.x,zOne.x+5, BLACK);
    drawRect(120,125,115,120,YELLOW);

  }
  if(!zTwo.alive){
    drawRect(zTwo.y,zTwo.y+5, zTwo.x,zTwo.x+5, BLACK);
    drawRect(120,125,125,130,YELLOW);
  }
  if(!zThree.alive){
    drawRect(zThree.y,zThree.y+5, zThree.x,zThree.x+5, BLACK);
    drawRect(120,125,135,140,YELLOW);
  }
}
void drawPlayer(){
  drawRect(plyr.y1, plyr.y2,plyr.x1,plyr.x2,WHITE);
}
void drawPlayerWhite(){
  drawRect(plyr.y1, plyr.y2,plyr.x1,plyr.x2,BLACK);
}

void drawLife(){
  drawRect(120,125,0,plyr.life*2,BLACK);
  int a =0;
  while(a<plyr.life*2){
    int b = a*5;
    if(a%2){
      drawRect(120,125, b,b+5,WHITE);
    }
    a=a+1;
  }
}

int checkDie(){
  if( hitObject(monOne.x1,monOne.x2,monOne.y1,monOne.y2)){
    plyr.alive=0;
  }
}

float a[3] = {1.1, 2.2, 3.3};
void monsterAttack(){
  char *attack = "pause.wav";
  play(attack);
  plyr.alive =1;
  int time = 0;
  int b =0;
  f3d_lcd_fillScreen(WHITE);
  f3d_lcd_drawString(0,100,"MONSTER ATTACK SHAKE THE BOARD TO LIVE!",BLACK,WHITE);
  delay(10);
  int xPrev =0;
  int yPrev =0;
  int zPrev =0;
  int xValue,yValue,zValue=0;
  while(b<3){
    time= time+1;
    
    drawRect(0,time,0,10,BLACK);
    
    f3d_gyro_getdata(a);
    xPrev = xValue;
    yPrev = yValue;
    zPrev = zValue;
    xValue = a[0];
    yValue = a[1];
    zValue = a[2];

    if(xValue!=xPrev && yValue!=yPrev && zValue !=zPrev){
      ++b;
    }
    if(b==1){
      f3d_lcd_fillScreen(YELLOW);
    }
    if(b==2){
      f3d_lcd_fillScreen(RED);
    }
    if(time==64){
      f3d_lcd_drawString(0,120,"HURRY HE IS HALFWAY DONE EATING YOU",BLACK,WHITE); 
      delay(1000);
    }
    if(time>128){
      f3d_lcd_drawString(0,120,"DAVE LOST 1 LIFE",BLACK,WHITE); 
      delay(1000);
      plyr.life = plyr.life-1;
      f3d_lcd_fillScreen(BLACK);
      runLevelTwo();
    }
  }//while
  f3d_lcd_drawString(0,140,"DAVE SURVIVED",BLACK,WHITE); 
  f3d_lcd_fillScreen(BLACK);
  runLevelTwo();
}



void onTick(){
  time++;
  //redraw monster and redraw player redraw the points and lives - 
  //will have to delete old then move then redraw //run gravity
  //Then Check if player is by monster or coins or obstacle and act accordingly
  prevY=plyr.yvel;
  gravity();
  gravityMon();
  monsterAI();
  drawDoor();
  drawCoins();
  checkDie(); //checks to see if monster collides with the player
  coinDisplay();

  if(monOne.xvel || monOne.yvel) {
    drawMonsterWhite();
    moveMonster();
    drawMonsterOne();
    coinCheck();
  }
  //monster stuff

  //delete old

  if(plyr.xvel||plyr.yvel){
    drawPlayerWhite();
  //moves stuff
    
    
    int a =zOne.alive;
    int b= zTwo.alive;
    int c= zThree.alive;
    movePlayer();
    coinCheck();
    drawPlayer();
    if(a!=zOne.alive || b!=zTwo.alive || c!=zThree.alive){
      char *coin = "coin.wav";
      play(coin);
    }
  //check whats still alive and delete if not alive, also reposn plyr if die

  //creates new
   
  }
}


void runLevelOne(){
  int game=1;
  f3d_lcd_fillScreen(WHITE);
  f3d_led_all_off();
  f3d_lcd_drawString(0,100,"LEVEL ONE TUTORIALMAP", BLUE, WHITE);
  delay(50);
  transition();

  createMapOne();
  while(game){
     f3d_nunchuk_read(&nunchuk);
     f3d_nunchuk_read(&nunchuk);
     
     int joyx = nunchuk.jx;
     int joyy = nunchuk.jy;
     int c = nunchuk.c;
     int z=0;
     z = nunchuk.z;
     
     //moving the x joystick
     if(joyx >240){
       plyr.xvel=1;
     }
     else if(joyx<15){
       plyr.xvel=-1;
     }
     else{
       plyr.xvel=0;
     }
     ////////////////////
     if(z){
       jump();
     }
     if(c){
       int wait =1;
       char *pause = "pause.wav";
       play(pause);
       // delay(10);
       while(wait){
	 f3d_nunchuk_read(&nunchuk);
	 //delay(10);
	 f3d_nunchuk_read(&nunchuk);
	 c = nunchuk.c;
	 if(c){
	   play(pause);
	   wait=0;
	 }
       }//while

     }

     if(doorCheck()){
       game =0;
       f3d_led_all_on();
       char *win = "flagpole.wav";
       play(win);
     }
     if(!plyr.alive){
       monsterAttack();
     }

     onTick();
  }//while
  printf("levelonecomplete");
  level=level+1;
  runLevelTwo();
}

void startGame(){
  plyr.x1 = 0;
  plyr.x2 = 5;
  plyr.y1 = 0;
  plyr.y2 = 5;
  plyr.xvel = 0;
  plyr.yvel=0;
  plyr.life=3;
  plyr.alive=1;
  plyr.points=0;

  door.x1=148;
  door.x2=155;
  door.y1=0;
  door.y2=15;
 
  runLevelOne();
}

void gameOver(){
  f3d_lcd_fillScreen(WHITE);
  f3d_lcd_drawString(0,90,"GAME OVER! RIP DAVE", BLUE, WHITE);
  levelDisplay();
  char *gameover = "gameover.wav";
  play(gameover);
  delay(200);
  transition();
  main();
}

void runLevelTwo(){
  int game=1;
  f3d_lcd_fillScreen(WHITE);
  f3d_led_all_off();
  f3d_lcd_drawString(0,90,"STARTING RANDOM LEVEL", BLUE, WHITE);
  levelDisplay();
  delay(50);
  transition();

  createMapTwo();
  while(game){
     f3d_nunchuk_read(&nunchuk);
     int joyx = nunchuk.jx;
     int joyy = nunchuk.jy;
     int c = nunchuk.c;
     int z=0;
     z = nunchuk.z;
     
     //moving the x joystick
     if(joyx >240){
       plyr.xvel=1;
     }
     else if(joyx<15){
       plyr.xvel=-1;
     }
     else{
       plyr.xvel=0;
     }
     ////////////////////
     if(z){
       jump();
     }
     if(c){
       int wait =1;
       char *pause = "pause.wav";
       play(pause);
       delay(1000);
       while(wait){
	 f3d_nunchuk_read(&nunchuk);
	 
	 int u = nunchuk.c;
	 if(u){
	   play(pause);
	   wait=0;
	 }
       }
     }
     
     if(doorCheck()){
       game =0;
       f3d_led_all_on();
       char *win = "flagpole.wav";
       play(win);
     
     }

     if(plyr.life==0){
       gameOver();
     }

     if(!plyr.alive){
       monsterAttack();
     }
     
     onTick();
  }//while
  printf("levelcomplete");
  level=level+1;
  runLevelTwo();
}
void monsterAI() {
  int move = randomy(1,20);
  int action = randomy(0, 100);
  if(move == 1) {
    monOne.xvel = 1;
  }
  if(move == 20) {
    monOne.xvel = -1;
  }
  if(action == 56) {
    jumpMon();
  }
  else {
  }
 }
void moveMonster(){
  if(!xMovCheckMon()) {
    monOne.x1 += monOne.xvel;
    monOne.x2 += monOne.xvel;
  }
  else {
    monOne.xvel = 0;
  }

  if(!yMovCheckMon()) {
    monOne.y1 += monOne.yvel;
    monOne.y2 += monOne.yvel;
  }
  else {
    monOne.yvel;
  }

}

void movePlayer(){
  
  if(!xMovCheck()){//does not hit side of something or wall after move
    plyr.x1+=plyr.xvel;
    plyr.x2+=plyr.xvel;
  }
  else{
    plyr.xvel=0;
  }
  
  if(!yMovCheck()){//does not land on something or ground or ceiling{
    plyr.y1+=plyr.yvel;
    plyr.y2+=plyr.yvel;
  }
  else{
    plyr.yvel=0;
  }
}

void coinCheck(){
  if(hitObject(zOne.x,zOne.x+5,zOne.y,zOne.y+5)){
    zOne.alive =0;
    drawCoins();
  }
  if(hitObject(zTwo.x,zTwo.x+5,zTwo.y,zTwo.y+5)){
    zTwo.alive =0;
    drawCoins();
  }
  if(hitObject(zThree.x,zThree.x+5,zThree.y,zThree.y+5)){
    zThree.alive =0;
    drawCoins();
  }
  if(hitObjectMon(zOne.x,zOne.x+5,zOne.y,zOne.y+5)){
    drawCoins();
  }
  if(hitObjectMon(zTwo.x,zTwo.x+5,zTwo.y,zTwo.y+5)){
    drawCoins();
  }
  if(hitObjectMon(zThree.x,zThree.x+5,zThree.y,zThree.y+5)){
    drawCoins();
  }

}

int hitObject(ex1,ex2,ey1,ey2){
  
  if( (ex1<=plyr.x2+plyr.xvel) && (ex2>=plyr.x2+plyr.xvel) &&  (ey1<=plyr.y2+plyr.yvel) && (ey2>=plyr.y2+plyr.yvel)  ){
    return 1;
  }
  
  if( (ex1<=plyr.x2+plyr.xvel) && (ex2>=plyr.x2+plyr.xvel) &&  (ey1<=plyr.y1+plyr.yvel) && (ey2>=plyr.y1+plyr.yvel)  ){
    return 1;
  }
  
  if( (ex1<=plyr.x1+plyr.xvel) && (ex2>=plyr.x1+plyr.xvel) &&  (ey1<=plyr.y2+plyr.yvel) && (ey2>=plyr.y2+plyr.yvel)  ){
    return 1;
  }
  
  if( (ex1<=plyr.x1+plyr.xvel) && (ex2>=plyr.x1+plyr.xvel) &&  (ey1<=plyr.y1+plyr.yvel) && (ey2>=plyr.y1+plyr.yvel)  ){
    return 1;
  }
  return 0;
}

int hitObjectMon(ex1, ex2, ey1, ey2) {
  
  if( (ex1<=monOne.x2+monOne.xvel) && (ex2>=monOne.x2+monOne.xvel) &&  (ey1<=monOne.y2+monOne.yvel) && (ey2>=monOne.y2+monOne.yvel)  ){
    return 1;
   }
  
  if( (ex1<=monOne.x2+monOne.xvel) && (ex2>=monOne.x2+monOne.xvel) &&  (ey1<=monOne.y1+monOne.yvel) && (ey2>=monOne.y1+monOne.yvel)  ){
    return 1;
  }
  
  if( (ex1<=monOne.x1+monOne.xvel) && (ex2>=monOne.x1+monOne.xvel) &&  (ey1<=monOne.y2+monOne.yvel) && (ey2>=monOne.y2+monOne.yvel)  ){
    return 1;
  }
  
  if( (ex1<=monOne.x1+monOne.xvel) && (ex2>=monOne.x1+monOne.xvel) &&  (ey1<=monOne.y1+monOne.yvel) && (ey2>=monOne.y1+monOne.yvel)  ){
    return 1;
  }
  return 0;
}


int xMovCheck(){
  if(plyr.x1+plyr.xvel<0 || plyr.x2+plyr.xvel>160 || hitObject(obOne.x1,obOne.x2, obOne.y1,obOne.y2)     ){
    return 1;
  }
  else return 0;
}

int xMovCheckMon() {
  if(monOne.x1+monOne.xvel<0 || monOne.x2+monOne.xvel>160 || hitObjectMon(obOne.x1,obOne.x2, obOne.y1,obOne.y2)){
    return 1;
  }
  else return 0;
}


int yMovCheck(){
  if(plyr.y1+plyr.yvel<=0 || plyr.y2+plyr.yvel>128 || hitObject(obOne.x1,obOne.x2,obOne.y1,obOne.y2)       ){
    return 1;
  }
  else return 0;
 }

int yMovCheckMon() {
  if(monOne.y1+monOne.yvel<=0 || monOne.y2+monOne.yvel>128 || hitObjectMon(obOne.x1,obOne.x2,obOne.y1,obOne.y2)       ){
    return 1;
  }
  else return 0;
}

int doorCheck(){
  if(hitObject(door.x1,door.x2,door.y1,door.y2)&& !zOne.alive && !zTwo.alive && !zThree.alive  ){
    return 1;
  }
  return 0;
}




void gravity(){
  if(!yMovCheck()){
    plyr.yvel = plyr.yvel-1;
  }
   if(yMovCheck()){
     plyr.yvel=0;
   }
}

void gravityMon() {
  if(!yMovCheckMon()) {
    monOne.yvel = monOne.yvel - 1;
  }
  if(yMovCheckMon()) {
    monOne.yvel = 0;
  }
}


