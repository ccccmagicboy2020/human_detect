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

#include "cfar_ca_emxAPI.h"
#include "time_detection.h"
#include "std_cv.h"
#include "freq_detection.h"
#include "stdio.h"

#define TMR_UNIT            (M4_TMR02)

#define	    N           	300        //CFAR窗口大小
#define		pro_N      	 	200         //CFAR保护单元大小
#define		PAD      		0.00000001 //虚警概率
  
#define   rr_threshold  0.6    //呼吸频率截取范围
#define	  secnum   		16

extern u8 change_flag1;
extern u8 change_flag2;
extern u8 change_flag3;
extern u8 change_flag4;

extern u16  AD_Value[512];
extern u16  AD_Value1[512];
extern u16  AD_buffer[4096];
extern u16  AD_buffer1[4096];
extern const float hamming_TAB2[4096];


/*Fast detection variable declaration*/

FIFO_DataType Fast_detection_data[16384] = {0};
u8 transformation = 0,ad_accum = 0,red = 1,blue = 1,state_transition_flag = 0;
u16  ad = 0,a = 0,b = 0, c =0, d =0, e =0, i = 0,k = 0,f = 0;
int delay_time  = 64, delay_time_num = 8, respirationfreq_num = 1;
/*Macro motion and micro variable declaration*/

//FIFO_DataType  motion_data[4096] = {0}; 
u8 Sampling_status = 0,quick_detection_result = 0;



float offsetmax = 0.65, offsetmin = 0.6;
/* Delay preset */

//int delay_time  = 64, delay_time_num = 8, respirationfreq_num = 1;


//int delay_time_num =(int)(delay_time* 2/ secnum) ;
//int respirationfreq_num =(int)(delay_time* 2/ secnum*0.125);
static void enable_flash_cache(en_functional_state_t state)
{
    EFM_Unlock();
    /* Enable flash. */
    EFM_FlashCmd(Enable);
    /* Wait flash ready. */
    while(Set != EFM_GetFlagStatus(EFM_FLAG_RDY))
    {
        ;
    }
    EFM_InstructionCacheCmd(state);
    EFM_Lock();
}

void quick_detection_measure(FIFO_DataType Fast_data[16384])
{
	u8 quick_detection_flag = 0;
	
	quick_detection_result = quick_detection(Fast_data, 
											/* win_size_time =  */2048, 
											/* stride_time =  */1024, 
											/* time_times =  */4,
											/* time_add =  */32, 
											/* win_size_freq =  */256, 
											/* stride_freq =  */102, 
											/* time_accum =  */8, 
										    /* xhz1 =  */2, 
										    /* freq_times =  */3, 
											/* respiration_times =  */17.5); 
			
				
	 if (quick_detection_result)
	 {
		quick_detection_flag = 1;
		//transformation++;
		Sampling_status = 1;
		LED_RED();	
	 }
	 else
	 {
		quick_detection_flag = 0;
		LED_GREEN_TWO();												 
	 }
	 printf("快%d \r\n",quick_detection_flag);

}

void presence_measure(FIFO_DataType pre_data[])
{
	u8 time_detection_result = 0, fre_detection_result = 0, Bigmotion_detection_flag = 0, Fretting_detection_result = 0;
	int respirationfreq_vote1[2] = {0};
	float offset = 0.6;
	
	time_detection_result = time_detection(/*in_data*/pre_data, 
														/*data_size*/4096 
														/*win_size_time*/,256 
														/*stride_time*/,128
														/*time_times*/,5
														/*time_add*/,40);
												 
	fre_detection_result  = freq_detection(/*in_data_freq*/pre_data,
														/*hamming_TAB2*/hamming_TAB2
														/*data_size*/	,4096
														/*win_size_freq*/,128
														/*stride_freq*/,64
														/*time_accum*/,16
														/*xhz1*/,2
														/*freq_times*/,6
														/*respiration_times*/,17.5
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
				
	Fretting_detection_result = Fretting_detection(pre_data, N, pro_N, PAD, offset, rr_threshold);
	//printf("Fretting_detection_result:%d \r\n",  Fretting_detection_result);
												 
	if(Bigmotion_detection_flag == 1)
	{
		LED_RED_TWO();
		printf("大 1 \r\n");
		red = 1;
		blue = 0;
		delay_time_num = (ceil)(delay_time* 2/ secnum); 
		respirationfreq_num=(ceil)(delay_time* 2/ secnum*0.125);
		//state_transition_flag = 0;
	}
	else if(Fretting_detection_result ==1)
	{
		LED_BLUE_TWO();
		printf("微 1 \r\n");
		blue = 1;
		red  = 0;
		delay_time_num = (ceil)(delay_time* 2/ secnum); 
		respirationfreq_num=(ceil)(delay_time* 2/ secnum*0.125);
		//state_transition_flag = 0;
	}
	else if( (!(time_detection_result)) &&  fre_detection_result  && respirationfreq_vote1[0] )
	{
		respirationfreq_num = respirationfreq_num-1;
		if(respirationfreq_num < 0)
		{
			LED_BLUE_TWO();
			printf("微 1 \r\n");
			blue = 1;
			red = 0;
			delay_time_num = (ceil)(delay_time* 2/ secnum);    //重置次数
			respirationfreq_num=(ceil)(delay_time* 2/ secnum*0.125);
			//state_transition_flag = 0;
		}
	    else
	    {
			delay_time_num = delay_time_num - 1;
			//printf("delay_time_num1：%d \r\n",  delay_time_num);
			if(delay_time_num == 0)
			{
				LED_GREEN();
				printf("无 \r\n");
				Sampling_status = 0;
			}
		}
															
	}
	else
	{
		delay_time_num = delay_time_num - 1;
        if(red == 1)
		{
			printf("大 0 \r\n");
						
		}
		else if(blue == 1)
		{
			printf("微 0 \r\n");
					
		}
					
		if(delay_time_num == 0)
		{
			LED_GREEN();
			printf("无 \r\n");
			Sampling_status = 0;
																  
		}
	}
	if(Sampling_status == 0)
	{
		delay_time_num = (ceil)(delay_time* 2/ secnum);    //重置次数
		respirationfreq_num=(ceil)(delay_time* 2/ secnum*0.125);
		a = 0,b = 0, c =0, d =0, e =0;
	}
												
	
}
void quick_mea(u16 AD[512])
{
	u32 adc_num = 0,adc_temp = 0,adc_average= 0;
	
	if(transformation == 0)
	{
		
		//PORT_SetBits(PortB, Pin06);
		for(k=0;k<15872;k++)   //滑窗
		{
			Fast_detection_data[k] =Fast_detection_data[k + 512]; 									
		}
		//PORT_ResetBits(PortB, Pin06);
									
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
			
			quick_detection_measure(Fast_detection_data);
			
	        for(i=0; i<16384; i++)
			{
				Fast_detection_data[i] = Fast_detection_data[i] + adc_average;
												 
			}	
		}
	}
}


void existence_mea(u16 AD[4096])
{
	u32 adc_temp1 = 0,adc_average1= 0, adc_num1 = 0;
		
	
	for(k=0;k<3584;k++)      //滑窗8s数据
	{
		Fast_detection_data[k] =Fast_detection_data[k + 512]; 									
	}
										
	for(i =0; i<512; i++)    //提取数据-8位取1位
	{
		//AD_256Value[i] = AD[i*8];
		Fast_detection_data[i+3584] = AD[i*8];//AD_256Value[i];
		c++;				 //c自加避免标志位一直置0	
       		
	}
	
	e++;
	if(e==4 || e==8)                 //累计8s刷新一次 包含旧数据8s 累计16s的处理数据
	{
		if(c==4096 || d==1)
		{
			c = 0;
			e = 0;
		}
	}
										
	if(c == 0)               //开始处理数据
	{
		d = 1;
		//PORT_SetBits(PortB, Pin06);
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
				
		presence_measure(Fast_detection_data);
				
		for(i=0; i<4096; i++)
		{
			Fast_detection_data[i] = Fast_detection_data[i] + adc_average1;
		}
		
		//PORT_ResetBits(PortB, Pin06);
	}
		 
	 
}	
					



int main(void)
{
  
	SysClkIni();
	led_init();
	//KEY_Init();
	usart_init();
	AdcConfig();
	timer0_init();
	ADC_StartConvert(M4_ADC1);
	enable_flash_cache(Enable);
    while(1)
    {	
		if(change_flag1==1)
		{
			quick_mea(AD_Value);
			change_flag1 = 0;
		}
		else if(change_flag2==1)
		{
			quick_mea(AD_Value1);
			change_flag2 = 0;
		}
		
		else if(change_flag3==1)
		{
			existence_mea(AD_buffer);
			change_flag3 = 0;
		}
		else if(change_flag4==1)
		{
			existence_mea(AD_buffer1);
			change_flag4 = 0;
		}
		
    }
}




