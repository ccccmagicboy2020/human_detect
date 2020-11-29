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
#include "cfar_ca.h"
#include "cfar_ca_emxAPI.h"
#include "find_peak_cv.h"
#include "time_detection.h"
#include "std_cv.h"
#include "freq_detection.h"
#include "fifo.h"

/////////////////////////////////////////////////////
//u16	in_data[4096]={0};				//raw大池子  8K
char	in_data[16384]={0};				//raw大池子  32K
char	in_data_mf[16384]={0};				//raw大池子  32K
extern int index;

//16384 size window
const float win[16384]=
{
0.080,0.080,0.080,0.080,0.080,0.080,0.080,0.080,0.081,0.081,0.081,0.081,0.081,0.081,0.082,0.082,0.082,0.083,0.083,0.083,0.083,0.084,0.084,0.085,0.085,0.085,0.086,0.086,0.087,0.087,0.088,0.088,0.089,0.089,0.090,0.091,0.091,0.092,0.092,0.093,0.094,0.095,0.095,0.096,0.097,0.097,0.098,0.099,0.100,0.101,0.102,0.102,0.103,0.104,0.105,0.106,0.107,0.108,0.109,0.110,0.111,0.112,0.113,0.114,0.115,0.116,0.117,0.118,0.120,0.121,0.122,0.123,0.124,0.125,0.127,0.128,0.129,0.130,0.132,0.133,0.134,0.136,0.137,0.138,0.140,0.141,0.143,0.144,0.146,0.147,0.149,0.150,0.152,0.153,0.155,0.156,0.158,0.159,0.161,0.162,0.164,0.166,0.167,0.169,0.171,0.172,0.174,0.176,0.178,0.179,0.181,0.183,0.185,0.186,0.188,0.190,0.192,0.194,0.196,0.197,0.199,0.201,0.203,0.205,0.207,0.209,0.211,0.213,0.215,0.217,0.219,0.221,0.223,0.225,0.227,0.229,0.231,0.233,0.236,0.238,0.240,0.242,0.244,0.246,0.248,0.251,0.253,0.255,0.257,0.260,0.262,0.264,0.266,0.269,0.271,0.273,0.275,0.278,0.280,0.282,0.285,0.287,0.290,0.292,0.294,0.297,0.299,0.301,0.304,0.306,0.309,0.311,0.314,0.316,0.319,0.321,0.324,0.326,0.329,0.331,0.334,0.336,0.339,0.341,0.344,0.346,0.349,0.351,0.354,0.357,0.359,0.362,0.364,0.367,0.370,0.372,0.375,0.378,0.380,0.383,0.386,0.388,0.391,0.394,0.396,0.399,0.402,0.404,0.407,0.410,0.412,0.415,0.418,0.421,0.423,0.426,0.429,0.432,0.434,0.437,0.440,0.443,0.445,0.448,0.451,0.454,0.456,0.459,0.462,0.465,0.468,0.470,0.473,0.476,0.479,0.482,0.484,0.487,0.490,0.493,0.496,0.498,0.501,0.504,0.507,0.510,0.512,0.515,0.518,0.521,0.524,0.527,0.529,0.532,0.535,0.538,0.541,0.544,0.546,0.549,0.552,0.555,0.558,0.560,0.563,0.566,0.569,0.572,0.575,0.577,0.580,0.583,0.586,0.589,0.591,0.594,0.597,0.600,0.603,0.605,0.608,0.611,0.614,0.617,0.619,0.622,0.625,0.628,0.631,0.633,0.636,0.639,0.642,0.644,0.647,0.650,0.653,0.655,0.658,0.661,0.663,0.666,0.669,0.672,0.674,0.677,0.680,0.682,0.685,0.688,0.690,0.693,0.696,0.698,0.701,0.704,0.706,0.709,0.712,0.714,0.717,0.719,0.722,0.725,0.727,0.730,0.732,0.735,0.737,0.740,0.743,0.745,0.748,0.750,0.753,0.755,0.758,0.760,0.763,0.765,0.768,0.770,0.772,0.775,0.777,0.780,0.782,0.785,0.787,0.789,0.792,0.794,0.796,0.799,0.801,0.803,0.806,0.808,0.810,0.813,0.815,0.817,0.819,0.822,0.824,0.826,0.828,0.830,0.833,0.835,0.837,0.839,0.841,0.843,0.845,0.848,0.850,0.852,0.854,0.856,0.858,0.860,0.862,0.864,0.866,0.868,0.870,0.872,0.874,0.876,0.878,0.880,0.882,0.883,0.885,0.887,0.889,0.891,0.893,0.894,0.896,0.898,0.900,0.902,0.903,0.905,0.907,0.908,0.910,0.912,0.913,0.915,0.917,0.918,0.920,0.922,0.923,0.925,0.926,0.928,0.929,0.931,0.932,0.934,0.935,0.937,0.938,0.939,0.941,0.942,0.944,0.945,0.946,0.948,0.949,0.950,0.951,0.953,0.954,0.955,0.956,0.958,0.959,0.960,0.961,0.962,0.963,0.964,0.965,0.967,0.968,0.969,0.970,0.971,0.972,0.973,0.974,0.974,0.975,0.976,0.977,0.978,0.979,0.980,0.981,0.981,0.982,0.983,0.984,0.984,0.985,0.986,0.987,0.987,0.988,0.988,0.989,0.990,0.990,0.991,0.991,0.992,0.992,0.993,0.993,0.994,0.994,0.995,0.995,0.996,0.996,0.996,0.997,0.997,0.997,0.998,0.998,0.998,0.998,0.999,0.999,0.999,0.999,0.999,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,0.999,0.999,0.999,0.999,0.999,0.998,0.998,0.998,0.998,0.997,0.997,0.997,0.996,0.996,0.996,0.995,0.995,0.994,0.994,0.993,0.993,0.992,0.992,0.991,0.991,0.990,0.990,0.989,0.988,0.988,0.987,0.987,0.986,0.985,0.984,0.984,0.983,0.982,0.981,0.981,0.980,0.979,0.978,0.977,0.976,0.975,0.974,0.974,0.973,0.972,0.971,0.970,0.969,0.968,0.967,0.965,0.964,0.963,0.962,0.961,0.960,0.959,0.958,0.956,0.955,0.954,0.953,0.951,0.950,0.949,0.948,0.946,0.945,0.944,0.942,0.941,0.939,0.938,0.937,0.935,0.934,0.932,0.931,0.929,0.928,0.926,0.925,0.923,0.922,0.920,0.918,0.917,0.915,0.913,0.912,0.910,0.908,0.907,0.905,0.903,0.902,0.900,0.898,0.896,0.894,0.893,0.891,0.889,0.887,0.885,0.883,0.882,0.880,0.878,0.876,0.874,0.872,0.870,0.868,0.866,0.864,0.862,0.860,0.858,0.856,0.854,0.852,0.850,0.848,0.845,0.843,0.841,0.839,0.837,0.835,0.833,0.830,0.828,0.826,0.824,0.822,0.819,0.817,0.815,0.813,0.810,0.808,0.806,0.803,0.801,0.799,0.796,0.794,0.792,0.789,0.787,0.785,0.782,0.780,0.777,0.775,0.772,0.770,0.768,0.765,0.763,0.760,0.758,0.755,0.753,0.750,0.748,0.745,0.743,0.740,0.737,0.735,0.732,0.730,0.727,0.725,0.722,0.719,0.717,0.714,0.712,0.709,0.706,0.704,0.701,0.698,0.696,0.693,0.690,0.688,0.685,0.682,0.680,0.677,0.674,0.672,0.669,0.666,0.663,0.661,0.658,0.655,0.653,0.650,0.647,0.644,0.642,0.639,0.636,0.633,0.631,0.628,0.625,0.622,0.619,0.617,0.614,0.611,0.608,0.605,0.603,0.600,0.597,0.594,0.591,0.589,0.586,0.583,0.580,0.577,0.575,0.572,0.569,0.566,0.563,0.560,0.558,0.555,0.552,0.549,0.546,0.544,0.541,0.538,0.535,0.532,0.529,0.527,0.524,0.521,0.518,0.515,0.512,0.510,0.507,0.504,0.501,0.498,0.496,0.493,0.490,0.487,0.484,0.482,0.479,0.476,0.473,0.470,0.468,0.465,0.462,0.459,0.456,0.454,0.451,0.448,0.445,0.443,0.440,0.437,0.434,0.432,0.429,0.426,0.423,0.421,0.418,0.415,0.412,0.410,0.407,0.404,0.402,0.399,0.396,0.394,0.391,0.388,0.386,0.383,0.380,0.378,0.375,0.372,0.370,0.367,0.364,0.362,0.359,0.357,0.354,0.351,0.349,0.346,0.344,0.341,0.339,0.336,0.334,0.331,0.329,0.326,0.324,0.321,0.319,0.316,0.314,0.311,0.309,0.306,0.304,0.301,0.299,0.297,0.294,0.292,0.290,0.287,0.285,0.282,0.280,0.278,0.275,0.273,0.271,0.269,0.266,0.264,0.262,0.260,0.257,0.255,0.253,0.251,0.248,0.246,0.244,0.242,0.240,0.238,0.236,0.233,0.231,0.229,0.227,0.225,0.223,0.221,0.219,0.217,0.215,0.213,0.211,0.209,0.207,0.205,0.203,0.201,0.199,0.197,0.196,0.194,0.192,0.190,0.188,0.186,0.185,0.183,0.181,0.179,0.178,0.176,0.174,0.172,0.171,0.169,0.167,0.166,0.164,0.162,0.161,0.159,0.158,0.156,0.155,0.153,0.152,0.150,0.149,0.147,0.146,0.144,0.143,0.141,0.140,0.138,0.137,0.136,0.134,0.133,0.132,0.130,0.129,0.128,0.127,0.125,0.124,0.123,0.122,0.121,0.120,0.118,0.117,0.116,0.115,0.114,0.113,0.112,0.111,0.110,0.109,0.108,0.107,0.106,0.105,0.104,0.103,0.102,0.102,0.101,0.100,0.099,0.098,0.097,0.097,0.096,0.095,0.095,0.094,0.093,0.092,0.092,0.091,0.091,0.090,0.089,0.089,0.088,0.088,0.087,0.087,0.086,0.086,0.085,0.085,0.085,0.084,0.084,0.083,0.083,0.083,0.083,0.082,0.082,0.082,0.081,0.081,0.081,0.081,0.081,0.081,0.080,0.080,0.080,0.080,0.080,0.080,0.080,0.080
};

void rgb_led(int mode);//RGB
int quick_detection(
																							FIFO_DataType	in_data[16384], 
																							int win_size_time, 
																							int stride_time, 
																							int time_times,
																							int time_add, 
																							int win_size_freq, 
																							int stride_freq, 
																							int time_accum, 
																							int xhz, 
																							int freq_times, 
																							int respiration_times
																							);
																							
int quick_detection(
																							FIFO_DataType	in_data[16384], 
																							int win_size_time, 
																							int stride_time, 
																							int time_times,
																							int time_add, 
																							int win_size_freq, 
																							int stride_freq, 
																							int time_accum, 
																							int xhz, 
																							int freq_times, 
																							int respiration_times
																							)
{
	int result = 0;
	int time_vote = 0;
	int freq_vote = 0;
	FIFO_DataType in_data_freq[4096] = {0};
	int i;
	int respirationfreq_vote[2];
	
	
	time_vote = time_detection(in_data, 16384, win_size_time, stride_time,
    time_times, time_add);
	
	if (time_vote)
	{
		//
		for(i=0;i<4096;i++)
		{
			in_data_freq[0] = in_data[i*4];
		}
		//
freq_vote = freq_detection(
																in_data_freq, 
																win, 
																4096, 
																win_size_freq, 
																stride_freq, 
																time_accum, 
																xhz, 
																freq_times, 
																respiration_times, 
																respirationfreq_vote);
		
		if	(freq_vote)
		{
			result = 1;
		}
		else
		{
			result = 0;
		}
	}
	else
	{
		result = 0;
	}
	
	printf("time_vote: %d\r\n", time_vote);
	printf("freq_vote: %d\r\n", freq_vote);
	printf("result: %d\r\n", result);
	
	return result;
}

void rgb_led(int mode)
{
	if (0 == mode)
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);	//RED
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);	
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);		
	}
	else if (1 == mode)
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);//GREEN
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
	}
	else if (2 == mode)
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);//BLUE
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
	}
	else if (3 == mode)
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);//white
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);	
	}
	else if (4 == mode)
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);//black(off)
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);		
	}
}

int main(void)
{
			int i = 0;
			int k = 0;
			unsigned int sum = 0;
			int trigger_flag0 = 0;
			int quick_detection_result = 0;
			short int raw_mean = 0;
	
			FIFO_Init(&FIFO_Data[0]);
	
			HAL_Init();
			Stm32_Clock_Init(200,25,2,7);
			
			delay_init(100);               	//初始化延时函数
			MY_ADC_Init();
			LED_Init();		//初始化LED	 
			
			//TIM3_Init(19, 4882);        // 20*4883/50=1953.2us
			TIM3_Init(19, 1220);        // 20*1221/50=488.4us
			
			TIM10_PWM_Init(500-1,100-1);    	//84M/84=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ  500-1
			TIM_SetTIM10Compare1(0); 	
			
			TIM4_PWM_Init(500-1,50-1);    	//84M/84=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ  500-1
			TIM_SetTIM4Compare1(0); 	
			
			MX_GPIO_Init();
			MX_CRC_Init();
			MX_USART1_UART_Init();
			
			while (1)
			{
				//move the raw data array
				if (2048 < FIFO_GetDataCount(&FIFO_Data[0]))
				{
					if (8 == i)
					{
						for(k=0;k<14336;k++)
						{
							in_data[k] = in_data[k + 2048];
						}
						printf("fifo number: %d - %d\r\n", i, FIFO_GetDataCount(&FIFO_Data[0]));
						FIFO_ReadData(&FIFO_Data[0], &in_data[2048*(i-1)], 2048);
						printf("fifo number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[0]));	
						trigger_flag0 = 1;
						//__disable_irq();
					}
					else
					{
						printf("fifo number: %d - %d\r\n", i, FIFO_GetDataCount(&FIFO_Data[0]));
						FIFO_ReadData(&FIFO_Data[0], &in_data[2048*i], 2048);
						printf("fifo number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[0]));						
						i++;
					}
				}
				
				//
				if (trigger_flag0)
				{
					trigger_flag0 = 0;
					sum = 0;
					
					//均值滤波
					//arm_mean_f32(in_data, 16384, &raw_mean);
					for(k=0;k<16384;k++)
					{
						sum += (unsigned short int)in_data[k];
					}
					raw_mean = (unsigned int )(sum / 16384 + 0.5);
					
					printf("sum and mean of raw: %d - %d\r\n", sum, raw_mean);
					for(k=0;k<16384;k++)
					{
						in_data_mf[k] = in_data[k] - raw_mean;
						//printf("0x%04X ", in_data_mf[k]);
						//printf("%d ", in_data_mf[k]);
					}
					
					quick_detection_result = quick_detection(
																							/* data =  */in_data_mf, 
																							///* data =  */in_data, 
																							/* win_size_time =  */2048, 
																							/* stride_time =  */1024, 
																							/* time_times =  */5,
																							/* time_add =  */50, 
																							/* win_size_freq =  */256, 
																							/* stride_freq =  */102, 
																							/* time_accum =  */8, 
																							/* xhz =  */2, 
																							/* freq_times =  */40, 
																							/* respiration_times =  */22
																							);
					
					if (quick_detection_result)
					{
						rgb_led(0);	//red
					}
					else
					{
						rgb_led(1);		//green
					}
					
//					index++;
//					if (0 == index % 2)
//					{
//						rgb_led(3);
//					}
//					else
//					{
//						rgb_led(4);
//					}
					
					//__enable_irq();    // 开启总中断
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


