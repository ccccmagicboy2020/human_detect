/*
 * File: auto_trip_50hz.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 11-Nov-2020 16:25:33
 */

/* Include Files */
#include "auto_trip_50hz.h"
#include <math.h>
#include <string.h>

/* Function Definitions */

/*
 * Arguments    : const float src[2560]
 *                float fs
 *                float lamda
 *                const float peaks[3]
 *                float res[2560]
 * Return Type  : void
 */
void auto_trip_50hz(const float src[2560], float fs, float lamda, const float
                    peaks[3], float res[2560])
{
  int i;
  int b_i;
  float P[36];
  static const signed char iv[36] = { 100, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0,
    0, 100, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 100
  };

  float w[6];
  float Eta;
  float mid[6];
  float y;
  float f;
  int i1;
  float K[6];
  float fv[36];
  int i2;
  float fv1[36];
  static const signed char iv1[36] = { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 };

  /*  生成对应的单位矩阵 */
  /*  小正数 保证矩阵P非奇异 */
  /* y_out = zeros(len, 1); */
  /* w_out = zeros(len, M_FIR); */
  for (i = 0; i < 36; i++) {
    P[i] = iv[i];
  }

  for (b_i = 0; b_i < 6; b_i++) {
    w[b_i] = 0.0F;
  }

  for (b_i = 0; b_i < 2560; b_i++) {
    /*  输入新的期望信号 */
    Eta = 6.28318548F * peaks[0] * (((float)b_i + 1.0F) - 1.0F) / fs;
    mid[0] = sinf(Eta);
    mid[1] = cosf(Eta);
    Eta = 6.28318548F * peaks[1] * (((float)b_i + 1.0F) - 1.0F) / fs;
    mid[2] = sinf(Eta);
    mid[3] = cosf(Eta);
    Eta = 6.28318548F * peaks[2] * (((float)b_i + 1.0F) - 1.0F) / fs;
    mid[4] = sinf(Eta);
    mid[5] = cosf(Eta);
    Eta = 0.0F;
    for (i = 0; i < 6; i++) {
      f = 0.0F;
      for (i1 = 0; i1 < 6; i1++) {
        f += mid[i1] * P[i1 + 6 * i];
      }

      Eta += f * mid[i];
    }

    y = lamda + Eta;

    /*  计算增益矢量 */
    /*  计算FIR滤波器输出 */
    Eta = 0.0F;
    for (i = 0; i < 6; i++) {
      f = 0.0F;
      for (i1 = 0; i1 < 6; i1++) {
        f += P[i + 6 * i1] * mid[i1];
      }

      K[i] = f / y;
      Eta += mid[i] * w[i];
    }

    Eta = src[b_i] - Eta;

    /*  计算估计的误差 */
    /*  计算滤波器系数矢量 */
    for (i = 0; i < 6; i++) {
      w[i] += K[i] * Eta;
      for (i1 = 0; i1 < 6; i1++) {
        i2 = i1 + 6 * i;
        fv1[i2] = (float)iv1[i2] - K[i1] * mid[i];
      }
    }

    for (i = 0; i < 6; i++) {
      for (i1 = 0; i1 < 6; i1++) {
        f = 0.0F;
        for (i2 = 0; i2 < 6; i2++) {
          f += fv1[i + 6 * i2] * P[i2 + 6 * i1];
        }

        fv[i + 6 * i1] = f / lamda;
      }
    }

    memcpy(&P[0], &fv[0], 36U * sizeof(float));

    /*  计算误差相关矩阵 */
    /*  变量更替 */
    /*  滤波结果存储 */
    res[b_i] = Eta;
  }
}

/*
 * File trailer for auto_trip_50hz.c
 *
 * [EOF]
 */
