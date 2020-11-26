/*
 * File: _coder_remove_pf_api.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 26-Nov-2020 19:16:41
 */

/* Include Files */
#include "_coder_remove_pf_api.h"
#include "_coder_remove_pf_mex.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;
emlrtContext emlrtContextGlobal = { true,/* bFirstTime */
  false,                               /* bInitialized */
  131594U,                             /* fVersionInfo */
  NULL,                                /* fErrorFunction */
  "remove_pf",                         /* fFunctionName */
  NULL,                                /* fRTCallStack */
  false,                               /* bDebugMode */
  { 2045744189U, 2170104910U, 2743257031U, 4284093946U },/* fSigWrd */
  NULL                                 /* fSigMem */
};

/* Function Declarations */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real_T **y_data, int32_T y_size[1]);
static real_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *time_accum,
  const char_T *identifier);
static real_T d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real_T **ret_data, int32_T ret_size[1]);
static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *data, const
  char_T *identifier, real_T **y_data, int32_T y_size[1]);
static const mxArray *emlrt_marshallOut(const emxArray_real_T *u);
static void emxFree_real_T(emxArray_real_T **pEmxArray);
static void emxInit_real_T(const emlrtStack *sp, emxArray_real_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush);
static real_T f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId);

/* Function Definitions */

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                real_T **y_data
 *                int32_T y_size[1]
 * Return Type  : void
 */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real_T **y_data, int32_T y_size[1])
{
  e_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y_data, y_size);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *time_accum
 *                const char_T *identifier
 * Return Type  : real_T
 */
static real_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *time_accum,
  const char_T *identifier)
{
  real_T y;
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = d_emlrt_marshallIn(sp, emlrtAlias(time_accum), &thisId);
  emlrtDestroyArray(&time_accum);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T
 */
static real_T d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId)
{
  real_T y;
  y = f_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 *                real_T **ret_data
 *                int32_T ret_size[1]
 * Return Type  : void
 */
static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real_T **ret_data, int32_T ret_size[1])
{
  static const int32_T dims[1] = { 2048 };

  const boolean_T bv[1] = { true };

  int32_T iv[1];
  emlrtCheckVsBuiltInR2012b(sp, msgId, src, "double", false, 1U, dims, &bv[0],
    iv);
  ret_size[0] = iv[0];
  *ret_data = (real_T *)emlrtMxGetData(src);
  emlrtDestroyArray(&src);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *data
 *                const char_T *identifier
 *                real_T **y_data
 *                int32_T y_size[1]
 * Return Type  : void
 */
static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *data, const
  char_T *identifier, real_T **y_data, int32_T y_size[1])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  b_emlrt_marshallIn(sp, emlrtAlias(data), &thisId, y_data, y_size);
  emlrtDestroyArray(&data);
}

/*
 * Arguments    : const emxArray_real_T *u
 * Return Type  : const mxArray *
 */
static const mxArray *emlrt_marshallOut(const emxArray_real_T *u)
{
  const mxArray *y;
  const mxArray *m;
  static const int32_T iv[1] = { 0 };

  y = NULL;
  m = emlrtCreateNumericArray(1, &iv[0], mxDOUBLE_CLASS, mxREAL);
  emlrtMxSetData((mxArray *)m, &u->data[0]);
  emlrtSetDimensions((mxArray *)m, u->size, 1);
  emlrtAssign(&y, m);
  return y;
}

/*
 * Arguments    : emxArray_real_T **pEmxArray
 * Return Type  : void
 */
static void emxFree_real_T(emxArray_real_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_real_T *)NULL) {
    if (((*pEmxArray)->data != (real_T *)NULL) && (*pEmxArray)->canFreeData) {
      emlrtFreeMex((*pEmxArray)->data);
    }

    emlrtFreeMex((*pEmxArray)->size);
    emlrtFreeMex(*pEmxArray);
    *pEmxArray = (emxArray_real_T *)NULL;
  }
}

/*
 * Arguments    : const emlrtStack *sp
 *                emxArray_real_T **pEmxArray
 *                int32_T numDimensions
 *                boolean_T doPush
 * Return Type  : void
 */
static void emxInit_real_T(const emlrtStack *sp, emxArray_real_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush)
{
  emxArray_real_T *emxArray;
  int32_T i;
  *pEmxArray = (emxArray_real_T *)emlrtMallocMex(sizeof(emxArray_real_T));
  if (doPush) {
    emlrtPushHeapReferenceStackR2012b(sp, (void *)pEmxArray, (void *)
      &emxFree_real_T);
  }

  emxArray = *pEmxArray;
  emxArray->data = (real_T *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int32_T *)emlrtMallocMex(sizeof(int32_T) * numDimensions);
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T
 */
static real_T f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId)
{
  real_T ret;
  static const int32_T dims = 0;
  emlrtCheckBuiltInR2012b(sp, msgId, src, "double", false, 0U, &dims);
  ret = *(real_T *)emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const mxArray * const prhs[3]
 *                int32_T nlhs
 *                const mxArray *plhs[1]
 * Return Type  : void
 */
void remove_pf_api(const mxArray * const prhs[3], int32_T nlhs, const mxArray
                   *plhs[1])
{
  emxArray_real_T *data_remove_pf;
  real_T (*data_data)[2048];
  int32_T data_size[1];
  real_T time_accum;
  real_T xhz;
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  (void)nlhs;
  st.tls = emlrtRootTLSGlobal;
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  emxInit_real_T(&st, &data_remove_pf, 1, true);

  /* Marshall function inputs */
  emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "data", (real_T **)&data_data,
                   data_size);
  time_accum = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[1]), "time_accum");
  xhz = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[2]), "xhz");

  /* Invoke the target function */
  remove_pf(*data_data, data_size, time_accum, xhz, data_remove_pf);

  /* Marshall function outputs */
  data_remove_pf->canFreeData = false;
  plhs[0] = emlrt_marshallOut(data_remove_pf);
  emxFree_real_T(&data_remove_pf);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void remove_pf_atexit(void)
{
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  remove_pf_xil_terminate();
  remove_pf_xil_shutdown();
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void remove_pf_initialize(void)
{
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, 0);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void remove_pf_terminate(void)
{
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/*
 * File trailer for _coder_remove_pf_api.c
 *
 * [EOF]
 */
