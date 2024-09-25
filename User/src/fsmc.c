#include "fsmc.h"

/**
*函数名：	 SRAM_GPIO_Config
*输入参数：void
*输出参数：void
*实现功能：初始化FSMC
*引脚配置：		
		FSMC_CLK:		PD3		FSMC_NE:		PD7		FSMC_NOE:		PD4		FSMC_NWE:		PD5		FSMC_NWAIT:	PD6
		FSMC_NADV:	PB7		FSMC_AD:
								0->	PD14	1->	PD15	 2->	PD0		3->	PD1		4->	PE7		  5->	PE8   6->	PE9		7->	PE10		
								8->	PE11	9->	PE12	10->PE13		11->PE14	12->PE15    13->PD8		14->PD9		15->PD10
**/
void FSMC_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  /* 使能SRAM相关的GPIO时钟 */
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE); 
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//使能FSMC时钟  
	 /*-- GPIO 配置 -----------------------------------------------------*/

  /* 通用 GPIO 配置 */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    
  /*DQ数据信号线AD0-AD15 针对引脚配置*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);
    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);
  
  /*控制信号线*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//FSMC_CLK
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource3,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//FSMC_NOE
  GPIO_Init(GPIOD, &GPIO_InitStructure);   
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//FSMC_NWE
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//FSMC_NWAIT
  GPIO_Init(GPIOD, &GPIO_InitStructure);   
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_FSMC); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//FSMC_NE1
  GPIO_Init(GPIOD, &GPIO_InitStructure);   
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource7,GPIO_AF_FSMC); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//FSMC_NL
  GPIO_Init(GPIOB, &GPIO_InitStructure);   
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_FSMC); 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//FSMC_NBL1
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource1,GPIO_AF_FSMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//FSMC_NBL0
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource0,GPIO_AF_FSMC); 
}	

/**
*函数名：		FSMC_Init
*输入参数： void
*输出参数：	void
*实现功能： 初始化外部SRAM
**/
void FSMC_Init(void)
{	
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	
	FSMC_GPIO_Config();
	
 	readWriteTiming.FSMC_AddressSetupTime = 8;	 ////地址建立时间（ADDSET）为8个HCLK,8/168M=1/21M=47ns
	readWriteTiming.FSMC_AddressHoldTime = 8;	 //地址保持时间（ADDHLD）模式A未用到	
	readWriteTiming.FSMC_DataSetupTime = 8;		 ////数据保持时间（DATAST）为9个HCLK 6*9=54ns	 	 

	readWriteTiming.FSMC_BusTurnAroundDuration = 200;	//设置总线转换周期，仅用于复用模式的NOR操作,
  readWriteTiming.FSMC_CLKDivision = 0x00;	//设置时钟分频，仅用于同步类型的存储器
  readWriteTiming.FSMC_DataLatency = 0x00;	//数据保持时间，仅用于同步型的NOR
  readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_B;	 //选择匹配SRAM的模式
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1; //选择FSMC映射的存储区域： Bank1 sram1
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Enable; 	//设置地址总线与数据总线是否复用
  FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_NOR; //设置要控制的存储器类型为FSMC_MemoryType_SRAM,FSMC_MemoryType_NOR
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;	//存储器数据宽度：16位
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;	//设置是否使用突发访问模式，仅用于同步类型的存储器
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;  //设置等待信号的有效极性，仅用于同步类型的存储器
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable;	//设置是否使能等待信号，仅用于同步类型的存储器
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   	//设置是否支持把非对齐的突发操作，仅用于同步类型的存储器
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;    //设置等待信号插入的时间，仅用于同步类型的存储器
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	  //存储器写使能 
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;    //不使用等待信号
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;   //不使用扩展模式，读写使用相同的时序
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;  	//突发写操作
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming;	//读写时序配置
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &readWriteTiming;	//读写同样时序，使用扩展模式时这个配置才有效
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //初始化FSMC配置
 	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);	  //使能BANK1区域1								  								
}



/**
 * @brief 简要描述函数的作用
 *
 * 这里可以提供关于函数的高级概述，包括它的输入和输出。
 *
 * @param[in]  参数1  参数1的描述，包括数据类型和含义
 * @param[out] 参数2  参数2的描述，包括数据类型和含义
 *
 * @return 函数返回值的描述，包括数据类型和含义，如果适用的话。
 *
 * @note 任何特殊注意事项或用法示例都可以在这里添加。
 */
void CycleCode_Renew(uint32_t CodeTime)
{
		double Temp_double = 0.00;
		//Temp_double = (double)(CodeTime - ((CodeTime / 1000) * (-0.002)))*14.7456;		//对输出累计误差进行微调	
		//Temp_double = (double)(CodeTime - ((CodeTime / 10000) * (-0.002)))*14.7456;		//对输出累计误差进行微调
		Temp_double = (double)CodeTime;
		Temp_double = (Temp_double - ((Temp_double / 1000) * (-0.002)))*14.7456;	
		//Temp_double = (double)CodeTime*14.7456;		//对输出累计误差进行微调
		//Temp_double = 48.6948; 频率为300.9khz
		FSMC_WR(1) = (uint16_t)((uint32_t)(Temp_double-0.5) & 0xFFFF);	// 脉冲周期低16位
		FSMC_WR(2) = (uint16_t)((uint32_t)(Temp_double-0.5) >> 16);		// 脉冲周期高8位，剩余8位空闲
		FSMC_WR(3) = 71;	// 脉冲宽度
//		FSMC_WR(3) = 36;	// 脉冲宽度	
}	

