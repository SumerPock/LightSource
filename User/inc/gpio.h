#ifndef __GPIO_H
#define __GPIO_H	

#include "stm32f4xx.h" 

#define LED0_ON			GPIO_SetBits(GPIOE, GPIO_Pin_6);
#define LED0_OFF		GPIO_ResetBits(GPIOE, GPIO_Pin_6);
#define LED0_Toogle	GPIO_ToggleBits(GPIOE, GPIO_Pin_6);

#define LED1_ON			GPIO_SetBits(GPIOE, GPIO_Pin_2);
#define LED1_OFF		GPIO_ResetBits(GPIOE, GPIO_Pin_2);
#define LED1_Toogle	GPIO_ToggleBits(GPIOE, GPIO_Pin_2);

#define LED2_ON			GPIO_SetBits(GPIOE, GPIO_Pin_3);
#define LED2_OFF		GPIO_ResetBits(GPIOE, GPIO_Pin_3);
#define LED2_Toogle	GPIO_ToggleBits(GPIOE, GPIO_Pin_3);

#define EN5_ON			GPIO_SetBits(GPIOB, GPIO_Pin_0);
#define EN5_OFF			GPIO_ResetBits(GPIOB, GPIO_Pin_0);
#define EN5_Toogle	GPIO_ToggleBits(GPIOB, GPIO_Pin_0);

#define EN6_ON			GPIO_SetBits(GPIOB, GPIO_Pin_1);
#define EN6_OFF			GPIO_ResetBits(GPIOB, GPIO_Pin_1);
#define EN6_Toogle	GPIO_ToggleBits(GPIOB, GPIO_Pin_1);

void User_GPIO_Init(void);

#endif
