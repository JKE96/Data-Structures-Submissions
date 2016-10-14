#**********************************************************
# Makefile
#
# These definitions set up the I/O interface for the PowerBox.
#
# Author: Ben Boskin(bboskin) and Jarod England(jkenglan)
# Date Created: OCt10th
# Last Modified by: Jarod
# Date Last Modified: Oct10th
# Assignment: lab6
# Part of: lab6
#/



TEMPLATEROOT = ..

# compilation flags for gdb

CFLAGS  = -O1 -g
ASFLAGS = -g 

# object files

OBJS=  $(STARTUP) main.o f3d_gyro.o f3d_led.o f3d_uart.o f3d_user_btn.o stm32f30x_spi.o stm32f30x_rcc.o stm32f30x_gpio.o stm32f30x_usart.o newlib_stubs.o f3d_delay.o f3d_lcd_sd.o glcdfont.o stm32f30x_tim.o

# include common make file

include $(TEMPLATEROOT)/c335.mk
