/*
 * File: test.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 11-Nov-2020 11:31:14
 */

/* Include Files */
#include "test.h"
#include <math.h>

/* Function Definitions */

/*
 * test 测试matlab coder
 *    求src矩阵的逆矩阵，并返回结果到res中
 * Arguments    : const float src[9]
 *                float res[9]
 * Return Type  : void
 */
void test(const float src[9], float res[9])
{
  int p1;
  float x[9];
  int p2;
  int p3;
  float absx11;
  float absx21;
  float absx31;
  int itmp;
  for (p1 = 0; p1 < 9; p1++) {
    x[p1] = src[p1];
  }

  p1 = 0;
  p2 = 3;
  p3 = 6;
  absx11 = fabsf(src[0]);
  absx21 = fabsf(src[1]);
  absx31 = fabsf(src[2]);
  if ((absx21 > absx11) && (absx21 > absx31)) {
    p1 = 3;
    p2 = 0;
    x[0] = src[1];
    x[1] = src[0];
    x[3] = src[4];
    x[4] = src[3];
    x[6] = src[7];
    x[7] = src[6];
  } else {
    if (absx31 > absx11) {
      p1 = 6;
      p3 = 0;
      x[0] = src[2];
      x[2] = src[0];
      x[3] = src[5];
      x[5] = src[3];
      x[6] = src[8];
      x[8] = src[6];
    }
  }

  x[1] /= x[0];
  x[2] /= x[0];
  x[4] -= x[1] * x[3];
  x[5] -= x[2] * x[3];
  x[7] -= x[1] * x[6];
  x[8] -= x[2] * x[6];
  if (fabsf(x[5]) > fabsf(x[4])) {
    itmp = p2;
    p2 = p3;
    p3 = itmp;
    absx11 = x[1];
    x[1] = x[2];
    x[2] = absx11;
    absx11 = x[4];
    x[4] = x[5];
    x[5] = absx11;
    absx11 = x[7];
    x[7] = x[8];
    x[8] = absx11;
  }

  x[5] /= x[4];
  x[8] -= x[5] * x[7];
  absx11 = (x[5] * x[1] - x[2]) / x[8];
  absx21 = -(x[1] + x[7] * absx11) / x[4];
  res[p1] = ((1.0F - x[3] * absx21) - x[6] * absx11) / x[0];
  res[p1 + 1] = absx21;
  res[p1 + 2] = absx11;
  absx11 = -x[5] / x[8];
  absx21 = (1.0F - x[7] * absx11) / x[4];
  res[p2] = -(x[3] * absx21 + x[6] * absx11) / x[0];
  res[p2 + 1] = absx21;
  res[p2 + 2] = absx11;
  absx11 = 1.0F / x[8];
  absx21 = -x[7] * absx11 / x[4];
  res[p3] = -(x[3] * absx21 + x[6] * absx11) / x[0];
  res[p3 + 1] = absx21;
  res[p3 + 2] = absx11;
}

/*
 * File trailer for test.c
 *
 * [EOF]
 */
