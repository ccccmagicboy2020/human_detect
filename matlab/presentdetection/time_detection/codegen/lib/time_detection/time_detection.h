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
#include "rtwtypes.h"
#include "time_detection_types.h"

/* Function Declarations */
extern bool time_detection(const double data[32768], int win_size_time, int
  stride_time, int time_times, int time_add);

#endif

/*
 * File trailer for time_detection.h
 *
 * [EOF]
 */
