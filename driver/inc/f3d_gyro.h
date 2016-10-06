/* f3d_gyro.h */

//This is the header file for the f3d_gyro.c. It holds the declarations for the gyro functions
//Author: Jarod England 
// Date Created: Sept. 2016
// Last Modified by: Jarod England  
// Date Last Modified: Oct 5. 2016 
// Assignment: Lab5
// Part of: lab5 






/* Code: */
#include <stm32f30x.h>

#define L3G_Sensitivity_250dps   (float) 114.285f
#define L3G_Sensitivity_500dps   (float) 57.1429f
#define L3G_Sensitivity_2000dps   (float) 14.285f

#define GYRO_CS_LOW() GPIO_ResetBits(GPIOE, GPIO_Pin_3) //SOMETHING NEEDED HERE! 
#define GYRO_CS_HIGH() GPIO_SetBits(GPIOE, GPIO_Pin_3) //SOMETHIGN NEEDED HERE!  



void f3d_gyro_interface_init();
void f3d_gyro_init(void);
void f3d_gyro_read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
void f3d_gyro_write(uint8_t*, uint8_t, uint16_t);
static uint8_t f3d_gyro_sendbyte(uint8_t);
void f3d_gyro_getdata(float *pfData);

/* f3d_gyro.h ends here */
