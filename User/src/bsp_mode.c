#include "bsp.h"


/**
 * @brief
 *
 * 这里可以提供关于函数的高级概述，包括它的输入和输出。
 *
 * @param[in]  参数1  参数1的描述，包括数据类型和含义
 * @param[out] 参数2  参数2的描述，包括数据类型和含义
 *
 * @return 函数返回值的描述，包括数据类型和含义，如果适用的话。
 *
 * @note 任何特殊注意事项或用法示例都可以在这里添加。
 *		V2.01 如果是伪随机模式，禁用
 *		正常模式，更新周期编码。 源代码：CycleCode_Renew(CycleCodeTime);
 */
 
void random_mode()
{
		if(b_Random_EN == false) 
		{
			CycleCode_Renew(CycleCodeTime); //正常模式
		}
		else
		{
			
		}
		
		//退出伪随机
		if(b_Random_EN == true)	//如果此前处于伪随机模式，
		{
			b_Random_EN = false;	//则退出伪随机模式
		}
}

