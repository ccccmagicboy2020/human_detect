/*******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co., Ltd. ("HDSC").
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
 ** \brief I2C slave irq sample
 **
 **   - 2018-11-01  1.0  Wangmin First version for Device Driver Library of I2C
 **     Slave interrupt example
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ddl.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/**
 * @brief I2c communication mode enum
 */
typedef enum
{
    Mode_Send = 0u,
    Mode_Rev = 1u,
}stc_i2c_com_mode_t;

/**
 * @brief I2c communication status enum
 */
typedef enum
{
    I2C_ComBusy = 0u,
    I2C_ComIdle = 1u,
}stc_i2c_com_status_t;

/**
 * @brief I2c communication structure
 */
typedef struct
{
    stc_i2c_com_mode_t    enMode;         /*!< I2C communication mode*/
    uint32_t              u32Length;      /*!< I2C communication data length*/
    uint8_t*              pBuf;           /*!< I2C communication data buffer pointer*/
    __IO uint32_t         u32DataIndex;   /*!< I2C communication data transfer index*/
    stc_i2c_com_status_t  enComStatus;    /*!< I2C communication status*/
}stc_i2c_communication_t;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Define I2C unit used for the example */
#define I2C_UNIT                        (M4_I2C1)
/* Define slave device address for example */
#define SLAVE_ADDRESS                   (0x06u)
//#define I2C_10BITS_ADDRESS              (1u)

/* Define port and pin for SDA and SCL */
#define I2C1_SCL_PORT                   (PortC)
#define I2C1_SCL_PIN                    (Pin04)
#define I2C1_SDA_PORT                   (PortC)
#define I2C1_SDA_PIN                    (Pin05)

/* Define Write and read data length for the example */
#define TEST_DATA_LEN                   (256u)
/* Define i2c baudrate */
#define I2C_BAUDRATE                    (400000ul)

/* LED0 Port/Pin definition */
#define  LED0_PORT                      (PortE)
#define  LED0_PIN                       (Pin06)

/* LED1 Port/Pin definition */
#define  LED1_PORT                      (PortA)
#define  LED1_PIN                       (Pin07)

/* LED0~1 toggle definition */
#define  LED0_TOGGLE()                  (PORT_Toggle(LED0_PORT, LED0_PIN))
#define  LED1_TOGGLE()                  (PORT_Toggle(LED1_PORT, LED1_PIN))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
uint8_t u8RxBuf[TEST_DATA_LEN];
static stc_i2c_communication_t stcI2cCom;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief  Slave receive data
 **
 ** \param  pu8RxData             Pointer to the data buffer
 ** \param  u32Size               Data size
 ** \retval en_result_t           Enumeration value:
 **         - Ok:                    Success
 **         - OperationInProgress:   Busy
 ******************************************************************************/
static en_result_t I2C_Slave_Receive_IT(uint8_t *pu8RxData, uint32_t u32Size)
{
    en_result_t enRet = Ok;

    if(I2C_ComIdle == stcI2cCom.enComStatus)
    {
        stcI2cCom.enComStatus = I2C_ComBusy;

        stcI2cCom.u32DataIndex = 0u;
        stcI2cCom.enMode = Mode_Rev;
        stcI2cCom.u32Length = u32Size;
        stcI2cCom.pBuf = pu8RxData;

        I2C_Cmd(I2C_UNIT, Enable);
        /* Config slave address match interrupt function*/
        I2C_IntCmd(I2C_UNIT, I2C_CR2_SLADDR0EN, Enable);
    }
    else
    {
        enRet = OperationInProgress;
    }

    return enRet;
}

/**
 ******************************************************************************
 ** \brief  Slave transmit data
 **
 ** \param  pu8TxData             Pointer to the data buffer
 ** \param  u32Size               Data size
 ** \retval en_result_t           Enumeration value:
 **         - Ok:                    Success
 **         - OperationInProgress:   Busy
 ******************************************************************************/
static en_result_t I2C_Slave_Transmit_IT(uint8_t *pu8TxData, uint32_t u32Size)
{
    en_result_t enRet = Ok;

    if(I2C_ComIdle == stcI2cCom.enComStatus)
    {
        stcI2cCom.enComStatus = I2C_ComBusy;

        stcI2cCom.u32DataIndex = 0u;
        stcI2cCom.enMode = Mode_Send;
        stcI2cCom.u32Length = u32Size;
        stcI2cCom.pBuf = pu8TxData;

        I2C_Cmd(I2C_UNIT, Enable);
        /* Config slave address match interrupt function*/
        I2C_IntCmd(I2C_UNIT, I2C_CR2_SLADDR0EN, Enable);
    }
    else
    {
        enRet = OperationInProgress;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief static function for buffer write.
 **
 ** \param  u8Data the data to be write.
 **
 ** \retval None
 **
 ******************************************************************************/
static void BufWrite(uint8_t u8Data)
{
    if(stcI2cCom.u32DataIndex < stcI2cCom.u32Length)
    {
        u8RxBuf[stcI2cCom.u32DataIndex] = u8Data;
        stcI2cCom.u32DataIndex++;
    }
}

/**
 *******************************************************************************
 ** \brief static function for buffer read.
 **
 ** \param None
 **
 ** \retval uint8_t the data read from the buffer.
 **
 ******************************************************************************/
static uint8_t BufRead(void)
{
    uint8_t temp;
    if(stcI2cCom.u32DataIndex < stcI2cCom.u32Length)
    {
        temp = u8RxBuf[stcI2cCom.u32DataIndex];
        stcI2cCom.u32DataIndex++;
    }
    else
    {
        temp = 0xFFu;
    }

    return temp;
}
/**
 *******************************************************************************
 ** \brief I2C EEI(communication error or event) interrupt callback function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void I2C_EEI_Callback(void)
{
    /* If address interrupt occurred */
    if(Set == I2C_GetStatus(I2C_UNIT, I2C_SR_SLADDR0F))
    {
        I2C_ClearStatus(I2C_UNIT, I2C_CLR_SLADDR0FCLR | I2C_CLR_NACKFCLR);

        if((Mode_Send == stcI2cCom.enMode) && (Set == I2C_GetStatus(I2C_UNIT, I2C_SR_TRA)))
        {
            /* Enable tx end interrupt function*/
            I2C_IntCmd(I2C_UNIT, I2C_CR2_TENDIE, Enable);
            /* Write the first data to DTR immediately */
            I2C_WriteData(I2C_UNIT, BufRead());

            /* Enable stop and NACK interrupt */
            I2C_IntCmd(I2C_UNIT, I2C_CR2_STOPIE | I2C_CR2_NACKIE, Enable);
        }
        else if((Mode_Rev == stcI2cCom.enMode) && (Reset == I2C_GetStatus(I2C_UNIT, I2C_SR_TRA)))
        {
            /* Config rx buffer full interrupt function*/
            I2C_IntCmd(I2C_UNIT, I2C_CR2_RFULLIE, Enable);

            /* Enable stop and NACK interrupt */
            I2C_IntCmd(I2C_UNIT, I2C_CR2_STOPIE | I2C_CR2_NACKIE, Enable);
        }
    }
    /* If NACK interrupt occurred */
    else if(Set == I2C_GetStatus(I2C_UNIT, I2C_SR_NACKF))
    {
        /* clear NACK flag*/
        I2C_ClearStatus(I2C_UNIT, I2C_CLR_NACKFCLR);
        /* Stop tx or rx process*/
        if(Set == I2C_GetStatus(I2C_UNIT, I2C_SR_TRA))
        {
            /* Config tx end interrupt function disable*/
            I2C_IntCmd(I2C_UNIT, I2C_CR2_TENDIE, Disable);
            I2C_ClearStatus(I2C_UNIT, I2C_CLR_TENDFCLR);

            /* Read DRR register to release */
            I2C_ReadData(I2C_UNIT);
        }
        else
        {
            /* Config rx buffer full interrupt function disable */
            I2C_IntCmd(I2C_UNIT, I2C_CR2_RFULLIE, Disable);
        }
    }
    /* If stop interrupt occurred */
    else if(Set == I2C_GetStatus(I2C_UNIT, I2C_SR_STOPF))
    {
        /* Disable all interrupt enable flag except SLADDR0IE*/
        I2C_IntCmd(I2C_UNIT,                                                   \
                   I2C_CR2_TENDIE | I2C_CR2_RFULLIE |                          \
                   I2C_CR2_STOPIE | I2C_CR2_NACKIE,                            \
                   Disable);
        /* Clear STOPF flag */
        I2C_ClearStatus(I2C_UNIT, I2C_CLR_STOPFCLR);
        I2C_Cmd(I2C_UNIT, Disable);

        stcI2cCom.enComStatus = I2C_ComIdle;
    }
}

/**
 *******************************************************************************
 ** \brief I2C TEI(transfer end) interrupt callback function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void I2C_TEI_Callback(void)
{
    if((Set == I2C_GetStatus(I2C_UNIT, I2C_SR_TENDF)) &&
        (Reset == I2C_GetStatus(I2C_UNIT, I2C_SR_ACKRF)))
    {
        I2C_WriteData(I2C_UNIT, BufRead());
    }
}

/**
 *******************************************************************************
 ** \brief I2C RXI(receive buffer full) interrupt callback function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void I2C_RXI_Callback(void)
{
    if(Set == I2C_GetStatus(I2C_UNIT, I2C_SR_RFULLF))
    {
        BufWrite(I2C_ReadData(I2C_UNIT));
    }
}

/**
 ******************************************************************************
 ** \brief  Initialize the I2C peripheral for slave
 **
 ** \param  None
 **
 ** \retval en_result_t                Enumeration value:
 **          - Ok:                     Success
 **          - ErrorInvalidParameter:  Invalid parameter
 ******************************************************************************/
static en_result_t Slave_Initialize(void)
{
    en_result_t enRet;
    stc_i2c_init_t stcI2cInit;
    stc_irq_regi_conf_t stcIrqRegiConf;
    stc_clk_freq_t stcClkFreq;

    I2C_DeInit(I2C_UNIT);

    stcI2cCom.enComStatus = I2C_ComIdle;

    /* Get system clock frequency */
    CLK_GetClockFreq(&stcClkFreq);

    MEM_ZERO_STRUCT(stcI2cInit);
    stcI2cInit.u32Pclk3 = stcClkFreq.pclk3Freq;
    stcI2cInit.u32Baudrate = I2C_BAUDRATE;
    enRet = I2C_Init(I2C_UNIT, &stcI2cInit);

    if(Ok == enRet)
    {
        /* Set slave address*/
    #ifdef I2C_10BITS_ADDRESS
        I2C_SlaveAdr0Config(I2C_UNIT, Enable, Adr10bit, SLAVE_ADDRESS);
    #else
        I2C_SlaveAdr0Config(I2C_UNIT, Enable, Adr7bit, SLAVE_ADDRESS);
    #endif
        /* Register EEI Int to Vect.No.001 */
        stcIrqRegiConf.enIRQn = Int001_IRQn;
        /* Select I2C Error or Event interrupt function */
        stcIrqRegiConf.enIntSrc = INT_I2C1_EEI;
        /* Callback function */
        stcIrqRegiConf.pfnCallback = &I2C_EEI_Callback;
        /* Registration IRQ */
        enIrqRegistration(&stcIrqRegiConf);
        /* Clear Pending */
        NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
        /* Set priority */
        NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_15);
        /* Enable NVIC */
        NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

        /* Register RXI Int to Vect.No.002 */
        stcIrqRegiConf.enIRQn = Int002_IRQn;
        /* Select I2C receive full interrupt function */
        stcIrqRegiConf.enIntSrc = INT_I2C1_RXI;
        /* Callback function */
        stcIrqRegiConf.pfnCallback = &I2C_RXI_Callback;
        /* Registration IRQ */
        enIrqRegistration(&stcIrqRegiConf);
        /* Clear Pending */
        NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
        /* Set priority */
        NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_15);
        /* Enable NVIC */
        NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

        /* Register TEI Int to Vect.No.003 */
        stcIrqRegiConf.enIRQn = Int003_IRQn;
        /* Select I2C TX end interrupt function */
        stcIrqRegiConf.enIntSrc = INT_I2C1_TEI;
        /* Callback function */
        stcIrqRegiConf.pfnCallback = &I2C_TEI_Callback;
        /* Registration IRQ */
        enIrqRegistration(&stcIrqRegiConf);
        /* Clear Pending */
        NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
        /* Set priority */
        NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_15);
        /* Enable NVIC */
        NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
    }
    return enRet;
}

/**
 ******************************************************************************
 ** \brief  Initialize the system clock for the sample
 **
 ** \param  None
 **
 ** \retval None
 ******************************************************************************/
static void SysClkIni(void)
{
    en_clk_sys_source_t     enSysClkSrc;
    stc_clk_sysclk_cfg_t    stcSysClkCfg;
    stc_clk_xtal_cfg_t      stcXtalCfg;
    stc_clk_mpll_cfg_t      stcMpllCfg;

    MEM_ZERO_STRUCT(enSysClkSrc);
    MEM_ZERO_STRUCT(stcSysClkCfg);
    MEM_ZERO_STRUCT(stcXtalCfg);
    MEM_ZERO_STRUCT(stcMpllCfg);

    /* Set bus clk div. */
    stcSysClkCfg.enHclkDiv = ClkSysclkDiv1;
    stcSysClkCfg.enExclkDiv = ClkSysclkDiv2;
    stcSysClkCfg.enPclk0Div = ClkSysclkDiv1;
    stcSysClkCfg.enPclk1Div = ClkSysclkDiv2;
    stcSysClkCfg.enPclk2Div = ClkSysclkDiv4;
    stcSysClkCfg.enPclk3Div = ClkSysclkDiv4;
    stcSysClkCfg.enPclk4Div = ClkSysclkDiv2;
    CLK_SysClkConfig(&stcSysClkCfg);

    /* Switch system clock source to MPLL. */
    /* Use Xtal32 as MPLL source. */
    stcXtalCfg.enMode = ClkXtalModeOsc;
    stcXtalCfg.enDrv = ClkXtalLowDrv;
    stcXtalCfg.enFastStartup = Enable;
    CLK_XtalConfig(&stcXtalCfg);
    CLK_XtalCmd(Enable);

    /* MPLL config. */
    stcMpllCfg.pllmDiv = 1u;
    stcMpllCfg.plln = 50u;
    stcMpllCfg.PllpDiv = 4u;  /* 100M */
    stcMpllCfg.PllqDiv = 4u;
    stcMpllCfg.PllrDiv = 4u;
    CLK_SetPllSource(ClkPllSrcXTAL);
    CLK_MpllConfig(&stcMpllCfg);

    /* flash read wait cycle setting */
    EFM_Unlock();
    EFM_SetLatency(EFM_LATENCY_2);
    EFM_Lock();

    /* Enable MPLL. */
    CLK_MpllCmd(Enable);

    /* Wait MPLL ready. */
    while(Set != CLK_GetFlagStatus(ClkFlagMPLLRdy))
    {
        ;
    }

    /* Switch system clock source to MPLL. */
    CLK_SetSysClkSource(CLKSysSrcMPLL);
}

/**
 *******************************************************************************
 ** \brief  Main function of template project
 **
 ** \param  None
 **
 ** \retval int32_t return value, if needed
 **
 ******************************************************************************/
int32_t  main(void)
{
    stc_port_init_t stcPortInit;

    memset(u8RxBuf, 0x01u, TEST_DATA_LEN);

    /* Initialize system clock*/
    SysClkIni();

    /*initiallize LED port*/
    MEM_ZERO_STRUCT(stcPortInit);
    stcPortInit.enPinMode = Pin_Mode_Out;
    stcPortInit.enExInt = Enable;
    stcPortInit.enPullUp = Enable;
    /* LED0 Port/Pin initialization */
    PORT_Init(LED0_PORT, LED0_PIN, &stcPortInit);
    /* LED1 Port/Pin initialization */
    PORT_Init(LED1_PORT, LED1_PIN, &stcPortInit);

    /* Initialize I2C port*/
    PORT_SetFunc(I2C1_SCL_PORT, I2C1_SCL_PIN, Func_I2c1_Scl, Disable);
    PORT_SetFunc(I2C1_SDA_PORT, I2C1_SDA_PIN, Func_I2c1_Sda, Disable);

    /* Enable I2C Peripheral*/
    PWC_Fcg1PeriphClockCmd(PWC_FCG1_PERIPH_I2C1, Enable);
    /* Initialize I2C peripheral and enable function*/
    Slave_Initialize();

    while(1)
    {
        while(Ok != I2C_Slave_Receive_IT(u8RxBuf, TEST_DATA_LEN));

        while(I2C_ComBusy == stcI2cCom.enComStatus);

        while(Ok != I2C_Slave_Transmit_IT(u8RxBuf, TEST_DATA_LEN));

        while(I2C_ComBusy == stcI2cCom.enComStatus);
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
