#include <stm32f30x.h>
#include <f3d_delay.h>
#include <f3d_systick.h>
#include <stm32f30x_misc.h>
#include <f3d_uart.h>
#include <f3d_user_btn.h>
#include <queue.h>
#include <stdio.h>
queue_t testQ;



/*void USART1_IRQHandler(void) {
  int ch; 

  if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)) {
    ch = USART_ReceiveData(USART1);
    if (!enqueue(&rxbuf, ch)) {}    // overflow case -- 
                                   // throw away data and perhaps flag status
  }
  if (USART_GetFlagStatus(USART1,USART_FLAG_TXE)) {
    ch = dequeue(&txbuf,  (int *) ch);
    if (ch) {
      USART_SendData(USART1, ch);
    }
    else {                        // Queue is empty, disable interrupt
      USART_ITConfig(USART1,USART_IT_TXE,DISABLE);
      TxPrimed = 0;               // signal putchar to reenable
    }
  }
}
/////////////////
*/
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

  init();


}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif
