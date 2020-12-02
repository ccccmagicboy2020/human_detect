/*
 * File: freq_detection.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 18-Nov-2020 14:09:05
 */

/* Include Files */
#include <stm32f411xe.h>
#include "freq_detection.h"
#include <math.h>
#include <stdio.h>
#include <arm_math.h>
#include "cfar_ca_emxAPI.h"
#include "remove_pf.h"
#include "fifo.h"


#define fixed short
#define int32_t int

//extern short buffer[8192];
//extern short im[8192];
extern int gst_spectrum_fix_fft (fixed fr[], fixed fi[], int m, int inverse);
extern void FFT(int dir,int points2, int32_t* real, int32_t* img);

#define TEST_LENGTH_SAMPLES 4096
float testInput[TEST_LENGTH_SAMPLES];
float testInput2[TEST_LENGTH_SAMPLES];
float testInput3[TEST_LENGTH_SAMPLES];
float data_remove_pf[2048];
/*
 * {
 * Function Name: freq_detection
 * Description: ���ݴ��ڱ�׼���ʱ���ж����
 * Input:
 *  data: һά����
 *  win_size_time: ʱ�򴰳�
 *  stride_time: ʱ�򲽳�
 *  time_times: ʱ��˷�����
 *  time_add: ʱ��ӷ�����
 * Output: None
 * Return:
 *  time_vote: ʱ���ж����������ֵ��
 * }
 * Arguments    : const double data[32768]
 *                int win_size_time
 *                int stride_time
 *                int time_times
 *                int time_add
 * Return Type  : bool
 */
int freq_detection(FIFO_DataType data[], const float win[], int data_size, int win_size_freq, int
  stride_freq, int time_accum, int xhz, int freq_times, int respiration_times, int respirationfreq_vote[2])
{
	int freq_vote;
	int i;
	int j;
	int half_size;
	int data_remove_size;
	int pf_result_size[2];
	int mean_size;
	double mean_value[100];
	double sum;
	double maxValue;
	double minValue;
	
	float respirationfreq_max;
	uint32_t pIndex;
	float respirationfreq_mean;
	/* ȥ����Ƶ����г����Χ2HzƵ��*/
	//emxArray_real_T *data_remove_pf;
	
	//emxInitArray_real_T(&data_remove_pf, 1);
	
	half_size = (int)(data_size/2);
	
//	int j;
	arm_rfft_fast_instance_f32 S;
	
	arm_rfft_fast_init_f32(&S, data_size);
	
	for(i=0; i<data_size; i++)
	{
			testInput[i] = data[i] * win[i];
			//testInput[i] = data[i];
			//printf("%.3lf,", testInput[i]);
	}
	
  arm_rfft_fast_f32(&S, testInput, testInput2, 0); 
	arm_cmplx_mag_f32(testInput2, testInput3, data_size);
	
	for (i=0;i<half_size;i++)	//
	{
		testInput3[i] = testInput3[i] / TEST_LENGTH_SAMPLES;
	}

	for (i=1;i<half_size;i++)	//
	{
		testInput3[i] = testInput3[i] * 2;
	}
	
	printf("fft abs value start:\r\n");
	
	for (i=0;i<half_size;i++)
	{
		//printf("%.3lf,", testInput3[i]);
	}
	
	printf("\r\nend\r\n");	

	printf("freq_detection: %d - %d - %d - %d - %d - %d - %d\r\n", data_size, win_size_freq, stride_freq, time_accum, xhz, freq_times, respiration_times);
	printf("xxxxxx - %d - %d - %d\r\n", half_size, time_accum, xhz);	
	remove_pf(testInput3, half_size, time_accum, xhz, 50, data_remove_pf, pf_result_size);    //ȥ����Ƶ����г����Χ2HzƵ��
	
	data_remove_size =  pf_result_size[0];
	printf("remove_pf size is: %d\r\n", data_remove_size);
	
	printf("fft remove_pf value start:\r\n");
	
//	for (i=0;i<data_remove_size;i++)
//	{
//			printf("%.3lf,", data_remove_pf[i]);
//	}  
	
	printf("\r\nend\r\n");	
	
	mean_size = (int)((data_remove_size - win_size_freq) / stride_freq + 1);
	printf("mean size is: %d\r\n", mean_size);
	
	sum = 0;
	
	for (i=0;i<mean_size;i++)
	{
		for (j=0;j<win_size_freq;j++)
		{
			sum += fabs(data_remove_pf[i*stride_freq + j]);
		}
		mean_value[i] = sum / win_size_freq;
		sum = 0;
		printf("freq_detection mean_value: %d - %.4lf\r\n", i, mean_value[i]);
	}	
	
	maxValue = mean_value[0];
	minValue = mean_value[0];
	for (int i=0; i<mean_size; i++)  
	{  
			if (maxValue<mean_value[i])  
			{  
					maxValue = mean_value[i];  
			}  
			if (minValue>mean_value[i])  
			{  
					minValue = mean_value[i];  
			}  
	}  
	
	printf("freq_detection max-min: %lf - %lf\r\n", maxValue, minValue);
	
	if (maxValue > minValue * freq_times)
	{
		freq_vote = 1;
	}
	else
	{
		freq_vote = 0;
	}
	
	arm_max_f32(data_remove_pf, time_accum* 0.5 + 1, &respirationfreq_max, &pIndex);
	printf("freq_detection max freq index-result: %d - %lf\r\n", pIndex, respirationfreq_max);
	
	arm_mean_f32(data_remove_pf, time_accum* 0.5 + 1, &respirationfreq_mean);
	printf("freq_detection mean freq result: %lf\r\n", respirationfreq_mean);
	
	if ((respirationfreq_max > minValue*respiration_times) || (respirationfreq_max > minValue*respiration_times*0.618))
	{
		respirationfreq_vote[0] = 1;
	}
	else
	{
		respirationfreq_vote[0] = 0;
	}
	//emxDestroyArray_real_T(data_remove_pf);
  return freq_vote;
}

/*
 * File trailer for freq_detection.c
 *
 * [EOF]
 */
