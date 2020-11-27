/*
 * File: remove_pf.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 26-Nov-2020 19:16:41
 */

#ifndef REMOVE_PF_H
#define REMOVE_PF_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "remove_pf_types.h"

/* Function Declarations */
extern void remove_pf(const float data_data[], int data_size, double
                      time_accum, double xhz, float data_remove_pf[2048], int result_size[2]);

#endif

/*
 * File trailer for remove_pf.h
 *
 * [EOF]
 */
