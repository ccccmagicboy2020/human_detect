/*
 * File: freq_detection.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 18-Nov-2020 14:09:05
 */

#ifndef FREQ_DETECTION_H
#define FREQ_DETECTION_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>

/* Function Declarations */
extern int freq_detection(const float data[], const float win[], int data_size, int win_size_freq, int
  stride_freq, int time_accum, int xhz, int freq_times, int respiration_times, int respirationfreq_vote[2]);

#endif

/*
 * File trailer for freq_detection.h
 *
 * [EOF]
 */
