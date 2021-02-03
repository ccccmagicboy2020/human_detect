/*
 * File: cfar_ca_initialize.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 11-Nov-2020 17:43:04
 */

/* Include Files */
#include "cfar_ca_initialize.h"
#include "cfar_ca.h"
#include "cfar_ca_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */

/*
 * Arguments    : void
 * Return Type  : void
 */
void cfar_ca_initialize(void)
{
  rt_InitInfAndNaN();
  isInitialized_cfar_ca = true;
}

/*
 * File trailer for cfar_ca_initialize.c
 *
 * [EOF]
 */
