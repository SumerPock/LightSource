#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "bsp.h"
extern union Union_TypeConversion Byte_TypeConversion;

// º¯ÊýÔ­ÐÍ
extern void HandleStartSymbol();
extern void HandleClaritySymbol(uint8_t inputCharacter);
extern void HandleEndSymbol();
extern void HandleValidData(uint8_t inputCharacter);
extern void UART_Inbuff(uint8_t inputCharacter);
#endif

