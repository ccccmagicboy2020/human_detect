/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: remove_pf.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 20-Nov-2020 14:07:45
 */

#ifndef REMOVE_PF_H
#define REMOVE_PF_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "remove_pf_types.h"

/* Function Declarations */
extern void remove_pf(const float data[2048], double time_accum, double xhz,
                      emxArray_real_T *data_remove_pf);

#endif

/*
 * File trailer for remove_pf.h
 *
 * [EOF]
 */
