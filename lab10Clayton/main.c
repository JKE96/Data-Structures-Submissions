#include <stm32f30x.h>
#include <f3d_delay.h>
#include <f3d_systick.h>
#include <stm32f30x_misc.h>
#include <f3d_uart.h>
#include <f3d_user_btn.h>
#include <queue.h>
#include <stdio.h>
queue_t testQ;

void init() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    f3d_uart_init();
    f3d_led_init();
    f3d_user_btn_init();
    f3d_systick_init();
}

int main(void) {
  f3d_lcd_fillScreen(WHITE);
  init();
  
  while(1) {
    putchar(getchar());
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif
