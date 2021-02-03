/*
 * File: find_peak_cv.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 17-Nov-2020 10:15:54
 */

/* Include Files */
#include "find_peak_cv.h"
#include "eml_setop.h"
#include "find_peak_cv_data.h"
#include "find_peak_cv_initialize.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */

/*
 * UNTITLED —∞’“Y÷·∑Â÷µµƒŒª÷√
 * Arguments    : const double y_value_data[]
 *                const int y_value_size[2]
 *                double peaks_data[]
 *                int peaks_size[2]
 *                double index_data[]
 *                int index_size[2]
 * Return Type  : void
 */
void find_peak_cv(const float y_value_data[], const int y_value_size, double
                  peaks_data[], int peaks_size[2], double index_data[], int
                  index_size[2])
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
  int iInfinite_data[1024];
  char previousdir;
  int iFinite_data[1024];
  int iPk_size[1];
  int iPk_data[1024];
  int c_data[2048];
  int c_size[1];
  int iInflect_data[1024];
  int iInflect_size[1];
  int iFinite_size[1];
  int y_data[2048];
  int idx_data[2048];
  if (!isInitialized_find_peak_cv) {
    find_peak_cv_initialize();
  }

  ny = y_value_size;
  nPk = 0;
  nInf = 0;
  dir = 'n';
  kfirst = 0;
  ykfirst = rtInf;
  isinfykfirst = true;
  for (k = 1; k <= ny; k++) {
    yk = y_value_data[k - 1];
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
    ykfirst = y_value_data[iFinite_data[k] - 1];
    if ((ykfirst > rtMinusInf) && (ykfirst - fmax(y_value_data[iFinite_data[k] -
          2], y_value_data[iFinite_data[k]]) >= 0.0)) {
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

  if (nPk > y_value_size) {
    ny = y_value_size;
    kfirst = y_value_size;
    if (0 <= ny - 1) {
      memcpy(&y_data[0], &idx_data[0], ny * sizeof(int));
    }

    nPk = y_value_size;
    if (0 <= kfirst - 1) {
      memcpy(&idx_data[0], &y_data[0], kfirst * sizeof(int));
    }
  }

  for (ny = 0; ny < nPk; ny++) {
    y_data[ny] = c_data[idx_data[ny] - 1];
  }

  peaks_size[0] = 1;
  peaks_size[1] = nPk;
  for (ny = 0; ny < nPk; ny++) {
    peaks_data[ny] = y_value_data[y_data[ny] - 1];
  }

  index_size[0] = 1;
  index_size[1] = nPk;
  for (ny = 0; ny < nPk; ny++) {
    index_data[ny] = (short)((short)(y_data[ny] - 1) + 1);
  }
}

/*
 * File trailer for find_peak_cv.c
 *
 * [EOF]
 */
