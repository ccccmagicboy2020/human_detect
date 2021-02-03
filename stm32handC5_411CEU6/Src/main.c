/* USER CODE BEGIN Header */


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "sys.h"
#include "crc.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"
#include "adc.h"
#include "timer.h"
#include "math.h" 
#include "arm_math.h" 
#include "delay.h"
#include "led.h"
#include "hamming.h"
#include "matlab.h"
#include "remove_pf.h"
#include "cfar_ca_emxAPI.h"
#include "time_detection.h"
#include "freq_detection.h"

//#define  FFT_LENGTH		2048		//FFT长度

#define	    N           	300        //CFAR窗口大小
#define		pro_N      	 	200         //CFAR保护单元大小
#define		PAD      		0.00000001 //虚警概率
  
#define   rr_threshold  0.6    //呼吸频率截取范围
#define	  secnum   		16

extern u8 change_flag;
extern u16  AD_Value[512];
extern const float hamming_TAB2[4096];
extern int delay_time  , delay_time_num , respirationfreq_num ;

extern float   offsetmax;     //门限偏置
extern float   offsetmin; 
extern double res_times;
extern u8 CUT;
/*Fast detection variable declaration*/

FIFO_DataType Fast_detection_data[16384] = {0};
u8 quick_detection_result = 0,transformation = 0,ad_accum = 0,red = 1,blue = 1;
u16  ad = 0,a = 0,b = 0, c =0, d =0, e =0, i = 0,k = 0,f = 0;

/*Macro motion and micro variable declaration*/

//FIFO_DataType  motion_data[4096] = {0}; 
u16  AD_256Value[256] = {0};
u16  AD_Value1[2048];
/* Delay preset */

//int delay_time  = 64, delay_time_num = 8, respirationfreq_num = 1;


//int delay_time_num =(int)(delay_time* 2/ secnum) ;
//int respirationfreq_num =(int)(delay_time* 2/ secnum*0.125);

	
					
void measure_fft(u16 AD[512])
{
    u8 quick_detection_result = 0,quick_detection_flag = 0,state_transition_flag = 0;
	u8  time_detection_result = 0, fre_detection_result = 0, Bigmotion_detection_flag = 0, Fretting_detection_result = 0;
	
	u32 adc_num = 0,adc_temp = 0,adc_average= 0, adc_num1 = 0,adc_temp1 = 0,adc_average1= 0;
	int respirationfreq_vote1[2] = {0};
	float offset = 0.35;
	
	printf("delay_time-delay_time_num-respirationfreq_num:%d %d %d \r\n", delay_time,delay_time_num,respirationfreq_num);				
	if(transformation == 0)
	{
		for(k=0;k<15872;k++)   //滑窗
		{
			Fast_detection_data[k] =Fast_detection_data[k + 512]; 									
		}
									
		for(i =0; i<512; i++)  //赋值
		{
			Fast_detection_data[i+15872] = AD[i];
			a++;
		}
		
		if(a==16384 || b==1)
		{
			a = 0;	 
		}
								
		if(a==0)
		{
			b = 1;
			for(i=0; i<16384; i++)
			{
				//printf("%04d ", Fast_detection_data[i]);
				adc_temp = Fast_detection_data[i];
				adc_num += adc_temp;
			}
												
			adc_average = adc_num/16384;
			//printf("sum and mean of raw: %d - %d\r\n", adc_num,adc_average);
			for(i=0; i<16384; i++)
			{
				Fast_detection_data[i] = Fast_detection_data[i] - adc_average;
				//printf("%04d ", Fast_detection_data[i]);
			}
			//printf("END \r\n");	
			quick_detection_result = quick_detection(Fast_detection_data, 
													/* win_size_time =  */2048, 
													/* stride_time =  */1024, 
													/* time_times =  */5.8,
													/* time_add =  */60, 
													/* win_size_freq =  */256, 
											        /* stride_freq =  */102, 
													/* time_accum =  */8, 
													/* xhz1 =  */2, 
													/* freq_times =  */9, 
													/* respiration_times =  */res_times); 
			
			if (quick_detection_result)
			{
				quick_detection_flag = 1;
				LED_RED();
				if(CUT == 0)
				{
					transformation ++;
				}
				else
				{
					transformation = 0;
				}
				
			}
		    else
		    {
				quick_detection_flag = 0;
				LED_GREEN();												 
			}
			//printf("quick_detection_flag：%d \r\n",  quick_detection_flag);
											 
			for(i=0; i<16384; i++)
			{
				Fast_detection_data[i] = Fast_detection_data[i] + adc_average;
												 
			}							
		}
	}
	/*Big motion detection and Fretting detection*/

	else if(transformation == 1)
    {
		for(k=0;k<512;k++)
		{
			AD_Value1[512*f + k] = AD[k];
	    }
		f++;
		if(f==4)
		{
			f = 0;
			ad_accum = 1;
		}
		
		
		if(ad_accum == 1)
		{
			for(k=0;k<2048;k++)      //滑窗8s数据
			{
				Fast_detection_data[k] =Fast_detection_data[k + 2048]; 									
			}
										
			for(i =0; i<256; i++)    //提取数据-8位取1位
			{
				AD_256Value[i] = AD_Value1[i*8];
				Fast_detection_data[i+e*256+2048] = AD_256Value[i];
				c++;				 //c自加避免标志位一直置0	
                ad_accum = 0;				
			}
			e++;
			if(e==8)                 //累计8s刷新一次 包含旧数据8s 累计16s的处理数据
			{
				if(c==2048 || d==1)
				{
					c = 0;
					e = 0;
				}
			}
										
			if(c == 0)               //开始处理数据
			{
				d = 1;
				for(i=0; i<4096; i++)
				{
					adc_temp1 = Fast_detection_data[i];
					adc_num1 += adc_temp1;
				}
														
				adc_average1 = adc_num1/4096;
				//printf("sum and mean of raw: %d - %d\r\n", adc_num1,adc_average1);
				for(i=0; i<4096; i++)
				{
					Fast_detection_data[i] = Fast_detection_data[i] - adc_average1;
					//printf("%d ",Fast_detection_data[i]);
				}	
				// printf("END DATA\r\n");
				
				/*Big motion detection*/
				
				time_detection_result = time_detection(/*in_data*/Fast_detection_data, 
														/*data_size*/4096 
														/*win_size_time*/,256 
														/*stride_time*/,128
														/*time_times*/,5 
														/*time_add*/,50);
												 
				fre_detection_result  = freq_detection(/*in_data_freq*/Fast_detection_data,
														/*hamming_TAB2*/hamming_TAB2
														/*data_size*/	,4096
														/*win_size_freq*/,128
														/*stride_freq*/,64
														/*time_accum*/,16
														/*xhz1*/,2
														/*freq_times*/,6.5
														/*respiration_times*/,res_times
														/*respirationfreq_vote*/,respirationfreq_vote1);
				//printf("time-fre-res: %d - %d - %d\r\n", time_detection_result, fre_detection_result, respirationfreq_vote1[0]);

				if(  time_detection_result == 1 && fre_detection_result == 1 )
				{
					Bigmotion_detection_flag = 1;
				}
				else
				{
					Bigmotion_detection_flag = 0;
				}
				//printf("Bigmotion_detection_flag:%d \r\n",  Bigmotion_detection_flag);
												
				if( fre_detection_result == 1 )
				{
					offset = offsetmax;
				}
				else
				{
					offset = offsetmin;
				}
				/*Fretting detection*/
				
				Fretting_detection_result = Fretting_detection(Fast_detection_data, N, pro_N, PAD, offset, rr_threshold);
				//printf("Fretting_detection_result:%d \r\n",  Fretting_detection_result);
												 
				if(Bigmotion_detection_flag == 1)
				{
					LED_RED_TWO();
					printf("红色 1 \r\n");
					red = 1;
					delay_time_num = (ceil)(delay_time* 2/ secnum); 
					respirationfreq_num=(ceil)(delay_time* 2/ secnum*0.125);
					state_transition_flag = 0;
				}
				else if(Fretting_detection_result ==1)
				{
					LED_BLUE_TWO();
					printf("蓝色 1 \r\n");
					blue = 1;
					delay_time_num = (ceil)(delay_time* 2/ secnum); 
					respirationfreq_num=(ceil)(delay_time* 2/ secnum*0.125);
					state_transition_flag = 0;
				}
				else if( (!(time_detection_result))   &&  (!(fre_detection_result))  && respirationfreq_vote1[0] )
				{
					LED_BLUE_TWO();
					printf("蓝色 1 \r\n");
					blue = 1;
					delay_time_num = (ceil)(delay_time* 2/ secnum);
					respirationfreq_num=(ceil)(delay_time* 2/ secnum*0.125);
					state_transition_flag = 0;
				}
				else if( (!(time_detection_result)) &&  fre_detection_result  && respirationfreq_vote1[0] )
				{
					respirationfreq_num = respirationfreq_num-1;
					if(respirationfreq_num < 0)
					{
						LED_BLUE_TWO();
						printf("蓝色 1 \r\n");
						blue = 1;
						delay_time_num = (ceil)(delay_time* 2/ secnum);    //重置次数
						respirationfreq_num=(ceil)(delay_time* 2/ secnum*0.125);
						state_transition_flag = 0;
					}
					else
					{
						delay_time_num = delay_time_num - 1;
						//printf("delay_time_num1：%d \r\n",  delay_time_num);
						if(delay_time_num == 0)
						{
							LED_GREEN();
							printf("绿色 \r\n");
							state_transition_flag++;
						}
					}
															
				}
				else
				{
					delay_time_num = delay_time_num - 1;
                    if(red == 1)
					{
						printf("红色 0 \r\n");
						
					}
					else if(blue == 1)
					{
						printf("蓝色 0 \r\n");
					
					}
					
					if(delay_time_num == 0)
					{
						LED_GREEN();
						printf("绿色 \r\n");
						state_transition_flag++;
																  
					}
				}
				
				if(state_transition_flag == 1)
				{
					state_transition_flag = 0;
					transformation = 0;
					delay_time_num = (ceil)(delay_time* 2/ secnum);    //重置次数
					respirationfreq_num=(ceil)(delay_time* 2/ secnum*0.125);
					a = 0,b = 0, c =0, d =0, e =0;
				}
												
				for(i=0; i<4096; i++)
				{
					Fast_detection_data[i] = Fast_detection_data[i] + adc_average1;
				}
			}
		}		
	}	
}


int main(void)
{

			HAL_Init();
			SystemInit();
			Stm32_Clock_Init(200,25,2,7);
			
			delay_init(100);               	//初始化延时函数
			MY_ADC_Init();
			LED_Init();		//初始化LED	 
			KEY_Init();
			
			TIM3_Init(488-1,50-1);        // 100us  3.9ms-TIM3_Init(40-1,5000-1); 
			
			
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
				  if(change_flag==1)
					{
						 measure_fft(AD_Value);
						 change_flag=0;
					}

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


