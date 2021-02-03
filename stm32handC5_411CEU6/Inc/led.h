#ifndef _LED_H
#define _LED_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/9/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
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
