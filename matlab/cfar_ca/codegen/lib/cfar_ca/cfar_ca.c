/*
 * File: cfar_ca.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 03-Dec-2020 09:43:05
 */

/* Include Files */
#include "cfar_ca.h"
#include "cfar_ca_emxutil.h"
#include <math.h>

/* Function Definitions */

/*
 * Arguments    : const double xc_data[]
 *                const int xc_size[2]
 *                double N
 *                double pro_N
 *                double PAD
 *                emxArray_real_T *b_index
 *                emxArray_real_T *XT
 * Return Type  : void
 */
void cfar_ca(const double xc_data[], const int xc_size[2], double N, double
             pro_N, double PAD, emxArray_real_T *b_index, emxArray_real_T *XT)
{
  double alpha;
  double index0;
  double index1;
  int i;
  int loop_ub;
  int b_i;
  double c_i;
  double y;
  int i1;
  int i2;
  int i3;
  int k;
  alpha = N * (pow(PAD, -1.0 / N) - 1.0);

  /*  门限系数 */
  index0 = ceil((N / 2.0 + 1.0) + pro_N / 2.0);
  index1 = ceil(((double)xc_size[1] - N / 2.0) - pro_N / 2.0);
  if (index1 < index0) {
    b_index->size[0] = 1;
    b_index->size[1] = 0;
  } else {
    i = b_index->size[0] * b_index->size[1];
    b_index->size[0] = 1;
    loop_ub = (int)(index1 - index0);
    b_index->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_index, i);
    for (i = 0; i <= loop_ub; i++) {
      b_index->data[i] = index0 + (double)i;
    }
  }

  /*  中间那一部分 */
  i = XT->size[0] * XT->size[1];
  XT->size[0] = 1;
  XT->size[1] = b_index->size[1];
  emxEnsureCapacity_real_T(XT, i);
  loop_ub = b_index->size[1];
  for (i = 0; i < loop_ub; i++) {
    XT->data[i] = 0.0;
  }

  /*  门限 */
  i = (int)(index1 + (1.0 - index0));
  for (b_i = 0; b_i < i; b_i++) {
    c_i = index0 + (double)b_i;
    index1 = (c_i - N / 2.0) - pro_N / 2.0;
    y = (c_i - pro_N / 2.0) - 1.0;
    if (index1 > y) {
      i1 = -1;
      loop_ub = -1;
    } else {
      i1 = (int)index1 - 2;
      loop_ub = (int)y - 1;
    }

    index1 = (c_i + pro_N / 2.0) + 1.0;
    y = (c_i + N / 2.0) + pro_N / 2.0;
    if (index1 > y) {
      i2 = -1;
      i3 = -1;
    } else {
      i2 = (int)index1 - 2;
      i3 = (int)y - 1;
    }

    loop_ub -= i1;
    if (loop_ub == 0) {
      index1 = 0.0;
    } else {
      index1 = xc_data[i1 + 1];
      for (k = 2; k <= loop_ub; k++) {
        index1 += xc_data[i1 + k];
      }
    }

    loop_ub = i3 - i2;
    if (loop_ub == 0) {
      y = 0.0;
    } else {
      y = xc_data[i2 + 1];
      for (k = 2; k <= loop_ub; k++) {
        y += xc_data[i2 + k];
      }
    }

    XT->data[(int)((c_i - N / 2.0) - pro_N / 2.0) - 1] = (index1 + y) / N *
      alpha;
  }
}

/*
 * File trailer for cfar_ca.c
 *
 * [EOF]
 */
