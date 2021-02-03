/*
 * File: std_cv.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 18-Nov-2020 15:32:27
 */

/* Include Files */
#include "std_cv.h"
#include "rt_nonfinite.h"
#include "std_cv_data.h"
#include "std_cv_initialize.h"
#include <math.h>

/* Function Definitions */

/*
 * std_cv 求可变数组的标准差
 *    此处显示详细说明
 * Arguments    : const double data_data[]
 *                const int data_size[1]
 * Return Type  : double
 */
double std_cv(const float data_data[], int data_size)
{
  double result;
  int n;
  double xbar;
  int k;
  double scale;
  double d;
  double t;
  if (!isInitialized_std_cv) {
    std_cv_initialize();
  }

  n = data_size;
  if (data_size == 0) {
    result = rtNaN;
  } else if (data_size == 1) {
    if ((!rtIsInf(data_data[0])) && (!rtIsNaN(data_data[0]))) {
      result = 0.0;
    } else {
      result = rtNaN;
    }
  } else {
    xbar = data_data[0];
    for (k = 2; k <= n; k++) {
      xbar += data_data[k - 1];
    }

    xbar /= (double)data_size;
    result = 0.0;
    scale = 3.3121686421112381E-170;
    for (k = 0; k < n; k++) {
      d = fabs(data_data[k] - xbar);
      if (d > scale) {
        t = scale / d;
        result = result * t * t + 1.0;
        scale = d;
      } else {
        t = d / scale;
        result += t * t;
      }
    }

    result = scale * sqrt(result);
    result /= sqrt((double)data_size - 1.0);
  }

  return result;
}

/*
 * File trailer for std_cv.c
 *
 * [EOF]
 */
