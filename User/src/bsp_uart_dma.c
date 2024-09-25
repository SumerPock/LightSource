#include "bsp.h"


/*******************************全局变量定义****************************************/
unsigned char	Uart_Rx_Buffer1[2048] = {0}; 
static unsigned short SizeUart_Rx_Buffer1 = 2048;
unsigned short Number_Rx_Buffer1 = 0;

unsigned char	Uart_Tx_Buffer1[256] = {0}; 
static unsigned short SizeUart_Tx_Buffer1 = 0;
unsigned short Number_Tx_Buffer1 = 0;

unsigned char	Uart_Rx_Buffer3[256] = {0}; 
static unsigned short SizeUart_Rx_Buffer3 = 256;
unsigned short Number_Rx_Buffer3 = 0;

unsigned char	Uart_Tx_Buffer3[256] = {0}; 
static unsigned short SizeUart_Tx_Buffer3 = 0;
 unsigned short Number_Tx_Buffer3 = 0;

/**
  * @brief  /通用串口配置函数：参数1：串口号；参数2：波特率；参数3/4：使用管脚号
  * @param  None
  * @retval None
  */
void Uart_Init(USART_TypeDef* USARTx , \
							 uint32_t bound , \
							 GPIO_TypeDef* GPIOx1 , \
							 uint8_t GPIO_PinSourcex1 , \
							 GPIO_TypeDef* GPIOx2 ,\
							 uint8_t GPIO_PinSourcex2 , \
							 bool IsSbus , \
							 unsigned char prepriority , \
							 unsigned char subpriority)
{
	uint8_t temp_IRQn = 0;
	Myuart_Config(USARTx , bound , IsSbus);
	MyGPIO_Config(USARTx , GPIOx1 , GPIO_PinSourcex1 , GPIOx2 , GPIO_PinSourcex2);
	
	/*串口接收DMA配置*/
	if(USARTx == USART1)		
	{
		MyDMA_Config(RCC_AHB1Periph_DMA2 , \
								 DMA2_Stream2 , \
								 DMA_Channel_4 , \
								 (uint32_t)&USART1->DR , \
							   /*(uint32_t)UART_RX_BUFFER[0] ,*/ 
								 (uint32_t)Uart_Rx_Buffer1 ,
								 DMA_DIR_PeripheralToMemory , \
							   /*UART_power_detection_rx_SIZE , \*/
								 SizeUart_Rx_Buffer1 ,
								 DMA_Priority_Low);    
		MyNVIC_Config(DMA2_Stream2_IRQn , 9 , 0 , ENABLE);
		temp_IRQn = USART1_IRQn;
	}
	
	//DMA1_Stream5 光源
	if(USARTx == USART2)		
	{
//		MyDMA_Config(RCC_AHB1Periph_DMA1 , \
//							   DMA1_Stream5 , \
//							   DMA_Channel_4 , \
//								 (uint32_t)&USART2->DR , \
//							   (uint32_t)UART_RX_BUFFER1, \
//								 DMA_DIR_PeripheralToMemory , \
//								 GY_UARTRXBuffer_SIZE , \
//								 DMA_Priority_Low);    
//		MyNVIC_Config(DMA1_Stream5_IRQn , 5 , 0 , ENABLE);
//		temp_IRQn = USART2_IRQn;
	}
	
	//DMA1_Stream1 电源检测
	if(USARTx == USART3)		
	{
		MyDMA_Config(RCC_AHB1Periph_DMA1 , 
								 DMA1_Stream1 , \
								 DMA_Channel_4, \
								 (uint32_t)&USART3->DR , \
						     /*(uint32_t)uart_power_detection_rx , */
								 (uint32_t)Uart_Rx_Buffer3 ,
								 DMA_DIR_PeripheralToMemory , \
								 /*UART_power_detection_rx_SIZE , \*/
								 SizeUart_Rx_Buffer3 ,
								 DMA_Priority_Low);    
		MyNVIC_Config(DMA1_Stream1_IRQn , 12 , 0 , ENABLE);
		temp_IRQn = USART3_IRQn;
	}
	
	if(USARTx == UART4)			//DMA1_Stream2
	{
//		MyDMA_Config(RCC_AHB1Periph_DMA1,DMA1_Stream2,DMA_Channel_4,(uint32_t)&UART4->DR,(uint32_t)UART_RX_BUFFER[3],DMA_DIR_PeripheralToMemory,UART_RXBuffer_SIZE,DMA_Priority_Low);    
//		MyNVIC_Config(DMA1_Stream2_IRQn,2,2,ENABLE);
//		temp_IRQn = UART4_IRQn;
	}
	if(USARTx == UART5)			//DMA1_Stream0
	{
//		MyDMA_Config(RCC_AHB1Periph_DMA1,DMA1_Stream0,DMA_Channel_4,(uint32_t)&UART5->DR,(uint32_t)UART_RX_BUFFER[4],DMA_DIR_PeripheralToMemory,UART_RXBuffer_SIZE,DMA_Priority_Low);    
//		MyNVIC_Config(DMA1_Stream0_IRQn , 15 , 0 , ENABLE);
//		temp_IRQn = UART5_IRQn;
	}
	
	//DMA2_Stream1 转台
	if(USARTx == USART6)		
	{
//		MyDMA_Config(RCC_AHB1Periph_DMA2 , \
//								 DMA2_Stream1 , \
//								 DMA_Channel_5 , \
//								 (uint32_t)&USART6->DR , \
//								 (uint32_t)UART_RX_BUFFER2 , \
//								 DMA_DIR_PeripheralToMemory , \
//								 ZT_UARTRXBuffer_SIZE ,  \
//								 DMA_Priority_Low);    
//		MyNVIC_Config(DMA2_Stream1_IRQn , 3 , 0 , ENABLE);
//		temp_IRQn = USART6_IRQn;
	}
	
	
	
	/*DMA发送中断设置 DMA2_Stream7 lm1266*/
	if(USARTx == USART1)	
	{
		MyDMA_Config(RCC_AHB1Periph_DMA2 , \
								 DMA2_Stream7 , \
								 DMA_Channel_4 , \
							   (uint32_t)&USART1->DR , \
								 /*(uint32_t)uart_power_detection_tx , \*/
								 (uint32_t)Uart_Tx_Buffer1 ,
								 DMA_DIR_MemoryToPeripheral , \
								 /*UART_power_detection_SIZE , \*/
								 SizeUart_Tx_Buffer1 ,
								 DMA_Priority_Low);
		MyNVIC_Config(DMA2_Stream7_IRQn , 10 , 0 , ENABLE);
		DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);   //使能中断
	}
	
	//DMA1_Stream6 光源串口发送
	if(USARTx == USART2)		
	{
//		MyDMA_Config(RCC_AHB1Periph_DMA1 , \
//								 DMA1_Stream6 , \
//								 DMA_Channel_4 , \
//								 (uint32_t)&USART2->DR ,\
//								 (uint32_t)dma_send_buffer_gy ,\
//							   DMA_DIR_MemoryToPeripheral , \
//								 GY_UARTTXBuffer_SIZE , \
//								 DMA_Priority_Low);
//		MyNVIC_Config(DMA1_Stream6_IRQn , 7 , 0 , ENABLE);
//		DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);   //使能中断
	}
	
	
	//DMA1_Stream3 电源检测
	if(USARTx == USART3)		
	{
		MyDMA_Config(RCC_AHB1Periph_DMA1 , 
								 DMA1_Stream3 , \
							   DMA_Channel_4 , \
							   (uint32_t)&USART3->DR , \
								 /*(uint32_t)uart_power_detection_tx , \*/
								 (uint32_t)Uart_Tx_Buffer3 ,
								 DMA_DIR_MemoryToPeripheral , \
								 /*UART_power_detection_SIZE , \*/
								 SizeUart_Tx_Buffer3 ,
								 DMA_Priority_Low);
		MyNVIC_Config(DMA1_Stream3_IRQn , 13 , 0 , ENABLE);
		DMA_ITConfig(DMA1_Stream3 , DMA_IT_TC,ENABLE);   //使能中断
	}
	
	if(USARTx == UART4)			//DMA1_Stream4
	{
//		MyDMA_Config(RCC_AHB1Periph_DMA1,DMA1_Stream4,DMA_Channel_4,(uint32_t)&UART4->DR,(uint32_t)UART_TX_BUFFER[3],DMA_DIR_MemoryToPeripheral,UART_TXBuffer_SIZE,DMA_Priority_Low);
//		MyNVIC_Config(DMA1_Stream4_IRQn,2,2,ENABLE);
//		DMA_ITConfig(DMA1_Stream4,DMA_IT_TC,ENABLE);   //使能中断
	}
	if(USARTx == UART5)			//DMA1_Stream7
	{
//		MyDMA_Config(RCC_AHB1Periph_DMA1,DMA1_Stream7,DMA_Channel_4,(uint32_t)&UART5->DR,(uint32_t)UART_TX_BUFFER[4],DMA_DIR_MemoryToPeripheral,UART_TXBuffer_SIZE,DMA_Priority_Low);
//		MyNVIC_Config(DMA1_Stream7_IRQn , 2 , 2 , ENABLE);
//		DMA_ITConfig(DMA1_Stream7 , DMA_IT_TC , ENABLE);   //使能中断
//		DMA_Cmd(DMA1_Stream7 , DISABLE);  
	}
	
	
	
	//DMA2_Stream6 转台控制
	if(USARTx == USART6)		
	{
//		MyDMA_Config(RCC_AHB1Periph_DMA2 ,\
//								 DMA2_Stream6 ,\
//								 DMA_Channel_5 ,\
//								 (uint32_t)&USART6->DR , \
//								 (uint32_t)dma_send_buffer_zt, \
//								 DMA_DIR_MemoryToPeripheral , \
//								 15 , \
//								 DMA_Priority_Low);
//		MyNVIC_Config(DMA2_Stream6_IRQn , 0 , 7 , ENABLE);
//		DMA_ITConfig(DMA2_Stream6,DMA_IT_TC,ENABLE);   //使能中断
	}
	
	/*采用DMA方式发送*/
	USART_DMACmd(USARTx,USART_DMAReq_Tx,ENABLE);	
	USART_ITConfig(USARTx,USART_IT_IDLE,ENABLE);  	
	USART_DMACmd(USARTx,USART_DMAReq_Rx,ENABLE);	  
	MyNVIC_Config(temp_IRQn , prepriority , subpriority , ENABLE);//打开串口中断   
	
  USART_Cmd(USARTx, ENABLE);    //启动串口 

	SizeUart_Tx_Buffer1 = 32;	
	SizeUart_Tx_Buffer3 = 32;
}


/**
  * @brief  USART1_IRQHandler
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void) 				
{
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET){
		DMA_Rx(USART1);
	}		
}

/**
  * @brief  串口1发送中断函数
  * @param  None
  * @retval None
  */
void DMA2_Stream7_IRQHandler(void)	
{
	if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//等待DMA2_Steam7传输完成
	{ 
		DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);						//清除DMA2_Steam7传输完成标志
		USART_ClearFlag(USART1,USART_FLAG_TC);								//清除串口2的标志位
		DMA_Cmd(DMA2_Stream7, DISABLE);                      	//关闭DMA传输 
	}	
}

/**
  * @brief  USART2_IRQHandler
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void) 
{
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET){
		DMA_Rx(USART2);
	}	
}

/**
  * @brief  DMA1_Stream6_IRQHandler 串口2发送中断函数
  * @param  None
  * @retval None
  */
void DMA1_Stream6_IRQHandler(void)	
{
	if(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!=RESET)//等待DMA2_Steam7传输完成
	{ 
		DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);				//清除DMA2_Steam7传输完成标志
		USART_ClearFlag(USART2,USART_FLAG_TC);						//清除串口2的标志位
		DMA_Cmd(DMA1_Stream6, DISABLE);                   //关闭DMA传输 
	}	
}

/**
  * @brief  USART3_IRQHandler
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET){
		DMA_Rx(USART3);
	}	
}


/**
  * @brief  USART3_IRQHandler 串口3发送中断函数
  * @param  None
  * @retval None
  */
void DMA1_Stream3_IRQHandler(void)	
{
	if(DMA_GetFlagStatus(DMA1_Stream3,DMA_FLAG_TCIF3)!=RESET)//等待DMA2_Steam7传输完成
	{ 
		DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);							//清除DMA2_Steam7传输完成标志
		DMA_Cmd(DMA1_Stream3, DISABLE);                      		//关闭DMA传输 
	}	
}


/**
  * @brief  UART4_IRQHandler
  * @param  None
  * @retval None
  */
void UART4_IRQHandler(void) 	
{
	if(USART_GetITStatus(UART4, USART_IT_IDLE) != RESET){
		DMA_Rx(UART4);
	}	
}


/**
  * @brief  DMA1_Stream4_IRQHandler  串口4发送中断函数
  * @param  None
  * @retval None
  */
void DMA1_Stream4_IRQHandler(void)	
{
	if(DMA_GetFlagStatus(DMA1_Stream4,DMA_FLAG_TCIF4)!=RESET)//等待DMA2_Steam7传输完成
	{ 
		DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);							//清除DMA2_Steam7传输完成标志
		DMA_Cmd(DMA1_Stream4, DISABLE);                      		//关闭DMA传输 
	}	
}

/**
  * @brief  UART5_IRQHandler 
  * @param  None
  * @retval None
  */
void UART5_IRQHandler(void) 			
{
	if(USART_GetITStatus(UART5, USART_IT_IDLE) != RESET){
		DMA_Rx(UART5);
	}	
}


/**
  * @brief  DMA1_Stream7_IRQHandler  串口5发送中断函数
  * @param  None
  * @retval None
  */
void DMA1_Stream7_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_Stream7,DMA_FLAG_TCIF7)!=RESET)//等待DMA2_Steam7传输完成
	{ 
		DMA_ClearFlag(DMA1_Stream7,DMA_FLAG_TCIF7);//清除DMA2_Steam7传输完成标志
		DMA_Cmd(DMA1_Stream7, DISABLE);                      //关闭DMA传输 
	}	
}



void USART6_IRQHandler(void) 			
{
	if(USART_GetITStatus(USART6, USART_IT_IDLE) != RESET){
		DMA_Rx(USART6);
	}	
}



void DMA2_Stream6_IRQHandler(void)	//串口6发送中断函数
{
	if(DMA_GetFlagStatus(DMA2_Stream6,DMA_FLAG_TCIF6)!=RESET)//等待DMA2_Steam7传输完成
	{ 
		DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);//清除DMA2_Steam7传输完成标志
		DMA_Cmd(DMA2_Stream6, DISABLE);                      //关闭DMA传输 
	}	
}


void processData(unsigned char *dataPtr) 
{
    // 在这里处理数据，使用 *dataPtr 访问数据
   //printf("Received value in File1: %d\n", *dataPtr);
	
	*dataPtr = Number_Rx_Buffer1;
}

//使用DMA进行数据接收时调用该函数,需要在串口接收中断中调用，因为使用DMA接收，接收缓存地址已提前预置，并且标志位也固定，因此不支持任意数组和标志位接收
void DMA_Rx(USART_TypeDef* USARTx)	
{
	//uint8_t num;
	DMA_Stream_TypeDef* DMAy_Streamx;
	uint32_t Flag;
	
	if(USARTx == USART1)
	{
		DMAy_Streamx = DMA2_Stream2;
		//num = 0;
		Flag = DMA_FLAG_TCIF2;
		Number_Rx_Buffer1 = SizeUart_Rx_Buffer1 - DMA_GetCurrDataCounter(DMA2_Stream2);/*数据长度计算*/
		if(RB_Get_FreeSize(&rb_uart1_rx) != 0)	/*队列入队*/
		{
			for(int i = 0 ; i < Number_Rx_Buffer1 ; i++)
			{
				RB_Write_Byte(&rb_uart1_rx, Uart_Rx_Buffer1[i]);
			}
		}	
	}
	
	if(USARTx == USART2)
	{
		DMAy_Streamx = DMA1_Stream5;
		//num = 1;
		Flag = DMA_FLAG_TCIF5;
	}
	
	if(USARTx == USART3)
	{
		DMAy_Streamx = DMA1_Stream1;
		//num = 2;
		Flag = DMA_FLAG_TCIF1;
		Number_Rx_Buffer3 = SizeUart_Rx_Buffer3 - DMA_GetCurrDataCounter(DMA1_Stream1);	/*数据长度计算*/
		if(RB_Get_FreeSize(&rb_uart3_rx) != 0)	/*队列入队*/
		{
			for(int i = 0 ; i < Number_Rx_Buffer3 ; i++)
			{
				RB_Write_Byte(&rb_uart3_rx, Uart_Rx_Buffer3[i]);
			}
		}
	}
	
	if(USARTx == UART4)
	{
		DMAy_Streamx = DMA1_Stream2;
		//num = 3;
		Flag = DMA_FLAG_TCIF2;
	}
	
	if(USARTx == UART5)
	{
		DMAy_Streamx = DMA1_Stream0;
		//num = 4;
		Flag = DMA_FLAG_TCIF0;
	}
	
	if(USARTx == USART6)
	{
		DMAy_Streamx = DMA2_Stream1;
		//num = 5;
		Flag = DMA_FLAG_TCIF1;
	}
	
	USARTx->SR;
  USARTx->DR; 																																		//清USART_IT_IDLE标志
  DMA_Cmd(DMAy_Streamx,DISABLE);																									//关闭DMA
	DMA_ClearFlag(DMAy_Streamx,Flag);																								//清除标志位
	//Uart_RData_Len[num] = UART_RXBuffer_SIZE - DMA_GetCurrDataCounter(DMAy_Streamx);	//获得接收帧帧长
  DMA_Cmd(DMAy_Streamx,ENABLE);																										//打开DMA
	//UART_DATA_FULL[num] = true;
}

//使用DMA做数据发送时调用该函数，在数据赋值到相应的数组后嗲用该函数，可在外部调用，因为使用DMA发送，发送地址已提前设置好，因此不支持任意数组发送
void DMA_Tx(USART_TypeDef* USARTx , uint16_t Len)		
{	
	DMA_Stream_TypeDef* DMAy_Streamx;
	
	if(USARTx == USART1){
		DMAy_Streamx = DMA2_Stream7;
	}	
	if(USARTx == USART2){
		DMAy_Streamx = DMA1_Stream6;
	}	
	if(USARTx == USART3){
		DMAy_Streamx = DMA1_Stream3;
	}	
	if(USARTx == UART4){
		DMAy_Streamx = DMA1_Stream4;
	}		
	if(USARTx == UART5){
		DMAy_Streamx = DMA1_Stream7;
	}		
	if(USARTx == USART6){
		DMAy_Streamx = DMA2_Stream6;
	}	
//  if(USARTx == USART_485) IO_ON(DIR_485);
	DMA_Cmd(DMAy_Streamx , DISABLE);                      //关闭DMA传输 
	DMA_SetCurrDataCounter(DMAy_Streamx , Len);	
	DMA_Cmd(DMAy_Streamx , ENABLE);   
//  if(USARTx == USART_485)
//  {
//    delay_us(4200*Len);
//    IO_OFF(DIR_485);
//  }
}


//串口的GPIO配置子函数，参数1为欲配置的串口号，参数2/4为所属区域，例如GPIOD，参数3/5为具体管脚号，例如GPIO_PinSource8
void MyGPIO_Config(USART_TypeDef* USARTx , GPIO_TypeDef* GPIOx1 , uint8_t GPIO_PinSourcex1 , GPIO_TypeDef* GPIOx2 , uint8_t GPIO_PinSourcex2)
{
	uint8_t AF_Flag;

 	GPIO_InitTypeDef GPIO_InitStructure;
	  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC |RCC_AHB1Periph_GPIOD, ENABLE); 
	  
	if((USARTx == USART1) || (USARTx == USART2) || (USARTx == USART3))	AF_Flag = 0x07;
	else	AF_Flag = 0x08;

	GPIO_PinAFConfig(GPIOx1,GPIO_PinSourcex1,AF_Flag);  
	GPIO_PinAFConfig(GPIOx2,GPIO_PinSourcex2,AF_Flag);
  
	GPIO_InitStructure.GPIO_Pin = 		(0x0001 << GPIO_PinSourcex1);//GPIO_Pin_8;//管脚指定
  GPIO_InitStructure.GPIO_Mode = 		GPIO_Mode_AF;	//管脚模式:输出口
  GPIO_InitStructure.GPIO_OType = 	GPIO_OType_PP;	//类型:推挽模式
  GPIO_InitStructure.GPIO_PuPd = 		GPIO_PuPd_UP;	//上拉下拉设置
	GPIO_InitStructure.GPIO_Speed = 	GPIO_Speed_2MHz;//IO口速度
  GPIO_Init(GPIOx1, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin = 		(0x0001 << GPIO_PinSourcex2);//GPIO_Pin_8;//管脚指定
  GPIO_InitStructure.GPIO_Mode = 		GPIO_Mode_AF;	//管脚模式:输出口
  GPIO_InitStructure.GPIO_OType = 	GPIO_OType_PP;	//类型:推挽模式
  GPIO_InitStructure.GPIO_PuPd = 		GPIO_PuPd_UP;	//上拉下拉设置
	GPIO_InitStructure.GPIO_Speed = 	GPIO_Speed_2MHz;//IO口速度
  GPIO_Init(GPIOx2, &GPIO_InitStructure);//初始化
}


//串口的USART部分配置子函数，参数1为欲配置的串口号，参数2为波特率
void Myuart_Config(USART_TypeDef* USARTx , uint32_t bound , bool IsSbus)
{
	USART_InitTypeDef USART_InitStructure;//定义串口结构体  
	
	if(USARTx == USART1)	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	if(USARTx == USART2)	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	if(USARTx == USART3)	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	if(USARTx == UART4)		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	if(USARTx == UART5)		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	
	if(USARTx == USART6)	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	
  if(IsSbus)
  {
		USART_InitStructure.USART_StopBits = USART_StopBits_2;  
		USART_InitStructure.USART_Parity = USART_Parity_Even;  
    USART_InitStructure.USART_BaudRate = 100000; 
  }
  else
  {
    USART_InitStructure.USART_StopBits = USART_StopBits_1;  
    USART_InitStructure.USART_Parity = USART_Parity_No;  
    USART_InitStructure.USART_BaudRate = bound; 
  }
  
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    
  
  USART_Init(USARTx,&USART_InitStructure);  //初始化串口 
}


//DMA配置函数：参数1：使用哪路DMA；参数2：使用的数据流序号；参数3：使用的通道序号；参数5：外设地址；参数6：内存地址；参数7：传输方向；参数8：缓冲区大小；参数9：优先级
void MyDMA_Config(uint32_t DMAx,DMA_Stream_TypeDef* DMAy_Streamx,uint32_t DMA_Channel_x,uint32_t cpar,uint32_t cmar,uint32_t dma_dir,uint16_t bufsize,uint32_t priority)
{
	DMA_InitTypeDef DMA_InitStructure;//定义DMA结构体
	
	RCC_AHB1PeriphClockCmd(DMAx, ENABLE);																			//启动DMA时钟
	DMA_DeInit(DMAy_Streamx);																									//DMA通道配置
	DMA_InitStructure.DMA_Channel = DMA_Channel_x;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;													//外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = cmar;															//内存地址
	DMA_InitStructure.DMA_DIR = dma_dir;																			//dma传输方向
	DMA_InitStructure.DMA_BufferSize = bufsize;																//设置DMA在传输时缓冲区的长度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;					//设置DMA的外设递增模式，一个外设
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;										//设置DMA的内存递增模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//外设数据字长
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;						//内存数据字长
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;															//设置DMA的传输模式
	DMA_InitStructure.DMA_Priority = priority;																//设置DMA的优先级别
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;    								//指定如果FIFO模式或直接模式将用于指定的流 ： 不使能FIFO模式  
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; 							//指定的Burst转移配置内存传输       
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 			//指定的Burst转移配置外围转移      
	DMA_Init(DMAy_Streamx, &DMA_InitStructure);  
	DMA_Cmd(DMAy_Streamx,ENABLE);																							//使能通道
}

//通用中断配置函数
void MyNVIC_Config(uint8_t NVIC_IRQChannel,uint8_t MainPriority,uint8_t SubPriority,FunctionalState NewState)		
{
	NVIC_InitTypeDef NVIC_InitStructure ;//定义中断结构体
	
	NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = MainPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = NewState;
	NVIC_Init(&NVIC_InitStructure);
}
