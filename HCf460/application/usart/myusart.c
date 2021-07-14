#include "myusart.h"
#include "hc32_ddl.h"
#include "bluetooth.h"

/**
 *******************************************************************************
 ** \brief USART RX irq callback function.
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/
static void UsartRxIrqCallback(void)
{
		uint16_t rev_data = 0;
		unsigned char rev_data1 = 0;
	
//	SEGGER_SYSVIEW_RecordEnterISR();	
		rev_data = USART_RecData(USART_CH);
	
		rev_data1 = (unsigned char)rev_data;
	
    uart_receive_input(rev_data1);
	
//	SEGGER_SYSVIEW_RecordExitISR();	
}

static void tuya_UsartRxIrqCallback(void)
{
		uint16_t rev_data = 0;
		unsigned char rev_data1 = 0;
	
//	SEGGER_SYSVIEW_RecordEnterISR();	
		rev_data = USART_RecData(USART_TUYA_CH);
	
		rev_data1 = (unsigned char)rev_data;
	
    uart_receive_input(rev_data1);
	
//	SEGGER_SYSVIEW_RecordExitISR();	
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
//	SEGGER_SYSVIEW_RecordEnterISR();	
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
//	SEGGER_SYSVIEW_RecordExitISR();			
}

void tuya_UsartRxErrProcess(void)
{
//	SEGGER_SYSVIEW_RecordEnterISR();	
    if (Set == USART_GetStatus(USART_TUYA_CH, UsartFrameErr))
    {
        USART_ClearStatus(USART_TUYA_CH, UsartFrameErr);
    }

    if (Set == USART_GetStatus(USART_TUYA_CH, UsartParityErr))
    {
        USART_ClearStatus(USART_TUYA_CH, UsartParityErr);
    }

    if (Set == USART_GetStatus(USART_TUYA_CH, UsartOverrunErr))
    {
        USART_ClearStatus(USART_TUYA_CH, UsartOverrunErr);
    }
//	SEGGER_SYSVIEW_RecordExitISR();	
}

void usart_init(void)
{
	en_result_t enRet = Ok;
  stc_irq_regi_conf_t tuya_stcIrqRegiCfg;
	stc_irq_regi_conf_t stcIrqRegiCfg;
	
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
    tuya_stcIrqRegiCfg.enIRQn = Int003_IRQn;
    tuya_stcIrqRegiCfg.pfnCallback = &tuya_UsartRxIrqCallback;
    tuya_stcIrqRegiCfg.enIntSrc = USART_TUYA_RI_NUM;
    enIrqRegistration(&tuya_stcIrqRegiCfg);
    NVIC_SetPriority(tuya_stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_ClearPendingIRQ(tuya_stcIrqRegiCfg.enIRQn);
    NVIC_EnableIRQ(tuya_stcIrqRegiCfg.enIRQn);    
		
    /* Set USART RX IRQ */
    stcIrqRegiCfg.enIRQn = Int004_IRQn;
    stcIrqRegiCfg.pfnCallback = &UsartRxIrqCallback;
    stcIrqRegiCfg.enIntSrc = USART_RI_NUM;
    enIrqRegistration(&stcIrqRegiCfg);
    NVIC_SetPriority(stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
    NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);

    /*Enable RX && TX function*/
    USART_FuncCmd(USART_CH, UsartRx, Enable);
    USART_FuncCmd(USART_CH, UsartTx, Enable);

    USART_FuncCmd(USART_TUYA_CH, UsartRx, Enable);
    USART_FuncCmd(USART_TUYA_CH, UsartTx, Enable);
		
#ifndef TUYA_ENABLE
		USART_FuncCmd(USART_CH, UsartRxInt, Enable);       //enalbe the rxd interrupt
#else
    USART_FuncCmd(USART_TUYA_CH, UsartRxInt, Enable);       //enalbe the rxd interrupt
#endif

}



