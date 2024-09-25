
#include "bsp.h"


bool RandomCode_Check(void)
{
	int i;
	bool ReturnValue = true;
	uint32_t Temp_u32;
	
	if((u16_RandomData[7] > 0) && (u16_RandomData[7] < 1017))	//数据有效范围1~1016
	{
		for(i=1; i<u16_RandomData[7]; i++)
		{
			Temp_u32 = u16_RandomData[6] + u16_RandomData[i+7];
			
			if((Temp_u32 < 40000) || (Temp_u32 > 120000))
			{
				ReturnValue = false;
				i = u16_RandomData[0] + 10;
			}
		}
	}
	else
	{	// 索引长度检查出错
		ReturnValue = false;
	}
	
	return ReturnValue;
}

/*
伪随机编码检查
格式：8字节标识名+2字节预留+2字节伪随机编码位数，共计占用4个32位空间，数组里还剩最大1020个空间可存放伪随机数据
*/
uint64_t RandomSaveData_Check (uint32_t Address)
{
	union Union_TypeConversion Format_Conversion;
	//uint32_t Random_Info[4];
	uint16_t i, Temp_u16, Number, BaseFreq, MinValue = 0xFFFF, MaxValue = 0x0, *p;
	bool CheckValue = true;
	
	Format_Conversion.TC_u32[0] = u32_FlashBuff[512*Address];
	Format_Conversion.TC_u32[1] = u32_FlashBuff[512*Address+1];
	
	for(i=0; i<6; i++)
	{
		if((Format_Conversion.TC_byte[i]<0x20) || (Format_Conversion.TC_byte[i]>0x7E))
		{	// 判断字符是否合法
			CheckValue = false;
			i=100;
		}
	}
	
	if(CheckValue)
	{	// 如果上述判断合法，则继续判断编码位长是否合规
		p=(uint16_t *)&u32_FlashBuff[512*Address+3];
		BaseFreq = *p;
		Number = *(p+1);
		if ((Number < 1) && (Number > 1016))
		{
			CheckValue = false;	// 正常的编码长度是2到1016
		}
		else
		{
			for(i=0; i<Number; i++)
			{
				Temp_u16 = *(p+2+i);
				if(Temp_u16 > MaxValue) MaxValue = Temp_u16;
				if(Temp_u16 < MinValue) MinValue = Temp_u16;
				if(((Temp_u16 + BaseFreq) < 40000) || ((Temp_u16 + BaseFreq) > 120000))
				{
					CheckValue = false;	// 数据越界
					i = Number + 10;
				}					
			}
		}
	}
	
	if(CheckValue)	// 
	{
		RandomNumber = Number;
		RandomBaseFreq = BaseFreq;
		RandomMinValue = MinValue;
		RandomMaxValue = MaxValue;
		Format_Conversion.TC_byte[7] = 0x0;
		return Format_Conversion.TC_uint64_t;
	}
	else return 0;
}
