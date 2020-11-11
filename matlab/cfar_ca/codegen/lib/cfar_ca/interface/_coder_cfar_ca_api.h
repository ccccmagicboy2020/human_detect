/*
 * File: _coder_cfar_ca_api.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 11-Nov-2020 17:43:04
 */

#ifndef _CODER_CFAR_CA_API_H
#define _CODER_CFAR_CA_API_H

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
extern void cfar_ca(real_T xc[1000], real_T N, real_T pro_N, real_T PAD,
                    emxArray_real_T *b_index, emxArray_real_T *XT);
extern void cfar_ca_api(const mxArray * const prhs[4], int32_T nlhs, const
  mxArray *plhs[2]);
extern void cfar_ca_atexit(void);
extern void cfar_ca_initialize(void);
extern void cfar_ca_terminate(void);
extern void cfar_ca_xil_shutdown(void);
extern void cfar_ca_xil_terminate(void);

#endif

/*
 * File trailer for _coder_cfar_ca_api.h
 *
 * [EOF]
 */
