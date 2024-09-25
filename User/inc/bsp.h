#ifndef __BSP_H
#define __BSP_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gpio.h"
#include "usart.h"
#include "i2c.h"
#include "delay.h"
#include "lcd.h"
#include "ui.h"
#include "delay.h"
#include "flash.h"
#include "fsmc.h"
#include "tim.h"
#include "soft_i2c.h"
#include "soft_i2c_lm75b.h"
#include "ring_buffer.h"
#include "ring_buffer_chapter.h"
#include "misc.h" 
#include "RandomCode.h"

#include "bsp_key.h"
#include "bsp_key_oc.h"
#include "bsp_uart_dma.h"
#include "bsp_uart.h"
#include "bsp_ui_showp_foundation.h"
#include "bsp_mode.h"

#define BUFFER_SIZE 256

/**
 * @brief 结构体描述:用于记录当前激光器的菜单状态
 *
 * 详细描述该结构体的用途、成员和其他相关信息。
 */
typedef struct
{
	unsigned char UIMode;	/**< 成员1的描述 */
}LIGHTSOURCE;

extern LIGHTSOURCE Lightsoutce;



typedef struct 
{
	unsigned char Mode;
	unsigned char temp;
	unsigned char cycle[5];
	unsigned char energy[5];
}RETDATA;
extern RETDATA retdata;

typedef struct
{
	unsigned char  discycle;
	unsigned int discycleshow;
	unsigned char  cbit_point;
	unsigned char  cdecimal_point;

	unsigned char disenergy;
	unsigned char bit_point;
	unsigned char decimal_point;
	unsigned int disenergyshow;
}DISPLAY;

struct Uart_DataPacket
{
    uint8_t Start_Flag;    // 启始符标记
    uint8_t Clarity_Flag;  // 透明符标记
    uint8_t End_Flag;      // 终止符标记
    uint8_t RX_BUF[16];    // 接收缓冲,最大15个字节，第一个空间存放有效数据个数
    uint8_t TX_BUF[16];    // 发送缓冲,最大15个字节，第一个空间存放有效数据个数
};

struct Menu_Index
{
	uint8_t Current_Index;  //当前菜单索引值
	uint8_t Next_Index;   	//下一个预选菜单索引值
	uint8_t Prior_Index;   	//前一个预选菜单索引值
};

extern signed int CycleCodeTime;			
extern signed int Temp_CycleCodeTime;
extern signed short int Temp_dB_Loss;
extern signed short int dB_Loss;

extern uint16_t RandomNumber, RandomBaseFreq, RandomMinValue, RandomMaxValue;

extern bool b_Ext_Sync;

extern unsigned short PauseTime;

extern struct Uart_DataPacket RemoteDataPacket;

extern struct Menu_Index Menu_Indexs;	// 结构体，菜单索引，在ui.c里声明

extern union Union_Byte LaserState;		// 共用体，激光器状态，在usart.c里声明

extern unsigned int u32_FlashBuff[2048];

extern unsigned short u16_RandomData[1024];

/**  **/
extern struct Menu_Index Menu_Indexs;
/**  **/
extern struct Uart_DataPacket RemoteDataPacket;
/**  **/
extern DISPLAY display;


//extern unsigned char ucdata;

extern unsigned short PauseTime;

extern bool b_Random_EN;

extern union Union_Byte LaserControl;	// 共用体，激光编码控制位

extern ring_buffer rb_uart1_rx;

extern ring_buffer rb_uart1_tx;

extern ring_buffer rb_uart3_rx;

extern ring_buffer rb_uart3_tx;

extern signed int CycleCodeTime;	// 周期预设50ms

extern void UART_Answer_LightData(USART_TypeDef* USARTx , RETDATA* rd);

extern unsigned char UART_RemoteCMD_Process(void);

extern void attenuation_number(control_number* ap , struct Uart_DataPacket* rp);


#endif

