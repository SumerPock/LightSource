
#include "bsp.h"

void UART_Answer_LightData(USART_TypeDef* USARTx , RETDATA* rd);
//void attenuation_number(control_number* ap , struct Uart_DataPacket* rp);

union Union_Byte LaserState;	// 共用体，激光器状态

//extern float LaserTEMP;
extern bool Packet_flag;


//extern uint16_t u16_RandomData[1024];	// 伪随机编码的数据数组，在RandomCode.c里声明
extern uint32_t u32_FlashBuff[2048];	// RandomCode.c里声明
//extern bool b_Random_EN;	// 伪随机编码使能
extern uint16_t RandomMinimum;	// 伪随机编码的最小值(基频)，在RandomCode.c里声明
extern int32_t RandomCodeTime;	// 伪随机周期，在RandomCode.c里声明



/*
*********************************************************************************************************
*	函 数 名: USART1_Config
*	功能说明: 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void USART1_Config(FunctionalState NewState)
{		
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		
	//GPIO端口时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	 	
	//引脚复用映射
	GPIO_PinAFConfig(GPIOA,	GPIO_PinSource9,	GPIO_AF_USART1); 	//GPIO_A9 复用为USART1
	GPIO_PinAFConfig(GPIOA,	GPIO_PinSource10,	GPIO_AF_USART1); 	//GPIO_A10复用为USART1
		
	//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度2MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 			//上拉
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOA,	&GPIO_InitStructure); 					//初始化PA9、PA10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOB,	&GPIO_InitStructure); 				//初始化PB10、PB11
	
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = 115200;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
		
	//USART NVIC 配置
	//配置相同的抢占优先级和响应优先级，根据他们在中断表中的排位顺序决定先处理哪一个
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	//使能串口
	//USART_Cmd(USART1, NewState);  	// 根据参数决定是否打开串口
	USART_Cmd(USART1, ENABLE);  	//强制打开串口
		
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	//开启相关中断
	//USART_ITConfig(USART1, USART_IT_RXNE, NewState);  	// 根据参数决定是否打开串口
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  	// 根据参数决定是否打开串口
		
//	// 串口解码部分的寄存器初始化
//	RemoteDataPacket.Start_Flag = 0;
//	RemoteDataPacket.Clarity_Flag = 0;
//	RemoteDataPacket.End_Flag = 0;
//	RemoteDataPacket.RX_BUF[0] = 0;
//	RemoteDataPacket.TX_BUF[0] = 0;	
//	LaserState.All = 0x0;
}


/*
*********************************************************************************************************
*	函 数 名: USART1_Config
*	功能说明: 
*	形    参：无
*	返 回 值: 无
操作usart1的串口关闭时会影响到usart3的通讯，原因未知，故将二者分开写
*********************************************************************************************************
*/
void USART3_Config(FunctionalState NewState)			
{		
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//GPIO端口时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
 	
	//引脚复用映射
	GPIO_PinAFConfig(GPIOB,	GPIO_PinSource10,	GPIO_AF_USART3); 	//GPIO_B10复用为USART3
	GPIO_PinAFConfig(GPIOB,	GPIO_PinSource11,	GPIO_AF_USART3); 	//GPIO_B11复用为USART3
	
	//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度2MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 			//上拉
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB,	&GPIO_InitStructure); 					//初始化PA9、PA10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB,	&GPIO_InitStructure); 				//初始化PB10、PB11
	
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = 115200;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	//USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_InitStructure.USART_Mode =  USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口3
	
	//USART NVIC 配置
	//配置相同的抢占优先级和响应优先级，根据他们在中断表中的排位顺序决定先处理哪一个
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	//使能串口
	USART_Cmd(USART3, ENABLE);
	USART_ClearFlag(USART3, USART_FLAG_TC);
	
	//开启相关中断
	//USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}



/*
*********************************************************************************************************
*	函 数 名: UART_SEND
*	功能说明: 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void UART_SEND(USART_TypeDef* USARTx , unsigned short Data)
{
	USART_SendData(USARTx , Data);         		
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
	{
	}
}


/*
*********************************************************************************************************
*	函 数 名: UART_SendInbuff
*	功能说明: 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void UART_SendInbuff(uint8_t TX_Buff[], uint8_t InChar)
{
	if(TX_Buff[0] > 10)
	{
		TX_Buff[0] = 0;
	}
	else
	{
		TX_Buff[0]++;
		TX_Buff[TX_Buff[0]] = InChar;
	}
}


#define MAX_UART_SEND_DATA_SIZE 128 // 数组大小根据需要调整

uint8_t UART_Send_Buffer[MAX_UART_SEND_DATA_SIZE]; 		// 用于存储参数的数组
uint8_t UART_Send_Buffer_Index = 0; 									// 用于记录数组索引
void UART_RemoteEncode(USART_TypeDef* USARTx, uint8_t TX_Buff[])
{
	unsigned char i = 0, CheckSum = 0, OutChar = 0;
	
	if((TX_Buff[0] > 0) && (TX_Buff[0] < 9))
	{
		// 清空发送缓冲区数组和索引
		memset(UART_Send_Buffer, 0, sizeof(UART_Send_Buffer));
		UART_Send_Buffer_Index = 0;
		//UART_SEND(USARTx, Start_Symbol);	
		UART_Send_Buffer[0] = Start_Symbol;
		CheckSum = 0;
		UART_Send_Buffer_Index = 1;
		
		for(i = 0; i < TX_Buff[0]; i++)
		{
			OutChar = TX_Buff[i + 1];
			CheckSum = CheckSum + OutChar;
			
			if (OutChar == Start_Symbol || OutChar == Clarity_Symbol || OutChar == End_Symbol)
			{
				//UART_SEND(USARTx, Clarity_Symbol);
				UART_Send_Buffer[UART_Send_Buffer_Index++] = Clarity_Symbol; // 将字符存入数组
			}
			
			//UART_SEND(USARTx, OutChar);
			UART_Send_Buffer[UART_Send_Buffer_Index++] = OutChar; // 将字符存入数组
		}
		
		if (CheckSum == Start_Symbol || CheckSum == Clarity_Symbol || CheckSum == End_Symbol)
		{	
			//UART_SEND(USARTx, Clarity_Symbol);
			UART_Send_Buffer[UART_Send_Buffer_Index++] = Clarity_Symbol; // 将字符存入数组
		}	
		
		//UART_SEND(USARTx, CheckSum);
		UART_Send_Buffer[UART_Send_Buffer_Index++] = CheckSum; // 将字符存入数组
		
		//UART_SEND(USARTx, End_Symbol);
		UART_Send_Buffer[UART_Send_Buffer_Index++] = End_Symbol; // 将字符存入数组
		
		// 在函数的最后，遍历数组并打印出来		
		if(USARTx == USART1)
		{
			memset(Uart_Tx_Buffer1, 0, sizeof(Uart_Tx_Buffer1));
			memcpy(Uart_Tx_Buffer1, UART_Send_Buffer, UART_Send_Buffer_Index + 2);
			DMA_Tx(USART1 , UART_Send_Buffer_Index + 2);
		}else if(USART3)
		{
			memset(Uart_Tx_Buffer3, 0, sizeof(Uart_Tx_Buffer3));
			memcpy(Uart_Tx_Buffer3, UART_Send_Buffer, UART_Send_Buffer_Index + 2);
			DMA_Tx(USART3 , UART_Send_Buffer_Index);
		}
	}
}


/*
*********************************************************************************************************
*	函 数 名: UART_RemoteEncode
*	功能说明: 
*	形    参：无
*	返 回 值: 无
* 我不想动它了
*********************************************************************************************************
DMA_Tx(USART1 , 5);
Uart_Tx_Buffer1*/
//void UART_RemoteEncode(USART_TypeDef* USARTx, uint8_t TX_Buff[])
//{
//	unsigned char i = 0, CheckSum = 0, OutChar = 0;
//	if((TX_Buff[0] > 0) && (TX_Buff[0] < 9))
//	{
//		UART_SEND(USARTx, Start_Symbol);	
//		CheckSum = 0;
//		for(i = 0; i < TX_Buff[0]; i++)
//		{
//			OutChar = TX_Buff[i + 1];
//			CheckSum = CheckSum + OutChar;
//			if (OutChar == Start_Symbol || OutChar == Clarity_Symbol || OutChar == End_Symbol )//待发送的字符与三个关键字相同
//			{
//				UART_SEND(USARTx, Clarity_Symbol);				
//			}
//			UART_SEND(USARTx, OutChar);
//		}
//		if (CheckSum==Start_Symbol || CheckSum==Clarity_Symbol || CheckSum==End_Symbol )//待发送的字符与三个关键字相同
//		{	
//			//判断校验和是否与关键字相同 
//			UART_SEND(USARTx, Clarity_Symbol);	
//		}	
//		UART_SEND(USARTx, CheckSum);
//		UART_SEND(USARTx, End_Symbol);		
//	}
//}




// 串口单字节指令应答
void UART_Answer_NoData(USART_TypeDef* USARTx)
{	
	static unsigned char ucdata = 0;
	static unsigned char g_ansondata = 0;
	
//	float TEMP;
//	TEMP = 100 + 2 * LaserTEMP;
//	if(TEMP < 0) TEMP = 0; else if(TEMP > 255) TEMP = 255;	// 数据取值范围检查
	//TEMP = 0x99;
	//LaserState.All = 0x50;//修改工作状态
	
	RemoteDataPacket.TX_BUF[0] = 0;	// Buff清零
	/*返回对应的包序号*/
	UART_SendInbuff(RemoteDataPacket.TX_BUF , RemoteDataPacket.RX_BUF[1]);	
	/*设备功能ID*/
	UART_SendInbuff(RemoteDataPacket.TX_BUF , g_ansondata);
	if(g_ansondata == 0x80)
	{
		UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		UART_SendInbuff(RemoteDataPacket.TX_BUF, ucdata);	
		UART_SendInbuff(RemoteDataPacket.TX_BUF, 0x0);
		UART_SendInbuff(RemoteDataPacket.TX_BUF, 0x0);	
	}
	else
	{
		UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		UART_SendInbuff(RemoteDataPacket.TX_BUF, ucdata);	
		UART_SendInbuff(RemoteDataPacket.TX_BUF, 0x0);
		UART_SendInbuff(RemoteDataPacket.TX_BUF, 0x0);	
	}
	UART_RemoteEncode(USART1 , RemoteDataPacket.TX_BUF);
	RemoteDataPacket.RX_BUF[1]++;
}

 //串口伪随机位长装订应答
void UART_Answer_RandomNumber(USART_TypeDef* USARTx, int16_t data)
{
	RemoteDataPacket.TX_BUF[0] = 0;	// Buff清零
	UART_SendInbuff(RemoteDataPacket.TX_BUF, RemoteDataPacket.RX_BUF[1]);	// 返回对应的包序号
	UART_SendInbuff(RemoteDataPacket.TX_BUF, 0xA0);
	UART_SendInbuff(RemoteDataPacket.TX_BUF, 0x0);						// 错误代码
	UART_SendInbuff(RemoteDataPacket.TX_BUF, 0x0);
	UART_SendInbuff(RemoteDataPacket.TX_BUF, 0x0);	
	UART_RemoteEncode(USART1, RemoteDataPacket.TX_BUF);
	RemoteDataPacket.RX_BUF[1]++;
}

 //串口伪随机数据装订应答
void UART_Answer_RandomData(USART_TypeDef* USARTx, int16_t data_16, uint32_t data_32)
{
	RemoteDataPacket.TX_BUF[0] = 0;	// Buff清零
	UART_SendInbuff(RemoteDataPacket.TX_BUF, RemoteDataPacket.RX_BUF[1]);	// 返回对应的包序号
	UART_SendInbuff(RemoteDataPacket.TX_BUF, 0xA1);
	UART_SendInbuff(RemoteDataPacket.TX_BUF, 0x0);						// 错误代码
	UART_SendInbuff(RemoteDataPacket.TX_BUF, 0x0);
	UART_SendInbuff(RemoteDataPacket.TX_BUF, 0x0);	
	UART_RemoteEncode(USART1, RemoteDataPacket.TX_BUF);
	RemoteDataPacket.RX_BUF[1]++;
}



 //串口通讯错误应答
void UART_AnswerError(USART_TypeDef* USARTx, uint8_t ErrorID)
{
	RemoteDataPacket.TX_BUF[0] = 0;	// Buff清零
	UART_SendInbuff(RemoteDataPacket.TX_BUF, RemoteDataPacket.RX_BUF[1]);	// 返回对应的包序号
	UART_SendInbuff(RemoteDataPacket.TX_BUF, 0xAE);
	UART_SendInbuff(RemoteDataPacket.TX_BUF, ErrorID);						// 错误代码
	UART_RemoteEncode(USARTx, RemoteDataPacket.TX_BUF);
}
