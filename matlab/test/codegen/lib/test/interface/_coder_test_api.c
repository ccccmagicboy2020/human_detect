/*
 * File: _coder_test_api.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 11-Nov-2020 11:31:14
 */

/* Include Files */
#include "_coder_test_api.h"
#include "_coder_test_mex.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;
emlrtContext emlrtContextGlobal = { true,/* bFirstTime */
  false,                               /* bInitialized */
  131594U,                             /* fVersionInfo */
  NULL,                                /* fErrorFunction */
  "test",                              /* fFunctionName */
  NULL,                                /* fRTCallStack */
  false,                               /* bDebugMode */
  { 2045744189U, 2170104910U, 2743257031U, 4284093946U },/* fSigWrd */
  NULL                                 /* fSigMem */
};

/* Function Declarations */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real32_T y[9]);
static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real32_T ret[9]);
static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  char_T *identifier, real32_T y[9]);
static const mxArray *emlrt_marshallOut(const real32_T u[9]);

/* Function Definitions */

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                real32_T y[9]
 * Return Type  : void
 */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real32_T y[9])
{
  c_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 *                real32_T ret[9]
 * Return Type  : void
 */
static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real32_T ret[9])
{
  static const int32_T dims[2] = { 3, 3 };

  emlrtCheckBuiltInR2012b(sp, msgId, src, "single|double", false, 2U, dims);
  emlrtImportArrayR2015b(sp, src, ret, 4, false);
  emlrtDestroyArray(&src);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const char_T *identifier
 *                real32_T y[9]
 * Return Type  : void
 */
static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  char_T *identifier, real32_T y[9])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  b_emlrt_marshallIn(sp, emlrtAlias(src), &thisId, y);
  emlrtDestroyArray(&src);
}

/*
 * Arguments    : const real32_T u[9]
 * Return Type  : const mxArray *
 */
static const mxArray *emlrt_marshallOut(const real32_T u[9])
{
  const mxArray *y;
  const mxArray *m;
  static const int32_T iv[2] = { 3, 3 };

  real32_T *pData;
  int32_T i;
  int32_T b_i;
  y = NULL;
  m = emlrtCreateNumericArray(2, &iv[0], mxSINGLE_CLASS, mxREAL);
  pData = (real32_T *)emlrtMxGetData(m);
  i = 0;
  for (b_i = 0; b_i < 3; b_i++) {
    pData[i] = u[3 * b_i];
    i++;
    pData[i] = u[3 * b_i + 1];
    i++;
    pData[i] = u[3 * b_i + 2];
    i++;
  }

  emlrtAssign(&y, m);
  return y;
}

/*
 * Arguments    : const mxArray * const prhs[1]
 *                int32_T nlhs
 *                const mxArray *plhs[1]
 * Return Type  : void
 */
void test_api(const mxArray * const prhs[1], int32_T nlhs, const mxArray *plhs[1])
{
  real32_T src[9];
  real32_T res[9];
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  (void)nlhs;
  st.tls = emlrtRootTLSGlobal;

  /* Marshall function inputs */
  emlrt_marshallIn(&st, emlrtAliasP(prhs[0]), "src", src);

  /* Invoke the target function */
  test(src, res);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(res);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void test_atexit(void)
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
  test_xil_terminate();
  test_xil_shutdown();
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void test_initialize(void)
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
void test_terminate(void)
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
 * File trailer for _coder_test_api.c
 *
 * [EOF]
 */
