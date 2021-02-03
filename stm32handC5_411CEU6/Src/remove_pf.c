/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: remove_pf.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 20-Nov-2020 14:07:45
 */

/* Include Files */
#include "remove_pf.h"
#include "cfar_ca_emxutil.h"
#include <math.h>

/* Function Definitions */

/*
 * {
 * Function Name: remove_pf
 * Description: 暴力去除工频及其谐波周围xHz频点
 * Input:
 *  data: 一维数组
 *  time_accum: 累积时间
 *  xhz: 去除频点数量
 * Output: None
 * Return:
 *  data_remove_pf: 去除工频干扰后的数据
 * }
 * Arguments    : const double data[2048]
 *                double time_accum
 *                double xhz
 *                emxArray_real_T *data_remove_pf
 * Return Type  : void
 */
void remove_pf(const float data[2048], double time_accum, double xhz,
               emxArray_real_T *data_remove_pf)
{
  double d;
  int loop_ub;
  double u_tmp;
  double u;
  double d1;
  int i;
  int i1;
  emxArray_real_T *part_2;
  int i2;
  int part_2_idx_0;
  int i3;
  int b_loop_ub;
  int b_i;
  d = xhz * time_accum;
  if (1.0 > d) {
    loop_ub = 0;
  } else {
    loop_ub = (int)d;
  }

  u_tmp = 2048.0 / (50.0 * time_accum);
  if (u_tmp < 0.0) {
    u = ceil(u_tmp);
  } else {
    u = floor(u_tmp);
  }

  d1 = (50.0 * time_accum * u + d) + 1.0;
  if (d1 > 2048.0) {
    i = 0;
    i1 = 0;
  } else {
    i = (int)d1 - 1;
    i1 = 2048;
  }

  emxInit_real_T(&part_2, 2);
  i2 = (int)(50.0 * time_accum - 2.0 * xhz * time_accum);
  part_2_idx_0 = part_2->size[0] * part_2->size[1];
  part_2->size[0] = i2;
  if (u_tmp < 0.0) {
    i3 = (int)ceil(u_tmp);
  } else {
    i3 = (int)floor(u_tmp);
  }

  part_2->size[1] = i3;
  emxEnsureCapacity_real_T(part_2, part_2_idx_0);
  b_loop_ub = i2 * i3;
  for (i2 = 0; i2 < b_loop_ub; i2++) {
    part_2->data[i2] = 0.0;
  }

  i2 = (int)u;
  for (b_i = 0; b_i < i2; b_i++) {
    d1 = (50.0 * time_accum * (((double)b_i + 1.0) - 1.0) + d) + 1.0;
    u_tmp = 50.0 * time_accum * ((double)b_i + 1.0) - d;
    if (d1 > u_tmp) {
      part_2_idx_0 = 0;
      i3 = 0;
    } else {
      part_2_idx_0 = (int)d1 - 1;
      i3 = (int)u_tmp;
    }

    b_loop_ub = i3 - part_2_idx_0;
    for (i3 = 0; i3 < b_loop_ub; i3++) {
      part_2->data[i3 + part_2->size[0] * b_i] = data[part_2_idx_0 + i3];
    }
  }

  part_2_idx_0 = part_2->size[0] * part_2->size[1];
  i2 = data_remove_pf->size[0];
  data_remove_pf->size[0] = ((loop_ub + part_2_idx_0) + i1) - i;
  emxEnsureCapacity_real_T(data_remove_pf, i2);
  for (i2 = 0; i2 < loop_ub; i2++) {
    data_remove_pf->data[i2] = data[i2];
  }

  for (i2 = 0; i2 < part_2_idx_0; i2++) {
    data_remove_pf->data[i2 + loop_ub] = part_2->data[i2];
  }

  emxFree_real_T(&part_2);
  b_loop_ub = i1 - i;
  for (i1 = 0; i1 < b_loop_ub; i1++) {
    data_remove_pf->data[(i1 + loop_ub) + part_2_idx_0] = data[i + i1];
  }
}

/*
 * File trailer for remove_pf.c
 *
 * [EOF]
 */
