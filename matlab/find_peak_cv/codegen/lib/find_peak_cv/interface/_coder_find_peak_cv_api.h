/*
 * File: _coder_find_peak_cv_api.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 16-Nov-2020 14:04:28
 */

#ifndef _CODER_FIND_PEAK_CV_API_H
#define _CODER_FIND_PEAK_CV_API_H

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
extern void find_peak_cv(real_T y_value_data[], int32_T y_value_size[1], real_T
  x_value_data[], int32_T x_value_size[2], real_T peaks_data[], int32_T
  peaks_size[1], real_T index_data[], int32_T index_size[1]);
extern void find_peak_cv_api(const mxArray * const prhs[2], int32_T nlhs, const
  mxArray *plhs[2]);
extern void find_peak_cv_atexit(void);
extern void find_peak_cv_initialize(void);
extern void find_peak_cv_terminate(void);
extern void find_peak_cv_xil_shutdown(void);
extern void find_peak_cv_xil_terminate(void);

#endif

/*
 * File trailer for _coder_find_peak_cv_api.h
 *
 * [EOF]
 */
