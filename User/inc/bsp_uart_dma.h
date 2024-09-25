#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "bsp.h"

extern unsigned char  Uart_Tx_Buffer3[256]; 
extern unsigned char	Uart_Rx_Buffer3[256]; 
extern unsigned char	Uart_Tx_Buffer3[256]; 
extern unsigned char	Uart_Tx_Buffer1[256];
	
extern unsigned short Number_Rx_Buffer1;
extern unsigned short Number_Rx_Buffer3;

extern void processData(unsigned char *dataPtr);

extern void DMA_Tx(USART_TypeDef* USARTx , uint16_t Len);
extern void Uart_Init(USART_TypeDef* USARTx , \
							 uint32_t bound , \
							 GPIO_TypeDef* GPIOx1 , \
							 uint8_t GPIO_PinSourcex1 , \
							 GPIO_TypeDef* GPIOx2 ,\
							 uint8_t GPIO_PinSourcex2 , \
							 bool IsSbus , \
							 unsigned char prepriority , \
							 unsigned char subpriority);
extern void DMA_Rx(USART_TypeDef* USARTx);
extern void MyNVIC_Config(uint8_t NVIC_IRQChannel,uint8_t MainPriority,uint8_t SubPriority,FunctionalState NewState);
extern void Myuart_Config(USART_TypeDef* USARTx , uint32_t bound , bool IsSbus);
extern void MyGPIO_Config(USART_TypeDef* USARTx , GPIO_TypeDef* GPIOx1 , uint8_t GPIO_PinSourcex1 , GPIO_TypeDef* GPIOx2 , uint8_t GPIO_PinSourcex2);
extern void MyDMA_Config(uint32_t DMAx,DMA_Stream_TypeDef* DMAy_Streamx,uint32_t DMA_Channel_x,uint32_t cpar,uint32_t cmar,uint32_t dma_dir,uint16_t bufsize,uint32_t priority);
#endif

