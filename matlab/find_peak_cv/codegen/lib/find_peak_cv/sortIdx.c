/*
 * File: sortIdx.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 16-Nov-2020 14:04:28
 */

/* Include Files */
#include "sortIdx.h"
#include "find_peak_cv.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Declarations */
static void merge(int idx_data[], double x_data[], int offset, int np, int nq,
                  int iwork_data[], double xwork_data[]);
static void merge_block(int idx_data[], double x_data[], int offset, int n, int
  preSortLevel, int iwork_data[], double xwork_data[]);
static void merge_pow2_block(int idx_data[], double x_data[], int offset);

/* Function Definitions */

/*
 * Arguments    : int idx_data[]
 *                double x_data[]
 *                int offset
 *                int np
 *                int nq
 *                int iwork_data[]
 *                double xwork_data[]
 * Return Type  : void
 */
static void merge(int idx_data[], double x_data[], int offset, int np, int nq,
                  int iwork_data[], double xwork_data[])
{
  int n_tmp;
  int j;
  int p;
  int iout;
  int q;
  int exitg1;
  if (nq != 0) {
    n_tmp = np + nq;
    for (j = 0; j < n_tmp; j++) {
      iout = offset + j;
      iwork_data[j] = idx_data[iout];
      xwork_data[j] = x_data[iout];
    }

    p = 0;
    q = np;
    iout = offset - 1;
    do {
      exitg1 = 0;
      iout++;
      if (xwork_data[p] >= xwork_data[q]) {
        idx_data[iout] = iwork_data[p];
        x_data[iout] = xwork_data[p];
        if (p + 1 < np) {
          p++;
        } else {
          exitg1 = 1;
        }
      } else {
        idx_data[iout] = iwork_data[q];
        x_data[iout] = xwork_data[q];
        if (q + 1 < n_tmp) {
          q++;
        } else {
          q = iout - p;
          for (j = p + 1; j <= np; j++) {
            iout = q + j;
            idx_data[iout] = iwork_data[j - 1];
            x_data[iout] = xwork_data[j - 1];
          }

          exitg1 = 1;
        }
      }
    } while (exitg1 == 0);
  }
}

/*
 * Arguments    : int idx_data[]
 *                double x_data[]
 *                int offset
 *                int n
 *                int preSortLevel
 *                int iwork_data[]
 *                double xwork_data[]
 * Return Type  : void
 */
static void merge_block(int idx_data[], double x_data[], int offset, int n, int
  preSortLevel, int iwork_data[], double xwork_data[])
{
  int nPairs;
  int bLen;
  int tailOffset;
  int nTail;
  nPairs = n >> preSortLevel;
  bLen = 1 << preSortLevel;
  while (nPairs > 1) {
    if ((nPairs & 1) != 0) {
      nPairs--;
      tailOffset = bLen * nPairs;
      nTail = n - tailOffset;
      if (nTail > bLen) {
        merge(idx_data, x_data, offset + tailOffset, bLen, nTail - bLen,
              iwork_data, xwork_data);
      }
    }

    tailOffset = bLen << 1;
    nPairs >>= 1;
    for (nTail = 0; nTail < nPairs; nTail++) {
      merge(idx_data, x_data, offset + nTail * tailOffset, bLen, bLen,
            iwork_data, xwork_data);
    }

    bLen = tailOffset;
  }

  if (n > bLen) {
    merge(idx_data, x_data, offset, bLen, n - bLen, iwork_data, xwork_data);
  }
}

/*
 * Arguments    : int idx_data[]
 *                double x_data[]
 *                int offset
 * Return Type  : void
 */
static void merge_pow2_block(int idx_data[], double x_data[], int offset)
{
  int b;
  int bLen;
  int bLen2;
  int nPairs;
  int k;
  int blockOffset;
  int j;
  int p;
  int iout;
  int q;
  int iwork[256];
  double xwork[256];
  int exitg1;
  for (b = 0; b < 6; b++) {
    bLen = 1 << (b + 2);
    bLen2 = bLen << 1;
    nPairs = 256 >> (b + 3);
    for (k = 0; k < nPairs; k++) {
      blockOffset = offset + k * bLen2;
      for (j = 0; j < bLen2; j++) {
        iout = blockOffset + j;
        iwork[j] = idx_data[iout];
        xwork[j] = x_data[iout];
      }

      p = 0;
      q = bLen;
      iout = blockOffset - 1;
      do {
        exitg1 = 0;
        iout++;
        if (xwork[p] >= xwork[q]) {
          idx_data[iout] = iwork[p];
          x_data[iout] = xwork[p];
          if (p + 1 < bLen) {
            p++;
          } else {
            exitg1 = 1;
          }
        } else {
          idx_data[iout] = iwork[q];
          x_data[iout] = xwork[q];
          if (q + 1 < bLen2) {
            q++;
          } else {
            iout -= p;
            for (j = p + 1; j <= bLen; j++) {
              q = iout + j;
              idx_data[q] = iwork[j - 1];
              x_data[q] = xwork[j - 1];
            }

            exitg1 = 1;
          }
        }
      } while (exitg1 == 0);
    }
  }
}

/*
 * Arguments    : double x_data[]
 *                const int x_size[1]
 *                int idx_data[]
 *                int idx_size[1]
 * Return Type  : void
 */
void sortIdx(double x_data[], const int x_size[1], int idx_data[], int idx_size
             [1])
{
  short unnamed_idx_0;
  int ib;
  int n;
  int b_n;
  double x4[4];
  short idx4[4];
  int iwork_data[400];
  double xwork_data[400];
  int nNaNs;
  int k;
  int i4;
  int idx_tmp;
  signed char perm[4];
  int quartetOffset;
  int i1;
  int i3;
  double d;
  double d1;
  unnamed_idx_0 = (short)x_size[0];
  idx_size[0] = unnamed_idx_0;
  ib = unnamed_idx_0;
  if (0 <= ib - 1) {
    memset(&idx_data[0], 0, ib * sizeof(int));
  }

  if (x_size[0] != 0) {
    n = x_size[0];
    b_n = x_size[0];
    x4[0] = 0.0;
    idx4[0] = 0;
    x4[1] = 0.0;
    idx4[1] = 0;
    x4[2] = 0.0;
    idx4[2] = 0;
    x4[3] = 0.0;
    idx4[3] = 0;
    ib = unnamed_idx_0;
    if (0 <= ib - 1) {
      memset(&iwork_data[0], 0, ib * sizeof(int));
    }

    ib = x_size[0];
    if (0 <= ib - 1) {
      memset(&xwork_data[0], 0, ib * sizeof(double));
    }

    nNaNs = 0;
    ib = -1;
    for (k = 0; k < b_n; k++) {
      if (rtIsNaN(x_data[k])) {
        idx_tmp = (b_n - nNaNs) - 1;
        idx_data[idx_tmp] = k + 1;
        xwork_data[idx_tmp] = x_data[k];
        nNaNs++;
      } else {
        ib++;
        idx4[ib] = (short)(k + 1);
        x4[ib] = x_data[k];
        if (ib + 1 == 4) {
          quartetOffset = k - nNaNs;
          if (x4[0] >= x4[1]) {
            i1 = 1;
            ib = 2;
          } else {
            i1 = 2;
            ib = 1;
          }

          if (x4[2] >= x4[3]) {
            i3 = 3;
            i4 = 4;
          } else {
            i3 = 4;
            i4 = 3;
          }

          d = x4[i1 - 1];
          d1 = x4[i3 - 1];
          if (d >= d1) {
            d = x4[ib - 1];
            if (d >= d1) {
              perm[0] = (signed char)i1;
              perm[1] = (signed char)ib;
              perm[2] = (signed char)i3;
              perm[3] = (signed char)i4;
            } else if (d >= x4[i4 - 1]) {
              perm[0] = (signed char)i1;
              perm[1] = (signed char)i3;
              perm[2] = (signed char)ib;
              perm[3] = (signed char)i4;
            } else {
              perm[0] = (signed char)i1;
              perm[1] = (signed char)i3;
              perm[2] = (signed char)i4;
              perm[3] = (signed char)ib;
            }
          } else {
            d1 = x4[i4 - 1];
            if (d >= d1) {
              if (x4[ib - 1] >= d1) {
                perm[0] = (signed char)i3;
                perm[1] = (signed char)i1;
                perm[2] = (signed char)ib;
                perm[3] = (signed char)i4;
              } else {
                perm[0] = (signed char)i3;
                perm[1] = (signed char)i1;
                perm[2] = (signed char)i4;
                perm[3] = (signed char)ib;
              }
            } else {
              perm[0] = (signed char)i3;
              perm[1] = (signed char)i4;
              perm[2] = (signed char)i1;
              perm[3] = (signed char)ib;
            }
          }

          idx_tmp = perm[0] - 1;
          idx_data[quartetOffset - 3] = idx4[idx_tmp];
          i3 = perm[1] - 1;
          idx_data[quartetOffset - 2] = idx4[i3];
          ib = perm[2] - 1;
          idx_data[quartetOffset - 1] = idx4[ib];
          i1 = perm[3] - 1;
          idx_data[quartetOffset] = idx4[i1];
          x_data[quartetOffset - 3] = x4[idx_tmp];
          x_data[quartetOffset - 2] = x4[i3];
          x_data[quartetOffset - 1] = x4[ib];
          x_data[quartetOffset] = x4[i1];
          ib = -1;
        }
      }
    }

    i4 = (b_n - nNaNs) - 1;
    if (ib + 1 > 0) {
      perm[1] = 0;
      perm[2] = 0;
      perm[3] = 0;
      if (ib + 1 == 1) {
        perm[0] = 1;
      } else if (ib + 1 == 2) {
        if (x4[0] >= x4[1]) {
          perm[0] = 1;
          perm[1] = 2;
        } else {
          perm[0] = 2;
          perm[1] = 1;
        }
      } else if (x4[0] >= x4[1]) {
        if (x4[1] >= x4[2]) {
          perm[0] = 1;
          perm[1] = 2;
          perm[2] = 3;
        } else if (x4[0] >= x4[2]) {
          perm[0] = 1;
          perm[1] = 3;
          perm[2] = 2;
        } else {
          perm[0] = 3;
          perm[1] = 1;
          perm[2] = 2;
        }
      } else if (x4[0] >= x4[2]) {
        perm[0] = 2;
        perm[1] = 1;
        perm[2] = 3;
      } else if (x4[1] >= x4[2]) {
        perm[0] = 2;
        perm[1] = 3;
        perm[2] = 1;
      } else {
        perm[0] = 3;
        perm[1] = 2;
        perm[2] = 1;
      }

      for (k = 0; k <= ib; k++) {
        idx_tmp = perm[k] - 1;
        i3 = (i4 - ib) + k;
        idx_data[i3] = idx4[idx_tmp];
        x_data[i3] = x4[idx_tmp];
      }
    }

    ib = (nNaNs >> 1) + 1;
    for (k = 0; k <= ib - 2; k++) {
      i1 = (i4 + k) + 1;
      i3 = idx_data[i1];
      idx_tmp = (b_n - k) - 1;
      idx_data[i1] = idx_data[idx_tmp];
      idx_data[idx_tmp] = i3;
      x_data[i1] = xwork_data[idx_tmp];
      x_data[idx_tmp] = xwork_data[i1];
    }

    if ((nNaNs & 1) != 0) {
      ib += i4;
      x_data[ib] = xwork_data[ib];
    }

    i3 = n - nNaNs;
    ib = 2;
    if (i3 > 1) {
      if (n >= 256) {
        i1 = i3 >> 8;
        if (i1 > 0) {
          for (ib = 0; ib < i1; ib++) {
            merge_pow2_block(idx_data, x_data, ib << 8);
          }

          ib = i1 << 8;
          i1 = i3 - ib;
          if (i1 > 0) {
            merge_block(idx_data, x_data, ib, i1, 2, iwork_data, xwork_data);
          }

          ib = 8;
        }
      }

      merge_block(idx_data, x_data, 0, i3, ib, iwork_data, xwork_data);
    }

    if ((nNaNs > 0) && (i3 > 0)) {
      for (k = 0; k < nNaNs; k++) {
        ib = i3 + k;
        xwork_data[k] = x_data[ib];
        iwork_data[k] = idx_data[ib];
      }

      for (k = i3; k >= 1; k--) {
        ib = (nNaNs + k) - 1;
        x_data[ib] = x_data[k - 1];
        idx_data[ib] = idx_data[k - 1];
      }

      if (0 <= nNaNs - 1) {
        memcpy(&x_data[0], &xwork_data[0], nNaNs * sizeof(double));
        memcpy(&idx_data[0], &iwork_data[0], nNaNs * sizeof(int));
      }
    }
  }
}

/*
 * File trailer for sortIdx.c
 *
 * [EOF]
 */
