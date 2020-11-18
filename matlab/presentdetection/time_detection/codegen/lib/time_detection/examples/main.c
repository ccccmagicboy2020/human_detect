/*
 * File: main.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 18-Nov-2020 13:52:24
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
#include "rt_nonfinite.h"
#include "time_detection.h"
#include "time_detection_terminate.h"

/* Function Declarations */
static void argInit_32768x1_real_T(double result[32768]);
static double argInit_real_T(void);
static void main_time_detection(void);

/* Function Definitions */

/*
 * Arguments    : double result[32768]
 * Return Type  : void
 */
static void argInit_32768x1_real_T(double result[32768])
{
  int idx0;

  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < 32768; idx0++) {
    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result[idx0] = argInit_real_T();
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
static void main_time_detection(void)
{
  double win_size_time_tmp;
  static double dv[32768];
  bool time_vote;

  /* Initialize function 'time_detection' input arguments. */
  /* Initialize function input argument 'data'. */
  win_size_time_tmp = argInit_real_T();

  /* Call the entry-point 'time_detection'. */
  argInit_32768x1_real_T(dv);
  time_vote = time_detection(dv, win_size_time_tmp, win_size_time_tmp,
    win_size_time_tmp, win_size_time_tmp);
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
  main_time_detection();

  /* Terminate the application.
     You do not need to do this more than one time. */
  time_detection_terminate();
  return 0;
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
