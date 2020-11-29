#include "led.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/4/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//初始化PB1为输出.并使能时钟	    
//LED IO初始化
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOF时钟
	
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2; //PF9,10
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);	//BLUE  0--mie 
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);	//RED，
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);//GREEN，
		
}

void Beepone(void)
{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
		delay_ms(50);	
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);	
}

void Beeptwo(void)
{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
    delay_ms(50);	
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
	  delay_ms(50);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
    delay_ms(50);	
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);	
}

void Beeplong(void)
{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
		delay_ms(500);	
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);	
}

void Beeplonglong(void)
{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
    delay_ms(50);	
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
	  delay_ms(50);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
    delay_ms(50);	
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);	
	  delay_ms(50);	
	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
    delay_ms(50);	
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
	
}



