#include "led.h"
#include "delay.h"



int delay_time  = 0, delay_time_num = 0, respirationfreq_num = 0;
float   offsetmax = 0;
float   offsetmin = 0;

double res_times = 0;
u8 CUT = 0;

void KEY_Init(void)
{
	  GPIO_InitTypeDef GPIO_Initure;    
    __HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIOB时钟
   
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_0|GPIO_PIN_1;            //PB8
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;        //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
//    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
	
	  if(KEY0 == 0)
	  {
		  res_times = 20.5;
	  }
	  else
	  {
	      res_times = 21.5;
	  }
	  
	  
	  if(KEY1 == 0)
	  {
		  CUT = 1;
	  }
	  else
	  {
	      CUT = 0;
	  }
	  
	  if(KEY7 == 0 && KEY8 == 0)
	  {
		   delay_time  = 32, delay_time_num = 4, respirationfreq_num = 0;			
	  }
	  else if(KEY7 == 0 && KEY8 == 1)
	  {
		   delay_time  = 64, delay_time_num = 8, respirationfreq_num = 1;			
	  }
	  else if(KEY7 == 1 && KEY8 == 0)
	  {
		  delay_time  = 192, delay_time_num = 24, respirationfreq_num = 3;			
	  }
	  else if(KEY7 == 1 && KEY8 == 1)
	  {
		  delay_time  = 320, delay_time_num = 40, respirationfreq_num = 5;			
	  }
	  
	  
	  if(KEY5 == 0 && KEY6 == 0)
	  {
		   offsetmax = 0.65;
           offsetmin = 0.6;		  
	  }
	  else if(KEY5 == 0 && KEY6 == 1)
	  {
		   offsetmax = 0.7;
           offsetmin = 0.65;	
	  }
	  else if(KEY5 == 1 && KEY6 == 0)
	  {
		   offsetmax = 0.75;
           offsetmin = 0.7;		
	  }
	  else if(KEY5 == 1 && KEY6 == 1)
	  {
		   offsetmax = 0.8;
           offsetmin = 0.75;			
	  }
	  
}


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
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);//GREEN
	
	
		
}

void LED_RED(void)
{
	 	    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);	//RED
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);	
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
}


void LED_RED_TWO(void)
{
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);	//RED
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);	
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
	  delay_ms(100);
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);	//RED
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);	
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
	  delay_ms(100);
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);	//RED
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);	
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
	 
}

void LED_GREEN(void)
{
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);	
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);	
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);//GREEN
}

void LED_GREEN_TWO(void)
{
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);	
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);	
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);//GREEN
	 delay_ms(100);
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);	
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);	
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);//GREEN
	 delay_ms(100);
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);	
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);	
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);//GREEN
}

void LED_BLUE(void)
{
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);	//BLUE
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);	
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
}

void LED_BLUE_TWO(void)
{
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);	//BLUE
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);	
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
	 delay_ms(100);
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);	//BLUE
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);	
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
	 delay_ms(100);
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);	//BLUE
     HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);	
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
	    
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



