/*
 * File: main.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 16-Nov-2020 14:04:28
 */

/*************************************************************************/
/* This automatically generated example C main file shows how to call    */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/*************************************************************************/

/* Include Files */
#include "main.h"
#include "find_peak_cv.h"
#include "find_peak_cv_terminate.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static void argInit_1xd200_real_T(double result_data[], int result_size[2]);
static void argInit_d200x1_real_T(double result_data[], int result_size[1]);
static double argInit_real_T(void);
static void main_find_peak_cv(void);

/* Function Definitions */

/*
 * Arguments    : double result_data[]
 *                int result_size[2]
 * Return Type  : void
 */
static void argInit_1xd200_real_T(double result_data[], int result_size[2])
{
  int idx1;

  /* Set the size of the array.
     Change this size to the value that the application requires. */
  result_size[0] = 1;
  result_size[1] = 2;

  /* Loop over the array to initialize each element. */
  for (idx1 = 0; idx1 < 2; idx1++) {
    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result_data[idx1] = argInit_real_T();
  }
}

/*
 * Arguments    : double result_data[]
 *                int result_size[1]
 * Return Type  : void
 */
static void argInit_d200x1_real_T(double result_data[], int result_size[1])
{
  int idx0;

  /* Set the size of the array.
     Change this size to the value that the application requires. */
  result_size[0] = 2;

  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < 2; idx0++) {
    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result_data[idx0] = argInit_real_T();
  }
}

/*
 * Arguments    : void
 * Return Type  : double
 */
static double argInit_real_T(void)
{
  return 0.0;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void main_find_peak_cv(void)
{
  double y_value_data[200];
  int y_value_size[1];
  double x_value_data[200];
  int x_value_size[2];
  double peaks_data[400];
  int peaks_size[1];
  double index_data[400];
  int index_size[1];

  /* Initialize function 'find_peak_cv' input arguments. */
  /* Initialize function input argument 'y_value'. */
  argInit_d200x1_real_T(y_value_data, y_value_size);

  /* Initialize function input argument 'x_value'. */
  argInit_1xd200_real_T(x_value_data, x_value_size);

  /* Call the entry-point 'find_peak_cv'. */
  find_peak_cv(y_value_data, y_value_size, x_value_data, x_value_size,
               peaks_data, peaks_size, index_data, index_size);
}

/*
 * Arguments    : int argc
 *                const char * const argv[]
 * Return Type  : int
 */
int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;

  /* The initialize function is being called automatically from your entry-point function. So, a call to initialize is not included here. */
  /* Invoke the entry-point functions.
     You can call entry-point functions multiple times. */
  main_find_peak_cv();

  /* Terminate the application.
     You do not need to do this more than one time. */
  find_peak_cv_terminate();
  return 0;
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
