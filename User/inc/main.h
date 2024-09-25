/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.h 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#define  VERSION_SELECTION   (Ver_2_2_0) 

/* 版本控制宏定义，版本具体区别请见 2023log.txt */
#define Ver_2_1_0 (1)		//2.1.0版本控制
#define Ver_2_1_1 (2)		//2.1.1版本控制，目前已作废
#define Ver_2_2_0 (3)		//2.1.2版本控制


#if (VERSION_SELECTION == Ver_2_1_0)	

#elif (VERSION_SELECTION == Ver_2_1_1)

#elif (VERSION_SELECTION == Ver_2_2_0)

#endif



static void Delay(__IO uint32_t nTime);
void Laser_StateTurn(FunctionalState OldState);
void Cross_StateTurn(FunctionalState OldState);
void Pulse_StateTurn(FunctionalState OldState);

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);

enum number
{
	row_0 = 0,
	row_1 = 6,
	row_2 = 12,
	row_3 = 18,
	row_4 = 24,
};

enum mybool
{
	mytrue = 1,
	myfalse = 0,
};

typedef struct 
{
	unsigned char control_number_low;
	unsigned char control_number_hight;
	unsigned char showflag : 1;
	unsigned short show;
	unsigned char Program_control_mode_1;
	unsigned char Program_control_mode_2;
}control_number;
extern control_number cn;

typedef struct 
{
	unsigned char flag_timer2_1s;
}FLAG_TIMER;
extern FLAG_TIMER flag_timer2;

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
