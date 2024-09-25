#include "bsp.h"

/**
 * @brief ok按键
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
void MenuTrig_OK(void)
{
	switch(Menu_Indexs.Current_Index)
	{
		case 0:	// 按OK键之前处于顶级菜单
			if ((b_Ext_Sync) || (Menu_Indexs.Next_Index>3))
			{	
				Menu_Indexs.Current_Index = Menu_Indexs.Next_Index;
				switch(Menu_Indexs.Current_Index)
				{
					/*精确频率编码*/
					case 1:
						Menu_Indexs.Next_Index = 0;
						Menu_Indexs.Prior_Index = 1;
						Temp_CycleCodeTime = CycleCodeTime;	// 将正式编码周期数值更新到临时变量中
						UI_Level_1();
						break;
					
					/*伪随机编码*/
					case 2:
						Menu_Indexs.Next_Index = 1;
						Menu_Indexs.Prior_Index = 1;
						UI_Level_2();
						Menu_Refresh();
						break;
					
					/*变间隔码*/
					case 3:
						Menu_Indexs.Next_Index = 30;
						Menu_Indexs.Prior_Index = 1;
						UI_Level_3();
						break;
					
					/*输出能量设定*/
					case 4:
						Menu_Indexs.Next_Index = 0;
						Menu_Indexs.Prior_Index = 1;
						Temp_dB_Loss = dB_Loss;	// 将正式激光能量衰减数值更新到临时变量中
						UI_Level_4();
						break;
					
					/*仿真模式*/
					case 5:
						Menu_Indexs.Next_Index = 50;
						Menu_Indexs.Prior_Index = 1;
						UI_Level_5();
						break;
					
					/*程控模式*/
					case 6:
						Menu_Indexs.Next_Index = 60;
						Menu_Indexs.Prior_Index = 1;
						UI_Level_6();	
						retdata.Mode = 0x80;
						UART_Answer_LightData(USART1 , &retdata);			
						break;
					
					default:
						break;
				}
			}
			break;
			
		case 1:	// 按OK键之前处于精确频率编码设置菜单
			if (Menu_Indexs.Prior_Index == 0xFF)		// Index=0xFF，说明是在询问是否退出
			{	
					Menu_Indexs.Next_Index = Menu_Indexs.Current_Index;
					Menu_Indexs.Prior_Index = Menu_Indexs.Current_Index;
					Menu_Indexs.Current_Index = 0;
					UI_Level_0();
					Menu_Refresh();
			}
			else
			{
				if(Menu_Indexs.Next_Index == 0)
				{
					if(Temp_CycleCodeTime < 100000) 
						Menu_Indexs.Next_Index = Menu_Indexs.Next_Index + 5;
					else 
						Menu_Indexs.Next_Index = Menu_Indexs.Next_Index + 6;	//如果按下OK之前是0，则+6，将设定值的最高位点亮
				}
				else 
					Menu_Indexs.Next_Index--;
				Menu_Refresh();
				if(Menu_Indexs.Next_Index == 0)
				{
					CycleCodeTime = Temp_CycleCodeTime;	// 确认后，将临时变量的数值同步到正式变量中。
					CycleCode_Renew(CycleCodeTime);
					LCD_Print(32, 176, "编码已更新", WHITE, BLUE);
					b_Random_EN = false;
				}
				else  	
					LCD_Print(0, 176, "              ", WHITE, BLUE);
			}
			break;

		case 2:	// 按OK键之前处于伪随机编码
			if (Menu_Indexs.Prior_Index == 0xFF)
			{
			}
			else
			{
				Menu_Indexs.Prior_Index = 0x20;
				Menu_Refresh();				
			}
			break;
		
		case 3:
			// 按OK键之前处于程控模式菜单
			if (Menu_Indexs.Prior_Index == 0xFF)
			{
				Menu_Indexs.Next_Index = Menu_Indexs.Current_Index;
				Menu_Indexs.Prior_Index = Menu_Indexs.Current_Index;
				Menu_Indexs.Current_Index = 0;
				UI_Level_0();
				Menu_Refresh();
			}
			else if(Menu_Indexs.Prior_Index > 10)
			{	// 说明要从已选择编码的菜单返回
				Menu_Indexs.Prior_Index = Menu_Indexs.Next_Index;
				Menu_Refresh();
			}
			else
			{
				Menu_Indexs.Prior_Index = 1;
				/* 2022 8 5
					 这句话在这里有什么意义？这个位置原本就是空白的 我加入衰减和周期后 进入程控模式 按下确定键
					 后会把我的字符盖住，所以我屏蔽了，如果这里我分析的有问题或者原本有用请联系我
				*/
				//LCD_Print(0, 176, "              ", WHITE, BLUE);
			}
			break;
			
		case 4:	// 按OK键之前处于能量衰减设置菜单
			if (Menu_Indexs.Prior_Index == 0xFF)
			{
					Menu_Indexs.Next_Index = Menu_Indexs.Current_Index;
					Menu_Indexs.Prior_Index = Menu_Indexs.Current_Index;
					Menu_Indexs.Current_Index = 0;
					UI_Level_0();
					Menu_Refresh();
			}
			else
			{
				if(Menu_Indexs.Next_Index == 0) 
					Menu_Indexs.Next_Index = Menu_Indexs.Next_Index+3;	//如果按下OK之前是0，则+3，将设定值的最高位点亮
				else 
					Menu_Indexs.Next_Index--;
				Menu_Refresh();

				if(Menu_Indexs.Next_Index == 0)
				{
					dB_Loss = Temp_dB_Loss;			//确认后，将临时变量的数值同步到正式变量中。
					
					#if (VERSION_SELECTION == Ver_2_1_0)
						
					#elif (VERSION_SELECTION == Ver_2_1_1)
						dB_Loss += 200; //2023.9.7上午刘辉提需求要改，固件加20DB，下午就要
					#elif (VERSION_SELECTION == Ver_2_2_0)
					
					#endif	
					
					LCD_Print(32, 176, "衰减已更新", WHITE, BLUE);				
					RemoteDataPacket.RX_BUF[2] = 0x41;					/*新增通过界面更新能量函数*/
					
					/*2023 . 9 .7 这俩个0，必须加上，不加会造成激光器不出光
					*/
					RemoteDataPacket.RX_BUF[3] = 0;
					RemoteDataPacket.RX_BUF[4] = 0;
					
					RemoteDataPacket.RX_BUF[5] = ((dB_Loss + 0)*10)/256;
					RemoteDataPacket.RX_BUF[6] = ((dB_Loss + 0)*10)%256;
					RemoteDataPacket.TX_BUF[0] = 0;	
					for(int i = 1 ; i < 7 ; i++)
					{
						UART_SendInbuff(RemoteDataPacket.TX_BUF, RemoteDataPacket.RX_BUF[i]);	
					}
					//不要问为什么发送俩次，老板的代码里是俩次，我不修改
					UART_RemoteEncode(USART3 , RemoteDataPacket.TX_BUF);
					RemoteDataPacket.RX_BUF[1]++; 	//包序号
					//若不加延时回造成DMA发送数据 错数
					delay_xms(10);
					UART_RemoteEncode(USART3 , RemoteDataPacket.TX_BUF);
				}
				else 
				{
					LCD_Print(0, 176, "              ", WHITE, BLUE);
				}
			}
			break;	
		
		case 6:
			// 按OK键之前处于程控模式菜单
			if (Menu_Indexs.Prior_Index == 0xFF)
			{
				//USART3_Config(DISABLE);			//关闭串口
				//USART1_Config(DISABLE);			//关闭串口
				Menu_Indexs.Next_Index = Menu_Indexs.Current_Index;
				Menu_Indexs.Prior_Index = Menu_Indexs.Current_Index;
				Menu_Indexs.Current_Index = 0;
				UI_Level_0();
				Menu_Refresh();
			}
			else if(Menu_Indexs.Prior_Index > 10)
			{	// 说明要从已选择编码的菜单返回
				Menu_Indexs.Prior_Index = Menu_Indexs.Next_Index;
				Menu_Refresh();
			}
			else
			{
				Menu_Indexs.Prior_Index = 1;
				/* 2022 8 5
					 这句话在这里有什么意义？这个位置原本就是空白的 我加入衰减和周期后 进入程控模式 按下确定键
					 后会把我的字符盖住，所以我屏蔽了，如果这里我分析的有问题或者原本有用请联系我
				*/
				//LCD_Print(0, 176, "              ", WHITE, BLUE);
			}
			
			
		default:
			break;
	}

}



/**
 * @brief 取消按键
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
void MenuTrig_Cancel(void)
{
	switch(Menu_Indexs.Current_Index)
	{
		/*0级取消 刷屏*/
		case 0:
			LCD_Init();
			UI_Level_0();
			Menu_Refresh();
			break;
		
		/*1级取消*/
		case 1:
			if (Menu_Indexs.Prior_Index == 0xFF)
			{
				Menu_Indexs.Prior_Index = 1;
				LCD_Print(0, 176, "              ", WHITE, BLUE);
			}
			else if(Menu_Indexs.Next_Index == 0)	//如果按下C之前是0，退出
			{
					LCD_Print(32, 176, "确认返回? ", WHITE, BLUE);
					Menu_Indexs.Prior_Index = 0xFF;
			}
			else if(Menu_Indexs.Next_Index == 6)
			{
				Menu_Indexs.Next_Index = 0;
				Menu_Indexs.Prior_Index = 1;
				Temp_CycleCodeTime = CycleCodeTime;	// 回退到0后，以前对临时变量的更改都作废，重新加载临时变量的数值。
				LCD_Print(32, 176, "修改已取消", WHITE, BLUE);
			}
			else
			{
				Menu_Indexs.Prior_Index = 1;
				Menu_Indexs.Next_Index++;
			}
			Menu_Refresh();
			break;
		
		/*2级取消*/
		case 2:
			if(Menu_Indexs.Prior_Index == 0x20)
			{
				Menu_Indexs.Prior_Index = Menu_Indexs.Next_Index;
				Menu_2_x(true, 1);
				Menu_2_x(true, 2);
				Menu_2_x(true, 3);
				Menu_2_x(true, 4);
				Menu_Refresh();
			}
			else
			{
				Menu_Indexs.Next_Index = Menu_Indexs.Current_Index;
				Menu_Indexs.Prior_Index = Menu_Indexs.Current_Index;
				Menu_Indexs.Current_Index = 0;
				UI_Level_0();
				Menu_Refresh();
			}
			break;
						
		case 4:
			if (Menu_Indexs.Prior_Index == 0xFF)
			{
				Menu_Indexs.Prior_Index = 1;
				LCD_Print(0, 176, "              ", WHITE, BLUE);
			}
			else if(Menu_Indexs.Next_Index == 0)	//如果按下C之前是0，退出
			{
					LCD_Print(32, 176, "确认返回? ", WHITE, BLUE);
					Menu_Indexs.Prior_Index = 0xFF;
			}
			else if(Menu_Indexs.Next_Index == 3)
			{
				Menu_Indexs.Next_Index = 0;
				Menu_Indexs.Prior_Index = 1;
				Temp_dB_Loss = dB_Loss;	// 回退到0后，以前对临时变量的更改都作废，重新加载临时变量的数值。
				LCD_Print(32, 176, "修改已取消", WHITE, BLUE);
			}
			else
			{
				Menu_Indexs.Prior_Index = 1;
				Menu_Indexs.Next_Index++;
			}
			Menu_Refresh();
			break;

		case 6:
			if (Menu_Indexs.Prior_Index == 0xFF)
			{
				/*再次进入程控模式*/
				LCD_Print(0, 176, "              ", WHITE, BLUE);
				
				LCD_Init();		
				UI_Level_6();
				Menu_Refresh();
				
				Menu_Indexs.Prior_Index = 1;
			}
			else
			{
				LCD_Print(32, 176, "确认返回? ", WHITE, BLUE);
				cn.Program_control_mode_1 = false;
				cn.Program_control_mode_2 = false;
				Menu_Indexs.Prior_Index = 0xFF;
			}
			break;
			
		case 3:
			if (Menu_Indexs.Prior_Index == 0xFF)
			{
				/*再次进入变间隔码模式*/
				LCD_Print(0, 176, "              ", WHITE, BLUE);
				
				LCD_Init();		
				UI_Level_3();
				Menu_Refresh();				
				
				Menu_Indexs.Prior_Index = 1;
			}
			else
			{
				LCD_Print(32, 176, "确认返回? ", WHITE, BLUE);
				cn.Program_control_mode_1 = false;
				cn.Program_control_mode_2 = false;
				Menu_Indexs.Prior_Index = 0xFF;
			}			
//			Menu_Indexs.Next_Index = Menu_Indexs.Current_Index;
//			Menu_Indexs.Prior_Index = Menu_Indexs.Current_Index;
//			Menu_Indexs.Current_Index = 0;
//			UI_Level_0();
//			Menu_Refresh();
			break;
		
		case 5:
			Menu_Indexs.Next_Index = Menu_Indexs.Current_Index;
			Menu_Indexs.Prior_Index = Menu_Indexs.Current_Index;
			Menu_Indexs.Current_Index = 0;
			UI_Level_0();
			Menu_Refresh();
			break;
		
		default:
			break;
	}
}




