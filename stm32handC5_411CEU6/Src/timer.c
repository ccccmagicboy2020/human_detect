#include "timer.h"
#include "led.h"
#include "adc.h"
#include "stdio.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ���ж���������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/4/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


TIM_HandleTypeDef TIM3_Handler;      //��ʱ����� 

TIM_HandleTypeDef TIM4_Handler;      	//��ʱ����� 
TIM_OC_InitTypeDef TIM4_CH2Handler;	//��ʱ��14ͨ��1���

TIM_HandleTypeDef TIM10_Handler;      	//��ʱ����� 
TIM_OC_InitTypeDef TIM10_CH1Handler;	//��ʱ��14ͨ��1���

u16 Timer_Count = 0;//�������


s16  AD_Value[2560] = {0};
u16 num ;

extern void measure_fft(void);

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
void TIM3_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;                          //ͨ�ö�ʱ��3
    TIM3_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM3_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM3_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM3_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE   
}

//TIM14 PWM���ֳ�ʼ�� 
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
void TIM4_PWM_Init(u16 arr,u16 psc)
{  
    TIM4_Handler.Instance=TIM4;          	//��ʱ��14
    TIM4_Handler.Init.Prescaler=psc;       //��ʱ����Ƶ
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//���ϼ���ģʽ
    TIM4_Handler.Init.Period=arr;          //�Զ���װ��ֵ
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM4_Handler);       //��ʼ��PWM
    
    TIM4_CH2Handler.OCMode=TIM_OCMODE_PWM1; //ģʽѡ��PWM1
    TIM4_CH2Handler.Pulse=arr/2;            //���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
	
    TIM4_CH2Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //����Ƚϼ���Ϊ�� 
	
    HAL_TIM_PWM_ConfigChannel(&TIM4_Handler,&TIM4_CH2Handler,TIM_CHANNEL_2);//����TIM14ͨ��1
	
    HAL_TIM_PWM_Start(&TIM4_Handler,TIM_CHANNEL_2);//����PWMͨ��1
}

void TIM10_PWM_Init(u16 arr,u16 psc)
{  
    TIM10_Handler.Instance=TIM10;          	//��ʱ��14
    TIM10_Handler.Init.Prescaler=psc;       //��ʱ����Ƶ
    TIM10_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//���ϼ���ģʽ
    TIM10_Handler.Init.Period=arr;          //�Զ���װ��ֵ
    TIM10_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM10_Handler);       //��ʼ��PWM
    
    TIM10_CH1Handler.OCMode=TIM_OCMODE_PWM1; //ģʽѡ��PWM1
    TIM10_CH1Handler.Pulse=arr/2;            //���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
    TIM10_CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //����Ƚϼ���Ϊ�� 
    HAL_TIM_PWM_ConfigChannel(&TIM10_Handler,&TIM10_CH1Handler,TIM_CHANNEL_1);//����TIM14ͨ��1
	
    HAL_TIM_PWM_Start(&TIM10_Handler,TIM_CHANNEL_1);//����PWMͨ��1
}




//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //����ITM3�ж�   
	}
}

//��ʱ���ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_TIM_PWM_Init()����
//htim:��ʱ�����
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM4_CLK_ENABLE();			//ʹ�ܶ�ʱ��10
	__HAL_RCC_TIM10_CLK_ENABLE();			//ʹ�ܶ�ʱ��10
	
	
	__HAL_RCC_GPIOB_CLK_ENABLE();			//����GPIOFʱ��
	
	GPIO_Initure.Pin=GPIO_PIN_7;           	//PF6
	
	
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//�����������
	GPIO_Initure.Pull=GPIO_PULLUP;          //����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
	
	GPIO_Initure.Alternate= GPIO_AF2_TIM4;	//PF9����ΪTIM14_CH1
	
	
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	
	GPIO_Initure.Pin=GPIO_PIN_8;           	//PF6
	
	
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//�����������
	GPIO_Initure.Pull=GPIO_PULLUP;          //����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
	
	GPIO_Initure.Alternate= GPIO_AF3_TIM10;	//PF9����ΪTIM14_CH1
	
	
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);	
}



//����TIMͨ��4��ռ�ձ�
//compare:�Ƚ�ֵ
void TIM_SetTIM4Compare1(u32 compare)
{
	TIM4->CCR2=compare; 
}
void TIM_SetTIM10Compare1(u32 compare1)
{
	TIM10->CCR1=compare1; 
}


//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}


//�ص���������ʱ���жϷ���������
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{ 
		u16 adc_data,i;
	  if(htim==(&TIM3_Handler))
    {
					adc_data = Get_Adc(5);
	//				temp_val+=adc_data;
					AD_Value[Timer_Count] = adc_data;
					
	//				printf("%d \r\n",adc_data);
	        	Timer_Count++;

						 if(Timer_Count == 2560)
						 {
								num = 1;
						 }

			
		
			

    }
}
