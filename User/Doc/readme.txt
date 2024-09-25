步进电机驱动器及缓冲器使能
1. 	EN5		PB0
2.	EN6		PB1

LED0:	PE6
LED1:	PC13(违反Datasheet里的设计准则要求，舍弃不用)

ARM USART1_TX:	PA9		-->		CPLD: Pin13
ARM USART1_RX:	PA10	<-- 	CPLD: Pin12	

ARM USART3_TX:	PB10	-->		CPLD: Pin65
ARM USART3_RX:	PB11	<-- 	CPLD: Pin66	

编码器信号 	BM1 --> CPLD:Pin57
			BM2 --> CPLD:Pin58
			BM3 --> CPLD:Pin59
激光输出J4	AN1	-->	CPLD:Pin37
红光输出J9	AN2	-->	CPLD:Pin45
取消按钮J10	AN3 --> CPLD:Pin47

编码器信号 	BM1 --> CPLD:Pin57
			BM2 --> CPLD:Pin58
			BM3 --> CPLD:Pin59
 
通讯协议
1.基本应答数据包
Byte_1: ID: 0x80
Byte_2: 产品工作状态，Bit0:0/1 激光器关闭/开启, Bit1:0/1 红光关闭/开启
Byte_3: 激光器温度，1LSB=0.5℃，0x0 = -50℃
Byte_4: 错误代码

通常用于代替itoa的备选方案是sprintf / snprintf。这些是stdio.h的一部分。

伪随机编码存储格式
12Byte名称 + 2Byte基频 + 2Byte长度 + 数组[1016*2Byte] = 2048byte = 2Kbytes