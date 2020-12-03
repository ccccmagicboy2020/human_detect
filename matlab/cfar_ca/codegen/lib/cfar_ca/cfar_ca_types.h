/*
 * File: cfar_ca_types.h
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 03-Dec-2020 09:43:05
 */

#ifndef CFAR_CA_TYPES_H
#define CFAR_CA_TYPES_H

/* Include Files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
  double *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  bool canFreeData;
};

#endif                                 /*struct_emxArray_real_T*/

#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T

typedef struct emxArray_real_T emxArray_real_T;

#endif                                 /*typedef_emxArray_real_T*/
#endif

/*
 * File trailer for cfar_ca_types.h
 *
 * [EOF]
 */
