/*
 * File: _coder_remove_pf_api.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 26-Nov-2020 19:16:41
 */

#ifndef _CODER_REMOVE_PF_API_H
#define _CODER_REMOVE_PF_API_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"

/* Type Definitions */
#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
  real_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_real_T*/

#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T

typedef struct emxArray_real_T emxArray_real_T;

#endif                                 /*typedef_emxArray_real_T*/

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void remove_pf(real_T data_data[], int32_T data_size[1], real_T
                      time_accum, real_T xhz, emxArray_real_T *data_remove_pf);
extern void remove_pf_api(const mxArray * const prhs[3], int32_T nlhs, const
  mxArray *plhs[1]);
extern void remove_pf_atexit(void);
extern void remove_pf_initialize(void);
extern void remove_pf_terminate(void);
extern void remove_pf_xil_shutdown(void);
extern void remove_pf_xil_terminate(void);

#endif

/*
 * File trailer for _coder_remove_pf_api.h
 *
 * [EOF]
 */
