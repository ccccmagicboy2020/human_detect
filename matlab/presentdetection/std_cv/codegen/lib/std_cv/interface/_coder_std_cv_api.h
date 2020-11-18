/*
 * File: _coder_std_cv_api.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 18-Nov-2020 15:32:27
 */

#ifndef _CODER_STD_CV_API_H
#define _CODER_STD_CV_API_H

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
extern real_T std_cv(real_T data_data[], int32_T data_size[1]);
extern void std_cv_api(const mxArray * const prhs[1], int32_T nlhs, const
  mxArray *plhs[1]);
extern void std_cv_atexit(void);
extern void std_cv_initialize(void);
extern void std_cv_terminate(void);
extern void std_cv_xil_shutdown(void);
extern void std_cv_xil_terminate(void);

#endif

/*
 * File trailer for _coder_std_cv_api.h
 *
 * [EOF]
 */
