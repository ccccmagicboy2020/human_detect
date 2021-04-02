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
/** \file wm8731.c
 **
 ** \brief I2S codec WM8731 driver API functions
 **
 **   - 2018-11-13  1.0  Wangmin First version for Device Driver Library
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ddl.h"
#include "wm8731.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define TIMEOUT                         (0x10000ul)

#define I2C_ADDRESS_W                   (0x00u)
#define I2C_ADDRESS_R                   (0x01u)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint16_t u16LeftHpOut, u16RightHpOut;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief Send command to WM8731
 **
 ** \param pstcI2c   Pointer to the I2C peripheral register, can
 **                                be M4_I2C1,M4_I2C2 or M4_I2C3.
 ** \param u8RegAddr Regist address
 ** \param u16Cmd    Command data
 ** \retval Process result
 **         - Ok     Coded initial success
 **         - Error  Coded initial faild
 ******************************************************************************/
en_result_t WM8731_SendCmd(M4_I2C_TypeDef* pstcI2c, uint8_t u8RegAddr, uint16_t u16Cmd)
{
    // B[15:9] Are Control Address Bits
    // B[8:0]  Are Control Data Bits
    uint8_t u8Data[2];
    uint8_t stat = 0u;
    en_result_t enRes = Ok;

    u8Data[0] = (uint8_t)(((uint16_t)u8RegAddr << 1u) | ((u16Cmd>>8) & 0x1u));
    u8Data[1] = (uint8_t)(u16Cmd & 0xFFu);
    while (1)
    {
        switch (stat)
        {
            case 0u:    // start condition
                if (Ok == I2C_Start(pstcI2c, TIMEOUT))
                {
                    stat = 1u;
                }
                else
                {
                    stat = 0xffu;
                    enRes = Error;
                }
                break;
            case 1u:    // device address
                if (Ok == I2C_TransAddr(pstcI2c, WM8731_ADDRESS, I2CDirTrans, TIMEOUT))
                {
                    stat = 2u;
                }
                else
                {
                    stat = 0xffu;
                    enRes = Error;
                }
                break;
            case 2u:    // send command
                if (Ok == I2C_TransData(pstcI2c, u8Data, 2ul, TIMEOUT))
                {
                    stat = 3u;
                }
                else
                {
                    stat = 0xffu;
                    enRes = Error;
                }
                break;
            case 3u:    // stop condition
                if (Ok == I2C_Stop(pstcI2c, TIMEOUT))
                {
                    stat = 0xffu;
                    enRes = Ok;
                }
                else
                {
                    stat = 0xffu;
                    enRes = Error;
                }
                break;
            default:
//                return enRes;
            break;
        }
        if (0xffu == stat)
        {
            break;
        }
    }
    return enRes;
}

/**
 ******************************************************************************
 ** \brief  Initialize WM8731
 **
 ** \param  pstcI2c   Pointer to the I2C peripheral register, can
 **                                be M4_I2C1,M4_I2C2 or M4_I2C3.
 ** \param  pstcReg   Pointer to structure which define the register configuration
 **
 ** \retval Process result
 **         - Error
 **         - Ok
 ******************************************************************************/
en_result_t WM8731_Init(M4_I2C_TypeDef* pstcI2c, stc_wm8731_reg_t* pstcReg)
{
    stc_i2c_init_t stcI2cInit;
    stc_clk_freq_t stcClkFreq;
    en_result_t enRet;

    I2C_DeInit(pstcI2c);

    /* Get system clock frequency */
    CLK_GetClockFreq(&stcClkFreq);

    MEM_ZERO_STRUCT(stcI2cInit);
    stcI2cInit.u32Pclk3 = stcClkFreq.pclk3Freq;
    stcI2cInit.u32Baudrate = 100000u;
    enRet = I2C_Init(pstcI2c, &stcI2cInit);

    if(enRet == Ok)
    {
        I2C_Cmd(pstcI2c, Enable);

        WM8731_SendCmd(pstcI2c, WM8731_REG_RESET,         pstcReg->RESET);    // Reset module
        WM8731_SendCmd(pstcI2c, WM8731_REG_LLINE_IN,      pstcReg->LLIN);     // Left line in settings
        WM8731_SendCmd(pstcI2c, WM8731_REG_RLINE_IN,      pstcReg->RLIN);     // Rigth line in settings
        WM8731_SendCmd(pstcI2c, WM8731_REG_LHPHONE_OUT,   pstcReg->LHOUT);    // Left headphone out settings
        WM8731_SendCmd(pstcI2c, WM8731_REG_RHPHONE_OUT,   pstcReg->RHOUT);    // Right headphone out settings
        WM8731_SendCmd(pstcI2c, WM8731_REG_ANALOG_PATH,   pstcReg->AAPC);     // Analog paths
        WM8731_SendCmd(pstcI2c, WM8731_REG_DIGITAL_PATH,  pstcReg->DAPC);     // Digital paths
        WM8731_SendCmd(pstcI2c, WM8731_REG_PDOWN_CTRL,    pstcReg->PDC);      // Power down control
        WM8731_SendCmd(pstcI2c, WM8731_REG_DIGITAL_IF,    pstcReg->DAIF);     // Digital interface
        WM8731_SendCmd(pstcI2c, WM8731_REG_SAMPLING_CTRL, pstcReg->SC);       // Sampling control
        WM8731_SendCmd(pstcI2c, WM8731_REG_ACTIVE_CTRL,   pstcReg->AC);       // Activate module

        u16LeftHpOut = pstcReg->LHOUT;   // Store left headphone out data
        u16RightHpOut = pstcReg->RHOUT;  // Store right headphone out data
    }

    return enRet;
}

/**
 ******************************************************************************
 ** \brief Set volume of headphone
 **
 ** \param pstcI2c   Pointer to the I2C peripheral register, can
 **                                be M4_I2C1,M4_I2C2 or M4_I2C3.
 ** \param u16LeftVol Left channel volume
 ** \param u16RightVol Right channel volume
 ** \retval Process result
 **         - Ok     Set operation success
 **         - Error  Set operation faild
 ******************************************************************************/
en_result_t WM8731_SetHpVolume(M4_I2C_TypeDef* pstcI2c, uint16_t u16LeftVol, uint16_t u16RightVol)
{
    u16LeftHpOut = u16LeftHpOut & 0xC0u;
    u16RightHpOut = u16RightHpOut & 0xC0u;
    en_result_t enRet;

    u16LeftHpOut |= u16LeftVol;
    u16RightHpOut |= u16RightVol;

    enRet = WM8731_SendCmd(pstcI2c, WM8731_REG_LHPHONE_OUT, u16LeftHpOut);
    enRet = WM8731_SendCmd(pstcI2c, WM8731_REG_RHPHONE_OUT, u16RightHpOut);

    return enRet;
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
