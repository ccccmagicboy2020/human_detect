/*
 * File: cfar_ca.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 03-Dec-2020 09:43:05
 */

#ifndef CFAR_CA_H
#define CFAR_CA_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "cfar_ca_types.h"

/* Function Declarations */
extern void cfar_ca(const double xc_data[], const int xc_size[2], double N,
                    double pro_N, double PAD, emxArray_real_T *b_index,
                    emxArray_real_T *XT);

#endif

/*
 * File trailer for cfar_ca.h
 *
 * [EOF]
 */
