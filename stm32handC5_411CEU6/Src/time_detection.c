/*
 * File: time_detection.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 18-Nov-2020 14:09:05
 */

/* Include Files */
#include "time_detection.h"
#include <math.h>
#include <stdio.h>
#include "std_cv.h"

/*
 * {
 * Function Name: time_detection
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
int time_detection(const float data[], int data_size, int win_size_time, int
  stride_time, int time_times, int time_add)
{
	int time_vote;
	int std_size;
	double std_value[20];
	int i;
	double maxValue;
	double minValue;
	
	double temp0;
	double temp1;
	
	std_size = (int)((data_size - win_size_time) / stride_time + 1);
	
	for (i=0;i<std_size;i++)
	{
		std_value[i] = std_cv(&data[i*stride_time], win_size_time);
		printf("time_detection std_value: %d - %lf\r\n", i, std_value[i]);
	}
	
{  
    maxValue = std_value[0];  
    minValue = std_value[0];  
    for (int i=0; i<std_size; i++)  
    {  
        if (maxValue<std_value[i])  
        {  
            maxValue = std_value[i];  
        }  
        if (minValue>std_value[i])  
        {  
            minValue = std_value[i];  
        }  
    }  
		
		printf("time_detection max-min: %lf - %lf\r\n", maxValue, minValue);
		
		temp0 = minValue * time_times;
		temp1 = minValue + time_add;
		
		printf("time_detection * - +: %lf - %lf\r\n", temp0, temp1);
		
		if (temp0 < temp1)
		{
			if (maxValue > temp0)
			{
				time_vote = 1;
			}
			else
			{
				time_vote = 0;
			}
		}
		else
		{
			if (maxValue > temp1)
			{
				time_vote = 1;
			}
			else
			{
				time_vote = 0;
			}
		}
}
  /*  根据滑窗数据的最大最小标准差进行时域判定 */
  return time_vote;
}

/*
 * File trailer for time_detection.c
 *
 * [EOF]
 */
