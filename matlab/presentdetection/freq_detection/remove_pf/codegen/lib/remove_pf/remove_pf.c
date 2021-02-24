/*
 * File: remove_pf.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 26-Nov-2020 19:16:41
 */

/* Include Files */
#include "remove_pf.h"
#include "remove_pf_emxutil.h"
#include <math.h>

/* Function Definitions */

/*
 * {
 * Function Name: remove_pf
 * Description: ����ȥ����Ƶ����г����ΧxHzƵ��
 * Input:
 *  data: һά����
 *  time_accum: �ۻ�ʱ��
 *  xhz: ȥ��Ƶ������
 * Output: None
 * Return:
 *  data_remove_pf: ȥ����Ƶ���ź������
 * }
 * Arguments    : const double data_data[]
 *                const int data_size[1]
 *                double time_accum
 *                double xhz
 *                emxArray_real_T *data_remove_pf
 * Return Type  : void
 */
void remove_pf(const double data_data[], const int data_size[1], double
               time_accum, double xhz, emxArray_real_T *data_remove_pf)
{
  double d;
  int loop_ub;
  double x;
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

  x = trunc((double)data_size[0] / (50.0 * time_accum));
  d1 = (50.0 * time_accum * x + d) + 1.0;
  if (d1 > data_size[0]) {
    i = 0;
    i1 = 0;
  } else {
    i = (int)d1 - 1;
    i1 = data_size[0];
  }

  emxInit_real_T(&part_2, 2);
  i2 = (int)(50.0 * time_accum - 2.0 * xhz * time_accum);
  part_2_idx_0 = part_2->size[0] * part_2->size[1];
  part_2->size[0] = i2;
  i3 = (int)x;
  part_2->size[1] = i3;
  emxEnsureCapacity_real_T(part_2, part_2_idx_0);
  b_loop_ub = i2 * i3;
  for (i2 = 0; i2 < b_loop_ub; i2++) {
    part_2->data[i2] = 0.0;
  }

  i2 = (int)trunc((double)data_size[0] / (50.0 * time_accum));
  for (b_i = 0; b_i < i2; b_i++) {
    d1 = (50.0 * time_accum * (((double)b_i + 1.0) - 1.0) + d) + 1.0;
    x = 50.0 * time_accum * ((double)b_i + 1.0) - d;
    if (d1 > x) {
      part_2_idx_0 = 0;
      i3 = 0;
    } else {
      part_2_idx_0 = (int)d1 - 1;
      i3 = (int)x;
    }

    b_loop_ub = i3 - part_2_idx_0;
    for (i3 = 0; i3 < b_loop_ub; i3++) {
      part_2->data[i3 + part_2->size[0] * b_i] = data_data[part_2_idx_0 + i3];
    }
  }

  part_2_idx_0 = part_2->size[0] * part_2->size[1];
  i2 = data_remove_pf->size[0];
  data_remove_pf->size[0] = ((loop_ub + part_2_idx_0) + i1) - i;
  emxEnsureCapacity_real_T(data_remove_pf, i2);
  for (i2 = 0; i2 < loop_ub; i2++) {
    data_remove_pf->data[i2] = data_data[i2];
  }

  for (i2 = 0; i2 < part_2_idx_0; i2++) {
    data_remove_pf->data[i2 + loop_ub] = part_2->data[i2];
  }

  emxFree_real_T(&part_2);
  b_loop_ub = i1 - i;
  for (i1 = 0; i1 < b_loop_ub; i1++) {
    data_remove_pf->data[(i1 + loop_ub) + part_2_idx_0] = data_data[i + i1];
  }
}

/*
 * File trailer for remove_pf.c
 *
 * [EOF]
 */