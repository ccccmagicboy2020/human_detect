/*
 * File: main.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 11-Nov-2020 16:25:33
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
#include "auto_trip_50hz.h"
#include "auto_trip_50hz_terminate.h"

/* Function Declarations */
static void argInit_1x2560_real32_T(float result[2560]);
static void argInit_1x3_real32_T(float result[3]);
static float argInit_real32_T(void);
static void main_auto_trip_50hz(void);

/* Function Definitions */

/*
 * Arguments    : float result[2560]
 * Return Type  : void
 */
static void argInit_1x2560_real32_T(float result[2560])
{
  int idx1;

  /* Loop over the array to initialize each element. */
  for (idx1 = 0; idx1 < 2560; idx1++) {
    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result[idx1] = argInit_real32_T();
  }
}

/*
 * Arguments    : float result[3]
 * Return Type  : void
 */
static void argInit_1x3_real32_T(float result[3])
{
  float result_tmp;

  /* Loop over the array to initialize each element. */
  /* Set the value of the array element.
     Change this value to the value that the application requires. */
  result_tmp = argInit_real32_T();
  result[0] = result_tmp;

  /* Set the value of the array element.
     Change this value to the value that the application requires. */
  result[1] = result_tmp;

  /* Set the value of the array element.
     Change this value to the value that the application requires. */
  result[2] = result_tmp;
}

/*
 * Arguments    : void
 * Return Type  : float
 */
static float argInit_real32_T(void)
{
  return 0.0F;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void main_auto_trip_50hz(void)
{
  float fs_tmp;
  float fv[2560];
  float fv1[3];
  float res[2560];

  /* Initialize function 'auto_trip_50hz' input arguments. */
  /* Initialize function input argument 'src'. */
  fs_tmp = argInit_real32_T();

  /* Initialize function input argument 'peaks'. */
  /* Call the entry-point 'auto_trip_50hz'. */
  argInit_1x2560_real32_T(fv);
  argInit_1x3_real32_T(fv1);
  auto_trip_50hz(fv, fs_tmp, fs_tmp, fv1, res);
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
  main_auto_trip_50hz();

  /* Terminate the application.
     You do not need to do this more than one time. */
  auto_trip_50hz_terminate();
  return 0;
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
