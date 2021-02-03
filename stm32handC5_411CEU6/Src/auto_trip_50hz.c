/*
 * File: auto_trip_50hz.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 13-Nov-2020 15:54:55
 */

/* Include Files */
#include "auto_trip_50hz.h"
#include <math.h>
#include <string.h>
#include "stdio.h"

/* Function Definitions */

/*
 * Arguments    : const double src[2560]
 *                double fs
 *                double lamda
 *                const double peaks[3]
 *                double res[2560]
 * Return Type  : void
 */
void auto_trip_50hz(const float src[2560], double fs, double lamda, const
                    double peaks[3], float res[2560])
{
  int i;
  int b_i;
  double P[36];
  static const signed char iv[36] = { 100, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0,
    0, 100, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 100
  };

  double w[6];
  double Eta;
  double mid[6];
  double b_lamda;
  double d;
  int i1;
  double K[6];
  double b_I[36];
  int I_tmp;
  double c_I[36];
  static const signed char d_I[36] = { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 };

  /*  生成对应的单位矩阵 */
  /*  小正数 保证矩阵P非奇异 */
  /* y_out = zeros(len, 1); */
  /* w_out = zeros(len, M_FIR); */
  for (i = 0; i < 36; i++) {
    P[i] = iv[i];
  }

  for (b_i = 0; b_i < 6; b_i++) {
    w[b_i] = 0.0;
  }

  for (b_i = 0; b_i < 2560; b_i++) {
    /*  输入新的期望信号 */
    Eta = 6.2831853071795862 * peaks[0] * (((double)b_i + 1.0) - 1.0) / fs;
    mid[0] = sin(Eta);
    mid[1] = cos(Eta);
    Eta = 6.2831853071795862 * peaks[1] * (((double)b_i + 1.0) - 1.0) / fs;
    mid[2] = sin(Eta);
    mid[3] = cos(Eta);
    Eta = 6.2831853071795862 * peaks[2] * (((double)b_i + 1.0) - 1.0) / fs;
    mid[4] = sin(Eta);
    mid[5] = cos(Eta);
    Eta = 0.0;
    for (i = 0; i < 6; i++) {
      d = 0.0;
      for (i1 = 0; i1 < 6; i1++) {
        d += mid[i1] * P[i1 + 6 * i];
      }

      Eta += d * mid[i];
    }

    b_lamda = lamda + Eta;
		//printf("%lf\r\n", b_lamda);

    /*  计算增益矢量 */
    /*  计算FIR滤波器输出 */
    Eta = 0.0;
    for (i = 0; i < 6; i++) {
      d = 0.0;
      for (i1 = 0; i1 < 6; i1++) {
        d += P[i + 6 * i1] * mid[i1];
      }

      K[i] = d / b_lamda;
      Eta += mid[i] * w[i];
    }

    Eta = src[b_i] - Eta;
		
		res[b_i] = Eta;
		//printf("%lf\r\n", Eta);

    /*  计算估计的误差 */
    /*  计算滤波器系数矢量 */
    for (i = 0; i < 6; i++) {
      w[i] += K[i] * Eta;
      for (i1 = 0; i1 < 6; i1++) {
        I_tmp = i1 + 6 * i;
        c_I[I_tmp] = (double)d_I[I_tmp] - K[i1] * mid[i];
      }
    }

    for (i = 0; i < 6; i++) {
      for (i1 = 0; i1 < 6; i1++) {
        d = 0.0;
        for (I_tmp = 0; I_tmp < 6; I_tmp++) {
          d += c_I[i + 6 * I_tmp] * P[I_tmp + 6 * i1];
        }

        b_I[i + 6 * i1] = d / lamda;
      }
    }
		
		//printf("bingo\r\n");
		
//		for (i = 0; i< 36; i++)
//		{
//			P[i] = b_I[i];
//			printf("P-%lf\r\n", P[i]);
//		}
    memcpy(&P[0], &b_I[0], 36U * sizeof(double));

    /*  计算误差相关矩阵 */
    /*  变量更替 */
    /*  滤波结果存储 */
    
		
		//printf("%d - %lf\r\n", b_i, Eta);
  }
}

/*
 * File trailer for auto_trip_50hz.c
 *
 * [EOF]
 */
