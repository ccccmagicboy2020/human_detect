/*
 * File: time_detection.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 18-Nov-2020 14:09:05
 */

/* Include Files */
#include "time_detection.h"
#include "rt_nonfinite.h"
#include "time_detection_data.h"
//#include "time_detection_emxutil.h"
#include "time_detection_initialize.h"
#include <math.h>

/* Function Declarations */
static double rt_roundd_snf(double u);

/* Function Definitions */

/*
 * Arguments    : double u
 * Return Type  : double
 */
static double rt_roundd_snf(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

/*
 * {
 * Function Name: time_detection
 * Description: 根据窗内标准差返回时域判定结果
 * Input:
 *  data: 一维数组
 *  win_size_time: 时域窗长
 *  stride_time: 时域步长
 *  time_times: 时域乘法门限
 *  time_add: 时域加法门限
 * Output: None
 * Return:
 *  time_vote: 时域判定结果（布尔值）
 * }
 * Arguments    : const double data[32768]
 *                int win_size_time
 *                int stride_time
 *                int time_times
 *                int time_add
 * Return Type  : bool
 */
bool time_detection(const double data[32768], int win_size_time, int stride_time,
                    int time_times, int time_add)
{
  bool time_vote;
  int qY;
  int idx;
  unsigned int x;
  emxArray_real_T *time;
  unsigned int y;
  int i;
  unsigned int q;
  int b_i;
  int n;
  double d;
  double xbar;
  int k;
  bool exitg1;
  double b_y;
  double scale;
  double t;
  if (!isInitialized_time_detection) {
    time_detection_initialize();
  }

  if (win_size_time < -2147450879) {
    qY = MAX_int32_T;
  } else {
    qY = 32768 - win_size_time;
  }

  if (stride_time == 0) {
    if (qY == 0) {
      idx = 0;
    } else if (qY < 0) {
      idx = MIN_int32_T;
    } else {
      idx = MAX_int32_T;
    }
  } else if (stride_time == 1) {
    idx = qY;
  } else if (stride_time == -1) {
    idx = -qY;
  } else {
    if (qY >= 0) {
      x = (unsigned int)qY;
    } else {
      x = (unsigned int)-qY;
    }

    if (stride_time >= 0) {
      y = (unsigned int)stride_time;
    } else if (stride_time == MIN_int32_T) {
      y = 2147483648U;
    } else {
      y = (unsigned int)-stride_time;
    }

    if (y == 0U) {
      q = MAX_uint32_T;
    } else {
      q = x / y;
    }

    x -= q * y;
    if ((x > 0U) && (x >= (y >> 1U) + (y & 1U))) {
      q++;
    }

    idx = (int)q;
    if ((qY < 0) != (stride_time < 0)) {
      idx = -(int)q;
    }
  }

  emxInit_real_T(&time, 1);
  if (idx > 2147483646) {
    qY = MAX_int32_T;
  } else {
    qY = idx + 1;
  }

  i = time->size[0];
  time->size[0] = qY;
  emxEnsureCapacity_real_T(time, i);
  if (idx > 2147483646) {
    qY = MAX_int32_T;
  } else {
    qY = idx + 1;
  }

  for (i = 0; i < qY; i++) {
    time->data[i] = 0.0;
  }

  /*  计算窗数量 */
  if (idx > 2147483646) {
    qY = MAX_int32_T;
  } else {
    qY = idx + 1;
  }

  for (b_i = 0; b_i < qY; b_i++) {
    d = (((double)b_i + 1.0) - 1.0) * (double)stride_time;
    if (d < 2.147483648E+9) {
      if (d >= -2.147483648E+9) {
        n = (int)d;
      } else {
        n = MIN_int32_T;
      }
    } else {
      n = MAX_int32_T;
    }

    if (win_size_time == 0) {
      time->data[b_i] = rtNaN;
    } else if (win_size_time == 1) {
      if (n > 2147483646) {
        idx = MAX_int32_T;
      } else {
        idx = n + 1;
      }

      xbar = data[idx - 1];
      if ((!rtIsInf(xbar)) && (!rtIsNaN(xbar))) {
        time->data[b_i] = 0.0;
      } else {
        time->data[b_i] = rtNaN;
      }
    } else {
      if (win_size_time == 0) {
        xbar = 0.0;
      } else {
        if (n > 2147483646) {
          idx = MAX_int32_T;
        } else {
          idx = n + 1;
        }

        xbar = data[idx - 1];
        for (k = 2; k <= win_size_time; k++) {
          if ((n < 0) && (k < MIN_int32_T - n)) {
            idx = MIN_int32_T;
          } else if ((n > 0) && (k > MAX_int32_T - n)) {
            idx = MAX_int32_T;
          } else {
            idx = n + k;
          }

          xbar += data[idx - 1];
        }
      }

      xbar /= (double)win_size_time;
      b_y = 0.0;
      scale = 3.3121686421112381E-170;
      for (k = 0; k < win_size_time; k++) {
        idx = k + 1;
        if ((n < 0) && (idx < MIN_int32_T - n)) {
          idx = MIN_int32_T;
        } else if ((n > 0) && (idx > MAX_int32_T - n)) {
          idx = MAX_int32_T;
        } else {
          idx += n;
        }

        d = fabs(data[idx - 1] - xbar);
        if (d > scale) {
          t = scale / d;
          b_y = b_y * t * t + 1.0;
          scale = d;
        } else {
          t = d / scale;
          b_y += t * t;
        }
      }

      b_y = scale * sqrt(b_y);
      time->data[b_i] = b_y / sqrt((double)win_size_time - 1.0);
    }

    /*  窗内标准差 */
  }

  n = time->size[0];
  if (time->size[0] <= 2) {
    if (time->size[0] == 1) {
      xbar = time->data[0];
    } else if ((time->data[0] < time->data[1]) || (rtIsNaN(time->data[0]) &&
                (!rtIsNaN(time->data[1])))) {
      xbar = time->data[1];
    } else {
      xbar = time->data[0];
    }
  } else {
    if (!rtIsNaN(time->data[0])) {
      idx = 1;
    } else {
      idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k <= time->size[0])) {
        if (!rtIsNaN(time->data[k - 1])) {
          idx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }

    if (idx == 0) {
      xbar = time->data[0];
    } else {
      xbar = time->data[idx - 1];
      i = idx + 1;
      for (k = i; k <= n; k++) {
        d = time->data[k - 1];
        if (xbar < d) {
          xbar = d;
        }
      }
    }
  }

  n = time->size[0];
  if (time->size[0] <= 2) {
    if (time->size[0] == 1) {
      b_y = time->data[0];
    } else if ((time->data[0] > time->data[1]) || (rtIsNaN(time->data[0]) &&
                (!rtIsNaN(time->data[1])))) {
      b_y = time->data[1];
    } else {
      b_y = time->data[0];
    }
  } else {
    if (!rtIsNaN(time->data[0])) {
      idx = 1;
    } else {
      idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k <= time->size[0])) {
        if (!rtIsNaN(time->data[k - 1])) {
          idx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }

    if (idx == 0) {
      b_y = time->data[0];
    } else {
      b_y = time->data[idx - 1];
      i = idx + 1;
      for (k = i; k <= n; k++) {
        d = time->data[k - 1];
        if (b_y > d) {
          b_y = d;
        }
      }
    }
  }

  d = rt_roundd_snf(b_y * (double)time_times);
  if (d < 2.147483648E+9) {
    if (d >= -2.147483648E+9) {
      b_i = (int)d;
    } else {
      b_i = MIN_int32_T;
    }
  } else if (d >= 2.147483648E+9) {
    b_i = MAX_int32_T;
  } else {
    b_i = 0;
  }

  n = time->size[0];
  if (time->size[0] <= 2) {
    if (time->size[0] == 1) {
      b_y = time->data[0];
    } else if ((time->data[0] > time->data[1]) || (rtIsNaN(time->data[0]) &&
                (!rtIsNaN(time->data[1])))) {
      b_y = time->data[1];
    } else {
      b_y = time->data[0];
    }
  } else {
    if (!rtIsNaN(time->data[0])) {
      idx = 1;
    } else {
      idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k <= time->size[0])) {
        if (!rtIsNaN(time->data[k - 1])) {
          idx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }

    if (idx == 0) {
      b_y = time->data[0];
    } else {
      b_y = time->data[idx - 1];
      i = idx + 1;
      for (k = i; k <= n; k++) {
        d = time->data[k - 1];
        if (b_y > d) {
          b_y = d;
        }
      }
    }
  }

  emxFree_real_T(&time);
  d = rt_roundd_snf(b_y + (double)time_add);
  if (d < 2.147483648E+9) {
    if (d >= -2.147483648E+9) {
      idx = (int)d;
    } else {
      idx = MIN_int32_T;
    }
  } else if (d >= 2.147483648E+9) {
    idx = MAX_int32_T;
  } else {
    idx = 0;
  }

  if (b_i < idx) {
    idx = b_i;
  }

  time_vote = (xbar > idx);

  /*  根据滑窗数据的最大最小标准差进行时域判定 */
  return time_vote;
}

/*
 * File trailer for time_detection.c
 *
 * [EOF]
 */
