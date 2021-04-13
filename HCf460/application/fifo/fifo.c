/*
 * fifo.c
 *
 *  Created on: 2020年11月27日
 *      Author: cccc
 */
#include "fifo.h"

FIFO_DataType FIFO_DataBuffer[FIFO_DATA_NUM][FIFO_DATA_SIZE];

FIFO_Struct FIFO_Data[FIFO_DATA_NUM] =
{
		{
				.BufferSize = FIFO_DATA_SIZE,
				.Buffer = FIFO_DataBuffer[0],
		},
		{
				.BufferSize = FIFO_DATA_SIZE,
				.Buffer = FIFO_DataBuffer[1],
		},		
};


/**
 * @brief 初始化FIFO队列数据：清空数据、复位读写指针
 */
void FIFO_Init(FIFO_Struct *FIFO_Data)
{
	FIFO_Data->DataCount = 0;
	FIFO_Data->Read_P = FIFO_Data->Buffer;
	FIFO_Data->Write_P = FIFO_Data->Buffer;
}


/**
 * @brief 将数据写入FIFO队列，当数据超出缓冲区的大小时，停止写入数据
 * @param FIFO_Data: 需要操作的FIFO队列数据指针
 * @param Data: 需要写入的数据指针
 * @param length: 需要写入的数据长度
 */
void FIFO_WriteData(FIFO_Struct *FIFO_Data,FIFO_DataType *Data, uint32_t length)
{
	//将数据依次写入到队列缓冲区
	for(uint32_t index = 0; index < length; index++)
	{
		//如果计数器大于等于缓冲容量
		if(FIFO_Data->DataCount > FIFO_Data->BufferSize -1)
		{
			//退出写数据
			return;
		}

		//写入一个数据
		*FIFO_Data->Write_P = *Data;
		//写指针移动一位
		FIFO_Data->Write_P++;
		//数据缓存指针移动一位
		Data++;
		//计数器自增1
		FIFO_Data->DataCount++;

		//如果写指针已经到达缓冲区边界
		if(FIFO_Data->Write_P >= FIFO_Data->Buffer + FIFO_Data->BufferSize)
		{
			//使写指针回到缓冲区起点
			FIFO_Data->Write_P = FIFO_Data->Buffer;
		}
	}
}


/**
 * @brief 向FIFO队列中写入一个数据
 * @param FIFO_Data: 需要操作的FIFO队列数据指针
 * @param Data: 要写入的数据
 */
void FIFO_WriteOneData(FIFO_Struct *FIFO_Data,FIFO_DataType Data)
{
	FIFO_WriteData(FIFO_Data, &Data, 1);
}


/**
 * @brief 将数据从FIFO队列中读出，当缓冲区为空时，停止读出数据
 * @param FIFO_Data: 需要操作的FIFO队列数据指针
 * @param Data: 用来存放读出数据的指针
 * @param length: 需要读出的数据长度
 */
void FIFO_ReadData(FIFO_Struct *FIFO_Data,FIFO_DataType *Data, uint32_t length)
{
	//将缓冲区数据依次读出到Data数组中
	for(uint32_t index = 0; index < length; index++)
	{
		//缓冲区数据计数器为0时
		if(FIFO_Data->DataCount == 0)
		{
			//退出读数据
			return;
		}

		//如果数据计数器大于写指针减去缓冲区起始位置（说明写入的数据已经到达过缓冲区边界）
		if(FIFO_Data->Write_P - FIFO_Data->Buffer < FIFO_Data->DataCount)
		{
			//确定数据初始位置，并传递给读指针
			FIFO_Data->Read_P = FIFO_Data->BufferSize - FIFO_Data->DataCount + FIFO_Data->Write_P;
		}
		//写入的数据还未到达过缓冲区边界
		else
		{
			//确定数据初始位置，并传递给读指针
			FIFO_Data->Read_P = FIFO_Data->Write_P - FIFO_Data->DataCount;
		}

		//读出一个数据
		*Data = *FIFO_Data->Read_P;
		//读指针移动一位
		FIFO_Data->Read_P++;
		//数据缓存指针移动一位
		Data++;
		//计数器自减1
		FIFO_Data->DataCount--;

		//如果读指针已经到达缓冲区边界
		if(FIFO_Data->Read_P >= FIFO_Data->Buffer + FIFO_Data->BufferSize)
		{
			//使读指针回到缓冲区起点
			FIFO_Data->Read_P = FIFO_Data->Buffer;
		}
	}
}


/**
 * @brief 从FIFO队列中读出一个数据
 * @param FIFO_Data: 需要操作的FIFO队列数据指针
 * @retval 读出的数据
 */
FIFO_DataType FIFO_ReadOneData(FIFO_Struct *FIFO_Data)
{
	FIFO_DataType tempData;

	FIFO_ReadData(FIFO_Data, &tempData, 1);

	return tempData;
}


/**
 * @brief 判断队列数据是否已满
 * @param FIFO_Data: 需要操作的FIFO队列数据指针
 * @retval 指示队列是否已满
 * 		@arg 0: 队列未满
 * 		@arg 1: 队列已满
 */
uint8_t FIFO_IsDataFull(FIFO_Struct *FIFO_Data)
{
	if(FIFO_Data->DataCount >= FIFO_Data->BufferSize)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/**
 * @brief 判断队列数据是否为空
 * @param FIFO_Data: 需要操作的FIFO队列数据指针
 * @retval 指示队列是否为空
 * 		@arg 0: 队列不空
 * 		@arg 1: 队列为空
 */
uint8_t FIFO_IsDataEmpty(FIFO_Struct *FIFO_Data)
{
	if(FIFO_Data->DataCount == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/**
 * @brief 获取队列数据的数量
 * @param FIFO_Data: 需要操作的FIFO队列数据指针
 * @retval 队列数据的数量
 */
uint32_t FIFO_GetDataCount(FIFO_Struct *FIFO_Data)
{
	return FIFO_Data->DataCount;
}


