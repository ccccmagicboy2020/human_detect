/*
 * File: cfar_ca.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 11-Nov-2020 17:43:04
 */

#ifndef CFAR_CA_H
#define CFAR_CA_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "cfar_ca_types.h"

/* Function Declarations */
extern void cfar_ca(const double xc[1000], double N, double pro_N, double PAD,
                    emxArray_real_T *b_index, emxArray_real_T *XT);

#endif

/*
 * File trailer for cfar_ca.h
 *
 * [EOF]
 */
