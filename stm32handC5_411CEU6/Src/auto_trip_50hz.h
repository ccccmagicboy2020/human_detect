/*
 * File: auto_trip_50hz.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 13-Nov-2020 15:54:55
 */

#ifndef AUTO_TRIP_50HZ_H
#define AUTO_TRIP_50HZ_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>

/* Function Declarations */
extern void auto_trip_50hz(const double src[1024], double fs, double lamda,
  const double peaks[3], double res[1024]);
extern void auto_trip_50hz2(const float src[1024], short fs,
  const float freq, float res[1024]);
#endif

/*
 * File trailer for auto_trip_50hz.h
 *
 * [EOF]
 */
