#ifndef __USART_H
#define __USART_H	 

#include "bsp.h"

#define Start_Symbol    0xEA
#define End_Symbol      0xED
#define Clarity_Symbol  0xEB

extern void UART_RemoteDecode(unsigned char InChar);

extern void USART1_Config(FunctionalState NewState);

extern void USART3_Config(FunctionalState NewState);

extern void UART_RemoteEncode(USART_TypeDef* USARTx, uint8_t TX_Buff[]);

void UART_Inbuff(unsigned char InChar);

void UART_Answer_RandomData(USART_TypeDef* USARTx, int16_t data_16, uint32_t data_32);

void UART_Answer_RandomNumber(USART_TypeDef* USARTx, int16_t data);

void UART_SendInbuff(uint8_t TX_Buff[], uint8_t InChar);

void UART_SEND(USART_TypeDef* USARTx, uint16_t Data);

void UART_RemoteEncode(USART_TypeDef* USARTx, uint8_t TX_Buff[]);

void UART_Answer_NoData(USART_TypeDef* USARTx); // 串口单字节指令应答

void UART_AnswerError(USART_TypeDef* USARTx, uint8_t ErrorID); // 串口通讯错误应答

void UART_SendInbuff(unsigned char TX_Buff[], uint8_t InChar);

struct Byte_8
{
	uint8_t B0;
	uint8_t B1;
	uint8_t B2;
	uint8_t B3;
	uint8_t B4;
	uint8_t B5;
	uint8_t B6;
	uint8_t B7;	
};

union Union_TypeConversion
{
	uint8_t 	TC_byte[8];
	uint16_t 	TC_u16[4];
	uint32_t 	TC_u32[2];
	uint8_t 	TC_uint8_t;	
	uint16_t 	TC_uint16_t;
	int16_t 	TC_int16_t;
	uint32_t	TC_uint32_t;
	int32_t		TC_int32_t;
	uint64_t	TC_uint64_t;
	int64_t		TC_int64_t;
	float 		TC_float;
	double		TC_double;
	//struct  Byte_8 bytes;
};

struct Bit{
	uint8_t b0 : 1;		//激光输出
	uint8_t b1 : 1; 	//中断1s触发信号
	uint8_t b2 : 1;		//红光
	uint8_t b3 : 1;
	uint8_t b4 : 1; 
	uint8_t b5 : 1;
	uint8_t b6 : 1;
	uint8_t b7 : 1; 
	uint8_t pulse_time ; 

};

union Union_Byte{
	uint16_t All;
	struct  Bit bits;
};


#endif  //__USART_H
