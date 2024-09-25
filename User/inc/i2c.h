#ifndef __I2C_H
#define __I2C_H	

#include "stm32f4xx.h" 

void IIC_Init(void);
uint8_t I2C_ReadOneByte(I2C_TypeDef *I2Cx, uint8_t I2C_Addr, uint8_t Reg_addr);
void I2C_WriteOneByte(I2C_TypeDef *I2Cx, uint8_t I2C_Addr, uint8_t Reg_addr, uint8_t value);
float I2C_LM75_Read(void);
float data_convert(uint16_t data);

#endif
