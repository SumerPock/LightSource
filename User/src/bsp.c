#include "bsp.h"

LIGHTSOURCE Lightsoutce = {0};

RETDATA retdata = {0 , 0 , {0} , {0}};

DISPLAY display = {0 , 0};

union Union_TypeConversion Byte_TypeConversion;

unsigned short PauseTime = 1000;	// 中断时间预设1s

bool b_Random_EN = false;	// 伪随机编码使能



union Union_Byte LaserControl;	// 共用体，激光编码控制位

/*菜单索引值*/
struct Menu_Index Menu_Indexs = {0, 0, 0};


/**
 * @brief 串口指令应答
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
unsigned char UART_RemoteCMD_Process(void)
{
	unsigned char Temp_u8 = 0, CheckSum = 0;
	unsigned short i = 0 , Temp_int16 = 0;
	unsigned int *p_u32 = 0;
	static unsigned short  	RandomIndex = 0;
	static unsigned char 		RandomRxError = 0;	// 伪随机编码接收时的出错计数器
	
	if(RemoteDataPacket.RX_BUF[0] > 2)
	{	
		for(i = 1 ; i < RemoteDataPacket.RX_BUF[0] ; i++)
		{
			CheckSum = CheckSum + RemoteDataPacket.RX_BUF[i];
		}
		if(CheckSum == RemoteDataPacket.RX_BUF[RemoteDataPacket.RX_BUF[0]])	// 相等则表示校验和正确
		{
			/*下位机下发数据光芯*/
			if((RemoteDataPacket.RX_BUF[2] >= 0x43) & (RemoteDataPacket.RX_BUF[2] <= 0x59))
			{
				memcpy(Uart_Tx_Buffer3, RemoteDataPacket.RX_BUF, Number_Rx_Buffer1);
				Uart_Tx_Buffer3[0] = 0XEA;
				Uart_Tx_Buffer3[Number_Rx_Buffer1 - 1] = 0XED;
				DMA_Tx(USART3 , Number_Rx_Buffer1);
			}
			/*光芯数据报告下位机*/

			#if (VERSION_SELECTION == Ver_2_2_0)
			/* 2.20不需要 + 0x20 所有再0xA3 与0xB0 的基础上 + 0x20 */
			else if((RemoteDataPacket.RX_BUF[2] >= 0xC3) & (RemoteDataPacket.RX_BUF[2] <= 0xD0))
				
			#elif (VERSION_SELECTION == Ver_2_1_0)
			
			else if((RemoteDataPacket.RX_BUF[2] >= 0xA3) & (RemoteDataPacket.RX_BUF[2] <= 0xB0))
			
			#endif
			
			{
				#if (VERSION_SELECTION == Ver_2_1_0)
					//校验计算
					RemoteDataPacket.RX_BUF[2] += 0x20;	//2023年9月6日修改 光芯的应答ID透传后由下位机从 ID + 60改为 ID + 80
					//增加校验,2023年9月6日修改 光芯的应答ID透传后由下位机从 ID + 60改为 ID + 80
					RemoteDataPacket.RX_BUF[Number_Rx_Buffer3 - 2] =  RemoteDataPacket.RX_BUF[Number_Rx_Buffer3 - 2] + 0x20;						
				#elif (VERSION_SELECTION == Ver_2_1_1)
					
				#elif (VERSION_SELECTION == Ver_2_2_0)
				
				#endif
				memcpy(Uart_Tx_Buffer1, RemoteDataPacket.RX_BUF, Number_Rx_Buffer3);
				Uart_Tx_Buffer1[0] = 0XEA;
				Uart_Tx_Buffer1[Number_Rx_Buffer3 - 1] = 0XED;				
				DMA_Tx(USART1 , Number_Rx_Buffer3);
			}
			
			switch(RemoteDataPacket.RX_BUF[2])
			{
				/*程控模式通讯握手*/
				case 0x00:	
				if(RemoteDataPacket.RX_BUF[0] == 3)//长度检查
				{
					retdata.Mode = 0x80;
					UART_Answer_LightData(USART1 , &retdata);
				}
				break;
				
				/*激光脉冲启动
				*因为这里是确定的要开激光，并且是从上位机下发的指令，实体按键不会产生变化，
				*所以这里不能使用系统变量进行调用，因为没有实体按键动作，变量也不会发生变化的，因此这里要给一个确定的值	
				*/
				case 0x01:	
				if(RemoteDataPacket.RX_BUF[0] == 3)
				{
					retdata.Mode = 0x81;
					UART_Answer_LightData(USART1 , &retdata);			
					RemoteDataPacket.RX_BUF[1]++;
					Laser_StateTurn(DISABLE);				
				}						
				break;
						
				/*激光脉冲关闭*/	
				case 0x2:				
				if(RemoteDataPacket.RX_BUF[0] == 3)	
				{	
					retdata.Mode = 0x82;
					UART_Answer_LightData(USART1 , &retdata);
					Laser_StateTurn(ENABLE);
				}
				break;
				
				/*红色十字光点亮*/
				case 0x3:	
				if(RemoteDataPacket.RX_BUF[0] == 3)
				{
					retdata.Mode = 0x83;
					UART_Answer_LightData(USART1 , &retdata);
					Cross_StateTurn(DISABLE);
				}
				break;
					
				/*红色十字光关闭*/
				case 0x4:	
				if(RemoteDataPacket.RX_BUF[0] == 3)
				{
					retdata.Mode = 0x84;
					UART_Answer_LightData(USART1 , &retdata);
					Cross_StateTurn(ENABLE);
				}
				break;
				
				/*用以模拟按键上翻操作*/
				case 0x6:	
				if(RemoteDataPacket.RX_BUF[0] == 3)
				{
					UART_Answer_NoData(USART1);
					MenuTrig_Up();
				}
				break;
				
				/*用以模拟按键下翻操作*/
				case 0x7:	
				if(RemoteDataPacket.RX_BUF[0] == 3)
				{
					UART_Answer_NoData(USART1);
					MenuTrig_Down();
				}
				break;
				
				/*用以模拟按键OK操作*/
				case 0x8:
				if(RemoteDataPacket.RX_BUF[0] == 3)
				{
					UART_Answer_NoData(USART1);
					MenuTrig_OK();
				}
				break;
				
				/*用以模拟按键Cancel操作*/
				case 0x9:
				if(RemoteDataPacket.RX_BUF[0] == 3)
				{
					UART_Answer_NoData(USART1);
					MenuTrig_Cancel();
				}
				break;
				
				/*中断1秒*/						
				case 0x10:	
				if(RemoteDataPacket.RX_BUF[0] == 5)
				{// 
					Byte_TypeConversion.TC_uint64_t = 0;
					Byte_TypeConversion.TC_byte[0] = RemoteDataPacket.RX_BUF[4];
					Byte_TypeConversion.TC_byte[1] = RemoteDataPacket.RX_BUF[3];
					PauseTime = Byte_TypeConversion.TC_uint16_t;
					Pulse_StateTurn(DISABLE);		//中断1s短时脉冲跳高
					TIM_Cmd(TIM2 , ENABLE); 		//使能定时器2,产生1ms的短触发脉冲,这里使用定时器主要是为了以后更改其余宽度时比较方便，
																			//因为只产生一次中断，也不会太占用资源						
					retdata.Mode = 0x90;
					UART_Answer_LightData(USART1 , &retdata);
				}
				break;	
					
				/*固定编码周期装订*/
				case 0x11:	
				if(RemoteDataPacket.RX_BUF[0] == 6)	// 首先进行命令包长度检查
				{		
					Byte_TypeConversion.TC_uint64_t = 0;						
					Byte_TypeConversion.TC_byte[0] = RemoteDataPacket.RX_BUF[5];
					Byte_TypeConversion.TC_byte[1] = RemoteDataPacket.RX_BUF[4];
					Byte_TypeConversion.TC_byte[2] = RemoteDataPacket.RX_BUF[3];						
					CycleCodeTime = Byte_TypeConversion.TC_uint32_t;
					
					if(b_Random_EN)	
					{//如果此前处于伪随机模式，则退出伪随机模式
						b_Random_EN = false;	
					}	
					if(LaserControl.bits.b0 == 1)		
					{//如果之前已经是开激光状态，则重新更新一次开光指令，
						//如果之前是关激光状态，则什么也不变，只保存数据，待下一次手动打开激光即可
						Laser_StateTurn(DISABLE);			//打开激光
					}		
					
					unsigned int my_ms = 0;			/*应小刘要求加入周期数值显示*/
					my_ms = RemoteDataPacket.RX_BUF[3] << 16 | RemoteDataPacket.RX_BUF[4] << 8 | RemoteDataPacket.RX_BUF[5];
					retdata.cycle[0] =  RemoteDataPacket.RX_BUF[3];
					retdata.cycle[1] =  RemoteDataPacket.RX_BUF[4];
					retdata.cycle[2] =  RemoteDataPacket.RX_BUF[5];
					
					if(cn.Program_control_mode_2 == 0xff)/*将数值拷贝值返回数据中*/
					{
						/*此处由于显示屏尺寸有限 当数据大于99.999时为100.000时即7位时会遮挡ms单位所以去点ms单位*/
						if(my_ms > 0x1869F)
						{
							display.discycle = 1;
							display.discycleshow = my_ms;
						}
						else
						{			
							display.discycle = 1;
							display.discycleshow = my_ms;						
						}
					}
					retdata.Mode = 0x91;
					UART_Answer_LightData(USART1 , &retdata);
				}
				break;

				/*伪随机编码数量检查*/
				case 0x20:
				if(RemoteDataPacket.RX_BUF[0] == 7)
				{
					Byte_TypeConversion.TC_uint64_t = 0;						
					Byte_TypeConversion.TC_byte[0] = RemoteDataPacket.RX_BUF[4];
					Byte_TypeConversion.TC_byte[1] = RemoteDataPacket.RX_BUF[3];
					u16_RandomData[7] = Byte_TypeConversion.TC_uint16_t;					// 编码位数
					Byte_TypeConversion.TC_byte[0] = RemoteDataPacket.RX_BUF[6];
					Byte_TypeConversion.TC_byte[1] = RemoteDataPacket.RX_BUF[5];
					u16_RandomData[6] = Byte_TypeConversion.TC_uint16_t;					// 基频
					RandomIndex = 0;
					RandomRxError = 0;
					if(u16_RandomData[7] > 0 && u16_RandomData[7] < 1017)	
					{		//数据有效范围1~1016
						UART_Answer_RandomNumber(USART1, u16_RandomData[7]);
					}
					else if(u16_RandomData[7] >= 1017 || u16_RandomData[7] == 0)
					{
						retdata.Mode = 0xAE;
						UART_Answer_LightData(USART1 , &retdata);		//上报0xAE错误码
					}
				}
				break;

				/*伪随机编码数据*/
				case 0x21:
				if(RemoteDataPacket.RX_BUF[0] == 7)
				{
					Byte_TypeConversion.TC_uint64_t = 0;
					Temp_u8 = RemoteDataPacket.RX_BUF[3];
					Byte_TypeConversion.TC_byte[0] = RemoteDataPacket.RX_BUF[4];
					Byte_TypeConversion.TC_byte[1] = Temp_u8 & 0x0F;			// 取低4位，这里代码是老板写的，我也不知道为什么他要这么写？
					Temp_int16 = Byte_TypeConversion.TC_uint16_t;					// 读取伪随机数据序号			
					Byte_TypeConversion.TC_byte[0] = RemoteDataPacket.RX_BUF[6];		
					Byte_TypeConversion.TC_byte[1] = RemoteDataPacket.RX_BUF[5];
					Byte_TypeConversion.TC_byte[2] = (Temp_u8 >> 4) & 0x0F;	// 取高4位，	
					u16_RandomData[Temp_int16 + 7] = (uint16_t)(Byte_TypeConversion.TC_uint32_t - u16_RandomData[6]);	// 将伪随机数据填入对应序号的数组中
					
					// 为节省空间，数据按照16Bit存储，用最小数据为基频，数据减基频存储
					if((Temp_int16 != (RandomIndex + 1)) || (Byte_TypeConversion.TC_uint32_t > 120000) ||(Byte_TypeConversion.TC_uint32_t < 40000))
					{
						RandomRxError++;
						UART_AnswerError(USART1, RandomIndex);
					}
					RandomIndex = Temp_int16;
					//为提高速度，数据部分不应答，等全部完毕再做应答
					//UART_Answer_RandomData(USART1, Temp_int16, Int32_RandomData[Temp_int16]);  
				}
				break;
					
				/*伪随机编码发送完毕检查*/
				case 0x22:
				if(RandomRxError == 0)	// =0说明接收到的数据序号都对
				{ //伪随机编码序列发送完毕后的检查
					if(RandomCode_Check())
					{				
						retdata.Mode = 0xA2;
						UART_Answer_LightData(USART1 , &retdata);				
						Laser_StateTurn(ENABLE);	//关闭激光输出，该指令上位机会下发一次，为稳妥起见，这里也做一次处理，
						//在主函数while循环里会根据b_Random_EN状态自动进入伪随机状态
						b_Random_EN = true;		//进入伪随机模式
					}
					else
					{
						retdata.Mode = 0xAE;
						UART_Answer_LightData(USART1 , &retdata);	//上报错误代码				
					}
				}
				else
				{
					UART_AnswerError(USART1, RandomRxError);
				}
				break;

				/*伪随机编码存储*/
				case 0x23:	
				if(RemoteDataPacket.RX_BUF[0] == 11)
				{
					uint8_t SaveCH = RemoteDataPacket.RX_BUF[3];			
					/*2组设置为武汉高德红外，4组设定为通用版本*/
					if((SaveCH > 0) && (SaveCH < 5))		//V3.01 改伪随机长度，原代码：if((SaveCH >0) && (SaveCH <5))
					{
						Byte_TypeConversion.TC_uint64_t = 0;						
						Byte_TypeConversion.TC_byte[0] = RemoteDataPacket.RX_BUF[4];
						Byte_TypeConversion.TC_byte[1] = RemoteDataPacket.RX_BUF[5];
						Byte_TypeConversion.TC_byte[2] = RemoteDataPacket.RX_BUF[6];
						Byte_TypeConversion.TC_byte[3] = RemoteDataPacket.RX_BUF[7];
						Byte_TypeConversion.TC_byte[4] = RemoteDataPacket.RX_BUF[8];
						Byte_TypeConversion.TC_byte[5] = RemoteDataPacket.RX_BUF[9];
						Byte_TypeConversion.TC_byte[6] = RemoteDataPacket.RX_BUF[10];
						for(i = 0 ; i < 4 ; i++)
						{
							u16_RandomData[i] = Byte_TypeConversion.TC_u16[i];
						}
						p_u32 = (uint32_t *)&u16_RandomData;						
						for(i = 0; i < 512; i++)
						{
							u32_FlashBuff[512*(SaveCH-1)+i] = *(p_u32+i);
						}												
						STMFLASH_Write(FLASH_USER_START_ADDR, u32_FlashBuff, 2048*4);							
						//UART_Answer_NoData(USART1);
						retdata.Mode = 0xA3;  
						UART_Answer_LightData(USART1 , &retdata);	   
						//V3.01 改伪随机长度，存储应答，原代码：UART_Answer_NoData(USART1);
					}
				}
				break;					
				
				/*伪随机编码删除*/
				case 0x24:	
				if(RemoteDataPacket.RX_BUF[0] == 4)
				{
					uint8_t SaveCH = RemoteDataPacket.RX_BUF[3];					
					if((SaveCH >0) && (SaveCH <5))
					{
						for(i=0; i<8; i++)
						{
							u16_RandomData[i] = 0x0;
						}
						p_u32 = (uint32_t *)&u16_RandomData;						
						for(i=0; i<512; i++)
						{
							u32_FlashBuff[512*(SaveCH-1)+i] = *(p_u32+i);
						}													
						STMFLASH_Write(FLASH_USER_START_ADDR, u32_FlashBuff, 2048*4);						
						//UART_Answer_NoData(USART1);
						retdata.Mode = 0xA4;  
						UART_Answer_LightData(USART1 , &retdata); //V3.01 改伪随机长度，删除应答，原代码：UART_Answer_NoData(USART1);
					}
				}
				break;
					
				/*能量设置*/
				case 0x41:				
				if(RemoteDataPacket.RX_BUF[0] == 7)
				{
					RemoteDataPacket.TX_BUF[0] = 0;
					for(int i = 1 ; i < 7 ; i++)
					{
						UART_SendInbuff(RemoteDataPacket.TX_BUF , RemoteDataPacket.RX_BUF[i]);	
					}
					
					#if (VERSION_SELECTION == Ver_2_1_0)
						UART_RemoteEncode(USART3, RemoteDataPacket.TX_BUF);	
					#elif (VERSION_SELECTION == Ver_2_1_1)
						static unsigned short temp = 0;			/*2023 9 7 日 要求增加固定DB 2000*/
						static unsigned char SendRemotBuff[16] = {0};
						memset(SendRemotBuff , 0 , sizeof(SendRemotBuff));
						memcpy(SendRemotBuff , RemoteDataPacket.TX_BUF , 16);
						temp = RemoteDataPacket.TX_BUF[5] << 8 | RemoteDataPacket.TX_BUF[6];
						temp += 2000;
						SendRemotBuff[6]= temp;					//先高后低
						SendRemotBuff[5]= temp >> 8;				
						UART_RemoteEncode(USART3, SendRemotBuff);
					#elif (VERSION_SELECTION == Ver_2_2_0)
						UART_RemoteEncode(USART3, RemoteDataPacket.TX_BUF);
					#endif
											
					RemoteDataPacket.RX_BUF[1]++;
					RemoteDataPacket.TX_BUF[0] = 0;	
					retdata.Mode = 0xc1;
					
					retdata.energy[0] =  RemoteDataPacket.RX_BUF[5];
					retdata.energy[1] =  RemoteDataPacket.RX_BUF[6];
					/*测试时发现，第一次总是不给上位机回复衰减值，这里强制复制一次*/
					retdata.energy[0] = RemoteDataPacket.TX_BUF[5];
					retdata.energy[1] = RemoteDataPacket.TX_BUF[6];
					UART_Answer_LightData(USART1 , &retdata);	
					
					attenuation_number(&cn , &RemoteDataPacket);		/*2022 8 3 应小刘要求加入衰减数值显示*/			
					unsigned short show = 0;				
					unsigned char bit_point = 0;				//显示的位数					
					unsigned char decimal_point = 0;		//显示的小数点位数					
					show = cn.control_number_hight | cn.control_number_low << 8;
					if(show >= 0x3E8 )									/*数值大于三位数*/
					{
						bit_point = 3;
						decimal_point = 1;
						show = show / 10;	
					}
					else
					{
						bit_point = 3;
						decimal_point = 1;
						show = show / 10;	
					}	
					//主要用于小数显示
					if(cn.Program_control_mode_1 == 0xff)
					{
						display.disenergy = 1;
						display.bit_point = bit_point;
						display.decimal_point = decimal_point;
						display.disenergyshow = show;
					}	

				}				
				break;
					
				/*仿真速率设定*/
				case 0x42:	
				if(RemoteDataPacket.RX_BUF[0] == 4)
				{
					RemoteDataPacket.TX_BUF[0] = 0;	
					for(int i = 1 ; i < 4 ; i++)
					{
						UART_SendInbuff(RemoteDataPacket.TX_BUF, RemoteDataPacket.RX_BUF[i]);	
					}
						UART_RemoteEncode(USART3, RemoteDataPacket.TX_BUF);
						RemoteDataPacket.RX_BUF[1]++;
						RemoteDataPacket.TX_BUF[0] = 0;
				}
				break;
					
				default:
				break;
				
			}
			return 0;
		}
		else
		{
			return 0x1;	//退出，报告校验和错误
		}
	}
	else
	{
		return 0x2;	//退出，报告数据长度不够
	}
}



/**
 * @brief 串口单字节指令发送，屎山代码，不想自己找麻烦就别动他
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
void UART_Answer_LightData(USART_TypeDef* USARTx , RETDATA* rd)
{	
	unsigned char cmode = rd->Mode;
	unsigned char ctmmp = rd->temp;
	unsigned char ccyle[5] 		= {rd->cycle[0] , rd->cycle[1] , rd->cycle[2]};
	unsigned char cenergy[5] 	= {rd->energy[0] , rd->energy[1] , rd->energy[2]};
	RemoteDataPacket.TX_BUF[0] = 0;	// Buff清零
	UART_SendInbuff(RemoteDataPacket.TX_BUF , RemoteDataPacket.RX_BUF[1]);		/*返回对应的包序号*/

	switch(cmode)
	{
		case 0x80://握手
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
			break;
		
		case 0x81://激光开
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
			break;

		case 0x82://激光关
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;

		case 0x83://开红光
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;
		
		case 0x84://关红光
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;

		case 0x89://程控模式
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;
		
		case 0x90://暂停
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;

		case 0x91://频率装订
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ccyle[0]);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ccyle[1]);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ccyle[2]);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;

		case 0xc1://能量装订
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cenergy[0]);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cenergy[1]);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cenergy[2]);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;

		case 0xA2://伪随机成功
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;

		case 0xAE://伪随机错误
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;
		
		case 0xA3://V3.01 改伪随机长度，伪随机存储成功   
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;
		
		case 0xA4://V3.01 改伪随机长度，伪随机删除成功
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;		
	
		default:
			break;
	}	
	UART_RemoteEncode(USART1 , RemoteDataPacket.TX_BUF);
	RemoteDataPacket.RX_BUF[1]++;
}


void attenuation_number(control_number* ap , struct Uart_DataPacket* rp)
{
	unsigned short num = 0;
	num = (rp->RX_BUF[5] << 8) | (rp->RX_BUF[6]);
	if( num <= 7500)
	{
		ap->control_number_low = rp->RX_BUF[5];
		ap->control_number_hight = rp->RX_BUF[6];
		//showflag
		//ap->showflag = mytrue;
	}
}

