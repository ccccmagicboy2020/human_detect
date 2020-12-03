/*
 * File: main.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 03-Dec-2020 09:43:05
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
#include "cfar_ca.h"
#include "cfar_ca_emxAPI.h"
#include "cfar_ca_terminate.h"

/* Function Declarations */
static void argInit_1xd2048_real_T(double result_data[], int result_size[2]);
static double argInit_real_T(void);
static void main_cfar_ca(void);

/* Function Definitions */

/*
 * Arguments    : double result_data[]
 *                int result_size[2]
 * Return Type  : void
 */
static void argInit_1xd2048_real_T(double result_data[], int result_size[2])
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
static void main_cfar_ca(void)
{
  emxArray_real_T *b_index;
  emxArray_real_T *XT;
  double xc_data[2048];
  int xc_size[2];
  double N_tmp;
  emxInitArray_real_T(&b_index, 2);
  emxInitArray_real_T(&XT, 2);

  /* Initialize function 'cfar_ca' input arguments. */
  /* Initialize function input argument 'xc'. */
  argInit_1xd2048_real_T(xc_data, xc_size);
  N_tmp = argInit_real_T();

  /* Call the entry-point 'cfar_ca'. */
  cfar_ca(xc_data, xc_size, N_tmp, N_tmp, N_tmp, b_index, XT);
  emxDestroyArray_real_T(XT);
  emxDestroyArray_real_T(b_index);
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
  main_cfar_ca();

  /* Terminate the application.
     You do not need to do this more than one time. */
  cfar_ca_terminate();
  return 0;
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
