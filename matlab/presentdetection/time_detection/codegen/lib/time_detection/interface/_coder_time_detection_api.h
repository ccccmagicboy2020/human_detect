/*
 * File: _coder_time_detection_api.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 18-Nov-2020 14:09:05
 */

#ifndef _CODER_TIME_DETECTION_API_H
#define _CODER_TIME_DETECTION_API_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern boolean_T time_detection(real_T data[32768], int32_T win_size_time,
  int32_T stride_time, int32_T time_times, int32_T time_add);
extern void time_detection_api(const mxArray * const prhs[5], int32_T nlhs,
  const mxArray *plhs[1]);
extern void time_detection_atexit(void);
extern void time_detection_initialize(void);
extern void time_detection_terminate(void);
extern void time_detection_xil_shutdown(void);
extern void time_detection_xil_terminate(void);

#endif

/*
 * File trailer for _coder_time_detection_api.h
 *
 * [EOF]
 */
