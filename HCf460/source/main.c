#include "hc32_ddl.h"
#include "myled.h"
#include "myusart.h"
#include "myadc.h"
#include "mytimer.h"
#include "sys.h"
#include "matlab.h"
#include "math.h" 
#include "arm_math.h" 
#include "hamming.h"
#include "matlab.h"
#include "uart_protocol.h"

#include "cfar_ca_emxAPI.h"
#include "time_detection.h"
#include "std_cv.h"
#include "freq_detection.h"
#include "stdio.h"
#include "fifo.h"

#define	  N           	300         //CFAR窗口大小
#define		pro_N      	 	200         //CFAR保护单元大小
#define		PAD      			0.00000001  //虚警概率
  
#define   rr_threshold  0.6    //呼吸频率截取范围
#define	  secnum   			16

extern u8 change_flag;
extern u16  AD_Value[512];
extern const float hamming_TAB2[4096];

/*Fast detection variable declaration*/

float   offsetmax =  0.65;     //门限偏置
float   offsetmin =  0.6; 

FIFO_DataType Fast_detection_data[16384] = {0};//big raw data pool

u8 quick_detection_result = 0;
u8 transformation = 0;
u8 ad_accum = 0;
u8 red = 1;
u8 blue = 1;
u16  ad = 0;
u16 a = 0;
u16 b = 0;
u16 c =0;
u16 d =0;
u16 e =0;
u16 i = 0;
u16 k = 0;
u16 f = 0;
int delay_time  = 64;
int delay_time_num = 8;
int respirationfreq_num = 1;
/*Macro motion and micro variable declaration*/

//FIFO_DataType  motion_data[4096] = {0}; 
u16  AD_256Value[256] = {0};
u16  AD_Value1[2048];
/* Delay preset */

//int delay_time  = 64, delay_time_num = 8, respirationfreq_num = 1;


//int delay_time_num =(int)(delay_time* 2/ secnum) ;
//int respirationfreq_num =(int)(delay_time* 2/ secnum*0.125);

void do_it(void)
{
	//
	if (2048 < FIFO_GetDataCount(&FIFO_Data[0]))
	{
		printf("fifo number: %d - %d\r\n", i, FIFO_GetDataCount(&FIFO_Data[0]));
		FIFO_ReadData(&FIFO_Data[0], &Fast_detection_data[2048*i], 2048);
		printf("fifo number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[0]));
	}
}
					
void measure_fft(u16 AD[512])
{
  u8 quick_detection_result = 0;
	u8 state_transition_flag = 0;
	u8 time_detection_result = 0;
	u8 fre_detection_result = 0;
	u8 Bigmotion_detection_flag = 0;
	u8 Fretting_detection_result = 0;
	
	u32 adc_num = 0,adc_temp = 0,adc_average= 0, adc_num1 = 0,adc_temp1 = 0,adc_average1= 0;
	int respirationfreq_vote1[2] = {0};
	float offset = 0.35;
				
	if(transformation == 0)  //快检测
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
													/* respiration_times =  */20.5); 
			
			if (quick_detection_result)
			{
				transformation = 1;
			}
		    else
		    {
				//						 
			}
											 
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
														/*respiration_times*/,20.5
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
					blue = 0;
					delay_time_num = (ceil)(delay_time* 2/ secnum); 
					respirationfreq_num=(ceil)(delay_time* 2/ secnum*0.125);
					state_transition_flag = 0;
				}
				else if(Fretting_detection_result ==1)
				{
					LED_BLUE_TWO();
					//printf("蓝色 1 \r\n");
					blue = 1;
					red = 0;
					delay_time_num = (ceil)(delay_time* 2/ secnum); 
					respirationfreq_num=(ceil)(delay_time* 2/ secnum*0.125);
					state_transition_flag = 0;
				}
				else if( (!(time_detection_result))   &&  (!(fre_detection_result))  && respirationfreq_vote1[0] )
				{
					LED_BLUE_TWO();
					//printf("蓝色 1 \r\n");
					blue = 1;
					red = 0;
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
						//printf("蓝色 1 \r\n");
						blue = 1;
						red = 0;
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
	SysClkIni();
	led_init();
	usart_init();
	AdcConfig();
	timer0_init();
	ADC_StartConvert(M4_ADC1);
	
	FIFO_Init(&FIFO_Data[0]);
	uart_protocol_init();
	
  while(1)
  {	
  	//算法
		do_it();
  	//uart access
  	uart_service();
  }
}


