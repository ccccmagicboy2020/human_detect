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


#define fixed short
#define int32_t int

//extern short buffer[8192];
//extern short im[8192];
extern int gst_spectrum_fix_fft (fixed fr[], fixed fi[], int m, int inverse);
extern void FFT(int dir,int points2, int32_t* real, int32_t* img);

#define TEST_LENGTH_SAMPLES 8192
q15_t testInput[TEST_LENGTH_SAMPLES];
q15_t testInput2[4096];

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
  stride_freq, int time_accum, int xhz, int freq_times)
{
	int freq_vote;
	int i;
//	int j;
	arm_cfft_radix4_instance_q15 S;
	
  arm_cfft_radix4_init_q15(&S, data_size, 0, 1);
	
	for(i=0; i<data_size; i++)
	{
			testInput[i*2+1] = 0;
			//testInput[i*2] = (short)(data[i]	*	win[i]);
			testInput[i*2] = (short)(data[i]	*	1000);
			//j = i % 20;
			//testInput[i*2] = arm_sin_q15(1638*j);
			//printf("%d\r\n", testInput[i*2]);
	}	
	
	//add window to the raw data and init image part
//	for (i=0;i<data_size;i++)
//	{
//		//buffer[i] = (short)(data[i]	*	win[i]);
//		buffer[i] = (short)(data[i]	*	1);
//		im[i] = 0;
//	}
	
	//8192=2^13
	//gst_spectrum_fix_fft(buffer, im, 10, 1);
	//FFT(1,10,buffer,im);
	
	arm_cfft_radix4_q15(&S, testInput);
	arm_cmplx_mag_q15(testInput, testInput2, data_size);
	
	
	printf("fft abs value start:\r\n");
	
	for (i=0; i<data_size;i++)
	{
		//buffer[i] = sqrt(buffer[i] * buffer[i] + im[i] * im[i]);
		//printf("freq_detection fft result: %d - %d\r\n", i, buffer[i]);
		//printf("%d,", buffer[i]);
		printf("%d,", testInput2[i]);
	}
	
	printf("\r\nend\r\n");
	
  return freq_vote;
}

/*
 * File trailer for freq_detection.c
 *
 * [EOF]
 */
