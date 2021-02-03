#include "matlab.h"
# include "sys.h"
#include "led.h"
#include "arm_math.h"
#include "stdio.h"
#include "cfar_ca.h"
#include "delay.h"
#include "cfar_ca_emxAPI.h"
#include "remove_pf_cui.h"
#include "time_detection.h"
#include "std_cv.h"
#include "freq_detection.h"


#define  FFT_LENGTH		4096		//FFT长度
#define   xhz     		2            //去除频点数量
#define	  secnum   		16      


float in_data1[4096] = {0}; 
//float in_data5[2048] = {0}; 
//float out_data1[2048] = {0};
float out[2048] = {0};
float out1[800] = {0};
//float Romove_50[2000] = {0};


extern const float hamming_TAB2[4096];
int power_freq = 50;


float fft_inputbuf[FFT_LENGTH*2], fft_outputbuf[FFT_LENGTH];	//FFT输入数组//FFT输出数组
//float Fretting_indata[2000] = {0};


int quick_detection(FIFO_DataType	in_data[16384],int win_size_time,int stride_time,double time_times,int time_add,int win_size_freq,  
										int stride_freq,  int time_accum,int xhz1, int freq_times,	double respiration_times)											 	
{
			u8 result = 0;
			u16 time_vote = 0,freq_vote = 0; 
			s16 in_data_freq[4096] = {0};
			int i;
			int respirationfreq_vote[2];
			
			time_vote = time_detection(in_data, 16384, win_size_time, stride_time,time_times, time_add);
			
			if (time_vote)
			{
				
						for(i=0;i<4096;i++)
						{
							in_data_freq[i] = in_data[i*4];
							//printf("%d, ", in_data_freq[i]);
						}
						
					    freq_vote = freq_detection(in_data_freq,hamming_TAB2,4096,win_size_freq,stride_freq,time_accum,xhz1,freq_times,respiration_times,	respirationfreq_vote); 
			
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
			
//			printf("time_vote: %d\r\n", time_vote);
//			printf("freq_vote: %d\r\n", freq_vote);
//			printf("result: %d\r\n", result);
			
			return result;
} 

int Fretting_detection(FIFO_DataType in_data5[4096],	double N, double pro_N, double PAD, double  offset, double rr_threshold)
{
	    arm_cfft_radix2_instance_f32 scfft;
//	    emxArray_real_T *b_index;
//			emxArray_real_T *XT;
		int flag_Fretting = 0, index_size[2];   //微动动作标志位
	    u16 i = 0 ,OP = 0,num= 0,num1 = 0,b = 0,data_remove_size = 0 ,XT_size  = 0;
	    float rm_data = 0,xc_max = 0,XT_max = 0,p = 0 ,p1 = 0;
       float xc2[m],XT1[m],b_index[2000];  //存储cefar门限数据
	   
	   int pf1_result_size[2];
	  
						
	//加窗处理		
			for(b=0;b<4096;b++)
			{
//			     printf("%d  ",in_data5[b]);	
				  in_data1[b] = in_data5[b] * hamming_TAB2[b];
	//			adc_data = in_data[t];
//			  printf("%d - %.2f \r\n",in_data5[b],in_data1[b]);	
				
			}	
//			printf("END \r\n");
	//  FFT处理
	
      arm_cfft_radix2_init_f32(&scfft,4096,0,1);//初始化scfft结构体，设定FFT相关参数
			for(b=0;b<4096;b++)
			{
					
					fft_inputbuf[2*b] = in_data1[b];
					fft_inputbuf[2*b+1]=0;//虚部全部为0
					//  printf("%d \r",AD_FFT[b] );
			}

			
			arm_cfft_radix2_f32(&scfft,fft_inputbuf);	//FFT计算（基2）
			arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,4096);	
			
			for(i=0;i<2048;i++)    //将0-100hz数据提取出来
			{
				  	fft_outputbuf[i] = (fft_outputbuf[i]) / 4096;
				    
//						printf("%.2f ",fft_outputbuf[i]);					
			}
			for(i=0;i<2048;i++)
			{
					fft_outputbuf[i+1] = (fft_outputbuf[i+1]) * 2;  //计算单边谱幅度并去除零频放大效应	
			}
//			printf("END \r\n");
			for(i=0; i<2048; i++)
			{ 
				   out[i] = fft_outputbuf[i];
//				  printf("%.2f ",out[i]);
			}
//			printf("END \r\n");
			/* 去除工频及其谐波周围2Hz频点*/
//			emxArray_real_T *data_remove_pf;
//	
//			emxInitArray_real_T(&data_remove_pf, 1);

			
			//remove_pf(out,secnum, xhz, data_remove_pf);    //去除工频及其谐波周围2Hz频点
			remove_pf_cui(out, 2048, secnum, xhz, power_freq,in_data1, pf1_result_size);    //去除工频及其谐波周围2Hz频点
			
			data_remove_size =  pf1_result_size[0];
		//	  printf("size is: %d \r\n", data_remove_pf->size[0]);
			for (i=0;i<data_remove_size;i++)   //1920
			{
					  rm_data =in_data1[i];
					  out[i] = rm_data;
				//	  printf("%.8f ", rm_data);
			}  
		//	printf("END1 remove wei \r\n");
		


			for(i = 0;i < 50*secnum;i++)  //上下翻转
			{
				 out1[50*secnum - (i +1)] = out[i] ;
			//	 printf("%.2lf ",Fretting_indata[i]);
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
				  
			//	  out[i] = out[i];
			//	  xc_max = 0;
				  
				  if( out[i] > xc_max)
					{
							 xc_max=  out[i] ;	
					}
	
//				 printf("%.2f  ",xc[i] );
			}
//			printf("end \r\n");
//			printf("xc_max %.2f ", xc_max );	
//			printf(" \r\n");
			
	    for(i=0; i < m; i++)   //原始数据归一化
			{
				   p = out[i];
				   xc2[i] = 10 * log( (fabs(p)/(xc_max)) +1 )/ log(10);
				   
			//	   printf("%.8f ", xc2[i]);
			}
//			printf("yuanEND \r\n");
//		  cfar_ca(xc, N, pro_N, PAD, b_index, XT); //1100
	    cfar_ca(out, 1600, N, pro_N, PAD, b_index, index_size, XT1);
	    XT_size = index_size[1];

//			for (i=0;i<XT_size;i++)
//	    {
//			   printf("%.2f  -  %.2f ", b_index[i], XT1[i]);
//	    }
//			printf("END1 \r\n");
//			for (i=0;i<XT_size;i++)
//			{
//					printf("XT:%.2f \r\n", XT1[i]);
//			}
//			printf("END1 \r\n");
//			index_size = b_index->size[1];
//	    XT_size = XT->size[1];
			
//			for (i=0;i<index_size;i++)
//			{
//					printf("%.2lf  ", b_index->data[i]);
//			}
//			printf("END \r\n");
		

//			for (i=0;i<XT_size;i++)
//			{
//					temp = XT->data[i];
//					printf(" %.2lf", temp);
//			} 
//			printf("END1 \r\n");

			


			for (i=0;i<XT_size;i++)
			{
				   
		
				   if( XT1[i] > XT_max)
					 {
								XT_max=  XT1[i] ;	
					 }
				  
	//				printf("%.2f ", XT1[i]);
			} 
//			printf("XT_max %.2f ", XT_max);
//	    printf("END \r\n");
//			
			for(i=0; i < XT_size; i++)   //门限数据归一化
			{
				   p1 = XT1[i];
				   XT1[i] = 10 * log( (fabs(p1)/(XT_max)) +1 )/ log(10);
				   
//				   printf("%.8f  ",XT1[i]);
			}
//			printf("menEND \r\n");
//			


//			/*微动检测*/

			for(i=(50*secnum-1);i< (50+ rr_threshold)* secnum;i++)
			{
	//			   printf("%.2lf ", xc2[i]);
				    OP = i - (N+ pro_N)/ 2;
		//		   printf("peaks-men: %.2lf - %.2lf\r\n", xc2[i],XT1[OP1]);
						if( xc2[i] > offset + XT1[OP])
						{
							  num++;
							  if(num == 1)
								{
								    flag_Fretting = 1;
				//					  printf("flag_Fretting:%d \r\n ",  flag_Fretting);
//									  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);	//RED
//									  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);	
//									  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);	
									  num = 0;
									
								}
						
						}
						else 
						{
							  num1++;
							  if(num1 == 11)
								{
								    flag_Fretting = 0;
					//				  printf("flag_Fretting:%d \r\n ",  flag_Fretting);
									  
//									
//									  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);//GREEN
//									  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
//									  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
									  num1 = 0;
								//  break;
								}
						}
							
					
					
	
			}
//      printf("END \r\n");
	
//	    emxDestroyArray_real_T(data_remove_pf);
//			emxDestroyArray_real_T(XT);
//			emxDestroyArray_real_T(b_index);
      return flag_Fretting;
			
}



//void Bigmotion_detection(float data_100[m])  //data[m]  需要处理数组的总长度 0-100hz--m=800
//{
//	  u8 t = 0,v = 0;
//	  
//	  u16 i = 0,a = 0,b = 0,j = 0;
//	  u32 val = 0;
//	  float pure_data1[m],  pure_data[2*m-200], peak_Average[m/100-1] ; //pure_data[2*m-200]滑窗数组总长度  m-待滑窗数组总长度 800-1400  700-1200
//																																			//peak_Average[m/100-1] 滑窗均值总长度
//	 
//	  float peak_Average_max = 0, peak_Average_min = 0;  //滑窗极值
//	  float TH_Bigmotion = 0;   //大动作门限值
//	
//		for(i=0; i<m; i++)
//	  {
//			  pure_data1[i] = data_100[i];
//		}
//		for(a=0; a<(m/100-1); a++)    //a滑窗次数   800-7次  700-6次
//		{
//			 for(b=0; b<200; b++)     //滑窗大小
//		   {
//			    pure_data[b + a*200] =  pure_data1[b + 100*a];
//			  
//		   }
//		}
//		
//		for(j=0; j<(m/100-1);j++)   //求取滑窗均值
//		{
//				for(t=0;t<200;t++)
//				{
//					val+=pure_data[t + j*200];
//				}
//				peak_Average[j] = val/200;
//		}
//		
//	  peak_Average_max = peak_Average[0], peak_Average_min = peak_Average[0];  //求取滑窗均值的最大最小值
//		for(v=0; v<(m/100-1); v++)
//		{
//				if(peak_Average[v] > peak_Average_max)
//				{
//						 peak_Average_max = peak_Average[v];
//				}
//				if(peak_Average[v] < peak_Average_min)
//				{
//				     peak_Average_min = peak_Average[v];
//				}
//		}
//		printf("max -min: %.2f - %.2f \r\n",  peak_Average_max,peak_Average_min);
//		TH_Bigmotion =  peak_Average_min * 3;
//		
//		if( peak_Average_max > TH_Bigmotion)
//		{
//			   flag_Bigmotion = 1;
//		}
//		else
//		{
//			   flag_Bigmotion = 0;  
//		}
//		printf("%d ",  flag_Bigmotion);
//		printf("END \r\n");
//}












