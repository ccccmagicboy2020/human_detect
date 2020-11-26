/*
 * File: remove_pf.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 26-Nov-2020 17:19:56
 */

/* Include Files */
#include "remove_pf.h"
#include "remove_pf_emxutil.h"
#include <math.h>

/* Function Declarations */
static int mul_s32_sat(int a, int b);
static void mul_wide_s32(int in0, int in1, unsigned int *ptrOutBitsHi, unsigned
  int *ptrOutBitsLo);
static double rt_roundd(double u);

/* Function Definitions */

/*
 * Arguments    : int a
 *                int b
 * Return Type  : int
 */
static int mul_s32_sat(int a, int b)
{
  int result;
  unsigned int u32_chi;
  unsigned int u32_clo;
  mul_wide_s32(a, b, &u32_chi, &u32_clo);
  if (((int)u32_chi > 0) || ((u32_chi == 0U) && (u32_clo >= 2147483648U))) {
    result = MAX_int32_T;
  } else if (((int)u32_chi < -1) || (((int)u32_chi == -1) && (u32_clo <
               2147483648U))) {
    result = MIN_int32_T;
  } else {
    result = (int)u32_clo;
  }

  return result;
}

/*
 * Arguments    : int in0
 *                int in1
 *                unsigned int *ptrOutBitsHi
 *                unsigned int *ptrOutBitsLo
 * Return Type  : void
 */
static void mul_wide_s32(int in0, int in1, unsigned int *ptrOutBitsHi, unsigned
  int *ptrOutBitsLo)
{
  unsigned int absIn0;
  unsigned int absIn1;
  int in0Hi;
  int in0Lo;
  int in1Hi;
  int in1Lo;
  unsigned int productLoLo;
  unsigned int outBitsLo;
  if (in0 < 0) {
    absIn0 = ~(unsigned int)in0 + 1U;
  } else {
    absIn0 = (unsigned int)in0;
  }

  if (in1 < 0) {
    absIn1 = ~(unsigned int)in1 + 1U;
  } else {
    absIn1 = (unsigned int)in1;
  }

  in0Hi = (int)(absIn0 >> 16U);
  in0Lo = (int)(absIn0 & 65535U);
  in1Hi = (int)(absIn1 >> 16U);
  in1Lo = (int)(absIn1 & 65535U);
  absIn0 = (unsigned int)in0Hi * in1Lo;
  absIn1 = (unsigned int)in0Lo * in1Hi;
  productLoLo = (unsigned int)in0Lo * in1Lo;
  in0Lo = 0;
  outBitsLo = productLoLo + (absIn1 << 16U);
  if (outBitsLo < productLoLo) {
    in0Lo = 1;
  }

  productLoLo = outBitsLo;
  outBitsLo += absIn0 << 16U;
  if (outBitsLo < productLoLo) {
    in0Lo++;
  }

  absIn0 = ((in0Lo + (unsigned int)in0Hi * in1Hi) + (absIn1 >> 16U)) + (absIn0 >>
    16U);
  if ((in0 != 0) && ((in1 != 0) && ((in0 > 0) != (in1 > 0)))) {
    absIn0 = ~absIn0;
    outBitsLo = ~outBitsLo;
    outBitsLo++;
    if (outBitsLo == 0U) {
      absIn0++;
    }
  }

  *ptrOutBitsHi = absIn0;
  *ptrOutBitsLo = outBitsLo;
}

/*
 * Arguments    : double u
 * Return Type  : double
 */
static double rt_roundd(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = 0.0;
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
 * Arguments    : const double data_data[]
 *                const int data_size[1]
 *                int time_accum
 *                int xhz
 *                emxArray_real_T *data_remove_pf
 * Return Type  : void
 */
void remove_pf(const double data_data[], const int data_size[1], int time_accum,
               int xhz, emxArray_real_T *data_remove_pf)
{
  int q1;
  int loop_ub;
  int y_tmp;
  double d;
  int i;
  int q0;
  int qY;
  int i1;
  double d1;
  int x;
  emxArray_real_T *part_2;
  int i2;
  int b_i;
  q1 = mul_s32_sat(xhz, time_accum);
  if (1 > q1) {
    loop_ub = 0;
  } else {
    loop_ub = q1;
  }

  y_tmp = mul_s32_sat(50, time_accum);
  d = rt_roundd((double)data_size[0] / (double)y_tmp);
  if (d < 2.147483648E+9) {
    if (d >= -2.147483648E+9) {
      i = (int)d;
    } else {
      i = MIN_int32_T;
    }
  } else {
    i = MAX_int32_T;
  }

  q0 = mul_s32_sat(y_tmp, i);
  if ((q0 < 0) && (q1 < MIN_int32_T - q0)) {
    qY = MIN_int32_T;
  } else if ((q0 > 0) && (q1 > MAX_int32_T - q0)) {
    qY = MAX_int32_T;
  } else {
    qY = q0 + q1;
  }

  if (qY > 2147483646) {
    qY = MAX_int32_T;
  } else {
    qY++;
  }

  if (qY > data_size[0]) {
    i = 0;
    i1 = 0;
  } else {
    i = qY - 1;
    i1 = data_size[0];
  }

  if (xhz > 1073741823) {
    q0 = MAX_int32_T;
  } else if (xhz <= -1073741824) {
    q0 = MIN_int32_T;
  } else {
    q0 = xhz << 1;
  }

  d1 = rt_roundd((double)data_size[0] / (double)mul_s32_sat(50, time_accum));
  if (d1 < 2.147483648E+9) {
    if (d1 >= -2.147483648E+9) {
      x = (int)d1;
    } else {
      x = MIN_int32_T;
    }
  } else {
    x = MAX_int32_T;
  }

  emxInit_real_T(&part_2, 2);
  q0 = mul_s32_sat(q0, time_accum);
  if ((y_tmp >= 0) && (q0 < y_tmp - MAX_int32_T)) {
    qY = MAX_int32_T;
  } else if ((y_tmp < 0) && (q0 > y_tmp - MIN_int32_T)) {
    qY = MIN_int32_T;
  } else {
    qY = y_tmp - q0;
  }

  i2 = part_2->size[0] * part_2->size[1];
  part_2->size[0] = qY;
  part_2->size[1] = x;
  emxEnsureCapacity_real_T(part_2, i2);
  if ((y_tmp >= 0) && (q0 < y_tmp - MAX_int32_T)) {
    qY = MAX_int32_T;
  } else if ((y_tmp < 0) && (q0 > y_tmp - MIN_int32_T)) {
    qY = MIN_int32_T;
  } else {
    qY = y_tmp - q0;
  }

  x *= qY;
  for (i2 = 0; i2 < x; i2++) {
    part_2->data[i2] = 0.0;
  }

  if (d < 2.147483648E+9) {
    if (d >= -2.147483648E+9) {
      i2 = (int)d;
    } else {
      i2 = MIN_int32_T;
    }
  } else {
    i2 = MAX_int32_T;
  }

  for (b_i = 0; b_i < i2; b_i++) {
    q0 = b_i + 1;
    if (q0 < -2147483647) {
      qY = MIN_int32_T;
    } else {
      qY = q0 - 1;
    }

    q0 = mul_s32_sat(y_tmp, qY);
    if ((q0 < 0) && (q1 < MIN_int32_T - q0)) {
      qY = MIN_int32_T;
    } else if ((q0 > 0) && (q1 > MAX_int32_T - q0)) {
      qY = MAX_int32_T;
    } else {
      qY = q0 + q1;
    }

    if (qY > 2147483646) {
      qY = MAX_int32_T;
    } else {
      qY++;
    }

    q0 = mul_s32_sat(y_tmp, b_i + 1);
    if ((q0 >= 0) && (q1 < q0 - MAX_int32_T)) {
      q0 = MAX_int32_T;
    } else if ((q0 < 0) && (q1 > q0 - MIN_int32_T)) {
      q0 = MIN_int32_T;
    } else {
      q0 -= q1;
    }

    if (qY > q0) {
      qY = 0;
      q0 = 0;
    } else {
      qY--;
    }

    x = q0 - qY;
    for (q0 = 0; q0 < x; q0++) {
      part_2->data[q0 + part_2->size[0] * b_i] = data_data[qY + q0];
    }
  }

  q0 = part_2->size[0] * part_2->size[1];
  i2 = data_remove_pf->size[0];
  data_remove_pf->size[0] = ((loop_ub + q0) + i1) - i;
  emxEnsureCapacity_real_T(data_remove_pf, i2);
  for (i2 = 0; i2 < loop_ub; i2++) {
    data_remove_pf->data[i2] = data_data[i2];
  }

  for (i2 = 0; i2 < q0; i2++) {
    data_remove_pf->data[i2 + loop_ub] = part_2->data[i2];
  }

  emxFree_real_T(&part_2);
  x = i1 - i;
  for (i1 = 0; i1 < x; i1++) {
    data_remove_pf->data[(i1 + loop_ub) + q0] = data_data[i + i1];
  }
}

/*
 * File trailer for remove_pf.c
 *
 * [EOF]
 */
