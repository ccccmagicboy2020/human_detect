#include "matlab.h"
#include "myled.h"
#include "arm_math.h"
#include "stdio.h"
#include "cfar_ca.h"

#include "cfar_ca_emxAPI.h"
#include "remove_pf_cui.h"
#include "time_detection.h"
#include "std_cv.h"
#include "freq_detection.h"

#define   FFT_LENGTH	4096		//FFT长度
#define   xhz     		2           //去除频点数量
#define	  secnum   		16      

extern float testInput[4096];
extern float testInput2[4096];
extern float testInput3[4096];//FFT输入数组//FFT输出数组
extern unsigned char upload_disable;
extern float breathe_freq;
extern float max_pp2_rt;
extern int study_flag;

//float in_data1[4096] = {0}; 
float out[2048] = {0};
float out1[800] = {0};

extern const float hamming_TAB2[4096];
int power_freq = 50;

extern	int breathe_upload_en;

void Delay_ms(unsigned int t);

int quick_detection(FIFO_DataType  in_data[16384],int win_size_time,int stride_time,float time_times,float time_add,int win_size_freq,  
								   int stride_freq,  int time_accum,int xhz1, float freq_times,	float respiration_times)											 	
{
	u8 result = 0;
	u16 time_vote = 0,freq_vote = 0,i =0; 
	int  respirationfreq_vote[2];
	s16 in_data_freq[4096] = {0};
	 
	time_vote = time_detection(in_data, 16384, win_size_time, stride_time,time_times, time_add);
	
	respirationfreq_vote[0] = 0;//output: result
	respirationfreq_vote[1] = 1;//input: enable
			
	if (time_vote)
	{
		for(i=0;i<4096;i++)
		{
			in_data_freq[i] = in_data[i*4];
			//printf("%d, ", in_data_freq[i]);
		}
						
		freq_vote = freq_detection(in_data_freq,hamming_TAB2,4096,win_size_freq,stride_freq,time_accum,xhz1,freq_times,respiration_times,respirationfreq_vote); 
			
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
			
	//printf("time_vote: %d\r\n", time_vote);
	//printf("freq_vote: %d\r\n", freq_vote);
	//printf("result: %d\r\n", result);
	return result;
} 

int Fretting_detection(FIFO_DataType in_data5[4096],double N, double pro_N, double PAD, double  offset, double rr_threshold)
{
	arm_rfft_fast_instance_f32 S;
	arm_rfft_fast_init_f32(&S, 4096);
	int flag_Fretting = 0, index_size[2];   //微动动作标志位
	u16 i = 0 ,OP = 0, b = 0, data_remove_size = 0 , XT_size  = 0;
	float rm_data = 0,xc_max = 0,XT_max = 0,p = 0 ,p1 = 0;
    float xc2[m],XT1[m],b_index[2000];  //存储cefar门限数据
	int pf1_result_size[2];
	
	float diff = 0;
	float diff_max = 0;//差值的最大值，但是没有用
	static int run_counter = 0;
	
	char float_str[64];
    
    static float diff_last = 0;
		static float diff_max2 = 0;
    static float breathe_freq_last = 0;
	  
	//加窗处理		
	for(b=0;b<4096;b++)
	{
		//printf("%d  ",in_data5[b]);	
		testInput[b] = in_data5[b] * hamming_TAB2[b];
		//adc_data = in_data[t];
	//	printf("%.2f \r\n",in_data1[b]);	
	}	
//	printf("END data1\r\n");
	//FFT处理
	
	arm_rfft_fast_f32(&S, testInput, testInput2, 0); 
	arm_cmplx_mag_f32(testInput2, testInput3, 4096);
			
	for(i=0;i<2048;i++)    //将0-100hz数据提取出来
	{
		testInput3[i] = (testInput3[i]) / 4096;
		//printf("%.2f ",fft_outputbuf[i]);					
	}
	for(i=0;i<2048;i++)
	{
		testInput3[i+1] = (testInput3[i+1]) * 2;  //计算单边谱幅度并去除零频放大效应	
	}
	//printf("END \r\n");
	for(i=0; i<2048; i++)
	{ 
		out[i] = testInput3[i];
		//printf("%.2f ",out[i]);
	}
	//printf("END \r\n");
	/* 去除工频及其谐波周围2Hz频点*/

	remove_pf_cui(out, 2048, secnum, xhz, power_freq,testInput, pf1_result_size);    //去除工频及其谐波周围2Hz频点
	data_remove_size =  pf1_result_size[0];
	//printf("size is: %d \r\n", data_remove_pf->size[0]);
	for (i=0;i<data_remove_size;i++)   //1920
	{
		rm_data =testInput[i];
		out[i] = rm_data;
		//printf("%.8f ", rm_data);
	}  
	//printf("END1 remove wei \r\n");

	for(i = 0;i < 50*secnum;i++)  //上下翻转
	{
		out1[50*secnum - (i +1)] = out[i] ;
		//printf("%.2lf ",Fretting_indata[i]);
	}

	for(i =0;i <50*secnum;i++)
	{
		out[i+800] = out[i]  ;				 
	}
	for(i = 0;i < 50*secnum;i++)
	{
		out[i] = out1[i]  ;				 
	}

    for(i=0; i < m; i++)
	{
		if( out[i] > xc_max)
		{
			xc_max=  out[i] ;	
		}
		//printf("%.2f  ",xc[i] );
	}
	//printf("end \r\n");
	//printf("xc_max %.2f ", xc_max );	
	//printf(" \r\n");
			
	for(i=0; i < m; i++)   //原始数据归一化
	{
		p = out[i];
		xc2[i] = 10 * log( (fabs(p)/(xc_max)) +1 )/ log(10);
		//printf("%.8f ", xc2[i]);
	}
	//printf("yuanEND \r\n");
	//cfar_ca(xc, N, pro_N, PAD, b_index, XT); //1100
	cfar_ca(out, 1600, N, pro_N, PAD, b_index, index_size, XT1);
	XT_size = index_size[1];

	for (i=0;i<XT_size;i++)
	{
		if( XT1[i] > XT_max)
		{
			XT_max=  XT1[i] ;	
		}
		//printf("%.2f ", XT1[i]);
	} 
	//printf("XT_max %.2f ", XT_max);
	//printf("END \r\n");		
	for(i=0; i < XT_size; i++)   //门限数据归一化
	{
		p1 = XT1[i];
		XT1[i] = 10 * log( (fabs(p1)/(XT_max)) +1 )/ log(10);
		//printf("%.8f  ",XT1[i]);
	}
	//printf("menEND \r\n");

	/*微动检测*/


	for(i=(50*secnum-1);i< (50+ rr_threshold)* secnum;i++)
	{
		//printf("%.2lf ", xc2[i]);
		OP = i - (N+ pro_N)/ 2;
		
		//printf("cfar_offset: %.3lf - %.3lf\r\n", offset, xc2[i] - XT1[OP]);
		
		diff = xc2[i] - XT1[OP];
		
		if (diff > diff_max)
		{
			diff_max = diff;
		}
		
		sprintf(float_str, "freq domain cfar trigger value: %.3lf-%.3lf-%.3lf\r\n", diff, offset, diff_max);
		SEGGER_RTT_printf(0, "%s", float_str);		
		
		if( xc2[i] > offset + XT1[OP])
		{
			flag_Fretting = 1;
			
			if (upload_disable == 0)
			//if (1)
			{		
				breathe_freq = 60*diff/4;
				
				if (breathe_freq < 6.0f)
				{
					breathe_freq = 6.0f;
				}
				else if (breathe_freq > 30.0f)
				{
					breathe_freq = 30.0f;
				}
				
				if (breathe_freq != breathe_freq_last)
				{
					if (breathe_upload_en)
					{
						mcu_dp_value_update(DPID_BREATHE_FREQ, (int)((breathe_freq*1.0f)+0.5f));
					}
					breathe_freq_last = breathe_freq;
				}
				
				Delay_ms(ALL_UPLOAD_DELAY);
				
				if (diff != diff_last)
				{
					//mcu_dp_value_update(DPID_FREQ_PARAMETER2_RT, (int)((diff*1000.0f)+0.5f));
					diff_last = diff;
							
					if (study_flag == 1)
					{									
						if (diff_max2 < diff)
						{
							diff_max2 = diff;
						}
						max_pp2_rt = diff_max2;
					}
					else
					{
						diff_max2 = 0;
					}
				}              
			}
			
#ifndef SAMPLE_USE_ONLY			
			break;
#endif
		}
		else
		{
#ifndef SAMPLE_USE_ONLY
			flag_Fretting = 0;
#endif
		}
	}
	
	if (0)
	//if (upload_disable == 0)
	{
		run_counter++;
		if(run_counter%4 == 1)//
		{		
			//upload offset and diff
			if (diff_max != (float)0)
			{
				if (0)	//bypass this
				{
					mcu_dp_value_update(DPID_FREQ_PARAMETER2_RT, (int)((diff_max*1000.0f)+0.5f));
				}
			}			
		}
	}	

    return flag_Fretting;			
}
















