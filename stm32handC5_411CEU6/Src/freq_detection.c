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

/*
 * {
 * Function Name: freq_detection
 * Description: 根据窗内标准差返回时域判定结果
 * Input:
 *  data: 一维数组
 *  win_size_time: 时域窗长
 *  stride_time: 时域步长
 *  time_times: 时域乘法门限
 *  time_add: 时域加法门限
 * Output: None
 * Return:
 *  time_vote: 时域判定结果（布尔值）
 * }
 * Arguments    : const double data[32768]
 *                int win_size_time
 *                int stride_time
 *                int time_times
 *                int time_add
 * Return Type  : bool
 */
int freq_detection(const float data[], const float win[], int data_size, int win_size_freq, int
  stride_freq, int time_accum, int xhz, int freq_times, int respiration_times, int respirationfreq_vote[2])
{
	int freq_vote;
	int i;
	int half_size;
	int data_remove_size;
	float	rm_data;
	/* 去除工频及其谐波周围2Hz频点*/
	emxArray_real_T *data_remove_pf;
	
	emxInitArray_real_T(&data_remove_pf, 1);
	
	half_size = (int)(data_size/2);
	
//	int j;
	arm_rfft_fast_instance_f32 S;
	
	arm_rfft_fast_init_f32(&S, data_size);
	
	for(i=0; i<data_size; i++)
	{
			//testInput[i] = data[i] * win[i];
			testInput[i] = data[i];
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
		printf("%.3lf,", testInput3[i]);
	}
	
	printf("\r\nend\r\n");	

	printf("xxxxxx - %d - %d - %d\r\n", half_size, time_accum, xhz);	
	remove_pf(testInput3, half_size, time_accum, xhz, data_remove_pf);    //去除工频及其谐波周围2Hz频点
	
	data_remove_size =  data_remove_pf->size[0];
	printf("remove_pf size is: %d\r\n", data_remove_size);
	
	printf("fft remove_pf value start:\r\n");
	
	for (i=0;i<data_remove_size;i++)
	{
			rm_data = data_remove_pf->data[i];
			printf("%.3lf,", rm_data);
	}  
	
	printf("\r\nend\r\n");	
	
	

	
	
	
	
	
	emxDestroyArray_real_T(data_remove_pf);
  return freq_vote;
}

/*
 * File trailer for freq_detection.c
 *
 * [EOF]
 */
