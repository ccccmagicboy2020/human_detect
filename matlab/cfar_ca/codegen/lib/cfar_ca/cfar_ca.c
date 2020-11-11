/*
 * File: cfar_ca.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 11-Nov-2020 17:43:04
 */

/* Include Files */
#include "cfar_ca.h"
#include "cfar_ca_data.h"
#include "cfar_ca_emxutil.h"
#include "cfar_ca_initialize.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static double rt_powd_snf(double u0, double u1);

/* Function Definitions */

/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_powd_snf(double u0, double u1)
{
  double y;
  double d;
  double d1;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else {
    d = fabs(u0);
    d1 = fabs(u1);
    if (rtIsInf(u1)) {
      if (d == 1.0) {
        y = 1.0;
      } else if (d > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d1 == 0.0) {
      y = 1.0;
    } else if (d1 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = rtNaN;
    } else {
      y = pow(u0, u1);
    }
  }

  return y;
}

/*
 * Arguments    : const double xc[1000]
 *                double N
 *                double pro_N
 *                double PAD
 *                emxArray_real_T *b_index
 *                emxArray_real_T *XT
 * Return Type  : void
 */
void cfar_ca(const double xc[1000], double N, double pro_N, double PAD,
             emxArray_real_T *b_index, emxArray_real_T *XT)
{
  double alpha;
  double index0;
  double index1;
  int i;
  double ndbl;
  double apnd;
  double cdiff;
  int nm1d2;
  int n;
  int b_i;
  int index_tmp;
  int k;
  int i1;
  if (!isInitialized_cfar_ca) {
    cfar_ca_initialize();
  }

  alpha = N * (rt_powd_snf(PAD, -1.0 / N) - 1.0);

  /*  门限系数 */
  index0 = ceil((N / 2.0 + 1.0) + pro_N / 2.0);
  index1 = ceil((1000.0 - N / 2.0) - pro_N / 2.0);
  if (rtIsNaN(index0) || rtIsNaN(index1)) {
    i = b_index->size[0] * b_index->size[1];
    b_index->size[0] = 1;
    b_index->size[1] = 1;
    emxEnsureCapacity_real_T(b_index, i);
    b_index->data[0] = rtNaN;
  } else if (index1 < index0) {
    b_index->size[0] = 1;
    b_index->size[1] = 0;
  } else if ((rtIsInf(index0) || rtIsInf(index1)) && (index0 == index1)) {
    i = b_index->size[0] * b_index->size[1];
    b_index->size[0] = 1;
    b_index->size[1] = 1;
    emxEnsureCapacity_real_T(b_index, i);
    b_index->data[0] = rtNaN;
  } else if (index0 == index0) {
    i = b_index->size[0] * b_index->size[1];
    b_index->size[0] = 1;
    nm1d2 = (int)(index1 - index0);
    b_index->size[1] = nm1d2 + 1;
    emxEnsureCapacity_real_T(b_index, i);
    for (i = 0; i <= nm1d2; i++) {
      b_index->data[i] = index0 + (double)i;
    }
  } else {
    ndbl = floor((index1 - index0) + 0.5);
    apnd = index0 + ndbl;
    cdiff = apnd - index1;
    if (fabs(cdiff) < 4.4408920985006262E-16 * fmax(fabs(index0), fabs(index1)))
    {
      ndbl++;
      apnd = index1;
    } else if (cdiff > 0.0) {
      apnd = index0 + (ndbl - 1.0);
    } else {
      ndbl++;
    }

    if (ndbl >= 0.0) {
      n = (int)ndbl;
    } else {
      n = 0;
    }

    i = b_index->size[0] * b_index->size[1];
    b_index->size[0] = 1;
    b_index->size[1] = n;
    emxEnsureCapacity_real_T(b_index, i);
    if (n > 0) {
      b_index->data[0] = index0;
      if (n > 1) {
        b_index->data[n - 1] = apnd;
        nm1d2 = (n - 1) / 2;
        for (k = 0; k <= nm1d2 - 2; k++) {
          index_tmp = k + 1;
          b_index->data[k + 1] = index0 + (double)index_tmp;
          b_index->data[(n - k) - 2] = apnd - (double)index_tmp;
        }

        if (nm1d2 << 1 == n - 1) {
          b_index->data[nm1d2] = (index0 + apnd) / 2.0;
        } else {
          b_index->data[nm1d2] = index0 + (double)nm1d2;
          b_index->data[nm1d2 + 1] = apnd - (double)nm1d2;
        }
      }
    }
  }

  /*  中间那一部分 */
  i = XT->size[0] * XT->size[1];
  XT->size[0] = 1;
  XT->size[1] = b_index->size[1];
  emxEnsureCapacity_real_T(XT, i);
  nm1d2 = b_index->size[1];
  for (i = 0; i < nm1d2; i++) {
    XT->data[i] = 0.0;
  }

  /*  门限 */
  i = (int)(index1 + (1.0 - index0));
  for (b_i = 0; b_i < i; b_i++) {
    apnd = index0 + (double)b_i;
    index1 = (apnd - N / 2.0) - pro_N / 2.0;
    ndbl = (apnd - pro_N / 2.0) - 1.0;
    if (index1 > ndbl) {
      index_tmp = -1;
      nm1d2 = -1;
    } else {
      index_tmp = (int)index1 - 2;
      nm1d2 = (int)ndbl - 1;
    }

    index1 = (apnd + pro_N / 2.0) + 1.0;
    ndbl = (apnd + N / 2.0) + pro_N / 2.0;
    if (index1 > ndbl) {
      n = -1;
      i1 = -1;
    } else {
      n = (int)index1 - 2;
      i1 = (int)ndbl - 1;
    }

    nm1d2 -= index_tmp;
    if (nm1d2 == 0) {
      index1 = 0.0;
    } else {
      index1 = xc[index_tmp + 1];
      for (k = 2; k <= nm1d2; k++) {
        index1 += xc[index_tmp + k];
      }
    }

    nm1d2 = i1 - n;
    if (nm1d2 == 0) {
      ndbl = 0.0;
    } else {
      ndbl = xc[n + 1];
      for (k = 2; k <= nm1d2; k++) {
        ndbl += xc[n + k];
      }
    }

    XT->data[(int)((apnd - N / 2.0) - pro_N / 2.0) - 1] = (index1 + ndbl) / N *
      alpha;
  }
}

/*
 * File trailer for cfar_ca.c
 *
 * [EOF]
 */
