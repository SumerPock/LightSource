

#include "bsp.h"

uint32_t GetSector(uint32_t Address);
uint32_t STMFLASH_ReadWord(uint32_t Address);

	/************************************************
	函数名称 ： STMFLASH_ReadWord
	功    能 ： 从目标地址读取一个32位数
	参    数 ： Address
	返 回 值 ： 
	备	  注 ： 
	*************************************************/
uint32_t STMFLASH_ReadWord(uint32_t Address)
{
	return *(__IO uint32_t*)Address;
} 

	/************************************************
	函数名称 ： Read_Memory
	功    能 ： 读出RAM或Flash中的数据
	参    数 ： Start_Address 起始地址
							size 					读取的数据个数（32bit）
							*p 						目标数据指针
	返 回 值 ： 无
	备	  注 ： 
	*************************************************/
void Read_Memory(uint32_t Start_Address, uint16_t size, uint32_t *p)
{
	uint16_t i;
  for (i=0; i<size; i++)
  {
    *(p+i) = *(__IO uint32_t*)(Start_Address+i*4);   //读FLASH中的数据，直接给出地址就行了。跟从内存中读数据一样。
  }
}


/************************************************
函数名称 ： STMFLASH_Write
功    能 ： 
参    数 ： WriteAddr:	起始地址(此地址必须为4的倍数!!)
						pBuffer:		数据指针
						size:	字数(就是要写入的32位数据的个数.)
返 回 值 ： 无
备	  注 ： 
*************************************************/
void STMFLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t size)
{
	FLASH_Status status = FLASH_COMPLETE;
	u32 addrx = 0;
	u32 endaddr = 0;
	int SectorStart,SectorEnd;
	//int SectorStart = 0;
	/*非法地址*/
	if(WriteAddr<ADDR_FLASH_SECTOR_0||WriteAddr%4)
	{
		return;	
	}
	/*FLASH解锁*/
	FLASH_Unlock();		
	/* 擦除用户区域 (用户区域指程序本身没有使用的空间，可以自定义)**/
	/* 清除各种FLASH的标志位 */  	
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR| 
                  FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
	/*FLASH擦除期间,必须禁止数据缓存*/
	FLASH_DataCacheCmd(DISABLE);
	/*
	写入的起始地址
	*/
	addrx = WriteAddr;		
	/*
	写入的结束地址
	*/
	endaddr = WriteAddr + size;	
//	endaddr=WriteAddr+NumToWrite*4;		
//	SectorStart = STMFLASH_GetFlashSector(addrx);
//	SectorEnd = STMFLASH_GetFlashSector(endaddr);
	
	SectorStart =GetSector(addrx);
	SectorEnd = GetSector(endaddr);
	
	/*只有主存储区,才需要执行擦除操作!!*/
	if(addrx<0X1FFF0000)			
	{
		status=FLASH_EraseSector(SectorStart, VoltageRange_3);/*VCC=2.7~3.6V之间!!*/
		/*扫清一切障碍.(对非FFFFFFFF的地方,先擦除)*/
//		while(SectorStart <= SectorEnd)
//		{
//			/*有非0XFFFFFFFF的地方,要擦除这个扇区*/
//			if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)
//			{   
//				
//				status=FLASH_EraseSector(SectorStart, VoltageRange_3);/*VCC=2.7~3.6V之间!!*/
//				if(status!=FLASH_COMPLETE)
//				{
//					/*发生错误*/
//					break;	
//				}
//			}else 
//			{
//				SectorStart += 8;
//			}
//		} 
	}
	if(status==FLASH_COMPLETE)
	{
		/*写数据*/
		while(WriteAddr<endaddr)
		{
			/*写入数据*/
			if(FLASH_ProgramWord(WriteAddr,*pBuffer)!=FLASH_COMPLETE)//
			{ 
				/*写入异常*/
				break;	
			}
			WriteAddr+=4;
			pBuffer++;
		} 
	}
	/*FLASH擦除结束,开启数据缓存*/
	FLASH_DataCacheCmd(ENABLE);	
	/*FLASH上锁*/
	FLASH_Lock();
}
	/************************************************
	函数名称 ： GetSector
	功    能 ： 
	参    数 ： Address
	返 回 值 ： sector
	备	  注 ： 
	*************************************************/
uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
  {
    sector = FLASH_Sector_11;  
  }

  return sector;
}
