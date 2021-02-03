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

//#define LED0 PFout(0)   	//LED0
//#define LED1 PFout(1)   	//LED1
//#define LED2 PFout(6)   	//LED2
#define   KEY0    HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0) 
#define   KEY1    HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1) 
#define   KEY5    HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5) 
#define   KEY6    HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6) 
#define   KEY7    HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7) 
#define   KEY8    HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8) 


void LED_Init(void);
void Beepone(void);
void Beeptwo(void);
void Beeplong(void);
void Beeplonglong(void);

void LED_RED(void);
void LED_GREEN(void);
void LED_BLUE(void);
void LED_RED_TWO(void);
void LED_GREEN_TWO(void);
void LED_BLUE_TWO(void);

void KEY_Init(void);

#endif
