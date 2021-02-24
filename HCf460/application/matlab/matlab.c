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
#include "fifo.h"

#define   FFT_LENGTH	4096		//FFT����
#define   xhz     		2           //ȥ��Ƶ������
#define	  secnum   		16      

extern float testInput[4096];
extern float testInput2[4096];
extern float testInput3[4096];//FFT��������//FFT�������


//float in_data1[4096] = {0}; 
float out[2048] = {0};
float out1[800] = {0};

extern const float hamming_TAB2[4096];
int power_freq = 50;

int quick_detection(FIFO_DataType  in_data[16384],int win_size_time,int stride_time,double time_times,int time_add,int win_size_freq,  
								   int stride_freq,  int time_accum,int xhz1, int freq_times,	double respiration_times)											 	
{
	u8 result = 0;
	u16 time_vote = 0,freq_vote = 0,i =0; 
	int  respirationfreq_vote[2];
	s16 in_data_freq[4096] = {0};
	 
	time_vote = time_detection(in_data, 16384, win_size_time, stride_time,time_times, time_add);
			
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
	int flag_Fretting = 0, index_size[2];   //΢��������־λ
	u16 i = 0 ,OP = 0,num= 0,num1 = 0,b = 0,data_remove_size = 0 ,XT_size  = 0;
	float rm_data = 0,xc_max = 0,XT_max = 0,p = 0 ,p1 = 0;
    float xc2[m],XT1[m],b_index[2000];  //�洢cefar��������
	int pf1_result_size[2];
	  
	//�Ӵ�����		
	for(b=0;b<4096;b++)
	{
		//printf("%d  ",in_data5[b]);	
		testInput[b] = in_data5[b] * hamming_TAB2[b];
		//adc_data = in_data[t];
	//	printf("%.2f \r\n",in_data1[b]);	
	}	
//	printf("END data1\r\n");
	//FFT����
	
	arm_rfft_fast_f32(&S, testInput, testInput2, 0); 
	arm_cmplx_mag_f32(testInput2, testInput3, 4096);
			
	for(i=0;i<2048;i++)    //��0-100hz������ȡ����
	{
		testInput3[i] = (testInput3[i]) / 4096;
		//printf("%.2f ",fft_outputbuf[i]);					
	}
	for(i=0;i<2048;i++)
	{
		testInput3[i+1] = (testInput3[i+1]) * 2;  //���㵥���׷��Ȳ�ȥ����Ƶ�Ŵ�ЧӦ	
	}
	//printf("END \r\n");
	for(i=0; i<2048; i++)
	{ 
		out[i] = testInput3[i];
		//printf("%.2f ",out[i]);
	}
	//printf("END \r\n");
	/* ȥ����Ƶ����г����Χ2HzƵ��*/

	remove_pf_cui(out, 2048, secnum, xhz, power_freq,testInput, pf1_result_size);    //ȥ����Ƶ����г����Χ2HzƵ��
	data_remove_size =  pf1_result_size[0];
	//printf("size is: %d \r\n", data_remove_pf->size[0]);
	for (i=0;i<data_remove_size;i++)   //1920
	{
		rm_data =testInput[i];
		out[i] = rm_data;
		//printf("%.8f ", rm_data);
	}  
	//printf("END1 remove wei \r\n");

	for(i = 0;i < 50*secnum;i++)  //���·�ת
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
			
	for(i=0; i < m; i++)   //ԭʼ���ݹ�һ��
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
	for(i=0; i < XT_size; i++)   //�������ݹ�һ��
	{
		p1 = XT1[i];
		XT1[i] = 10 * log( (fabs(p1)/(XT_max)) +1 )/ log(10);
		//printf("%.8f  ",XT1[i]);
	}
	//printf("menEND \r\n");

	/*΢�����*/

	for(i=(50*secnum-1);i< (50+ rr_threshold)* secnum;i++)
	{
		//printf("%.2lf ", xc2[i]);
		OP = i - (N+ pro_N)/ 2;
		//printf("peaks-men: %.2lf - %.2lf\r\n", xc2[i],XT1[OP1]);
		if( xc2[i] > offset + XT1[OP])
		{
			num++;
			if(num == 1)
			{
				flag_Fretting = 1;
				//printf("flag_Fretting:%d \r\n ",  flag_Fretting);
				num = 0;
			}
						
		}
		else 
		{
			num1++;
			if(num1 == 11)
			{
				flag_Fretting = 0;
				num1 = 0;
								
			}
		}
	}
    return flag_Fretting;			
}















