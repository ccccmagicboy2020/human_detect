/*
 * File: std_cv_initialize.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 18-Nov-2020 15:32:27
 */

/* Include Files */
#include "std_cv_initialize.h"
#include "rt_nonfinite.h"
#include "std_cv.h"
#include "std_cv_data.h"

/* Function Definitions */

/*
 * Arguments    : void
 * Return Type  : void
 */
void std_cv_initialize(void)
{
  rt_InitInfAndNaN();
  isInitialized_std_cv = true;
}

/*
 * File trailer for std_cv_initialize.c
 *
 * [EOF]
 */
