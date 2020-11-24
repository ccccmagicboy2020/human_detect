/*
 * File: test_simulink.h
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

#ifndef RTW_HEADER_test_simulink_h_
#define RTW_HEADER_test_simulink_h_
#include "STM32_Config.h"
#include "test_simulink_External_Functions.h"
#ifndef test_simulink_COMMON_INCLUDES_
# define test_simulink_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* test_simulink_COMMON_INCLUDES_ */

#include "test_simulink_types.h"

/* GPIOA output mask value declaration. */
extern uint16_t GPIOA_maskWrite;

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#define test_simulink_M                (rtM)

/* Block signals (default storage) */
typedef struct {
  real_T PulseGenerator;               /* '<Root>/Pulse Generator' */
  real_T PulseGenerator1;              /* '<Root>/Pulse Generator1' */
  real_T PulseGenerator2;              /* '<Root>/Pulse Generator2' */
} B_test_simulink;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  int32_T clockTickCounter;            /* '<Root>/Pulse Generator' */
  int32_T clockTickCounter_a;          /* '<Root>/Pulse Generator1' */
  int32_T clockTickCounter_i;          /* '<Root>/Pulse Generator2' */
} DW_test_simulink;

/* Real-time Model Data Structure */
struct tag_RTM_test_simulink {
  const char_T *errorStatus;
};

/* Block signals (default storage) */
extern B_test_simulink rtB;

/* Block states (default storage) */
extern DW_test_simulink rtDW;

/* Model entry point functions */
extern void test_simulink_initialize(void);
extern void test_simulink_step(void);

/* Real-time Model object */
extern RT_MODEL_test_simulink *const rtM;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'test_simulink'
 */
#endif                                 /* RTW_HEADER_test_simulink_h_ */

/* File trailer for Real-Time Workshop generated code.
 *
 * [EOF] test_simulink.h
 */
