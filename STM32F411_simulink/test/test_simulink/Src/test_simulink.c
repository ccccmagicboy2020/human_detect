/*
 * File: test_simulink.c
 *
 * Code generated for Simulink model :test_simulink.
 *
 * Model version      : 1.3
 * Simulink Coder version    : 9.3 (R2020a) 18-Nov-2019
 * TLC version       : 9.3 (Jan 23 2020)
 * C/C++ source code generated on  : Fri Nov 20 18:30:46 2020
 *
 * Target selection: stm32.tlc
 * Embedded hardware selection: STM32CortexM
 * Code generation objectives: Unspecified
 * Validation result: Not run
 *
 *
 *
 * ******************************************************************************
 * * attention
 * *
 * * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * *
 * ******************************************************************************
 */

#include "test_simulink.h"
#include "test_simulink_private.h"

/* GPIOA output mask value definition. */
uint16_t GPIOA_maskWrite;

/* Block signals (default storage) */
B_test_simulink rtB;

/* Block states (default storage) */
DW_test_simulink rtDW;

/* Real-time model */
RT_MODEL_test_simulink rtM_;
RT_MODEL_test_simulink *const rtM = &rtM_;

/* Model step function */
void test_simulink_step(void)
{
  rtB.PulseGenerator = ((rtDW.clockTickCounter < 5000) && (rtDW.clockTickCounter
    >= 0));
  if (rtDW.clockTickCounter >= 9999) {
    rtDW.clockTickCounter = 0;
  } else {
    rtDW.clockTickCounter++;
  }

  rtB.PulseGenerator1 = ((rtDW.clockTickCounter_a < 5000) &&
    (rtDW.clockTickCounter_a >= 0));
  if (rtDW.clockTickCounter_a >= 9999) {
    rtDW.clockTickCounter_a = 0;
  } else {
    rtDW.clockTickCounter_a++;
  }

  rtB.PulseGenerator2 = ((rtDW.clockTickCounter_i < 5000) &&
    (rtDW.clockTickCounter_i >= 0));
  if (rtDW.clockTickCounter_i >= 9999) {
    rtDW.clockTickCounter_i = 0;
  } else {
    rtDW.clockTickCounter_i++;
  }

  {
    /* Set GPIOA output mask value. */
    GPIOA_maskWrite = GPIOA->ODR;
    GPIOA_maskWrite &= 0xFFF8 ;
    GPIOA_maskWrite |= (uint16_t)rtB.PulseGenerator << 0;
    GPIOA_maskWrite |= (uint16_t)rtB.PulseGenerator1 << 1;
    GPIOA_maskWrite |= (uint16_t)rtB.PulseGenerator2 << 2;

    /* Write GPIOA input value */
    GPIOA->ODR = (uint16_t)GPIOA_maskWrite;
  }
}

/* Model initialize function */
void test_simulink_initialize(void)
{
  /* (no initialization code required) */
}

/* File trailer for Real-Time Workshop generated code.
 *
 * [EOF] test_simulink.c
 */
