/*
 * File: sort.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 16-Nov-2020 14:04:28
 */

/* Include Files */
#include "sort.h"
#include "find_peak_cv.h"
#include "rt_nonfinite.h"
#include "sortIdx.h"

/* Function Definitions */

/*
 * Arguments    : double x_data[]
 *                const int x_size[1]
 *                int idx_data[]
 *                int idx_size[1]
 * Return Type  : void
 */
void sort(double x_data[], const int x_size[1], int idx_data[], int idx_size[1])
{
  int dim;
  int i;
  int vlen;
  int vwork_size[1];
  int vstride;
  int k;
  double vwork_data[400];
  int iidx_data[400];
  int iidx_size[1];
  dim = 0;
  if (x_size[0] != 1) {
    dim = -1;
  }

  if (dim + 2 <= 1) {
    i = x_size[0];
  } else {
    i = 1;
  }

  vlen = i - 1;
  vwork_size[0] = i;
  idx_size[0] = x_size[0];
  vstride = 1;
  for (k = 0; k <= dim; k++) {
    vstride *= x_size[0];
  }

  for (dim = 0; dim < vstride; dim++) {
    for (k = 0; k <= vlen; k++) {
      vwork_data[k] = x_data[dim + k * vstride];
    }

    sortIdx(vwork_data, vwork_size, iidx_data, iidx_size);
    for (k = 0; k <= vlen; k++) {
      i = dim + k * vstride;
      x_data[i] = vwork_data[k];
      idx_data[i] = iidx_data[k];
    }
  }
}

/*
 * File trailer for sort.c
 *
 * [EOF]
 */
