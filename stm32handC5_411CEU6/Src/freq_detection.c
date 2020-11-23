/*
 * File: freq_detection.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 18-Nov-2020 14:09:05
 */

/* Include Files */
#include "freq_detection.h"
#include <math.h>
#include <stdio.h>

#define fixed short

extern fixed buffer[8192];
extern fixed im[8192];
extern int gst_spectrum_fix_fft (fixed fr[], fixed fi[], int m, int inverse);

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
int freq_detection(const float data[], const float win[], int data_size, int win_size_freq, int
  stride_freq, int time_accum, int xhz, int freq_times)
{
	int freq_vote;
	int i;
	
	//add window to the raw data and init image part
	for (i=0;i<data_size;i++)
	{
		buffer[i] = (short)(data[i]	*	win[i]);
		im[i] = 0;
	}
	
	//8192=2^13
	gst_spectrum_fix_fft(buffer, im, 13, 0);
	
	printf("fft abs value start:\r\n");
	
	for (i=0; i<data_size;i++)
	{
		buffer[i] = sqrt(buffer[i] * buffer[i] + im[i] * im[i]);
		//printf("freq_detection fft result: %d - %d\r\n", i, buffer[i]);
		printf("%d,", buffer[i]);
	}
	
	printf("\r\nend\r\n");
	
  return freq_vote;
}

/*
 * File trailer for freq_detection.c
 *
 * [EOF]
 */
