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
#include "sys.h"
#include "arm_math.h" 


float time_threshold[16] = {0};
u8 quick_count = 0;

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
int quick_time_detection(FIFO_DataType data[], int data_size, int win_size_time, int
  stride_time, double time_times, int time_add)
{
	int time_vote;
	int std_size;
	float std_value[100] = {0};
	float pResult;
	int i;
	int j;
	double maxValue;
	double minValue;
	double temp0;
	double temp1;
	float data_temp[2048] = {0};
	float quick_temp = 0,quick_num = 0,quick_ave = 0,temp_min = 0;
	
	std_size = (int)((data_size - win_size_time) / stride_time + 1);
	
//	printf("time_detection: %d - %d - %d - %d - %d\r\n", data_size, win_size_time, stride_time, time_times, time_add);
//	printf("time_detection std_size: %d\r\n", std_size);
	
	for (i=0;i<std_size;i++)
	{
		for(j=0;j<win_size_time;j++)
		{
			data_temp[j] = data[i*stride_time + j];
		}		
		arm_std_f32(data_temp, win_size_time, &pResult);
		std_value[i] = pResult;
		//printf("time_detection std_value: %d - %lf\r\n", i, std_value[i]);
	}
	
  
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
		
//		printf("time_detection max-min: %lf - %lf\r\n", maxValue, minValue);
	if(quick_count < 16)
    {
		time_threshold[quick_count] =  minValue/16;
		quick_count ++;
	}
    else
	{
		for(i=0;i<15;i++)
		{
			time_threshold[i] = time_threshold[i+1];
		}
		time_threshold[15] =  minValue/16;
	}
	/*求取均值*/
	
	for(i=0;i<16;i++)
	{
		quick_temp = time_threshold[i];
	    quick_num += quick_temp;
	}
	quick_ave = quick_num/16;
	
	temp0 = quick_ave * time_times;
	temp1 = quick_ave + time_add;
	
	if(temp0 > temp1)
	{
		temp_min =  temp1;
	}
	else
	{
		temp_min =  temp0;
	}
	
	//printf("time_detection: %lf  %lf\r\n", maxValue, temp_min);
	if( maxValue > temp_min)
	{
		time_vote = 1;
	}
	else
	{
		time_vote = 0;
	}
	
		
//		printf("time_detection * - +: %lf - %lf\r\n", temp0, temp1);
    return time_vote;
}


int time_detection(FIFO_DataType data[], int data_size, int win_size_time, int
  stride_time, float time_times, int time_add)
{
	int time_vote;
	int std_size;
	float pResult;
	float std_value[100] = {0};
	int i;
	int j;
	float maxValue;
	float minValue;
	
	float temp0;
	float temp1;
	float temp2;
	float data_temp[2048] = {0};
	
	std_size = (int)((data_size - win_size_time) / stride_time + 1);
	
//	printf("time_detection: %d - %d - %d - %d - %d\r\n", data_size, win_size_time, stride_time, time_times, time_add);
//	printf("time_detection std_size: %d\r\n", std_size);
	
	for (i=0;i<std_size;i++)
	{
		for(j=0;j<win_size_time;j++)
		{
			data_temp[j] = data[i*stride_time + j];
		}		
		arm_std_f32(data_temp, win_size_time, &pResult);
		std_value[i] = pResult;
		//printf("time_detection std_value: %d - %lf\r\n", i, std_value[i]);
	}
	
 
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
		
//		printf("time_detection max-min: %lf - %lf\r\n", maxValue, minValue);
		
		temp0 = minValue * time_times;
		temp1 = minValue + time_add;
				
		if (temp0 < temp1)
		{
			temp2 = temp0;
		}
		else
		{
			temp2 = temp1;
		}

		if (maxValue > temp2)
		{
			time_vote = 1;
		}
		else
		{
			time_vote = 0;
		}

		printf("time domain *: %.2lf - %.2lf\r\n", time_times, maxValue/minValue);
		printf("time domain +: %d - %.2lf\r\n", time_add, maxValue - minValue);

  /*  根据滑窗数据的最大最小标准差进行时域判定 */
  return time_vote;
}

/*
 * File trailer for time_detection.c
 *
 * [EOF]
 */
