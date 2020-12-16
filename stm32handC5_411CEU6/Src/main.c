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
#include "hamming.h"
#include "auto_trip_50hz.h"

#define  FFT_LENGTH		2048		//FFT长度

extern u8 change_flag;

u16 num ;
float  adc_data = 0;
extern float in_data[2048];  
extern const float hamming_TAB1[2048];

float fft_inputbuf[FFT_LENGTH*2], fft_outputbuf[FFT_LENGTH];	//FFT输入数组//FFT输出数组




//u8 status=0,Key=0,Key1=0;
//u16 t1[60][90]={0};
//u16 t[256]= {0} ;
//u16 t2[60] ={0};


void measure_fft(void)
{
			arm_cfft_radix2_instance_f32 scfft;
			u32 temp_val=0,temp=0,fft_value=0;
			u16 i,b,c,t,adc_value=0;
	
	//			 for(num=0;num<2560;num++)
	//			 {
	//							adc_data = in_data[num]; 
	//							printf("%d ",adc_data);	
	//			 }
	//			 printf("END \r\n");
	//    for(t=0;t<2048;t++)
	//		{
	//			adc_data = hamming_TAB1[t];
	//			printf("%f  ",adc_data);
	//		}
	//		 printf("END \r\n");
			for(t=0;t<2048;t++)
			{
				adc_value = in_data[t]; 
				temp_val+=adc_value;
		
			}	
			temp = temp_val/2048;
	//    printf("temp=%d \r\n", temp);
			for(t=0;t<2048;t++)
			{
				in_data[t] = (in_data[t]-temp) ;
				in_data[t] = in_data[t] *hamming_TAB1[t];
	//			adc_data = in_data[t];
	//			printf("%.2f  ",adc_data);	
			}		
	//	  printf("END \r\n");
			arm_cfft_radix2_init_f32(&scfft,2048,0,1);//初始化scfft结构体，设定FFT相关参数


			for(b=0;b<2048;b++)
			{
					
					fft_inputbuf[2*b] = in_data[b];
					fft_inputbuf[2*b+1]=0;//虚部全部为0
					//  printf("%d \r",AD_FFT[b] );
			}

			
			arm_cfft_radix2_f32(&scfft,fft_inputbuf);	//FFT计算（基2）
			arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,2048);	
					
//			for(i=0;i<2048;i++)
//			{
//					fft_value = fft_outputbuf[i];
//				
//					printf("%d ",fft_value);					
//			}
//			printf("END \r\n");
			for(c=1;c<2048;c++)
			{
				  fft_value = fft_outputbuf[i];
				  fft_outputbuf[i] = fft_value*2;
			}
			


}

const short src[1024]=
{
-11,-8,-11,-4,-4,-4,-2,3,0,2,3,0,3,-2,-3,-5,-4,0,4,3,-1,4,-2,-2,-2,0,1,9,13,4,-3,-2,-1,-1,-3,-6,-6,-5,-10,-11,-11,-6,-4,-10,-9,-10,-10,-5,-12,-9,-7,-9,-12,-5,-15,-12,-12,-15,-15,-11,-12,-3,-11,-13,-22,-11,-17,-19,-13,-17,-15,-16,-18,-16,-18,-14,-18,-16,-17,-13,-14,-8,-8,-17,-18,-14,-15,-16,-16,-12,-6,-6,-5,-11,-12,-8,-6,-15,-14,-9,-9,-11,-10,-8,-10,-10,-13,-13,-16,-14,-11,-10,-8,-4,-15,-16,-16,-13,-9,-11,-7,-11,-9,-11,-15,-15,-13,-8,-9,-3,-6,-6,-6,-7,-10,-10,-9,-14,-6,-9,-3,-3,-3,-3,-4,-6,-8,-4,-3,-2,-4,-4,-1,0,-3,-4,-3,-4,-3,-4,-2,-2,1,-3,-4,-4,-6,-8,-7,-8,-4,-1,3,4,3,-1,4,6,4,7,2,4,6,4,9,14,3,-1,5,4,1,7,10,8,9,15,13,9,5,-1,4,6,9,5,2,3,-1,5,1,1,-2,-2,1,-2,5,2,7,1,-1,-1,-1,-3,-3,-4,-8,-3,-2,-5,-7,-10,-12,-8,-7,-4,-3,-3,-4,-4,-8,-5,-13,-8,-10,-8,-11,-10,-8,-10,-13,-10,-9,-14,-10,-4,-7,-3,-4,-3,-6,-10,-4,-4,-4,-1,-3,-4,-7,-4,-3,-9,-9,-7,-6,-4,-6,-3,-7,-3,-4,-4,-3,0,-4,1,1,-4,-3,-3,-3,-2,-1,-2,-3,0,2,3,5,9,11,8,9,5,1,4,6,9,7,10,13,10,13,11,14,16,27,32,31,32,32,26,33,16,16,33,32,37,37,26,23,22,17,24,12,8,9,17,8,13,13,7,7,6,7,1,2,4,5,1,4,3,-1,0,1,-3,-3,0,-3,-3,-3,-4,-7,-6,-4,-7,-2,0,1,2,0,-3,-1,-2,-4,-5,-8,-4,-4,-3,-11,-7,-5,-12,-11,-10,-13,-15,-13,-12,-14,-9,-11,-13,-16,-16,-16,-11,-7,-7,-7,-4,-6,-5,-16,-12,-11,-6,-6,-5,-8,-5,-9,-5,-6,-4,-9,-4,-3,0,-3,-1,2,6,3,-2,1,2,2,5,3,11,13,13,8,5,6,4,7,14,10,21,25,18,11,9,14,13,10,11,7,12,6,10,11,6,16,30,14,16,12,21,23,23,9,7,13,13,11,13,9,7,3,6,8,12,12,8,18,25,37,19,32,26,25,23,10,9,4,3,9,6,7,8,8,5,1,-1,-1,0,4,3,5,6,6,-2,-3,-1,3,3,3,1,-1,-3,-3,-4,-6,-4,-8,-6,-4,-3,-5,-3,-4,-3,-4,-8,-4,-4,-3,-3,-1,3,2,6,1,-2,-4,-1,-2,1,1,2,-1,0,-4,1,-3,-7,-10,-3,-1,0,-3,0,-2,-3,-5,-9,-7,-3,-3,-3,4,6,4,6,3,-1,3,2,1,0,1,5,4,1,2,-1,5,6,14,14,30,17,18,14,12,18,15,15,16,23,26,38,37,27,37,40,41,37,39,44,48,46,42,48,34,35,35,23,31,34,36,33,36,25,25,31,23,16,13,14,15,10,12,10,8,7,6,5,0,2,2,-1,4,2,0,2,-1,1,-3,-3,-4,0,6,13,3,6,10,5,6,2,4,1,1,0,-3,3,-2,2,-3,-6,-4,-4,-6,-5,-9,-11,-8,-7,-6,-7,-3,-3,-1,-1,-2,6,4,2,-2,1,10,7,8,13,20,9,4,8,8,6,8,11,8,4,6,5,3,-4,-3,0,-3,0,1,2,6,6,5,6,15,11,7,23,11,15,33,32,33,8,14,22,10,15,6,7,6,5,1,-3,-3,-1,1,4,5,3,6,26,30,26,12,36,35,26,34,39,39,41,37,30,12,4,9,4,6,9,4,12,10,10,7,13,12,12,31,27,31,24,24,38,32,33,24,14,9,10,8,2,1,-4,-3,-5,-10,-11,-12,-11,-14,-10,-9,-11,-5,-11,-9,-6,-9,-4,-4,-3,-4,-3,-6,-7,-13,-6,-5,-9,-7,-7,-7,-10,-8,-10,-11,-9,-12,-15,-11,-15,-15,-21,-18,-21,-14,-16,-22,-20,-13,-10,-6,-4,-3,-4,-7,-7,-15,-14,-7,-6,-9,-12,-10,-15,-12,-15,-16,-16,-13,-14,-5,-6,-10,-6,-6,-3,-1,-4,-3,-4,-3,-9,-9,-11,-15,-16,-18,-17,-15,-14,-13,-9,-9,-6,-10,-11,-9,-8,-10,-4,-3,-3,4,7,1,-1,8,4,6,5,7,7,8,11,5,11,12,11,8,15,5,5,6,5,5,4,0,-2,5,6,9,8,7,5,4,5,-1,9,3,7,-1,3,-3,-6,-7,-9,-12,-12,-11,-9,-6,-11,-12,-10,-8,-13,-6,-16,-13,-12,-8,-8,-12,-13,-16,-13,-21,-23,-39,-32,-28,-25,-27,-25,-25,-25,-20,-19,-27,-23,-24,-15,-10,-16,-21,-18,-21,-11,-16,-17,-17,-15,-13,-15,-15,-19,-20,-17,-16,-22,-25,-34,-28,-26,-28,-16,-18,-11,-14,-11,-13,-14,-12,-12,-11,-7,-6,-12,-10,-6,-8,-11,-4,-4,-5,1,4,1,-2,-4,-3,-3,-3,-2,0,-3,-4,-4,-9,-12,-8,-13,-8,-9,-10,-8
};

//const float peaks[3]={100, 100, 100};

short res[1024];

void test1(void);

void test1(void)
{
	int i;
	
//	for (i=0;i<2560;i++)
//	{
//		printf("%f\r\n", src[i]);
//	}
	
//	auto_trip_50hz(src, 1024, 0.99, peaks, res);
	
	auto_trip_50hz3(src, 1024, 50, res);
	
	for (i=0;i<1024;i++)
	{
			printf("%d - %d\r\n", i, res[i]);
	}
}

int main(void)
{
			u16 num = 0;
			//test1();

			HAL_Init();
			Stm32_Clock_Init(200,25,2,7);
			
			delay_init(100);               	//初始化延时函数
			MY_ADC_Init();
			LED_Init();		//初始化LED	 
			
			
			TIM3_Init(40-1,5000-1);        // 100us  3.9ms
			
			
			TIM10_PWM_Init(500-1,100-1);    	//84M/84=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ  500-1
			TIM_SetTIM10Compare1(0); 	
			
			TIM4_PWM_Init(500-1,50-1);    	//84M/84=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ  500-1
			TIM_SetTIM4Compare1(0); 	
			
			MX_GPIO_Init();
			MX_CRC_Init();
			MX_USART1_UART_Init();
		 
//			printf("test\r\n");
			
			while (1)
			{
					num++;
					//printf("test\r\n");
					
				
				  if(change_flag==1)
					{
						 measure_fft();
						 change_flag=0;
					}
					
					delay_ms(100);
					
					test1();
//					 for(num=0;num<2560;num++)
//					 {
//							adc_data = in_data[num]; 
//							printf("%d ",adc_data);	
//					 }
//					 
//					 printf("END \r\n");
					
			}
  
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


