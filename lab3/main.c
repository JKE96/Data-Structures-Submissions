#include <stm32f30x.h>  // Pull in include files for F30x standard drivers

#include <f3d_led.h>     // Pull in include file for the local drivers

#include <f3d_user_btn.h>

// Simple looping delay function
void delay(void) {
  int i = 500000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void) {
  f3d_led_init();
  f3d_user_btn_init();
  int light =8;
  while(1){
    if(light>15){
      f3d_led_all_off();
      delay();
      light=8;
    }
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){
      //pause
    }
    else{
      f3d_led_on(light);
    delay();
    light++;
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
