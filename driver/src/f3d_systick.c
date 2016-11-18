/* f3d_systick.c ---
 *
 * Filename: f3d_systick.c
 * Description:
 * Author: Bryce Himebaugh
 * Maintainer:
 * Created: Thu Nov 14 07:57:37 2013
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

/* Copyright (c) 2004-2007 The Trustees of Indiana University and
 * Indiana University Research and Technology Corporation.
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */

/* Code: */

#include <f3d_systick.h>
#include <f3d_led.h>
#include <f3d_user_btn.h>
#include <f3d_uart.h>

volatile int systick_flag = 0;

void f3d_systick_init(void) {
  SysTick_Config(SystemCoreClock / 100);
}

int light = 0;

void SysTick_Handler(void) {
  f3d_led_off(light);
  light = (light + 1) % 8;
  f3d_led_on(light);

  if (user_btn_read()) {
    SysTick_Config(SystemCoreClock / 10);
  } else {
    SysTick_Config(SystemCoreClock / 100);
  }
}

/* f3d_systick.c ends here */
