#include "bsp.h"

/**
 * @brief ���ϰ���
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
void MenuTrig_Up(void)
{
	switch(Menu_Indexs.Current_Index)
	{
		/*�Ϸ�ѡ����*/
		case 0:
			Menu_Indexs.Next_Index++;
			if(Menu_Indexs.Next_Index == 3)
			{
				Menu_Indexs.Next_Index = 4;
			}
			else if(Menu_Indexs.Next_Index == 5)
			{
				Menu_Indexs.Next_Index = 6;
			}
			else if(Menu_Indexs.Next_Index >= 7)
			{
				Menu_Indexs.Next_Index = 1;
			}
			else
			{
			}			
			Menu_Refresh();
			Menu_Indexs.Prior_Index = Menu_Indexs.Next_Index;
			break;
			
		/*�Ϸ�ѡ�������趨*/
		case 1:
			switch(Menu_Indexs.Next_Index)// ��Ϊ��������Ҫǣ���������㣬�ٶȻ���Щ��������switch...case����
			{	
				case 1:
					Temp_CycleCodeTime = Temp_CycleCodeTime + 1;//0.001ms
					break;
				case 2:
					Temp_CycleCodeTime = Temp_CycleCodeTime + 10;//0.010ms
					break;
				case 3:
					Temp_CycleCodeTime = Temp_CycleCodeTime + 100;//0.100ms
					break;
				case 4:
					Temp_CycleCodeTime = Temp_CycleCodeTime + 1000;//1.000ms
					break;
				case 5:
					Temp_CycleCodeTime = Temp_CycleCodeTime + 10000;//10.000ms
					break;
				case 6:
					Temp_CycleCodeTime = Temp_CycleCodeTime + 100000;//100 .000ms
					break;
				default:
					break;		
			}
			if(Temp_CycleCodeTime > 120000) 
			{
				Temp_CycleCodeTime = 120000;
			}
			Menu_Refresh();
			break;
			
		case 2:
			if(Menu_Indexs.Prior_Index < 10)// ��С��10��˵���ڽ���α�������ѡ�񣬿�ִ������Ĵ���
			{	
				Menu_Indexs.Next_Index++;
				if(Menu_Indexs.Next_Index > 4)
					Menu_Indexs.Next_Index = 1;
				Menu_Refresh();
				Menu_Indexs.Prior_Index = Menu_Indexs.Next_Index;
			}
			break;
			
		case 3:
			break;
		
		/*�Ϸ�ѡ�������趨*/
		case 4:
			switch(Menu_Indexs.Next_Index)	// ��Ϊ��������Ҫǣ���������㣬�ٶȻ���Щ��������switch...case����
			{	
				case 1:		//��λ
					Temp_dB_Loss = Temp_dB_Loss + 1;
					break;
				
				case 2:		//ʮλ
					Temp_dB_Loss = Temp_dB_Loss + 10;
					break;
				
				case 3:		//��λ
					Temp_dB_Loss = Temp_dB_Loss + 100;
					break;
				
				default:
					break;		
			}
			if(Temp_dB_Loss > 750)
			{
				Temp_dB_Loss = 750;
			}
			Menu_Refresh();
			break;
			
		case 5:
			break;
		
		case 6:
			break;
		
		default:
			break;
	}
}

/**
 * @brief ���°���
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
void MenuTrig_Down(void)
{
	switch(Menu_Indexs.Current_Index)
	{
		
		/*�·�ѡ����*/
		case 0:
			Menu_Indexs.Next_Index--;			
			if(Menu_Indexs.Next_Index == 5)
			{
				Menu_Indexs.Next_Index = 4;
			}
			else if(Menu_Indexs.Next_Index == 3)
			{
				Menu_Indexs.Next_Index = 2;
			}
			else if(Menu_Indexs.Next_Index <= 0)
			{
				Menu_Indexs.Next_Index = 6;
			}
			else
			{
			}
			Menu_Refresh();
			Menu_Indexs.Prior_Index = Menu_Indexs.Next_Index;
			break;
			
		/*�·������趨*/
		case 1:
			switch(Menu_Indexs.Next_Index)// ��Ϊ��������Ҫǣ���������㣬�ٶȻ���Щ��������switch...case����
			{	
				case 1:
					Temp_CycleCodeTime = Temp_CycleCodeTime - 1;
					break;
				case 2:
					Temp_CycleCodeTime = Temp_CycleCodeTime - 10;
					break;
				case 3:
					Temp_CycleCodeTime = Temp_CycleCodeTime - 100;
					break;
				case 4:
					Temp_CycleCodeTime = Temp_CycleCodeTime - 1000;
					break;
				case 5:
					if(Temp_CycleCodeTime > 19000)
						Temp_CycleCodeTime = Temp_CycleCodeTime - 10000;
					break;
				case 6:
					if(Temp_CycleCodeTime > 109000)
						Temp_CycleCodeTime = Temp_CycleCodeTime - 100000;
					break;
				default:
					break;		
			}
//			if(Temp_CycleCodeTime < 9000) 
//			{
//				Temp_CycleCodeTime = 9000;
//			}
			/*	2024.3.15�����������������	*/
			if(Temp_CycleCodeTime < 0) 
			{
				Temp_CycleCodeTime = 0;
			}
			Menu_Refresh();
			break;
			
		
		case 2:
			if(Menu_Indexs.Prior_Index < 10)// ��С��10��˵���ڽ���α�������ѡ�񣬿�ִ������Ĵ���
			{
				Menu_Indexs.Next_Index--;
				if(Menu_Indexs.Next_Index==0)
				{
					Menu_Indexs.Next_Index = 4;
				}
				Menu_Refresh();
				Menu_Indexs.Prior_Index = Menu_Indexs.Next_Index;
			}
			break;
			
		case 3:
			break;
		
		/*�·������趨*/
		case 4:
			switch(Menu_Indexs.Next_Index)	// ��Ϊ��������Ҫǣ���������㣬�ٶȻ���Щ��������switch...case����
			{	
				case 1:
					Temp_dB_Loss = Temp_dB_Loss - 1;
					break;
				
				case 2:
					if(Temp_dB_Loss > 10)
					{
						Temp_dB_Loss = Temp_dB_Loss - 10;
					}
					break;
					
				case 3:
					if(Temp_dB_Loss > 100)
					{
						Temp_dB_Loss = Temp_dB_Loss - 100;
					}
					break;
					
				default:
					break;		
			}
			if(Temp_dB_Loss < 0) 
			{
				Temp_dB_Loss = 0;
			}
			Menu_Refresh();
			break;
			
		case 5:
			break;
		
		case 6:
			break;
		
		default:
			break;
	}
}


