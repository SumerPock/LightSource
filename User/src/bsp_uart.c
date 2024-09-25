#include "bsp.h"

#define START_SYMBOL    0xEA
#define END_SYMBOL      0xED
#define CLARITY_SYMBOL  0xEB

void HandleEndSymbol(void);
void HandleStartSymbol(void);
void HandleValidData(uint8_t inputCharacter);

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
void UART_RemoteDecode(uint8_t inputCharacter)
{
    if (RemoteDataPacket.Start_Flag == 1)
    {
        if (RemoteDataPacket.Clarity_Flag == 1)
        {
            if (inputCharacter == START_SYMBOL || inputCharacter == CLARITY_SYMBOL || inputCharacter == END_SYMBOL)
            {
                UART_Inbuff(inputCharacter);
            }
            else
            {
                RemoteDataPacket.Start_Flag = 0;
                RemoteDataPacket.Clarity_Flag = 0;
                RemoteDataPacket.RX_BUF[0] = 0;
                // 添加错误处理代码
            }
        }
        else if (inputCharacter == CLARITY_SYMBOL)
        {
            RemoteDataPacket.Clarity_Flag = 1;
        }
        else if (inputCharacter == END_SYMBOL)
        {
            HandleEndSymbol();
        }
        else if (inputCharacter == START_SYMBOL)
        {
            HandleStartSymbol();
        }
        else
        {
            HandleValidData(inputCharacter);
        }
    }
    else if (inputCharacter == START_SYMBOL)
    {
        HandleStartSymbol();
    }
    // 最后的 Else，未启动且不是启始符，判定该数据不是有效数据，忽略它不执行任何操作
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
void HandleStartSymbol()
{
    RemoteDataPacket.Start_Flag = 1;
    RemoteDataPacket.RX_BUF[0] = 0;
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
void HandleEndSymbol()
{
    RemoteDataPacket.Start_Flag = 0;
    RemoteDataPacket.Clarity_Flag = 0;
    RemoteDataPacket.End_Flag = 1;
		UART_RemoteCMD_Process(); 				// 数据包接收完毕，插入数据处理代码
    RemoteDataPacket.End_Flag = 0;    // 处理完毕，End标记清零!!!
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
void HandleValidData(uint8_t inputCharacter)
{
    UART_Inbuff(inputCharacter);
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
void HandleClaritySymbol(uint8_t inputCharacter)
{
    RemoteDataPacket.Clarity_Flag = 1;
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
void UART_Inbuff(uint8_t inputCharacter)
{
    if (RemoteDataPacket.RX_BUF[0] > 10)
    {
        RemoteDataPacket.RX_BUF[0] = 0;
        RemoteDataPacket.Start_Flag = 0;
        RemoteDataPacket.Clarity_Flag = 0;
        RemoteDataPacket.End_Flag = 0;
    }
    else
    {
        RemoteDataPacket.RX_BUF[0]++;
        RemoteDataPacket.RX_BUF[RemoteDataPacket.RX_BUF[0]] = inputCharacter;
        RemoteDataPacket.Clarity_Flag = 0;
    }
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
//void SendDataArray(USART_TypeDef* USARTx, unsigned char* dataArray, unsigned int length)
//{
//    for (unsigned int i = 0; i < length; i++)
//    {
//        Dong_Uart_Send(USARTx, dataArray[i]);
//    }
//}

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
//void Dong_Uart_Send(USART_TypeDef* USARTx , unsigned char Data)
//{
//	// 等待发送缓冲区为空
//	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
//	{
//	}

//	// 将数据发送到 USART 数据寄存器
//	USART_SendData(USARTx , Data);         		

//	// 等待发送完成
//	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
//	{
//	}
//}

