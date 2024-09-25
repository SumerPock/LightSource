#include "bsp.h"
#include "main.h"

/**
 * @brief 主菜单界面
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
void UI_Init(void)
{
	Menu_Indexs.Current_Index = 0;
	Menu_Indexs.Next_Index = 1;
	Menu_Indexs.Prior_Index = Menu_Indexs.Next_Index;
	UI_Level_0();
	Menu_Refresh();
}


/**
 * @brief 0级菜单，也就是1-6种模式选择
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
void UI_Level_0(void)
{
	LCD_Clear(BLUE);					//按指定底色清屏
	Menu_Title();
	Menu_0_1(true);
	Menu_0_2(true);
	Menu_0_3(true);
	Menu_0_4(true);
	Menu_0_5(true);
	Menu_0_6(true);
	Menu_Infobar();
	
	if(b_Random_EN)	//如果此前处于伪随机模式，则退出伪随机模式
	{
		b_Random_EN = false;	
	}
	
	Lightsoutce.UIMode = 0;
}

/**
 * @brief 1级菜单，精确频率码
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
void UI_Level_1(void)
{
//char str[12];
//snprintf(str, 10, "%d", 50.013);
	LCD_Clear(BLUE);	//按指定底色清屏
	Menu_Title();			//标题				
	Menu_0_1(true);
	Menu_1();
	LCD_Print(32+16*7, 36*2+32+16, "ms", WHITE, BLUE);
	Menu_Infobar();
	Lightsoutce.UIMode = 1;
}

/**
 * @brief 2级菜单，伪随机
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
void UI_Level_2(void)
{
	LCD_Clear(BLUE);		//按指定底色清屏
	Menu_Title();				//标题		
	Menu_0_2(true);
	Menu_2_x(true, 1);
	Menu_2_x(true, 2);
	Menu_2_x(true, 3);
	Menu_2_x(true, 4);
	Menu_Infobar();
	Lightsoutce.UIMode = 2;
}

/**
 * @brief 界面3，变间隔码
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
void UI_Level_3(void)
{
	LCD_Clear(BLUE);					//按指定底色清屏
	Menu_Title();
	Menu_0_3(true);
	LCD_Print(16, 36*2 + 32 + 16, 	"1)基频", WHITE, BLUE);
	LCD_Print(48, 36*3 + 32 + 16, 	"50.000 ms", WHITE, BLUE);
	LCD_Print(16, 36*4 + 32 + 24, 	"2)间隔调制", WHITE, BLUE);
	LCD_Print(48, 36*5 + 32 + 24, 	"10011011", WHITE, BLUE);
	Menu_Infobar();
	Lightsoutce.UIMode = 3;
}

/**
 * @brief 界面4，输出能量设定
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
void UI_Level_4(void)
{	
	LCD_Clear(BLUE);
	Menu_Title();
	Menu_0_4(true);
	LCD_Print(16, 36*2+32+16, 	"衰减:    dB", WHITE, BLUE);
	Menu_4();
	Menu_Infobar();
	Lightsoutce.UIMode = 4;
}

/**
 * @brief 仿真模式
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
void UI_Level_5(void)
{
	char str[12];
	snprintf(str, 10, "%d", 123456);	
	LCD_Clear(BLUE);					//按指定底色清屏
	Menu_Title();
	Menu_0_5(true);
	LCD_Print(48, 36*2+32+16, 	" 40 ms", WHITE, BLUE);
	LCD_Print(48, 36*3+32+16, 	" 60 ms", WHITE, BLUE);
	LCD_Print(48, 36*4+32+16, 	" 80 ms", WHITE, BLUE);
	LCD_Print(48, 36*5+32+16, 	"100 ms", WHITE, BLUE);
	Menu_Infobar();
	Lightsoutce.UIMode = 5;
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
void Program_control(control_number* cp)
{
	cp->Program_control_mode_1 = 0xFF;
	cp->Program_control_mode_2 = 0xFF;
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
void UI_Level_6(void)
{
	LCD_Clear(BLUE);					//按指定底色清屏
	Menu_Title();
	Menu_0_6(true);
	LCD_Print(0, 36*2+32+16, "接受上位机控制", WHITE, BLUE);
	//LCD_Print(0 , 36*2 + 32 + 16 + 32 + 16 , 	"衰减:    dB" , WHITE , BLUE);
	LCD_Print(0 , 36*2 + 32 + 16 + 32 + 16 , 				    "衰减:       " , WHITE , BLUE);
	LCD_Print(0 , 36*2 + 32 + 16 + 32 + 16 + 32 + 16 , 	"周期:       " , WHITE , BLUE);
	/*此处加入 FLAG 以证明用户进入了程控模式*/
	Program_control(&cn);
	Menu_Infobar();
	//USART1_Config(ENABLE);
	Lightsoutce.UIMode = 6; 	
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
void Menu_Title(void)
{
	LCD_Print(0, 16, 			"激光目标模拟器", WHITE, BLUE);
}


/**
 * @brief 型号及软件版本号
 *
 * 这里可以提供关于函数的高级概述，包括它的输入和输出。
 *
 * @param[in]  参数1  参数1的描述，包括数据类型和含义
 * @param[out] 参数2  参数2的描述，包括数据类型和含义
 *
 * @return 函数返回值的描述，包括数据类型和含义，如果适用的话。
 *
 * @note 任何特殊注意事项或用法示例都可以在这里添加。
 * 2.11 加入固定衰减20DB
 */
void Menu_Infobar(void)
{
	#if (VERSION_SELECTION == Ver_2_1_0)
		LCD_ShowString(100 , 300 , "Ver2.10 CL202-B" , WHITE);
	#elif (VERSION_SELECTION == Ver_2_1_1)
		LCD_ShowString(100 , 300 , "Ver2.11 CL202-B" , WHITE); 
	#elif (VERSION_SELECTION == Ver_2_2_0)
		LCD_ShowString(100 , 300 , "Ver2.20 CL202-B" , WHITE); 
	#endif	
}

