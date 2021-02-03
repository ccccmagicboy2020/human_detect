#ifndef _MATLAB_H
#define _MATLAB_H
#include "sys.h"
#include "fifo.h"

#define   m    1600    //不同的分辨率对应的m值不同


//void  Romove_powerline_interference(float in_data[2048],float out_data[2048],u16 peak_50);
//extern void Bigmotion_detection(float data[m]);

extern int Fretting_detection(FIFO_DataType in_data5[4096],	double N, double pro_N, double PAD, double  offset, double rr_threshold);
extern int quick_detection(FIFO_DataType	in_data[16384],int win_size_time,int stride_time,double time_times,int time_add,int win_size_freq,  
										int stride_freq,  int time_accum,int xhz1, int freq_times,	double respiration_times);
#endif

