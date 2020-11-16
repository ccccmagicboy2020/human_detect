/*
 * File: find_peak_cv_initialize.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 16-Nov-2020 14:04:28
 */

/* Include Files */
#include "find_peak_cv_initialize.h"
#include "find_peak_cv.h"
#include "find_peak_cv_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */

/*
 * Arguments    : void
 * Return Type  : void
 */
void find_peak_cv_initialize(void)
{
  rt_InitInfAndNaN();
  isInitialized_find_peak_cv = true;
}

/*
 * File trailer for find_peak_cv_initialize.c
 *
 * [EOF]
 */
