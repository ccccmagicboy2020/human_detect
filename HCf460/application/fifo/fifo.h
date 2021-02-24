/*
 * fifo.h
 *
 *  Created on: 2021年2月22日
 *      Author: cccc
 */

#ifndef SOURCE_ALWHALESLIB_SYSEXTEND_INC_FIFO_H_
#define SOURCE_ALWHALESLIB_SYSEXTEND_INC_FIFO_H_

#include "sys.h"
/**
 * 考虑到FIFO数据成员可能会是不同类型的数据，
 * 因此在此处重命名一个数据类型，
 * 并将此数据类型作为FIFO数据结构库的成员类型。
 * 这么做的好处是可以方便的修改FIFO的成员数据类型。
 */
//typedef short int FIFO_DataType;
//typedef char FIFO_DataType;
typedef short int FIFO_DataType;

typedef struct
{
	uint32_t BufferSize;				//数据缓存容量
	FIFO_DataType *Buffer;				//数据缓存
	FIFO_DataType *Read_P;				//读指针
	FIFO_DataType *Write_P;				//写指针
	uint32_t DataCount;					//剩余数据量
} FIFO_Struct;

#define FIFO_DATA_NUM		2			//两支fifo
#define FIFO_DATA_SIZE		2500		//fifo的大小, 5K

extern FIFO_Struct FIFO_Data[FIFO_DATA_NUM];

void FIFO_Init(FIFO_Struct *FIFO_Data);
void FIFO_WriteData(FIFO_Struct *FIFO_Data,FIFO_DataType *Data, uint32_t length);
void FIFO_WriteOneData(FIFO_Struct *FIFO_Data,FIFO_DataType Data);
void FIFO_ReadData(FIFO_Struct *FIFO_Data,FIFO_DataType *Data, uint32_t length);
FIFO_DataType FIFO_ReadOneData(FIFO_Struct *FIFO_Data);
uint8_t FIFO_IsDataFull(FIFO_Struct *FIFO_Data);
uint8_t FIFO_IsDataEmpty(FIFO_Struct *FIFO_Data);
uint32_t FIFO_GetDataCount(FIFO_Struct *FIFO_Data);

#endif /* SOURCE_ALWHALESLIB_SYSEXTEND_INC_FIFO_H_ */
