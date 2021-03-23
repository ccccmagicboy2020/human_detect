#include "myusart.h"
#include "hc32_ddl.h"
#include "bluetooth.h"

void ClkInit(void)
{
    stc_clk_xtal_cfg_t   stcXtalCfg;
    stc_clk_mpll_cfg_t   stcMpllCfg;
    en_clk_sys_source_t  enSysClkSrc;
    stc_clk_sysclk_cfg_t stcSysClkCfg;

    MEM_ZERO_STRUCT(enSysClkSrc);
    MEM_ZERO_STRUCT(stcSysClkCfg);
    MEM_ZERO_STRUCT(stcXtalCfg);
    MEM_ZERO_STRUCT(stcMpllCfg);

    /* Set bus clk div. */
    stcSysClkCfg.enHclkDiv  = ClkSysclkDiv1;
    stcSysClkCfg.enExclkDiv = ClkSysclkDiv2;
    stcSysClkCfg.enPclk0Div = ClkSysclkDiv1;
    stcSysClkCfg.enPclk1Div = ClkSysclkDiv2;
    stcSysClkCfg.enPclk2Div = ClkSysclkDiv4;
    stcSysClkCfg.enPclk3Div = ClkSysclkDiv4;
    stcSysClkCfg.enPclk4Div = ClkSysclkDiv2;
    CLK_SysClkConfig(&stcSysClkCfg);

    /* Switch system clock source to MPLL. */
    /* Use Xtal as MPLL source. */
    stcXtalCfg.enMode = ClkXtalModeOsc;
    stcXtalCfg.enDrv = ClkXtalLowDrv;
    stcXtalCfg.enFastStartup = Enable;
    CLK_XtalConfig(&stcXtalCfg);
    CLK_XtalCmd(Enable);

    /* MPLL config. */
    stcMpllCfg.pllmDiv = 1u; /* XTAL 8M / 1 */
    stcMpllCfg.plln = 50u;   /* 8M*50 = 400M */
    stcMpllCfg.PllpDiv = 4u; /* MLLP = 100M */
    stcMpllCfg.PllqDiv = 4u; /* MLLQ = 100M */
    stcMpllCfg.PllrDiv = 4u; /* MLLR = 100M */
    CLK_SetPllSource(ClkPllSrcXTAL);
    CLK_MpllConfig(&stcMpllCfg);

    /* flash read wait cycle setting */
    EFM_Unlock();
    EFM_SetLatency(EFM_LATENCY_4);
    EFM_Lock();

    /* Enable MPLL. */
    CLK_MpllCmd(Enable);

    /* Wait MPLL ready. */
    while (Set != CLK_GetFlagStatus(ClkFlagMPLLRdy))
    {
    }

    /* Switch system clock source to MPLL. */
    CLK_SetSysClkSource(CLKSysSrcMPLL);
}

/**
 *******************************************************************************
 ** \brief USART RX irq callback function.
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/
static void tuya_UsartRxIrqCallback(void)
{
    uart_receive_input(USART_RecData(USART_TUYA_CH));
}

/**
 *******************************************************************************
 ** \brief USART RX error process function.
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/
void UsartRxErrProcess(void)
{
    if (Set == USART_GetStatus(USART_CH, UsartFrameErr))
    {
        USART_ClearStatus(USART_CH, UsartFrameErr);
    }

    if (Set == USART_GetStatus(USART_CH, UsartParityErr))
    {
        USART_ClearStatus(USART_CH, UsartParityErr);
    }

    if (Set == USART_GetStatus(USART_CH, UsartOverrunErr))
    {
        USART_ClearStatus(USART_CH, UsartOverrunErr);
    }
}

void usart_init(void)
{
	en_result_t enRet = Ok;
    stc_irq_regi_conf_t tuya_stcIrqRegiCfg;
	uint32_t u32Fcg1Periph = PWC_FCG1_PERIPH_USART1 | PWC_FCG1_PERIPH_USART2 | \
                             PWC_FCG1_PERIPH_USART3 | PWC_FCG1_PERIPH_USART4;
    const stc_usart_uart_init_t stcInitCfg = {
        UsartIntClkCkNoOutput,
        UsartClkDiv_1,
        UsartDataBits8,
        UsartDataLsbFirst,
        UsartOneStopBit,
        UsartParityNone,
        UsartSamleBit8,
        UsartStartBitFallEdge,
        UsartRtsEnable,
    };
	ClkInit();
	
    /* Enable peripheral clock */
    PWC_Fcg1PeriphClockCmd(u32Fcg1Periph, Enable);

    /* Initialize USART IO */
    PORT_SetFunc(USART_RX_PORT, USART_RX_PIN, USART_RX_FUNC, Disable);
    PORT_SetFunc(USART_TX_PORT, USART_TX_PIN, USART_TX_FUNC, Disable);

    PORT_SetFunc(USART_TUYA_RX_PORT, USART_TUYA_RX_PIN, USART_TUYA_RX_FUNC, Disable);
    PORT_SetFunc(USART_TUYA_TX_PORT, USART_TUYA_TX_PIN, USART_TUYA_TX_FUNC, Disable); 

	    /* Initialize UART */
    enRet = USART_UART_Init(USART_CH, &stcInitCfg);
    if (enRet != Ok)
    {
        while (1)
        {
        }
    }

    //USART_TUYA_CH
    enRet = USART_UART_Init(USART_TUYA_CH, &stcInitCfg);
    if (enRet != Ok)
    {
        while (1)
        {
        }
    }    

    /* Set baudrate */
    enRet = USART_SetBaudrate(USART_CH, USART_BAUDRATE);
    if (enRet != Ok)
    {
        while (1)
        {
        }
    }
    //USART_TUYA_CH
    enRet = USART_SetBaudrate(USART_TUYA_CH, USART_BAUDRATE);
    if (enRet != Ok)
    {
        while (1)
        {
        }
    }    

    /* Set TUYA USART RX IRQ */
    tuya_stcIrqRegiCfg.enIRQn = Int000_IRQn;
    tuya_stcIrqRegiCfg.pfnCallback = &tuya_UsartRxIrqCallback;
    tuya_stcIrqRegiCfg.enIntSrc = USART_TUYA_RI_NUM;
    enIrqRegistration(&tuya_stcIrqRegiCfg);
    NVIC_SetPriority(tuya_stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_ClearPendingIRQ(tuya_stcIrqRegiCfg.enIRQn);
    NVIC_EnableIRQ(tuya_stcIrqRegiCfg.enIRQn);    

    /*Enable RX && TX function*/
    USART_FuncCmd(USART_CH, UsartRx, Enable);
    USART_FuncCmd(USART_CH, UsartTx, Enable);

    USART_FuncCmd(USART_TUYA_CH, UsartRx, Enable);
    USART_FuncCmd(USART_TUYA_CH, UsartTx, Enable);
    USART_FuncCmd(USART_TUYA_CH, UsartRxInt, Enable);       //enalbe the rxd interrupt
}



