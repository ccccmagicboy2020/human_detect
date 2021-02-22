/*
 * File: time_detection.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 18-Nov-2020 14:09:05
 */

#ifndef TIME_DETECTION_H
#define TIME_DETECTION_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
//#include "stm32f4xx.h"
#include "sys.h"
#include "fifo.h"

/* Function Declarations */
extern int time_detection(FIFO_DataType data[], int data_size, int win_size_time, int
  stride_time, double time_times, int time_add);

#endif

/*
 * File trailer for time_detection.h
 *
 * [EOF]
 */
