/*
 * File: _coder_auto_trip_50hz_api.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 13-Nov-2020 17:07:01
 */

#ifndef _CODER_AUTO_TRIP_50HZ_API_H
#define _CODER_AUTO_TRIP_50HZ_API_H

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
extern void auto_trip_50hz(real_T src[2560], real_T fs, real_T lamda, real_T
  peaks[3], real_T res[2560]);
extern void auto_trip_50hz_api(const mxArray * const prhs[4], int32_T nlhs,
  const mxArray *plhs[1]);
extern void auto_trip_50hz_atexit(void);
extern void auto_trip_50hz_initialize(void);
extern void auto_trip_50hz_terminate(void);
extern void auto_trip_50hz_xil_shutdown(void);
extern void auto_trip_50hz_xil_terminate(void);

#endif

/*
 * File trailer for _coder_auto_trip_50hz_api.h
 *
 * [EOF]
 */
