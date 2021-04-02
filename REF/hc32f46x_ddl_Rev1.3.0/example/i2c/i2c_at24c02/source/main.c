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
 ** \brief At24c02 write and read example.
 **
 **   - 2018-11-01  1.0  Wangmin First version for Device Driver Library example
 **                      for E2PROM AT24C02
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
/* Define I2C unit used for the example */
#define I2C_UNIT                        (M4_I2C1)
/* Define E2PROM device address */
#define E2_ADDRESS                      (0x50u)
/* AT24C02 page length is 8byte*/
#define PAGE_LEN                        (8u)
/* Define test address for read and write */
#define DATA_TEST_ADDR                  (0x00u)
/* Define port and pin for SDA and SCL */
#define I2C1_SCL_PORT                   (PortC)
#define I2C1_SCL_PIN                    (Pin04)
#define I2C1_SDA_PORT                   (PortC)
#define I2C1_SDA_PIN                    (Pin05)

#define TIMEOUT                         (0x40000ul)

/* LED0 Port/Pin definition */
#define LED0_PORT                       (PortE)
#define LED0_PIN                        (Pin06)

/* LED1 Port/Pin definition */
#define LED1_PORT                       (PortA)
#define LED1_PIN                        (Pin07)

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

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief  I2C memory write
 **
 ** \param  u8DevAddr             The slave address
 ** \param  u16MemAddr            The memory address
 ** \param  pu8Data               Pointer to the data buffer
 ** \param  u32Size               Data size
 ** \param  u32TimeOut            Time out count
 ** \retval en_result_t           Enumeration value:
 **         - Ok:                 Success
 **         - Error:              Receive NACK
 **         - ErrorTimeout:       Time out
 ******************************************************************************/
static en_result_t I2C_Mem_Write(uint16_t u8DevAddr, uint16_t u16MemAddr, uint8_t *pu8Data, uint32_t u32Size, uint32_t u32TimeOut)
{
    en_result_t enRet;
    uint8_t u16MemAddrTemp = (uint8_t)u16MemAddr;

    I2C_Cmd(I2C_UNIT, Enable);

    I2C_SoftwareResetCmd(I2C_UNIT, Enable);
    I2C_SoftwareResetCmd(I2C_UNIT, Disable);
    enRet = I2C_Start(I2C_UNIT,u32TimeOut);
    if(Ok == enRet)
    {
        enRet = I2C_TransAddr(I2C_UNIT, u8DevAddr, I2CDirTrans, u32TimeOut);

        if(Ok == enRet)
        {
            enRet = I2C_TransData(I2C_UNIT, &u16MemAddrTemp, 1u, u32TimeOut);
            if(Ok == enRet)
            {
                enRet = I2C_TransData(I2C_UNIT, pu8Data, u32Size, u32TimeOut);
            }
        }
    }

    I2C_Stop(I2C_UNIT,u32TimeOut);
    I2C_Cmd(I2C_UNIT, Disable);

    return enRet;
}

/**
 ******************************************************************************
 ** \brief  I2C memory read
 **
 ** \param  u8DevAddr             The slave address
 ** \param  u16MemAddr            The memory address
 ** \param  pu8Data               Pointer to the data buffer
 ** \param  u32Size               Data size
 ** \param  u32TimeOut            Time out count
 ** \retval en_result_t           Enumeration value:
 **         - Ok:                 Success
 **         - ErrorTimeout:       Time out
 ******************************************************************************/
static en_result_t I2C_Mem_Read(uint8_t u8DevAddr, uint16_t u16MemAddr, uint8_t *pu8Data, uint32_t u32Size, uint32_t u32TimeOut)
{
    en_result_t enRet;

    I2C_Cmd(I2C_UNIT, Enable);

    I2C_SoftwareResetCmd(I2C_UNIT, Enable);
    I2C_SoftwareResetCmd(I2C_UNIT, Disable);
    enRet = I2C_Start(I2C_UNIT,u32TimeOut);
    if(Ok == enRet)
    {
        enRet = I2C_TransAddr(I2C_UNIT, (uint8_t)u8DevAddr, I2CDirTrans, u32TimeOut);

        if(Ok == enRet)
        {
            enRet = I2C_TransData(I2C_UNIT, (uint8_t const*)&u16MemAddr, 1u, u32TimeOut);
            if(Ok == enRet)
            {
                enRet = I2C_Restart(I2C_UNIT,u32TimeOut);
                if(Ok == enRet)
                {
                    if(1ul == u32Size)
                    {
                        I2C_AckConfig(I2C_UNIT, I2c_NACK);
                    }

                    enRet = I2C_TransAddr(I2C_UNIT, (uint8_t)u8DevAddr, I2CDirReceive, u32TimeOut);
                    if(Ok == enRet)
                    {
                        enRet = I2C_MasterDataReceiveAndStop(I2C_UNIT, pu8Data, u32Size, u32TimeOut);
                    }

                    I2C_AckConfig(I2C_UNIT, I2c_ACK);
                }

            }
        }
    }

    if(Ok != enRet)
    {
        I2C_Stop(I2C_UNIT,u32TimeOut);
    }
    
    I2C_Cmd(I2C_UNIT, Disable);
    return enRet;
}

/**
 ******************************************************************************
 ** \brief  Initialize the I2C peripheral for e2prom
 ** \param  None
 ** \retval en_result_t           Enumeration value:
 **         - Ok:                 Success
 **         - ErrorTimeout:       Time out
 ******************************************************************************/
static en_result_t E2_Initialize(void)
{
    stc_i2c_init_t stcI2cInit;
    stc_clk_freq_t stcClkFreq;
    en_result_t enRet;

    I2C_DeInit(I2C_UNIT);

    /* Get system clock frequency */
    CLK_GetClockFreq(&stcClkFreq);

    MEM_ZERO_STRUCT(stcI2cInit);
    stcI2cInit.u32Pclk3 = stcClkFreq.pclk3Freq;
    stcI2cInit.u32Baudrate = 100000ul;
    stcI2cInit.u32SclTime = 0ul;
    enRet = I2C_Init(I2C_UNIT, &stcI2cInit);

    I2C_BusWaitCmd(I2C_UNIT, Enable);

    return enRet;
}

/**
 ******************************************************************************
 ** \brief  Initialize the system clock for the sample
 ** \param  None
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
    /* Use Xtal as MPLL source. */
    stcXtalCfg.enMode = ClkXtalModeOsc;
    stcXtalCfg.enDrv = ClkXtalLowDrv;
    stcXtalCfg.enFastStartup = Enable;
    CLK_XtalConfig(&stcXtalCfg);
    CLK_XtalCmd(Enable);

    /* MPLL config. */
    stcMpllCfg.pllmDiv = 1u;
    stcMpllCfg.plln = 50u;
    stcMpllCfg.PllpDiv = 4u; /* 100M */
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
 ** \param  None
 ** \retval int32_t return value, if needed
 ******************************************************************************/
int32_t main(void)
{
    uint8_t u8TxBuf[PAGE_LEN];
    uint8_t u8RxBuf[PAGE_LEN];
    uint32_t i;
    stc_port_init_t stcPortInit;

    for(i=0ul; i<PAGE_LEN; i++)
    {
        u8TxBuf[i] = (uint8_t)i+1u;
    }
    memset(u8RxBuf, 0x00, PAGE_LEN);

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
    E2_Initialize();

    /* E2prom byte write*/
    I2C_Mem_Write(E2_ADDRESS, DATA_TEST_ADDR, u8TxBuf, 1u, TIMEOUT);

    /* 5mS delay for e2prom*/
    Ddl_Delay1ms(5ul);

    /* E2prom ramdom read*/
    I2C_Mem_Read(E2_ADDRESS, DATA_TEST_ADDR, u8RxBuf, 1u, TIMEOUT);

    /* Compare the data */
    if(0x01u != u8RxBuf[0])
    {
        /* e2prom byte write error*/
        while(1)
        {
            LED0_TOGGLE();
            Ddl_Delay1ms(500ul);
        }
    }

    /* 5mS delay for e2prom*/
    Ddl_Delay1ms(5ul);
    /* E2prom page write*/
    I2C_Mem_Write(E2_ADDRESS, DATA_TEST_ADDR, u8TxBuf, PAGE_LEN, TIMEOUT);

    /* 5mS delay for e2prom*/
    Ddl_Delay1ms(5ul);

    /* E2prom sequential read*/
    I2C_Mem_Read(E2_ADDRESS, DATA_TEST_ADDR, u8RxBuf, PAGE_LEN, TIMEOUT);

    /* Compare the data */
    for(i=0ul; i<PAGE_LEN; i++)
    {
        if(u8TxBuf[i] != u8RxBuf[i])
        {
            /* e2prom page write error*/
            while(1)
            {
                LED0_TOGGLE();
                Ddl_Delay1ms(500ul);
            }
        }
    }

    /* e2prom sample success*/
    while(1)
    {
        LED1_TOGGLE();
        Ddl_Delay1ms(500ul);
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
