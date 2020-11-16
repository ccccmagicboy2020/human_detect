/*
 * File: findpeaks.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 16-Nov-2020 14:04:28
 */

/* Include Files */
#include "findpeaks.h"
#include "eml_setop.h"
#include "find_peak_cv.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */

/*
 * Arguments    : const double Yin_data[]
 *                const int Yin_size[1]
 *                const double varargin_1_data[]
 *                double Ypk_data[]
 *                int Ypk_size[1]
 *                double Xpk_data[]
 *                int Xpk_size[2]
 * Return Type  : void
 */
void findpeaks(const double Yin_data[], const int Yin_size[1], const double
               varargin_1_data[], double Ypk_data[], int Ypk_size[1], double
               Xpk_data[], int Xpk_size[2])
{
  int ny;
  int nPk;
  int nInf;
  char dir;
  int kfirst;
  double ykfirst;
  bool isinfykfirst;
  int k;
  double yk;
  bool isinfyk;
  int iInfinite_size[1];
  int iInfinite_data[200];
  char previousdir;
  int iFinite_data[200];
  int iPk_size[1];
  int iPk_data[200];
  int c_data[400];
  int c_size[1];
  int iInflect_data[200];
  int iInflect_size[1];
  int iFinite_size[1];
  int y_data[400];
  int idx_data[400];
  ny = Yin_size[0];
  nPk = 0;
  nInf = 0;
  dir = 'n';
  kfirst = 0;
  ykfirst = rtInf;
  isinfykfirst = true;
  for (k = 1; k <= ny; k++) {
    yk = Yin_data[k - 1];
    if (rtIsNaN(yk)) {
      yk = rtInf;
      isinfyk = true;
    } else if (rtIsInf(yk) && (yk > 0.0)) {
      isinfyk = true;
      nInf++;
      iInfinite_data[nInf - 1] = k;
    } else {
      isinfyk = false;
    }

    if (yk != ykfirst) {
      previousdir = dir;
      if (isinfyk || isinfykfirst) {
        dir = 'n';
      } else if (yk < ykfirst) {
        dir = 'd';
        if (('d' != previousdir) && (previousdir == 'i')) {
          nPk++;
          iFinite_data[nPk - 1] = kfirst;
        }
      } else {
        dir = 'i';
      }

      ykfirst = yk;
      kfirst = k;
      isinfykfirst = isinfyk;
    }
  }

  if (1 > nPk) {
    ny = 0;
  } else {
    ny = nPk;
  }

  if (1 > nInf) {
    iInfinite_size[0] = 0;
  } else {
    iInfinite_size[0] = nInf;
  }

  nPk = 0;
  for (k = 0; k < ny; k++) {
    ykfirst = Yin_data[iFinite_data[k] - 1];
    if ((ykfirst > rtMinusInf) && (ykfirst - fmax(Yin_data[iFinite_data[k] - 2],
          Yin_data[iFinite_data[k]]) >= 0.0)) {
      nPk++;
      iPk_data[nPk - 1] = iFinite_data[k];
    }
  }

  if (1 > nPk) {
    iPk_size[0] = 0;
  } else {
    iPk_size[0] = nPk;
  }

  do_vectors(iPk_data, iPk_size, iInfinite_data, iInfinite_size, c_data, c_size,
             iInflect_data, iInflect_size, iFinite_data, iFinite_size);
  if (c_size[0] < 1) {
    nPk = 0;
  } else {
    nPk = c_size[0];
  }

  if (nPk > 0) {
    y_data[0] = 1;
    ny = 1;
    for (k = 2; k <= nPk; k++) {
      ny++;
      y_data[k - 1] = ny;
    }
  }

  if (0 <= nPk - 1) {
    memcpy(&idx_data[0], &y_data[0], nPk * sizeof(int));
  }

  if (nPk > Yin_size[0]) {
    ny = Yin_size[0];
    kfirst = Yin_size[0];
    if (0 <= ny - 1) {
      memcpy(&y_data[0], &idx_data[0], ny * sizeof(int));
    }

    nPk = Yin_size[0];
    if (0 <= kfirst - 1) {
      memcpy(&idx_data[0], &y_data[0], kfirst * sizeof(int));
    }
  }

  for (ny = 0; ny < nPk; ny++) {
    y_data[ny] = c_data[idx_data[ny] - 1];
  }

  Ypk_size[0] = nPk;
  for (ny = 0; ny < nPk; ny++) {
    Ypk_data[ny] = Yin_data[y_data[ny] - 1];
  }

  Xpk_size[0] = 1;
  Xpk_size[1] = nPk;
  for (ny = 0; ny < nPk; ny++) {
    Xpk_data[ny] = varargin_1_data[y_data[ny] - 1];
  }
}

/*
 * File trailer for findpeaks.c
 *
 * [EOF]
 */
