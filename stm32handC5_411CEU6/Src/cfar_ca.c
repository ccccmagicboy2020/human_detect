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
# include "sys.h"
#include <math.h>
#include <stdio.h>

/* Function Declarations */
//static double rt_powd_snf(double u0, double u1);

/* Function Definitions */

/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
//static double rt_powd_snf(double u0, double u1)
//{
//  double y;
//  double d;
//  double d1;
//  if (rtIsNaN(u0) || rtIsNaN(u1)) {
//    y = rtNaN;
//  } else {
//    d = fabs(u0);
//    d1 = fabs(u1);
//    if (rtIsInf(u1)) {
//      if (d == 1.0) {
//        y = 1.0;
//      } else if (d > 1.0) {
//        if (u1 > 0.0) {
//          y = rtInf;
//        } else {
//          y = 0.0;
//        }
//      } else if (u1 > 0.0) {
//        y = 0.0;
//      } else {
//        y = rtInf;
//      }
//    } else if (d1 == 0.0) {
//      y = 1.0;
//    } else if (d1 == 1.0) {
//      if (u1 > 0.0) {
//        y = u0;
//      } else {
//        y = 1.0 / u0;
//      }
//    } else if (u1 == 2.0) {
//      y = u0 * u0;
//    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
//      y = sqrt(u0);
//    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
//      y = rtNaN;
//    } else {
//      y = pow(u0, u1);
//    }
//  }

//  return y;
//}

/*
 * Arguments    : const double xc[1000]
 *                double N
 *                double pro_N
 *                double PAD
 *                emxArray_real_T *b_index
 *                emxArray_real_T *XT
 * Return Type  : void
 */
void cfar_ca(const float xc_data[], int xc_size, double N, double pro_N, double PAD, float b_index[], int b_index_size[2], float XT[])
{
  double alpha;
  double index0;
  double index1;
  int i;
  int loop_ub;
  int b_i;
  double c_i;
  double y;
  int i1;
  int i2;
  int i3;
  int k;
	
	int temp;
	
  alpha = N * (pow(PAD, -1.0 / N) - 1.0);

  /*  门限系数 */
  index0 = ceil((N / 2.0 + 1.0) + pro_N / 2.0);
  index1 = ceil(((double)xc_size - N / 2.0) - pro_N / 2.0);

	loop_ub = (int)(index1 - index0);
	b_index_size[1] = loop_ub + 1;

	for (i = 0; i <= loop_ub; i++) {
	b_index[i] = index0 + (double)i;
	}

  /*  中间那一部分 */
  loop_ub = b_index_size[1];
  for (i = 0; i < loop_ub; i++) {
    XT[i] = 0.0;
  }

  /*  门限 */
  i = (int)(index1 + (1.0 - index0));
  for (b_i = 0; b_i < i; b_i++) {
    c_i = index0 + (double)b_i;
    index1 = (c_i - N / 2.0) - pro_N / 2.0;
    y = (c_i - pro_N / 2.0) - 1.0;
    if (index1 > y) {
      i1 = -1;
      loop_ub = -1;
    } else {
      i1 = (int)index1 - 2;
      loop_ub = (int)y - 1;
    }

    index1 = (c_i + pro_N / 2.0) + 1.0;
    y = (c_i + N / 2.0) + pro_N / 2.0;
    if (index1 > y) {
      i2 = -1;
      i3 = -1;
    } else {
      i2 = (int)index1 - 2;
      i3 = (int)y - 1;
    }

    loop_ub -= i1;
    if (loop_ub == 0) {
      index1 = 0.0;
    } else {
      index1 = xc_data[i1 + 1];
      for (k = 2; k <= loop_ub; k++) {
        index1 += xc_data[i1 + k];
      }
    }

    loop_ub = i3 - i2;
    if (loop_ub == 0) {
      y = 0.0;
    } else {
      y = xc_data[i2 + 1];
      for (k = 2; k <= loop_ub; k++) {
        y += xc_data[i2 + k];
      }
    }

		temp = (int)((c_i - N / 2.0) - pro_N / 2.0) - 1;
    XT[temp] = (index1 + y) / N *
      alpha;
	  
		//printf("index and XT: %d - %lf\r\n", temp, XT[temp]);
  }
}

/*
 * File trailer for cfar_ca.c
 *
 * [EOF]
 */
