#include <f3d_systick.h>
#include <f3d_uart.h>
#include <queue.h>

volatile int systick_flag = 0; // why is this volatile????

int light =8;

void f3d_systick_init(void) {
  SysTick_Config(SystemCoreClock/SYSTICK_INT_SEC);
}

void SysTick_Handler(void) {
  // putchar(getchar()); // for queue
 
  int check = getchar();
  if(check != 0){
    putchar(check);
  }

 if(user_btn_read()){
    SysTick_Config(SystemCoreClock/10);
  }
  else{
    SysTick_Config(SystemCoreClock/100);
  }

  if (!queue_empty(&txbuf)) { 
    flush_uart(); // gotta create this in your uart!
  }

  light++;
  f3d_led_all_off();
  if(light>15){
    light=8;
  }
  f3d_led_on(light);

}
/* f3d_systick.c ends here */
