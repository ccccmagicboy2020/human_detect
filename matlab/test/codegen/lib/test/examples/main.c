/*
 * File: main.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 11-Nov-2020 11:31:14
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
#include "test.h"
#include "test_terminate.h"

/* Function Declarations */
static void argInit_3x3_real32_T(float result[9]);
static float argInit_real32_T(void);
static void main_test(void);

/* Function Definitions */

/*
 * Arguments    : float result[9]
 * Return Type  : void
 */
static void argInit_3x3_real32_T(float result[9])
{
  float result_tmp;
  int idx0;

  /* Loop over the array to initialize each element. */
  result_tmp = argInit_real32_T();
  for (idx0 = 0; idx0 < 3; idx0++) {
    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result[idx0] = result_tmp;

    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result[idx0 + 3] = result_tmp;

    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result[idx0 + 6] = result_tmp;
  }
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
static void main_test(void)
{
  float fv[9];
  float res[9];

  /* Initialize function 'test' input arguments. */
  /* Initialize function input argument 'src'. */
  /* Call the entry-point 'test'. */
  argInit_3x3_real32_T(fv);
  test(fv, res);
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
  main_test();

  /* Terminate the application.
     You do not need to do this more than one time. */
  test_terminate();
  return 0;
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
