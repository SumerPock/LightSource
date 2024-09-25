#ifndef __FSMC_H
#define __FSMC_H

#include "stm32f4xx.h" 

/**
*宏定义名：FSMC_WR
*输入参数：offset
*实现功能：其中FSMC_Bank1_ADDR为基地址，offset为偏移地址，offset具体偏移地址取决去需要配置的寄存器
这个宏函数的作用是根据传入的 offset 计算出外部存储器或外设的地址，
然后返回一个指向该地址的 volatile uint16_t 类型的指针。这允许您以16位方式读写该地址的内容，
通常用于与外部设备进行通信或配置
**/
#define FSMC_WR(offset) *((volatile uint16_t*) (FSMC_Bank1_BaseADDR + (offset<<1)))
#define FSMC_Bank1_BaseADDR    ((uint32_t)(0x60000000))		


//函数声明	
void FSMC_GPIO_Config(void);	//初始化FSMC_GPIO
void FSMC_Init(void);					//初始化外部SRAM
void CycleCode_Renew(uint32_t CodeTime);

#endif
