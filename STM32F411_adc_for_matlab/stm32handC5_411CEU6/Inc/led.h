#ifndef _LED_H
#define _LED_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/9/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

#define LED0 PFout(0)   	//LED0
#define LED1 PFout(1)   	//LED1
#define LED2 PFout(6)   	//LED2

void LED_Init(void);
void Beepone(void);
void Beeptwo(void);
void Beeplong(void);
void Beeplonglong(void);
#endif
