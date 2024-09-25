
#include "bsp.h"

static uint32_t ulTimeOut_Time;
static uint8_t I2C_Err = 0;

/************************************************
函数名称： IIC_Init
功    能： 初始化IIC
参    数： 无
返 回 值： 无，配置了PB6和PB9端口，修改了I2C.c的全局变量ulTimeOut_Time数值
作    者： gaoyao
*************************************************/
void IIC_Init(void)
{			
  GPIO_InitTypeDef	GPIO_InitStructure;
	I2C_InitTypeDef 	I2C_InitStructure;
	RCC_ClocksTypeDef	rcc_clocks;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,	ENABLE);	//使能GPIOB时钟  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,		ENABLE);	//使能I2C1时钟
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1,		ENABLE);	//Reset I2Cx IP
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1,		DISABLE);	//Release reset signal of I2Cx IP
	
	/*I2C1 configuration		PB6: I2C1_SCL		PB9: I2C1_SDA  */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1); //注意，此处不能合并写成GPIO_PinSource6|GPIO_PinSource9
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;						//复用第二功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;				//2MHz
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;					//开漏方式输出
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;				//不上拉也不下拉
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9;
  GPIO_Init(GPIOB, &GPIO_InitStructure);									//初始化
	
	/* I2C Struct Initialize */
	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &I2C_InitStructure);

	/* I2C Initialize */
	I2C_Cmd(I2C1, ENABLE);
	
	/*超时设置*/
	RCC_GetClocksFreq(&rcc_clocks);
	ulTimeOut_Time = (rcc_clocks.SYSCLK_Frequency /10000);
	
}

/************************************************
函数名称：	I2C_ReadOneByte
功    能：	从IIC总线读取一个字节数据
参    数：	I2Cx, 使用的I2C编号；
						I2C_addr, 从设备的I2C地址
						reg_addr, 要读取的从设备寄存器地址

返 回 值：	uint8_t, I2C总线上读取到的数据
						修改了I2C.c的全局变量I2C_Err的标记，读I2C数据后需要检查I2C_Err是否为0，若为0，表示读取I2C数据时出错（超时），读到的数据可能不正确。

作    者：	gaoyao
*************************************************/
uint8_t I2C_ReadOneByte(I2C_TypeDef *I2Cx, uint8_t I2C_Addr, uint8_t Reg_addr)
{
	__IO uint8_t readout;
	__IO uint8_t readout2;
	uint32_t tmr;

	tmr = ulTimeOut_Time;
	while((--tmr)&&I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	if(tmr==0) I2C_Err = 1;

	I2C_GenerateSTART(I2Cx, ENABLE);	//发送I2C的START信号，接口自动由从设备变成主设备
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)));
	if(tmr==0) I2C_Err = 1;

	I2C_Send7bitAddress(I2Cx,I2C_Addr,I2C_Direction_Transmitter);
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)));
	if(tmr==0) I2C_Err = 1;

	I2C_SendData(I2Cx, Reg_addr);
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED)));
	if(tmr==0) I2C_Err = 1;

	I2C_GenerateSTART(I2Cx, ENABLE);
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)));
	if(tmr==0) I2C_Err = 1;

	//I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Receiver);
	I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Receiver);
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)));
	if(tmr==0) I2C_Err = 1;
	
//	I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Receiver);
//	tmr = ulTimeOut_Time;
//	while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)));
//	if(tmr==0) I2C_Err = 1;

	//I2C_AcknowledgeConfig(I2Cx, DISABLE);
	I2C_AcknowledgeConfig(I2Cx,ENABLE);
	//I2C_GenerateSTOP(I2Cx, ENABLE);
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)))); /* EV7 */
	if(tmr==0) I2C_Err = 1;


	//
	I2C_AcknowledgeConfig(I2Cx, DISABLE);

	I2C_GenerateSTOP(I2Cx, ENABLE);
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)))); /* EV7 */
	if(tmr==0) I2C_Err = 1;


	readout = I2C_ReceiveData(I2Cx);
	readout2 = I2C_ReceiveData(I2Cx);
	I2C_AcknowledgeConfig(I2Cx, ENABLE);

	return readout;
}

/************************************************
函数名称：	I2C_WriteOneByte
功    能：	向从设备写入一个字节数据
参    数：	I2Cx, 使用的I2C编号；
						I2C_addr, 从设备的I2C地址
						reg_addr, 要读取的从设备寄存器地址
						value, 需要写入的数据

返 回 值：	uint8_t, I2C总线上读取到的数据
						修改了I2C.c的全局变量I2C_Err的标记，读I2C数据后需要检查I2C_Err是否为0，若为0，表示读取I2C数据时出错（超时），读到的数据可能不正确。

作    者：	gaoyao
*************************************************/
void I2C_WriteOneByte(I2C_TypeDef *I2Cx, uint8_t I2C_Addr, uint8_t Reg_addr, uint8_t value)
{
	uint32_t tmr;

	tmr = ulTimeOut_Time;
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	while((--tmr)&&I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	if(tmr==0) I2C_Err = 1;

	I2C_GenerateSTART(I2Cx, ENABLE);
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)));
	if(tmr==0) I2C_Err = 1;

	I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Transmitter);
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)));
	if(tmr==0) I2C_Err = 1;

	I2C_SendData(I2Cx, Reg_addr);
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)));
	if(tmr==0) I2C_Err = 1;

	I2C_SendData(I2Cx, value);
	tmr = ulTimeOut_Time;
	while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)));
	if(tmr==0) I2C_Err = 1;

	I2C_GenerateSTOP(I2Cx, ENABLE);
	//I2C_AcknowledgeConfig(I2Cx, DISABLE);

}


float I2C_LM75_Read(void)
{
	uint8_t TempH,TempL;
	uint16_t data;
	//uint32_t TimeOut;

	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2C2,ENABLE);
	//TimeOut=0x1000;
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
	{
		//if ((TimeOut--) == 0)
			//printf("I2C GenerateSTART 0 Error\r\n");
	}
	I2C_Send7bitAddress(I2C2, 0x90,I2C_Direction_Transmitter);
	//TimeOut=0x1000;
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		//if ((TimeOut--) == 0)
			//printf("I2C Send7bitAddress 0 Error\r\n");
	}

	I2C_SendData(I2C2,0x00);

	//TimeOut=0x1000;
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		//if ((TimeOut--) == 0)
			//printf("I2C SendData 0 Error\r\n");
	}

	I2C_GenerateSTART(I2C2,ENABLE);
	//TimeOut=0x1000;
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
	{
		//if ((TimeOut--) == 0)
			//printf("I2C GenerateSTART 1 Error\r\n");
	}

	I2C_Send7bitAddress(I2C2, 0x90,I2C_Direction_Receiver);

	//TimeOut=0x1000;
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		//if ((TimeOut--) == 0)
			//printf("I2C I2C_Send7bitAddress 1 Error\r\n");
	} 

	I2C_AcknowledgeConfig(I2C2,ENABLE);
	//TimeOut=0x1000;
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		//if ((TimeOut--) == 0)
			//printf("I2C I2C_ReceiveData 0 Error\r\n");
	}
	TempH = I2C_ReceiveData(I2C2);

	//TimeOut=0x1000;

	I2C_AcknowledgeConfig(I2C2,DISABLE);
	I2C_GenerateSTOP(I2C2,ENABLE);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		//if ((TimeOut--) == 0)
			//printf("I2C I2C_ReceiveData 1 Error\r\n");
	}
	TempL = I2C_ReceiveData(I2C2);

	data=((uint16_t)TempH << 8) | TempL ;

	return data_convert(data);
}

float data_convert(uint16_t data)
{
	uint8_t is_negative=data&(uint16_t)(0x8000);
	uint8_t vaild_data_part=(((data<<1)&(uint16_t)(0xFF00))>>8);

	return is_negative==1 ? -(((int)vaild_data_part)*0.5):((int)vaild_data_part)*0.5;
}
