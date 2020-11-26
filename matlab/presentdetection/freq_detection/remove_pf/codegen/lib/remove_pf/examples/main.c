/*
 * File: main.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 26-Nov-2020 17:19:56
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
#include "remove_pf.h"
#include "remove_pf_emxAPI.h"
#include "remove_pf_terminate.h"

/* Function Declarations */
static void argInit_d2048x1_real_T(double result_data[], int result_size[1]);
static int argInit_int32_T(void);
static double argInit_real_T(void);
static void main_remove_pf(void);

/* Function Definitions */

/*
 * Arguments    : double result_data[]
 *                int result_size[1]
 * Return Type  : void
 */
static void argInit_d2048x1_real_T(double result_data[], int result_size[1])
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
 * Return Type  : int
 */
static int argInit_int32_T(void)
{
  return 0;
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
static void main_remove_pf(void)
{
  emxArray_real_T *data_remove_pf;
  double data_data[2048];
  int data_size[1];
  int time_accum_tmp;
  emxInitArray_real_T(&data_remove_pf, 1);

  /* Initialize function 'remove_pf' input arguments. */
  /* Initialize function input argument 'data'. */
  argInit_d2048x1_real_T(data_data, data_size);
  time_accum_tmp = argInit_int32_T();

  /* Call the entry-point 'remove_pf'. */
  remove_pf(data_data, data_size, time_accum_tmp, time_accum_tmp, data_remove_pf);
  emxDestroyArray_real_T(data_remove_pf);
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
  main_remove_pf();

  /* Terminate the application.
     You do not need to do this more than one time. */
  remove_pf_terminate();
  return 0;
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
