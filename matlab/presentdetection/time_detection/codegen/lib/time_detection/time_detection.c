/*
 * File: time_detection.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 18-Nov-2020 13:52:24
 */

/* Include Files */
#include "time_detection.h"
#include "rt_nonfinite.h"
#include "time_detection_data.h"
#include "time_detection_emxutil.h"
#include "time_detection_initialize.h"
#include <math.h>

/* Function Definitions */

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
 *                double win_size_time
 *                double stride_time
 *                double time_times
 *                double time_add
 * Return Type  : bool
 */
bool time_detection(const double data[32768], double win_size_time, double
                    stride_time, double time_times, double time_add)
{
  bool time_vote;
  emxArray_real_T *time;
  int n;
  int i;
  emxArray_real_T *absdiff;
  int b_i;
  int n_tmp;
  int idx;
  double xbar;
  int k;
  bool exitg1;
  double t;
  double scale;
  double ex;
  if (!isInitialized_time_detection) {
    time_detection_initialize();
  }

  emxInit_real_T(&time, 1);
  n = (int)((32768.0 - win_size_time) / stride_time + 1.0);
  i = time->size[0];
  time->size[0] = n;
  emxEnsureCapacity_real_T(time, i);
  for (i = 0; i < n; i++) {
    time->data[i] = 0.0;
  }

  /*  计算窗数量 */
  emxInit_real_T(&absdiff, 1);
  for (b_i = 0; b_i < n; b_i++) {
    n_tmp = (int)floor(win_size_time - 1.0);
    i = n_tmp + 1;
    if (i == 0) {
      time->data[b_i] = rtNaN;
    } else if (i == 1) {
      xbar = data[(int)((((double)b_i + 1.0) - 1.0) * stride_time + 1.0) - 1];
      if ((!rtIsInf(xbar)) && (!rtIsNaN(xbar))) {
        time->data[b_i] = 0.0;
      } else {
        time->data[b_i] = rtNaN;
      }
    } else {
      if (i == 0) {
        xbar = 0.0;
      } else {
        xbar = data[(int)((((double)b_i + 1.0) - 1.0) * stride_time + 1.0) - 1];
        for (k = 2; k <= n_tmp + 1; k++) {
          xbar += data[(int)((((double)b_i + 1.0) - 1.0) * stride_time +
                             (((double)k - 1.0) + 1.0)) - 1];
        }
      }

      xbar /= (double)i;
      idx = absdiff->size[0];
      absdiff->size[0] = i;
      emxEnsureCapacity_real_T(absdiff, idx);
      for (k = 0; k <= n_tmp; k++) {
        absdiff->data[k] = fabs(data[(int)((((double)b_i + 1.0) - 1.0) *
          stride_time + ((double)k + 1.0)) - 1] - xbar);
      }

      xbar = 0.0;
      scale = 3.3121686421112381E-170;
      for (k = 0; k <= n_tmp; k++) {
        if (absdiff->data[k] > scale) {
          t = scale / absdiff->data[k];
          xbar = xbar * t * t + 1.0;
          scale = absdiff->data[k];
        } else {
          t = absdiff->data[k] / scale;
          xbar += t * t;
        }
      }

      xbar = scale * sqrt(xbar);
      time->data[b_i] = xbar / sqrt((double)(n_tmp + 1) - 1.0);
    }

    /*  窗内标准差 */
  }

  emxFree_real_T(&absdiff);
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
        scale = time->data[k - 1];
        if (xbar < scale) {
          xbar = scale;
        }
      }
    }
  }

  n = time->size[0];
  if (time->size[0] <= 2) {
    if (time->size[0] == 1) {
      t = time->data[0];
    } else if ((time->data[0] > time->data[1]) || (rtIsNaN(time->data[0]) &&
                (!rtIsNaN(time->data[1])))) {
      t = time->data[1];
    } else {
      t = time->data[0];
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
      t = time->data[0];
    } else {
      t = time->data[idx - 1];
      i = idx + 1;
      for (k = i; k <= n; k++) {
        scale = time->data[k - 1];
        if (t > scale) {
          t = scale;
        }
      }
    }
  }

  n = time->size[0];
  if (time->size[0] <= 2) {
    if (time->size[0] == 1) {
      ex = time->data[0];
    } else if ((time->data[0] > time->data[1]) || (rtIsNaN(time->data[0]) &&
                (!rtIsNaN(time->data[1])))) {
      ex = time->data[1];
    } else {
      ex = time->data[0];
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
      ex = time->data[0];
    } else {
      ex = time->data[idx - 1];
      i = idx + 1;
      for (k = i; k <= n; k++) {
        scale = time->data[k - 1];
        if (ex > scale) {
          ex = scale;
        }
      }
    }
  }

  emxFree_real_T(&time);
  time_vote = (xbar > fmin(t * time_times, ex + time_add));

  /*  根据滑窗数据的最大最小标准差进行时域判定 */
  return time_vote;
}

/*
 * File trailer for time_detection.c
 *
 * [EOF]
 */
