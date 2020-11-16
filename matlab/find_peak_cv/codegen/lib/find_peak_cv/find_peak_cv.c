/*
 * File: find_peak_cv.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 16-Nov-2020 14:04:28
 */

/* Include Files */
#include "find_peak_cv.h"
#include "find_peak_cv_data.h"
#include "find_peak_cv_initialize.h"
#include "findpeaks.h"
#include "rt_nonfinite.h"
#include "sort.h"

/* Function Definitions */

/*
 * UNTITLED —∞’“Y÷·∑Â÷µµƒŒª÷√
 * Arguments    : const double y_value_data[]
 *                const int y_value_size[1]
 *                const double x_value_data[]
 *                const int x_value_size[2]
 *                double peaks_data[]
 *                int peaks_size[1]
 *                double index_data[]
 *                int index_size[1]
 * Return Type  : void
 */
void find_peak_cv(const double y_value_data[], const int y_value_size[1], const
                  double x_value_data[], const int x_value_size[2], double
                  peaks_data[], int peaks_size[1], double index_data[], int
                  index_size[1])
{
  double locs_data[400];
  int locs_size[2];
  int iidx_data[400];
  int iidx_size[1];
  int loop_ub;
  int i;
  (void)x_value_size;
  if (!isInitialized_find_peak_cv) {
    find_peak_cv_initialize();
  }

  findpeaks(y_value_data, y_value_size, x_value_data, peaks_data, peaks_size,
            locs_data, locs_size);
  sort(peaks_data, peaks_size, iidx_data, iidx_size);
  index_size[0] = iidx_size[0];
  loop_ub = iidx_size[0];
  for (i = 0; i < loop_ub; i++) {
    index_data[i] = iidx_data[i];
  }
}

/*
 * File trailer for find_peak_cv.c
 *
 * [EOF]
 */
