/* USER CODE BEGIN Header */


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "sys.h"
#include "crc.h"
#include "usart.h"
#include "gpio.h"
#include "app_x-cube-ai.h"
#include "stdio.h"
#include "adc.h"
#include "timer.h"
#include "math.h" 
#include "arm_math.h" 
#include "delay.h"
#include "led.h"

#define FFT_LENGTH		128 		//FFT长度

extern u16 Timer_Count;//溢出次数

extern s16 AD_Value[1280];   

float fft_inputbuf[FFT_LENGTH*4], fft_outputbuf[FFT_LENGTH*2];	//FFT输入数组//FFT输出数组

s16 in_data[2560]={0};


u8 status=0,Key=0,Key1=0;
u16 t1[60][90]={0};
u16 t[256]= {0} ;
u16 t2[60] ={0};
extern u16 num ;

//void measure_fft(void)
//{
//		arm_cfft_radix2_instance_f32 scfft;
//	 
//		u16 i,a,b,c;
//	  s16 max=0, min = 0,cut;
//	  s16 AD_FFT[256] = {0};
//    float s;		
//	
//		
//		const float hamming_TAB1[128]={0.08,0.080562849,0.082250017,0.085057376,0.088978056,0.094002462,0.100118299,0.1073106,0.115561765,0.124851601,0.135157375,
//		0.146453867,0.158713432,0.171906069,0.185999493,0.200959216,0.216748629,0.233329092,0.25066003,0.268699032,0.287401952,0.306723023,0.326614961,0.347029088,
//		0.367915447,0.389222926,0.410899381,0.432891768,0.455146266,0.477608416,0.500223248,0.522935421,0.545689353,0.568429363,0.591099802,0.61364519,0.636010357,
//		0.65814057,0.679981673,0.701480217,0.722583593,0.743240156,0.763399356,0.78301186,0.802029674,0.820406257,0.838096639,0.855057529,0.87124742,0.886626693,
//		0.901157713,0.914804919	,0.927534914,0.939316547,0.950120985,0.959921789,0.968694973,0.976419069,0.983075175,0.988647001,0.993120913,0.996485962,0.998733914,
//		0.999859266,0.999859266,0.998733914,0.996485962,0.993120913,0.988647001,0.983075175,0.976419069,0.968694973,0.959921789,0.950120985,0.939316547,0.927534914,
//		0.914804919,0.901157713,0.886626693,0.87124742,0.855057529,0.838096639,0.820406257,0.802029674,0.78301186,0.763399356,0.743240156,0.722583593,0.701480217,
//		0.679981673,0.65814057,0.636010357,0.61364519,0.591099802,0.568429363,0.545689353,0.522935421,0.500223248,0.477608416,0.455146266,0.432891768,0.410899381,
//		0.389222926,0.367915447,0.347029088,0.326614961,0.306723023,0.287401952,0.268699032,0.25066003,0.233329092,0.216748629,0.200959216,0.185999493,0.171906069,
//		0.158713432,0.146453867,0.135157375,0.124851601,0.115561765,0.1073106,0.100118299,0.094002462,0.088978056,0.085057376,0.082250017,0.080562849,0.08};
//	
//   
//    
//		arm_cfft_radix2_init_f32(&scfft,256,0,1);//初始化scfft结构体，设定FFT相关参数

//    for(a=0;a<90;a++)
//		{
//				for(b=0;b<128;b++)
//				{
//					AD_FFT[b] = AD_Value[10*a+b]*hamming_TAB1[b];
//					fft_inputbuf[2*b] = AD_FFT[b];
//				  fft_inputbuf[2*b+1]=0;//虚部全部为0
//        //  printf("%d \r",AD_FFT[b] );
//				}
//			//	printf(" \n");
//				for(b=128;b<256;b++)
//				{
//					fft_inputbuf[2*b] =0;
//				  fft_inputbuf[2*b+1]=0;//虚部全部为0
//				}
//				__HAL_TIM_SET_COUNTER(&TIM3_Handler,0);//重设TIM3定时器的计数器值
//			
//        arm_cfft_radix2_f32(&scfft,fft_inputbuf);	//FFT计算（基4）
//      //  time=__HAL_TIM_GET_COUNTER(&TIM3_Handler)+(u32)Timer_Count*500;//计算所用时间
//				//sprintf((char*)buf,"%0.3fms\r\n",time/1000);	
//				arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,256);	
//				
//			
//	

//		
//			
//				
//			
//   
//}


int main(void)
{
			int i = 0;
			HAL_Init();
			Stm32_Clock_Init(200,25,2,7);
			
			delay_init(100);               	//初始化延时函数
			MY_ADC_Init();
			LED_Init();		//初始化LED	 
			
			
			TIM3_Init(10-1,50000-1);        // 1ms
			
			
			TIM10_PWM_Init(500-1,100-1);    	//84M/84=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ  500-1
			TIM_SetTIM10Compare1(0); 	
			
			TIM4_PWM_Init(500-1,50-1);    	//84M/84=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ  500-1
			TIM_SetTIM4Compare1(0); 	
			
			MX_GPIO_Init();
			MX_CRC_Init();
			MX_USART1_UART_Init();
		 
			printf("test\r\n");
			
			while (1)
			{
					 if(num==1)
					 {
							 if(Timer_Count == 2560)
							 {
											for(i=0;i<2560;i++)
											{
													 in_data[i]=AD_Value[i];
											//		 printf("%d  ",in_data[i] );
											}
											Timer_Count = 0;
										
							 }	
					 }
					 if(num!=1)
					 {
							if(Timer_Count == 2560)
							{
								 	for(i=0;i<1280;i++)
									{
											in_data[i] = in_data[i+1280];
									}
									
									for(i=1280;i<2560;i++)
									{ 
										 	in_data[i] =  AD_Value[i-1280];
									}
							}
									
							
					 }
					
			}
  /* USER CODE END 3 */
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


