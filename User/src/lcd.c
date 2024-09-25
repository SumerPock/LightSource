
#include "lcd_font.h"
#include "bmp.h"
#include "bsp.h"


uint16_t BACK_COLOR = BLUE;   //背景色

/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD_Writ_Bus(uint8_t dat) 
{	
	uint8_t i;			  
	for(i=0;i<8;i++)
	{			  
		LCD_SCLK_Clr;
		if(dat&0x80)
		   LCD_SDIN_Set;
		else 
		   LCD_SDIN_Clr;
		LCD_SCLK_Set;
		dat<<=1;   
	}			
}

/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_DC_Set;//写数据
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
	LCD_DC_Set;//写数据
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_Clr;//写命令
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	LCD_WR_REG(0x2a);//列地址设置
	LCD_WR_DATA(x1);
	LCD_WR_DATA(x2);
	LCD_WR_REG(0x2b);//行地址设置
	LCD_WR_DATA(y1);
	LCD_WR_DATA(y2);
	LCD_WR_REG(0x2c);//储存器写
}

/************************************************
函数名称： LCD_Init
功    能： 初始化LCD屏的SPI接口
参    数： 无
返 回 值： 无
作    者： gaoyao
*************************************************/
void LCD_PortInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC,ENABLE);//使能PORTA、PORTC时钟

	//GPIO初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
//	GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_15);

	//GPIO初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
}

void LCD_Init(void)
{
	LCD_PortInit();
	
	LCD_RES_Clr;
	delay_ms(10);//	delay_ms(200);
	LCD_RES_Set;
	delay_ms(10);//	delay_ms(200);
	LCD_BLK_Set;
	delay_ms(10);//	delay_ms(200);
	
	//************* Start Initial Sequence **********// 
	LCD_WR_REG(0xCF);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0xC1);
	LCD_WR_DATA8(0X30);
	LCD_WR_REG(0xED);
	LCD_WR_DATA8(0x64);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0X12);
	LCD_WR_DATA8(0X81);
	LCD_WR_REG(0xE8);
	LCD_WR_DATA8(0x85);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x79);
	LCD_WR_REG(0xCB);
	LCD_WR_DATA8(0x39);
	LCD_WR_DATA8(0x2C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x34);
	LCD_WR_DATA8(0x02);
	LCD_WR_REG(0xF7);
	LCD_WR_DATA8(0x20);
	LCD_WR_REG(0xEA);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0xC0); //Power control
	LCD_WR_DATA8(0x1D); //VRH[5:0]
	LCD_WR_REG(0xC1); //Power control
	LCD_WR_DATA8(0x12); //SAP[2:0];BT[3:0]
	LCD_WR_REG(0xC5); //VCM control
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x3F);
	LCD_WR_REG(0xC7); //VCM control
	LCD_WR_DATA8(0x92);
	LCD_WR_REG(0x3A); // Memory Access Control
	LCD_WR_DATA8(0x55);
	LCD_WR_REG(0x36); // Memory Access Control
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x08);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC8);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x78);
	else LCD_WR_DATA8(0xA8);
	LCD_WR_REG(0xB1);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x12);
	LCD_WR_REG(0xB6); // Display Function Control
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0xA2);

	LCD_WR_REG(0x44);
	LCD_WR_DATA8(0x02);

	LCD_WR_REG(0xF2); // 3Gamma Function Disable
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0x26); //Gamma curve selected
	LCD_WR_DATA8(0x01);
	LCD_WR_REG(0xE0); //Set Gamma
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x22);
	LCD_WR_DATA8(0x1C);
	LCD_WR_DATA8(0x1B);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x48);
	LCD_WR_DATA8(0xB8);
	LCD_WR_DATA8(0x34);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0XE1); //Set Gamma
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x23);
	LCD_WR_DATA8(0x24);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x38);
	LCD_WR_DATA8(0x47);
	LCD_WR_DATA8(0x4B);
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x06);
	LCD_WR_DATA8(0x30);
	LCD_WR_DATA8(0x38);
	LCD_WR_DATA8(0x0F);
	LCD_WR_REG(0x11); //Exit Sleep
	delay_ms(5);//	delay_ms(120);
	LCD_WR_REG(0x29); //Display on
}

/******************************************************************************
      函数说明：LCD清屏函数
      入口数据：无
      返回值：  无
******************************************************************************/
void LCD_Clear(uint16_t Color)
{
	uint16_t i,j;  	
	LCD_Address_Set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
	 {
	  for (j=0;j<LCD_H;j++)
	   	{
        	LCD_WR_DATA(Color);	 			 
	    }

	  }
}
;
/******************************************************************************
      函数说明：LCD显示汉字
      入口数据：x,y   起始坐标
                index 汉字的序号
                size  字号
      返回值：  无
******************************************************************************/
void LCD_ShowChinese(uint16_t x, uint16_t y, uint8_t index, uint8_t size, uint16_t color, uint16_t backcolor)	
{  
	uint8_t i, j, size1;
	const uint8_t *temp;
	
	x = x+8;
	if(size==16)
	{
		temp = Hzk16;
	}//选择字号
	if(size==32)
	{
		temp = Hzk32;
	}
  LCD_Address_Set(x , y , x + size - 1 , y + size - 1); //设置一个汉字的区域
  size1 = size * size / 8;//一个汉字所占的字节
	temp+= index * size1;//写入的起始位置
	for(j = 0 ; j < size1 ; j++)
	{
		for(i = 0 ; i < 8 ; i++)
		{
		 	if((*temp&(1<<i)) != 0)//从数据的低位开始读
			{
				LCD_WR_DATA(color);//点亮
			}
			else
			{
				LCD_WR_DATA(backcolor);//不点亮
			}
		}
		temp++;
	 }
}


/******************************************************************************
      函数说明：LCD画点
      入口数据：x,y   起始坐标
      返回值：  无
******************************************************************************/
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_Address_Set(x,y,x,y);//设置光标位置 
	LCD_WR_DATA(color);
} 


/******************************************************************************
      函数说明：LCD画一个大的点
      入口数据：x,y   起始坐标
      返回值：  无
******************************************************************************/
void LCD_DrawPoint_big(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_Fill(x-1,y-1,x+1,y+1,color);
} 


/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
      返回值：  无
******************************************************************************/
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{          
	uint16_t i,j; 
	LCD_Address_Set(xsta,ysta,xend,yend);      //设置光标位置 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)LCD_WR_DATA(color);//设置光标位置 	    
	} 					  	    
}


/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
      返回值：  无
******************************************************************************/
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向 
	else if (delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平线 
	else {incy=-1;delta_y=-delta_x;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		LCD_DrawPoint(uRow,uCol,color);//画点
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}


/******************************************************************************
      函数说明：画矩形
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
      返回值：  无
******************************************************************************/
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}

/******************************************************************************
      函数说明：画圆
      入口数据：x0,y0   圆心坐标
                r       半径
      返回值：  无
******************************************************************************/
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color)
{
	int a,b;
	//int di;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//判断要画的点是否过远
		{
			b--;
		}
	}
}


/******************************************************************************
      函数说明：显示字符
      入口数据：x,y    起点坐标
                num    要显示的字符
                mode   1叠加方式  0非叠加方式
      返回值：  无
******************************************************************************/
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode,uint16_t color)
{
    uint8_t temp;
    uint8_t pos,t;
	  uint16_t x0=x;     
    if(x>LCD_W-16||y>LCD_H-16)return;	    //设置窗口		   
	num=num-' ';//得到偏移后的值
	LCD_Address_Set(x,y,x+8-1,y+16-1);      //设置光标位置 
	if(!mode) //非叠加方式
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(uint16_t)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_WR_DATA(color);
				    else LCD_WR_DATA(BACK_COLOR);
				    temp>>=1; 
				    x++;
		    }
			x=x0;
			y++;
		}	
	}else//叠加方式
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(uint16_t)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos,color);//画一个点     
		        temp>>=1; 
		    }
		}
	}   	   	 	  
}


/******************************************************************************
      函数说明：显示16*32半角字符
      入口数据：x,y    起点坐标
                num    要显示的字符
								color/backcolor  颜色/背景色
      返回值：  无
******************************************************************************/
void LCD_ShowBigChar(uint16_t x, uint16_t y, uint8_t index, uint16_t color, uint16_t backcolor)
{
	uint8_t i, j, size1;
	const uint8_t *temp;
	
	x = x+8;
	temp = Half_zk3216;//选择字号
  LCD_Address_Set(x,y,x+16-1,y+32-1); //设置一个汉字的区域
  size1=64;//一个汉字所占的字节
	index = index - ' ';
	temp+=index*size1;//写入的起始位置
	for(j=0;j<size1;j++)
	{
		for(i=0;i<8;i++)
		{
		 	if((*temp&(1<<i))!=0)//从数据的低位开始读
			{
				LCD_WR_DATA(color);//点亮
			}
			else
			{
				LCD_WR_DATA(backcolor);//不点亮
			}
		}
		temp++;
	 }
}

/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y    起点坐标
                *p     字符串起始地址
								color/backcolor  颜色/背景色
      返回值：  无
******************************************************************************/
void LCD_ShowBigString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t color, uint16_t backcolor)
{         
    while(*p!='\0')
    {       
        if(x>LCD_W-16){x=0;y+=32;}
        if(y>LCD_H-32){y=x=0;}
        LCD_ShowBigChar(x, y, *p, color, backcolor);
        x+=16;
        p++;
    }  
}

// 检索汉字字符
uint8_t Search_ch32x32(const uint8_t *p)
{
	uint8_t j=0, Index=255;
	while(Font32x32_Index[j] > 127)
	{
		if(Font32x32_Index[j] == *p)
		{
			if(Font32x32_Index[j+1] == *(p+1))
			{
				Index = j>>1;
				break;
			}
		}
		j+=2;
	}
	return Index;
}


// 输出全角汉字和半角字符的混合字符串
void LCD_Print(uint16_t x, uint16_t y, char *p, uint16_t color, uint16_t backcolor)
{
	uint8_t char_Index;
	uint8_t single_ch[3];
	
	while(*p != '\0')
	{
		if(*p > 127)
		{	// 是全角汉字
			single_ch[0] = *p;
			single_ch[1] = *(p+1);
			single_ch[2] = '\0';
			if(x>LCD_W-40){x=0;y+=32;}
      if(y>LCD_H-40){y=x=0;}
			char_Index = Search_ch32x32(single_ch);// 搜索全角汉字在字库中的位置
			LCD_ShowChinese(x, y, char_Index, 32, color, backcolor);   //
			x += 32;
			p += 2;
		}
		else
		{	// 是半角字符
			if(x>LCD_W-24){x=0;y+=32;}
      if(y>LCD_H-40){y=x=0;}
			single_ch[0] = *p;
			single_ch[1] = '\0';
			LCD_ShowBigString(x, y, single_ch, color, backcolor);
			x += 16;
			p += 1;
		}
	}
}

/* 高亮显示数字的某个位
// 输入参数较多，一共10个，调用的时候需要特别注意
	X_Pos、Y_Pos：	显示的坐标位置
	ShowNumber：		需要显示的数字
	Show_ENOBs：		显示的位数（不足的前面用空格补齐）
	RadixPointBit：	小数点后的位数（例如：数字5000，位数3，则显示5.000）
	HighlightBit：	需要高亮显示的倒数位数（0代表都不高亮，1代表最低位高亮，注意剔除小数点）
	color、backcolor、hcolor、hbcolor：正常文字颜色，背景色，高亮颜色、高亮的背景色
*/
void Number_Highlight_Show(uint16_t X_Pos, uint16_t Y_Pos, uint32_t ShowNumber, uint8_t Show_ENOBs, uint8_t RadixPointBit, uint8_t HighlightBit, 
													 uint16_t color, uint16_t backcolor, uint16_t hcolor, uint16_t hbcolor)
{
	char str[10], char_temp[2];
	uint8_t i, str_lenght;
	if(Show_ENOBs > 8) Show_ENOBs = 8;	// 数据保护措施
	if(RadixPointBit > Show_ENOBs) RadixPointBit = Show_ENOBs;// 数据保护措施
	if(HighlightBit > Show_ENOBs) HighlightBit = 0;// 数据保护措施
	switch(Show_ENOBs)
	{	// 根据需要保留的长度，截取末位，因为用幂运算要牵扯浮点运算，速度会慢些，这里用switch...case处理
		case 3:
			ShowNumber = ShowNumber % 1000;
			break;
		case 4:
			ShowNumber = ShowNumber % 10000;
			break;
		case 5:
			ShowNumber = ShowNumber % 100000;
			break;
		case 6:
			ShowNumber = ShowNumber % 1000000;
			break;
		case 7:
			ShowNumber = ShowNumber % 10000000;
			break;
		default:
			ShowNumber = 99999999;
			break;		
	}
	
	snprintf(str, Show_ENOBs+1, "%d", ShowNumber);	// 数字转字符串
	str_lenght = (uint8_t)strlen(str);	//获取字符串长度，用于后面的循环处理
	if(str_lenght < Show_ENOBs)	// 如果数据长度小于要求长度，为保证数据对齐前面补空格
	{
		for(i=0; i<str_lenght+1; i++)	//将数据后移，+1里面存放的是/0也需要移动
		{	//循环后移
			str[Show_ENOBs-i] = str[str_lenght-i];
		}
		for(i=0; i<(Show_ENOBs-str_lenght); i++)
		{	//前面补空格
			str[i] = ' ';//0x20;
		}
	}

	if(RadixPointBit > 0)	// 如果小数点位数大于0，则后移加点
	{
		str_lenght = (uint8_t)strlen(str);	//再次更新字符串长度			
		for(i=0; i<(RadixPointBit+1); i++)
		{	//小数点后面的字符依次后移一位
			str[str_lenght-i+1] = str[str_lenght-i];
		}
		str[str_lenght-RadixPointBit] = '.';//0x2E;
		if(str[str_lenght-RadixPointBit-1] == ' ') str[str_lenght-RadixPointBit-1] = '0';	//如果小数点前一位是空格，则替换为0
	}
	
	
	
	//以下代码逐个输出字符，根据需要将相应字符高亮
	i=0;
	char_temp[1] = '\0';
	Show_ENOBs++;	//因为增加了一个小数点，相应总长度变化
	if(HighlightBit > RadixPointBit) HighlightBit++;	// 如果高亮的位数大于小数点后的位数，则高亮位数+1，因为小数点占用了1位
	while(str[i]!='\0')
  {
		char_temp[0] = str[i];
		if(HighlightBit == (Show_ENOBs - i))
			LCD_Print(X_Pos+16*i, Y_Pos, char_temp, hcolor, hbcolor);
		else
			LCD_Print(X_Pos+16*i, Y_Pos, char_temp, color, backcolor);
		i++;
	}
}


/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y    起点坐标
                *p     字符串起始地址
      返回值：  无
******************************************************************************/
void LCD_ShowString(uint16_t x, uint16_t y, const char *p, uint16_t color)
{         
    while(*p!='\0')
    {       
        if(x>LCD_W-16){x=0;y+=16;}
        if(y>LCD_H-16){y=x=0;LCD_Clear(RED);}
        LCD_ShowChar(x,y,*p,0,color);
        x+=8;
        p++;
    }  
}


/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}


/******************************************************************************
      函数说明：显示数字
      入口数据：x,y    起点坐标
                num    要显示的数字
                len    要显示的数字个数
      返回值：  无
******************************************************************************/
void LCD_ShowNum(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t color)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+8*t,y,' ',0,color);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+8*t,y,temp+48,0,color); 
	}
} 


/******************************************************************************
      函数说明：显示小数
      入口数据：x,y    起点坐标
                num    要显示的小数
                len    要显示的数字个数
      返回值：  无
******************************************************************************/
void LCD_ShowNum1(uint16_t x , uint16_t y , float num , uint8_t len , uint16_t color)
{         	
	uint8_t t = 0 , temp = 0;
	uint16_t num1 = 0;
	num1 = num * 100;
	for(t = 0 ; t < len ; t++)
	{
		temp = (num1 / mypow(10 , len - t - 1)) % 10;
		if(t == (len - 2))
		{
			LCD_ShowChar(x + 8 * (len - 2) , y , '.' , 0 , color);
			t++;
			len += 1;
		}
	 	LCD_ShowChar(x + 8 * t , y , temp + 48 , 0 , color);
	}
}


/******************************************************************************
      函数说明：显示40x40图片
      入口数据：x,y    起点坐标
      返回值：  无
******************************************************************************/
void LCD_ShowPicture(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	int i;
	  LCD_Address_Set(x1,y1,x2,y2);
		for(i=0;i<1600;i++)
	  { 	
			LCD_WR_DATA8(image[i*2+1]);
			LCD_WR_DATA8(image[i*2]);
	  }			
}



/************************************************
函数名称： LCD_Write_Byte
功    能： 向LCD写入8bit数据
参    数： TxData，向LCD的SPI总线写入的数据
返 回 值： 从SPI总线接收到的数据
作    者： gaoyao
*************************************************/
uint16_t LCD_Write_Byte(uint16_t TxData)
{
	LCD_RES_Toogle;
	while (SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE) == RESET);  //等待发送区空
	SPI_SendData(SPI1, TxData);                              //通过外设SPIx发送一个byte  数据
	while (SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE) == RESET); //等待接收完一个byte
	return SPI_ReceiveData(SPI1);                            //返回通过SPIx最近接收的数据
}

