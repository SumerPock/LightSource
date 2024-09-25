#include "bsp.h"

LIGHTSOURCE Lightsoutce = {0};

RETDATA retdata = {0 , 0 , {0} , {0}};

DISPLAY display = {0 , 0};

union Union_TypeConversion Byte_TypeConversion;

unsigned short PauseTime = 1000;	// �ж�ʱ��Ԥ��1s

bool b_Random_EN = false;	// α�������ʹ��



union Union_Byte LaserControl;	// �����壬����������λ

/*�˵�����ֵ*/
struct Menu_Index Menu_Indexs = {0, 0, 0};


/**
 * @brief ����ָ��Ӧ��
 *
 * ��������ṩ���ں����ĸ߼�������������������������
 *
 * @param[in]  ����1  ����1�������������������ͺͺ���
 * @param[out] ����2  ����2�������������������ͺͺ���
 *
 * @return ��������ֵ�������������������ͺͺ��壬������õĻ���
 *
 * @note �κ�����ע��������÷�ʾ����������������ӡ�
 */
unsigned char UART_RemoteCMD_Process(void)
{
	unsigned char Temp_u8 = 0, CheckSum = 0;
	unsigned short i = 0 , Temp_int16 = 0;
	unsigned int *p_u32 = 0;
	static unsigned short  	RandomIndex = 0;
	static unsigned char 		RandomRxError = 0;	// α����������ʱ�ĳ��������
	
	if(RemoteDataPacket.RX_BUF[0] > 2)
	{	
		for(i = 1 ; i < RemoteDataPacket.RX_BUF[0] ; i++)
		{
			CheckSum = CheckSum + RemoteDataPacket.RX_BUF[i];
		}
		if(CheckSum == RemoteDataPacket.RX_BUF[RemoteDataPacket.RX_BUF[0]])	// ������ʾУ�����ȷ
		{
			/*��λ���·����ݹ�о*/
			if((RemoteDataPacket.RX_BUF[2] >= 0x43) & (RemoteDataPacket.RX_BUF[2] <= 0x59))
			{
				memcpy(Uart_Tx_Buffer3, RemoteDataPacket.RX_BUF, Number_Rx_Buffer1);
				Uart_Tx_Buffer3[0] = 0XEA;
				Uart_Tx_Buffer3[Number_Rx_Buffer1 - 1] = 0XED;
				DMA_Tx(USART3 , Number_Rx_Buffer1);
			}
			/*��о���ݱ�����λ��*/

			#if (VERSION_SELECTION == Ver_2_2_0)
			/* 2.20����Ҫ + 0x20 ������0xA3 ��0xB0 �Ļ����� + 0x20 */
			else if((RemoteDataPacket.RX_BUF[2] >= 0xC3) & (RemoteDataPacket.RX_BUF[2] <= 0xD0))
				
			#elif (VERSION_SELECTION == Ver_2_1_0)
			
			else if((RemoteDataPacket.RX_BUF[2] >= 0xA3) & (RemoteDataPacket.RX_BUF[2] <= 0xB0))
			
			#endif
			
			{
				#if (VERSION_SELECTION == Ver_2_1_0)
					//У�����
					RemoteDataPacket.RX_BUF[2] += 0x20;	//2023��9��6���޸� ��о��Ӧ��ID͸��������λ���� ID + 60��Ϊ ID + 80
					//����У��,2023��9��6���޸� ��о��Ӧ��ID͸��������λ���� ID + 60��Ϊ ID + 80
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
				/*�̿�ģʽͨѶ����*/
				case 0x00:	
				if(RemoteDataPacket.RX_BUF[0] == 3)//���ȼ��
				{
					retdata.Mode = 0x80;
					UART_Answer_LightData(USART1 , &retdata);
				}
				break;
				
				/*������������
				*��Ϊ������ȷ����Ҫ�����⣬�����Ǵ���λ���·���ָ�ʵ�尴����������仯��
				*�������ﲻ��ʹ��ϵͳ�������е��ã���Ϊû��ʵ�尴������������Ҳ���ᷢ���仯�ģ��������Ҫ��һ��ȷ����ֵ	
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
						
				/*��������ر�*/	
				case 0x2:				
				if(RemoteDataPacket.RX_BUF[0] == 3)	
				{	
					retdata.Mode = 0x82;
					UART_Answer_LightData(USART1 , &retdata);
					Laser_StateTurn(ENABLE);
				}
				break;
				
				/*��ɫʮ�ֹ����*/
				case 0x3:	
				if(RemoteDataPacket.RX_BUF[0] == 3)
				{
					retdata.Mode = 0x83;
					UART_Answer_LightData(USART1 , &retdata);
					Cross_StateTurn(DISABLE);
				}
				break;
					
				/*��ɫʮ�ֹ�ر�*/
				case 0x4:	
				if(RemoteDataPacket.RX_BUF[0] == 3)
				{
					retdata.Mode = 0x84;
					UART_Answer_LightData(USART1 , &retdata);
					Cross_StateTurn(ENABLE);
				}
				break;
				
				/*����ģ�ⰴ���Ϸ�����*/
				case 0x6:	
				if(RemoteDataPacket.RX_BUF[0] == 3)
				{
					UART_Answer_NoData(USART1);
					MenuTrig_Up();
				}
				break;
				
				/*����ģ�ⰴ���·�����*/
				case 0x7:	
				if(RemoteDataPacket.RX_BUF[0] == 3)
				{
					UART_Answer_NoData(USART1);
					MenuTrig_Down();
				}
				break;
				
				/*����ģ�ⰴ��OK����*/
				case 0x8:
				if(RemoteDataPacket.RX_BUF[0] == 3)
				{
					UART_Answer_NoData(USART1);
					MenuTrig_OK();
				}
				break;
				
				/*����ģ�ⰴ��Cancel����*/
				case 0x9:
				if(RemoteDataPacket.RX_BUF[0] == 3)
				{
					UART_Answer_NoData(USART1);
					MenuTrig_Cancel();
				}
				break;
				
				/*�ж�1��*/						
				case 0x10:	
				if(RemoteDataPacket.RX_BUF[0] == 5)
				{// 
					Byte_TypeConversion.TC_uint64_t = 0;
					Byte_TypeConversion.TC_byte[0] = RemoteDataPacket.RX_BUF[4];
					Byte_TypeConversion.TC_byte[1] = RemoteDataPacket.RX_BUF[3];
					PauseTime = Byte_TypeConversion.TC_uint16_t;
					Pulse_StateTurn(DISABLE);		//�ж�1s��ʱ��������
					TIM_Cmd(TIM2 , ENABLE); 		//ʹ�ܶ�ʱ��2,����1ms�Ķ̴�������,����ʹ�ö�ʱ����Ҫ��Ϊ���Ժ����������ʱ�ȽϷ��㣬
																			//��Ϊֻ����һ���жϣ�Ҳ����̫ռ����Դ						
					retdata.Mode = 0x90;
					UART_Answer_LightData(USART1 , &retdata);
				}
				break;	
					
				/*�̶���������װ��*/
				case 0x11:	
				if(RemoteDataPacket.RX_BUF[0] == 6)	// ���Ƚ�����������ȼ��
				{		
					Byte_TypeConversion.TC_uint64_t = 0;						
					Byte_TypeConversion.TC_byte[0] = RemoteDataPacket.RX_BUF[5];
					Byte_TypeConversion.TC_byte[1] = RemoteDataPacket.RX_BUF[4];
					Byte_TypeConversion.TC_byte[2] = RemoteDataPacket.RX_BUF[3];						
					CycleCodeTime = Byte_TypeConversion.TC_uint32_t;
					
					if(b_Random_EN)	
					{//�����ǰ����α���ģʽ�����˳�α���ģʽ
						b_Random_EN = false;	
					}	
					if(LaserControl.bits.b0 == 1)		
					{//���֮ǰ�Ѿ��ǿ�����״̬�������¸���һ�ο���ָ�
						//���֮ǰ�ǹؼ���״̬����ʲôҲ���䣬ֻ�������ݣ�����һ���ֶ��򿪼��⼴��
						Laser_StateTurn(DISABLE);			//�򿪼���
					}		
					
					unsigned int my_ms = 0;			/*ӦС��Ҫ�����������ֵ��ʾ*/
					my_ms = RemoteDataPacket.RX_BUF[3] << 16 | RemoteDataPacket.RX_BUF[4] << 8 | RemoteDataPacket.RX_BUF[5];
					retdata.cycle[0] =  RemoteDataPacket.RX_BUF[3];
					retdata.cycle[1] =  RemoteDataPacket.RX_BUF[4];
					retdata.cycle[2] =  RemoteDataPacket.RX_BUF[5];
					
					if(cn.Program_control_mode_2 == 0xff)/*����ֵ����ֵ����������*/
					{
						/*�˴�������ʾ���ߴ����� �����ݴ���99.999ʱΪ100.000ʱ��7λʱ���ڵ�ms��λ����ȥ��ms��λ*/
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

				/*α��������������*/
				case 0x20:
				if(RemoteDataPacket.RX_BUF[0] == 7)
				{
					Byte_TypeConversion.TC_uint64_t = 0;						
					Byte_TypeConversion.TC_byte[0] = RemoteDataPacket.RX_BUF[4];
					Byte_TypeConversion.TC_byte[1] = RemoteDataPacket.RX_BUF[3];
					u16_RandomData[7] = Byte_TypeConversion.TC_uint16_t;					// ����λ��
					Byte_TypeConversion.TC_byte[0] = RemoteDataPacket.RX_BUF[6];
					Byte_TypeConversion.TC_byte[1] = RemoteDataPacket.RX_BUF[5];
					u16_RandomData[6] = Byte_TypeConversion.TC_uint16_t;					// ��Ƶ
					RandomIndex = 0;
					RandomRxError = 0;
					if(u16_RandomData[7] > 0 && u16_RandomData[7] < 1017)	
					{		//������Ч��Χ1~1016
						UART_Answer_RandomNumber(USART1, u16_RandomData[7]);
					}
					else if(u16_RandomData[7] >= 1017 || u16_RandomData[7] == 0)
					{
						retdata.Mode = 0xAE;
						UART_Answer_LightData(USART1 , &retdata);		//�ϱ�0xAE������
					}
				}
				break;

				/*α�����������*/
				case 0x21:
				if(RemoteDataPacket.RX_BUF[0] == 7)
				{
					Byte_TypeConversion.TC_uint64_t = 0;
					Temp_u8 = RemoteDataPacket.RX_BUF[3];
					Byte_TypeConversion.TC_byte[0] = RemoteDataPacket.RX_BUF[4];
					Byte_TypeConversion.TC_byte[1] = Temp_u8 & 0x0F;			// ȡ��4λ������������ϰ�д�ģ���Ҳ��֪��Ϊʲô��Ҫ��ôд��
					Temp_int16 = Byte_TypeConversion.TC_uint16_t;					// ��ȡα����������			
					Byte_TypeConversion.TC_byte[0] = RemoteDataPacket.RX_BUF[6];		
					Byte_TypeConversion.TC_byte[1] = RemoteDataPacket.RX_BUF[5];
					Byte_TypeConversion.TC_byte[2] = (Temp_u8 >> 4) & 0x0F;	// ȡ��4λ��	
					u16_RandomData[Temp_int16 + 7] = (uint16_t)(Byte_TypeConversion.TC_uint32_t - u16_RandomData[6]);	// ��α������������Ӧ��ŵ�������
					
					// Ϊ��ʡ�ռ䣬���ݰ���16Bit�洢������С����Ϊ��Ƶ�����ݼ���Ƶ�洢
					if((Temp_int16 != (RandomIndex + 1)) || (Byte_TypeConversion.TC_uint32_t > 120000) ||(Byte_TypeConversion.TC_uint32_t < 40000))
					{
						RandomRxError++;
						UART_AnswerError(USART1, RandomIndex);
					}
					RandomIndex = Temp_int16;
					//Ϊ����ٶȣ����ݲ��ֲ�Ӧ�𣬵�ȫ���������Ӧ��
					//UART_Answer_RandomData(USART1, Temp_int16, Int32_RandomData[Temp_int16]);  
				}
				break;
					
				/*α������뷢����ϼ��*/
				case 0x22:
				if(RandomRxError == 0)	// =0˵�����յ���������Ŷ���
				{ //α����������з�����Ϻ�ļ��
					if(RandomCode_Check())
					{				
						retdata.Mode = 0xA2;
						UART_Answer_LightData(USART1 , &retdata);				
						Laser_StateTurn(ENABLE);	//�رռ����������ָ����λ�����·�һ�Σ�Ϊ�������������Ҳ��һ�δ���
						//��������whileѭ��������b_Random_EN״̬�Զ�����α���״̬
						b_Random_EN = true;		//����α���ģʽ
					}
					else
					{
						retdata.Mode = 0xAE;
						UART_Answer_LightData(USART1 , &retdata);	//�ϱ��������				
					}
				}
				else
				{
					UART_AnswerError(USART1, RandomRxError);
				}
				break;

				/*α�������洢*/
				case 0x23:	
				if(RemoteDataPacket.RX_BUF[0] == 11)
				{
					uint8_t SaveCH = RemoteDataPacket.RX_BUF[3];			
					/*2������Ϊ�人�ߵº��⣬4���趨Ϊͨ�ð汾*/
					if((SaveCH > 0) && (SaveCH < 5))		//V3.01 ��α������ȣ�ԭ���룺if((SaveCH >0) && (SaveCH <5))
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
						//V3.01 ��α������ȣ��洢Ӧ��ԭ���룺UART_Answer_NoData(USART1);
					}
				}
				break;					
				
				/*α�������ɾ��*/
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
						UART_Answer_LightData(USART1 , &retdata); //V3.01 ��α������ȣ�ɾ��Ӧ��ԭ���룺UART_Answer_NoData(USART1);
					}
				}
				break;
					
				/*��������*/
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
						static unsigned short temp = 0;			/*2023 9 7 �� Ҫ�����ӹ̶�DB 2000*/
						static unsigned char SendRemotBuff[16] = {0};
						memset(SendRemotBuff , 0 , sizeof(SendRemotBuff));
						memcpy(SendRemotBuff , RemoteDataPacket.TX_BUF , 16);
						temp = RemoteDataPacket.TX_BUF[5] << 8 | RemoteDataPacket.TX_BUF[6];
						temp += 2000;
						SendRemotBuff[6]= temp;					//�ȸߺ��
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
					/*����ʱ���֣���һ�����ǲ�����λ���ظ�˥��ֵ������ǿ�Ƹ���һ��*/
					retdata.energy[0] = RemoteDataPacket.TX_BUF[5];
					retdata.energy[1] = RemoteDataPacket.TX_BUF[6];
					UART_Answer_LightData(USART1 , &retdata);	
					
					attenuation_number(&cn , &RemoteDataPacket);		/*2022 8 3 ӦС��Ҫ�����˥����ֵ��ʾ*/			
					unsigned short show = 0;				
					unsigned char bit_point = 0;				//��ʾ��λ��					
					unsigned char decimal_point = 0;		//��ʾ��С����λ��					
					show = cn.control_number_hight | cn.control_number_low << 8;
					if(show >= 0x3E8 )									/*��ֵ������λ��*/
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
					//��Ҫ����С����ʾ
					if(cn.Program_control_mode_1 == 0xff)
					{
						display.disenergy = 1;
						display.bit_point = bit_point;
						display.decimal_point = decimal_point;
						display.disenergyshow = show;
					}	

				}				
				break;
					
				/*���������趨*/
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
			return 0x1;	//�˳�������У��ʹ���
		}
	}
	else
	{
		return 0x2;	//�˳����������ݳ��Ȳ���
	}
}



/**
 * @brief ���ڵ��ֽ�ָ��ͣ�ʺɽ���룬�����Լ����鷳�ͱ���
 *
 * ��������ṩ���ں����ĸ߼�������������������������
 *
 * @param[in]  ����1  ����1�������������������ͺͺ���
 * @param[out] ����2  ����2�������������������ͺͺ���
 *
 * @return ��������ֵ�������������������ͺͺ��壬������õĻ���
 *
 * @note �κ�����ע��������÷�ʾ����������������ӡ�
 */
void UART_Answer_LightData(USART_TypeDef* USARTx , RETDATA* rd)
{	
	unsigned char cmode = rd->Mode;
	unsigned char ctmmp = rd->temp;
	unsigned char ccyle[5] 		= {rd->cycle[0] , rd->cycle[1] , rd->cycle[2]};
	unsigned char cenergy[5] 	= {rd->energy[0] , rd->energy[1] , rd->energy[2]};
	RemoteDataPacket.TX_BUF[0] = 0;	// Buff����
	UART_SendInbuff(RemoteDataPacket.TX_BUF , RemoteDataPacket.RX_BUF[1]);		/*���ض�Ӧ�İ����*/

	switch(cmode)
	{
		case 0x80://����
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
			break;
		
		case 0x81://���⿪
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
			break;

		case 0x82://�����
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;

		case 0x83://�����
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;
		
		case 0x84://�غ��
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;

		case 0x89://�̿�ģʽ
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;
		
		case 0x90://��ͣ
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ctmmp);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;

		case 0x91://Ƶ��װ��
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ccyle[0]);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ccyle[1]);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , ccyle[2]);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;

		case 0xc1://����װ��
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cenergy[0]);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cenergy[1]);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cenergy[2]);	
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;

		case 0xA2://α����ɹ�
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;

		case 0xAE://α�������
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;
		
		case 0xA3://V3.01 ��α������ȣ�α����洢�ɹ�   
				UART_SendInbuff(RemoteDataPacket.TX_BUF , cmode);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);
				UART_SendInbuff(RemoteDataPacket.TX_BUF , 0x00);	
		break;
		
		case 0xA4://V3.01 ��α������ȣ�α���ɾ���ɹ�
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

