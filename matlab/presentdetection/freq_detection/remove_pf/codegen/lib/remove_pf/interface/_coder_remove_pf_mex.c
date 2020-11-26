/*
 * File: _coder_remove_pf_mex.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 26-Nov-2020 19:16:41
 */

/* Include Files */
#include "_coder_remove_pf_mex.h"
#include "_coder_remove_pf_api.h"

/* Function Declarations */
MEXFUNCTION_LINKAGE void remove_pf_mexFunction(int32_T nlhs, mxArray *plhs[1],
  int32_T nrhs, const mxArray *prhs[3]);

/* Function Definitions */

/*
 * Arguments    : int32_T nlhs
 *                mxArray *plhs[1]
 *                int32_T nrhs
 *                const mxArray *prhs[3]
 * Return Type  : void
 */
void remove_pf_mexFunction(int32_T nlhs, mxArray *plhs[1], int32_T nrhs, const
  mxArray *prhs[3])
{
  const mxArray *outputs[1];
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  st.tls = emlrtRootTLSGlobal;

  /* Check for proper number of arguments. */
  if (nrhs != 3) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 3, 4, 9,
                        "remove_pf");
  }

  if (nlhs > 1) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 9,
                        "remove_pf");
  }

  /* Call the function. */
  remove_pf_api(prhs, nlhs, outputs);

  /* Copy over outputs to the caller. */
  emlrtReturnArrays(1, plhs, outputs);
}

/*
 * Arguments    : int32_T nlhs
 *                mxArray *plhs[]
 *                int32_T nrhs
 *                const mxArray *prhs[]
 * Return Type  : void
 */
void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs, const mxArray
                 *prhs[])
{
  mexAtExit(&remove_pf_atexit);

  /* Module initialization. */
  remove_pf_initialize();

  /* Dispatch the entry-point. */
  remove_pf_mexFunction(nlhs, plhs, nrhs, prhs);

  /* Module termination. */
  remove_pf_terminate();
}

/*
 * Arguments    : void
 * Return Type  : emlrtCTX
 */
emlrtCTX mexFunctionCreateRootTLS(void)
{
  emlrtCreateRootTLS(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1);
  return emlrtRootTLSGlobal;
}

/*
 * File trailer for _coder_remove_pf_mex.c
 *
 * [EOF]
 */
