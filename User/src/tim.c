#include "tim.h"

static unsigned int SYS_Count = 0;

void TIM3_Int_Init(unsigned short arr , unsigned int psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 				
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  			
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);	//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 						
	TIM_Cmd(TIM3,ENABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;			         //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01;        //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM3_IRQHandler(void)
{ 
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		flag_timer2.flag_timer2_1s = mytrue;
					//flag_timer2.flag_timer2_1s = mytrue;
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
	}
	
}

void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); //使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 				
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  			
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);	//初始化TIM3
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); 						//允许定时器3更新中断
	TIM_Cmd(TIM2,DISABLE); //不使能定时器2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;			         //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00;        //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler(void)
{ 
		if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
	{
		SYS_Count++;		
		if(SYS_Count == 1)				//中断计时时间到
		{
			Pulse_StateTurn(ENABLE);			//中断1s短时脉冲跳低
			SYS_Count = 0;					//计时器清零
			TIM_Cmd(TIM2,DISABLE); 	//关闭计时器，等待下次由串口命令时再开启
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}
