#ifndef __RANDOMCODE_H
#define __RANDOMCODE_H	

#include <stdbool.h>
#include "stm32f4xx.h"
#include "bsp.h"

bool RandomCode_Check(void);
uint64_t RandomSaveData_Check (uint32_t Address);
bool Random_Flash2Buff(void);

extern uint16_t u16_RandomData[1024];
extern unsigned int u32_FlashBuff[2048];




#endif
