/*
 * File: time_detection_initialize.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 18-Nov-2020 13:52:24
 */

/* Include Files */
#include "time_detection_initialize.h"
#include "rt_nonfinite.h"
#include "time_detection.h"
#include "time_detection_data.h"

/* Function Definitions */

/*
 * Arguments    : void
 * Return Type  : void
 */
void time_detection_initialize(void)
{
  rt_InitInfAndNaN();
  isInitialized_time_detection = true;
}

/*
 * File trailer for time_detection_initialize.c
 *
 * [EOF]
 */
