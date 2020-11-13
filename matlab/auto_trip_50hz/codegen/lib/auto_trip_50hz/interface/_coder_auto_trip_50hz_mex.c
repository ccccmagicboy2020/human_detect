/*
 * File: _coder_auto_trip_50hz_mex.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 13-Nov-2020 17:07:01
 */

/* Include Files */
#include "_coder_auto_trip_50hz_mex.h"
#include "_coder_auto_trip_50hz_api.h"

/* Function Declarations */
MEXFUNCTION_LINKAGE void auto_trip_50hz_mexFunction(int32_T nlhs, mxArray *plhs
  [1], int32_T nrhs, const mxArray *prhs[4]);

/* Function Definitions */

/*
 * Arguments    : int32_T nlhs
 *                mxArray *plhs[1]
 *                int32_T nrhs
 *                const mxArray *prhs[4]
 * Return Type  : void
 */
void auto_trip_50hz_mexFunction(int32_T nlhs, mxArray *plhs[1], int32_T nrhs,
  const mxArray *prhs[4])
{
  const mxArray *outputs[1];
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  st.tls = emlrtRootTLSGlobal;

  /* Check for proper number of arguments. */
  if (nrhs != 4) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 4, 4,
                        14, "auto_trip_50hz");
  }

  if (nlhs > 1) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 14,
                        "auto_trip_50hz");
  }

  /* Call the function. */
  auto_trip_50hz_api(prhs, nlhs, outputs);

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
  mexAtExit(&auto_trip_50hz_atexit);

  /* Module initialization. */
  auto_trip_50hz_initialize();

  /* Dispatch the entry-point. */
  auto_trip_50hz_mexFunction(nlhs, plhs, nrhs, prhs);

  /* Module termination. */
  auto_trip_50hz_terminate();
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
 * File trailer for _coder_auto_trip_50hz_mex.c
 *
 * [EOF]
 */
