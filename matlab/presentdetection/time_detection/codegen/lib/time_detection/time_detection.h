/*
 * File: time_detection.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 18-Nov-2020 13:52:24
 */

#ifndef TIME_DETECTION_H
#define TIME_DETECTION_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "time_detection_types.h"

/* Function Declarations */
extern bool time_detection(const double data[32768], double win_size_time,
  double stride_time, double time_times, double time_add);

#endif

/*
 * File trailer for time_detection.h
 *
 * [EOF]
 */
