/*******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co.,Ltd All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co.,Ltd ("HDSC").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software contains source code for use with HDSC
 * components. This software is licensed by HDSC to be adapted only
 * for use in systems utilizing HDSC components. HDSC shall not be
 * responsible for misuse or illegal use of this software for devices not
 * supported herein. HDSC is providing this software "AS IS" and will
 * not be responsible for issues arising from incorrect user implementation
 * of the software.
 *
 * Disclaimer:
 * HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
 * REGARDING THE SOFTWARE (INCLUDING ANY ACCOMPANYING WRITTEN MATERIALS),
 * ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
 * WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
 * WARRANTY OF NONINFRINGEMENT.
 * HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
 * LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
 * INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
 * SAVINGS OR PROFITS,
 * EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
 * INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
 * FROM, THE SOFTWARE.
 *
 * This software may be replicated in part or whole for the licensed use,
 * with the restriction that this Disclaimer and Copyright notice must be
 * included with each copy of this software, whether used in part or whole,
 * at all times.
 */
/******************************************************************************/
/** \file main.c
 **
 ** \brief The example for KEYSCAN function demonstration
 **
 **   - 2018-10-24  1.0  Zhangxl First version for sample of keyscan module.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ddl.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* KEYOUT port, pin definition */
#define  KEYOUT_PORT        (PortA)
#define  KEYOUT0_PIN        (Pin04)
#define  KEYOUT1_PIN        (Pin05)
#define  KEYOUT2_PIN        (Pin06)

/* KEYIN port, pin definition */
#define  KEYIN_PORT         (PortD)
#define  KEYIN0_PIN         (Pin12)
#define  KEYIN1_PIN         (Pin13)
#define  KEYIN2_PIN         (Pin14)


/* LED0 Port/Pin definition */
#define  LED0_PORT          (PortE)
#define  LED0_PIN           (Pin06)

/* LED1 Port/Pin definition */
#define  LED1_PORT          (PortA)
#define  LED1_PIN           (Pin07)

/* LED2 Port/Pin definition */
#define  LED2_PORT          (PortB)
#define  LED2_PIN           (Pin05)

/* LED3 Port/Pin definition */
#define  LED3_PORT          (PortB)
#define  LED3_PIN           (Pin09)

/* LED0~3 toggle definition */
#define  LED0_TOGGLE()      (PORT_Toggle(LED0_PORT, LED0_PIN))
#define  LED1_TOGGLE()      (PORT_Toggle(LED1_PORT, LED1_PIN))
#define  LED2_TOGGLE()      (PORT_Toggle(LED2_PORT, LED2_PIN))
#define  LED3_TOGGLE()      (PORT_Toggle(LED3_PORT, LED3_PIN))

/* uncomment this line if wants to print information to Terminal I/O window */
//#define  __PRINT_TO_TERMINAL

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief ExtInt12 as key row 0 callback function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void KeyRow0_Callback(void)
{
    if (Set == EXINT_IrqFlgGet(ExtiCh12))
    {
        LED0_TOGGLE();

#ifdef __PRINT_TO_TERMINAL
        printf("Key row 0, col %d is pressed.\n", KEYSCAN_GetColIdx());
#endif
        /* clear int request flag */
        EXINT_IrqFlgClr(ExtiCh12);
    }
}

/**
 *******************************************************************************
 ** \brief ExtInt13 as key row 1 callback function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void KeyRow1_Callback(void)
{
    if (Set == EXINT_IrqFlgGet(ExtiCh13))
    {
        LED1_TOGGLE();

#ifdef __PRINT_TO_TERMINAL
        printf("Key row 1, col %d is pressed.\n", KEYSCAN_GetColIdx());
#endif
        /* clear int request flag */
        EXINT_IrqFlgClr(ExtiCh13);
    }
}

/**
 *******************************************************************************
 ** \brief ExtInt14 as key row 2 callback function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void KeyRow2_Callback(void)
{
    if (Set == EXINT_IrqFlgGet(ExtiCh14))
    {
        LED2_TOGGLE();

#ifdef __PRINT_TO_TERMINAL
        printf("Key row 2, col %d is pressed.\n", KEYSCAN_GetColIdx());
#endif
        /* clear int request flag */
        EXINT_IrqFlgClr(ExtiCh14);
    }
}

/**
 *******************************************************************************
 ** \brief LED 0~3 init function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void Led_Init(void)
{
    stc_port_init_t stcPortInit;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcPortInit);

    stcPortInit.enPinMode = Pin_Mode_Out;

    /* LED0 Port/Pin initialization */
    PORT_Init(LED0_PORT, LED0_PIN, &stcPortInit);

    /* LED1 Port/Pin initialization */
    PORT_Init(LED1_PORT, LED1_PIN, &stcPortInit);

    /* LED2 Port/Pin initialization */
    PORT_Init(LED2_PORT, LED2_PIN, &stcPortInit);

    /* LED3 Port/Pin initialization */
    PORT_Init(LED3_PORT, LED3_PIN, &stcPortInit);
}

/**
 *******************************************************************************
 ** \brief Key row 0 init function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void KeyRow0_Init(void)
{
    stc_exint_config_t stcExtiConfig;
    stc_irq_regi_conf_t stcIrqRegiConf;
    stc_port_init_t stcPortInit;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcExtiConfig);
    MEM_ZERO_STRUCT(stcIrqRegiConf);
    MEM_ZERO_STRUCT(stcPortInit);

    /**************************************************************************/
    /* External Int Ch.12 for keyin0                                          */
    /**************************************************************************/
    stcExtiConfig.enExitCh = ExtiCh12;

    /* Filter setting */
    stcExtiConfig.enFilterEn = Enable;
    stcExtiConfig.enFltClk = Pclk3Div8;
    /* falling edge for keyscan function */
    stcExtiConfig.enExtiLvl = ExIntFallingEdge;
    EXINT_Init(&stcExtiConfig);

    /* Set PD12 as External Int Ch.12 input */
    MEM_ZERO_STRUCT(stcPortInit);
    stcPortInit.enExInt = Enable;
    stcPortInit.enPullUp = Enable;
    PORT_Init(KEYIN_PORT, KEYIN0_PIN, &stcPortInit);

    /* Select External Int Ch.12 */
    stcIrqRegiConf.enIntSrc = INT_PORT_EIRQ12;

    /* Register External Int to Vect.No.000 */
    stcIrqRegiConf.enIRQn = Int000_IRQn;

    /* Callback function */
    stcIrqRegiConf.pfnCallback = &KeyRow0_Callback;

    /* Registration IRQ */
    enIrqRegistration(&stcIrqRegiConf);

    /* Clear pending */
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);

    /* Set priority */
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_15);

    /* Enable NVIC */
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
}

/**
 *******************************************************************************
 ** \brief Key row 1 init function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void KeyRow1_Init(void)
{
    stc_exint_config_t stcExtiConfig;
    stc_irq_regi_conf_t stcIrqRegiConf;
    stc_port_init_t stcPortInit;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcExtiConfig);
    MEM_ZERO_STRUCT(stcIrqRegiConf);
    MEM_ZERO_STRUCT(stcPortInit);

    /**************************************************************************/
    /* External Int Ch.13 for keyin1                                          */
    /**************************************************************************/
    stcExtiConfig.enExitCh = ExtiCh13;

    /* Filter setting */
    stcExtiConfig.enFilterEn = Enable;
    stcExtiConfig.enFltClk = Pclk3Div8;
    /* falling edge for keyscan function */
    stcExtiConfig.enExtiLvl = ExIntFallingEdge;
    EXINT_Init(&stcExtiConfig);

    /* Set PD13 as External Int Ch.13 input */
    MEM_ZERO_STRUCT(stcPortInit);
    stcPortInit.enExInt = Enable;
    stcPortInit.enPullUp = Enable;
    PORT_Init(KEYIN_PORT, KEYIN1_PIN, &stcPortInit);

    /* Select External Int Ch.13 */
    stcIrqRegiConf.enIntSrc = INT_PORT_EIRQ13;

    /* Register External Int to Vect.No.001 */
    stcIrqRegiConf.enIRQn = Int001_IRQn;

    /* Callback function */
    stcIrqRegiConf.pfnCallback = &KeyRow1_Callback;

    /* Registration IRQ */
    enIrqRegistration(&stcIrqRegiConf);

    /* Clear Pending */
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);

    /* Set priority */
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_15);

    /* Enable NVIC */
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
}

/**
 *******************************************************************************
 ** \brief Key row 2 init function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void KeyRow2_Init(void)
{
    stc_exint_config_t stcExtiConfig;
    stc_irq_regi_conf_t stcIrqRegiConf;
    stc_port_init_t stcPortInit;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcExtiConfig);
    MEM_ZERO_STRUCT(stcIrqRegiConf);
    MEM_ZERO_STRUCT(stcPortInit);

    /**************************************************************************/
    /* External Int Ch.14 for keyin2                                          */
    /**************************************************************************/
    stcExtiConfig.enExitCh = ExtiCh14;

    /* Filter setting */
    stcExtiConfig.enFilterEn = Enable;
    stcExtiConfig.enFltClk = Pclk3Div8;
    /* falling edge for keyscan function */
    stcExtiConfig.enExtiLvl = ExIntFallingEdge;
    EXINT_Init(&stcExtiConfig);

    /* Set PD14 as External Int Ch.14 input */
    MEM_ZERO_STRUCT(stcPortInit);
    stcPortInit.enExInt = Enable;
    stcPortInit.enPullUp = Enable;
    PORT_Init(KEYIN_PORT, KEYIN2_PIN, &stcPortInit);

    /* Select External Int Ch.14 */
    stcIrqRegiConf.enIntSrc = INT_PORT_EIRQ14;

    /* Register External Int to Vect.No.002 */
    stcIrqRegiConf.enIRQn = Int002_IRQn;

    /* Callback function */
    stcIrqRegiConf.pfnCallback = &KeyRow2_Callback;

    /* Registration IRQ */
    enIrqRegistration(&stcIrqRegiConf);

    /* Clear Pending */
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);

    /* Set priority */
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_15);

    /* Enable NVIC */
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
}

/**
 *******************************************************************************
 ** \brief Key column init function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void KeyCol_Init(void)
{
    stc_port_init_t stcPortInit;
    stc_keyscan_config_t stcKeyscanConfig;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcPortInit);

    /* enable internal pull-up */
    //stcPortInit.enPullUp = Enable;
    PORT_Init(KEYOUT_PORT, (KEYOUT0_PIN | KEYOUT1_PIN | KEYOUT2_PIN), &stcPortInit);

    /* Set corresponding pins to KEYSCAN function */
    PORT_SetFunc(KEYOUT_PORT, (KEYOUT0_PIN | KEYOUT1_PIN | KEYOUT2_PIN), Func_Key, Disable);

    /* enable KEYSCAN module source clock */
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_PERIPH_KEY, Enable);

    /* output Hiz state 512 clocks*/
    stcKeyscanConfig.enHizCycle = Hiz512;

    /* output Low state 512 clocks */
    stcKeyscanConfig.enLowCycle = Low512;

    /* use HCLK as scan clock */
    stcKeyscanConfig.enKeyscanClk = KeyscanHclk;

    /* KEYOUT 0~3 are selected as column */
    stcKeyscanConfig.enKeyoutSel = Keyout0To2;

    /* KEYIN 12~14 are selected as row */
    stcKeyscanConfig.u16KeyinSel = (Keyin12 | Keyin13 | Keyin14);

    KEYSCAN_Init(&stcKeyscanConfig);
}

/**
 *******************************************************************************
 ** \brief  main function for KEYSCAN function
 **
 ** \param  None
 **
 ** \return int32_t Return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    /* LED init */
    Led_Init();

#ifdef __PRINT_TO_TERMINAL
    /* Uart printf port initialize */
    Ddl_UartInit();
#endif

    /* Key row 0~2 init */
    KeyRow0_Init();
    KeyRow1_Init();
    KeyRow2_Init();

    /* Key column 0~2 init */
    KeyCol_Init();

    /* Start KEYSCAN function*/
    KEYSCAN_Start();

    while(1)
    {
        ;
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
