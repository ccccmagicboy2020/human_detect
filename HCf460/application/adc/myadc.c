#include "myadc.h"
#include "hc32_ddl.h"

uint16_t m_au16Adc1SaValue[ADC1_CH_COUNT];
uint16_t m_au16Adc2SaValue[ADC2_CH_COUNT];

static void AdcSetChannelPinMode(const M4_ADC_TypeDef *ADCx,
                                 uint32_t u32Channel,
                                 en_pin_mode_t enMode);
static void AdcSetPinMode(uint8_t u8AdcPin, en_pin_mode_t enMode);





/**
 *******************************************************************************
 ** \brief  ADC clock configuration.
 **
 ** \note   1) ADCLK max frequency is 60MHz.
 **         2) If PCLK2 and PCLK4 are selected as the ADC clock,
 **            the following conditions must be met:
 **            a. ADCLK(PCLK2) max 60MHz;
 **            b. PCLK4 : ADCLK = 1:1, 2:1, 4:1, 8:1, 1:2, 1:4
 **
 ******************************************************************************/
void AdcClockConfig(void)
{
	//PCLK2:50--->>>ADC_CLK
	//PCLK4:100
}

/**
 *******************************************************************************
 ** \brief  ADC initial configuration.
 **
 ******************************************************************************/
void AdcInitConfig(void)
{
    stc_adc_init_t stcAdcInit;

    MEM_ZERO_STRUCT(stcAdcInit);

    stcAdcInit.enResolution = AdcResolution_12Bit;
    stcAdcInit.enDataAlign  = AdcDataAlign_Right;
    stcAdcInit.enAutoClear  = AdcClren_Enable;
    stcAdcInit.enScanMode   = AdcMode_SAContinuous;

		/* 1. Enable ADC1. */
    PWC_Fcg3PeriphClockCmd(PWC_FCG3_PERIPH_ADC1, Enable);
    /* 2. Initialize ADC1. */
    ADC_Init(M4_ADC1, &stcAdcInit);
	
    stcAdcInit.enResolution = AdcResolution_12Bit;
    stcAdcInit.enDataAlign  = AdcDataAlign_Right;
    stcAdcInit.enAutoClear  = AdcClren_Enable;
    stcAdcInit.enScanMode   = AdcMode_SAContinuous;	
	
    /* 1. Enable ADC2. */
    PWC_Fcg3PeriphClockCmd(PWC_FCG3_PERIPH_ADC2, Enable);
    /* 2. Initialize ADC2. */
    ADC_Init(M4_ADC2, &stcAdcInit);
}

/**
 *******************************************************************************
 ** \brief  ADC channel configuration.
 **
 ******************************************************************************/
void AdcChannelConfig(void)
{
    stc_adc_ch_cfg_t stcChCfg;
    uint8_t au8Adc1SaSampTime[ADC1_SA_CHANNEL_COUNT] = ADC1_SA_CHANNEL_SAMPLE_TIME;
		uint8_t au8Adc2SaSampTime[ADC2_SA_CHANNEL_COUNT] = ADC2_SA_CHANNEL_SAMPLE_TIME;

    MEM_ZERO_STRUCT(stcChCfg);
	
    /* 1. Set the ADC pin to analog mode. */
    AdcSetChannelPinMode(M4_ADC1, ADC1_CHANNEL, Pin_Mode_Ana);  // Pin_Mode_Ana = 2  GPIO Analog mode	

		/* 2. Add ADC channel. */
    stcChCfg.u32Channel  = ADC1_SA_CHANNEL;
    stcChCfg.u8Sequence  = ADC_SEQ_A;
    stcChCfg.pu8SampTime = au8Adc1SaSampTime;
    ADC_AddAdcChannel(M4_ADC1, &stcChCfg);

    /* 3. Configure the average channel if you need. */
		//ADC_ConfigAvg(M4_ADC1, AdcAvcnt_64);
    //ADC_ConfigAvg(M4_ADC1, AdcAvcnt_128);
		ADC_ConfigAvg(M4_ADC1, AdcAvcnt_256);
    /* 4. Add average channel if you need. */
    ADC_AddAvgChannel(M4_ADC1, ADC1_AVG_CHANNEL);
/////////////////////////////////////////////////////////////////////////////////
    /* 0. Remap channels and pins */
    //ADC_ChannleRemap(M4_ADC2, ADC2_CH1, ADC1_IN0);
    /* 1. Set the ADC pin to analog mode. */
    AdcSetChannelPinMode(M4_ADC2, ADC2_CHANNEL, Pin_Mode_Ana);  // Pin_Mode_Ana = 2  GPIO Analog mode	

		/* 2. Add ADC channel. */
    stcChCfg.u32Channel  = ADC2_SA_CHANNEL;
    stcChCfg.u8Sequence  = ADC_SEQ_A;
    stcChCfg.pu8SampTime = au8Adc2SaSampTime;
    ADC_AddAdcChannel(M4_ADC2, &stcChCfg);

    /* 3. Configure the average channel if you need. */
		//ADC_ConfigAvg(M4_ADC2, AdcAvcnt_64);
    //ADC_ConfigAvg(M4_ADC2, AdcAvcnt_128);
		ADC_ConfigAvg(M4_ADC2, AdcAvcnt_256);
    /* 4. Add average channel if you need. */
    ADC_AddAvgChannel(M4_ADC2, ADC2_AVG_CHANNEL);
	
		//finally
		ADC_ConfigPga((en_adc_pga_factor_t)PGA_FACTOR, AdcPgaNegative_VSSA);
    /* Add PGA pin */
    ADC_AddPgaChannel(ADC1_PGA_CHANNEL);
    /* Enable PGA. */
    ADC_PgaCmd(Enable);
}

/**
 *******************************************************************************
 ** \brief  Config the pin which is mapping the channel to analog or digit mode.
 **
 ******************************************************************************/
static void AdcSetChannelPinMode(const M4_ADC_TypeDef *ADCx,
                                 uint32_t u32Channel,
                                 en_pin_mode_t enMode)
{
    uint8_t u8ChIndex;
#if (ADC_CH_REMAP)
    uint8_t u8AdcPin;
#else
    uint8_t u8ChOffset = 0u;
#endif

    if (M4_ADC1 == ADCx)
    {
        u32Channel &= ADC1_PIN_MASK_ALL;
    }
    else
    {
        u32Channel &= ADC2_PIN_MASK_ALL;
#if (!ADC_CH_REMAP)
        u8ChOffset = 4u;
#endif
    }

    u8ChIndex = 0u;
    while (0u != u32Channel)
    {
        if (u32Channel & 0x1ul)
        {
#if (ADC_CH_REMAP)
            u8AdcPin = ADC_GetChannelPinNum(ADCx, u8ChIndex);
            AdcSetPinMode(u8AdcPin, enMode);
#else
            AdcSetPinMode((u8ChIndex+u8ChOffset), enMode);
#endif
        }

        u32Channel >>= 1u;
        u8ChIndex++;
    }
}

/**
 *******************************************************************************
 ** \brief  Set an ADC pin as analog input mode or digit mode.
 **
 ******************************************************************************/
static void AdcSetPinMode(uint8_t u8AdcPin, en_pin_mode_t enMode)
{
    en_port_t enPort = PortA;
    en_pin_t enPin   = Pin00;
    bool bFlag       = true;
    stc_port_init_t stcPortInit;

    MEM_ZERO_STRUCT(stcPortInit);
    stcPortInit.enPinMode = enMode;
    stcPortInit.enPullUp  = Disable;

    switch (u8AdcPin)
    {
    case ADC1_IN0:
        enPort = PortA;
        enPin  = Pin00;
        break;

    case ADC1_IN1:
        enPort = PortA;
        enPin  = Pin01;
        break;

    case ADC1_IN2:
        enPort = PortA;
        enPin  = Pin02;
        break;

    case ADC1_IN3:
        enPort = PortA;
        enPin  = Pin03;
        break;

    case ADC12_IN4:
        enPort = PortA;
        enPin  = Pin04;
        break;

    case ADC12_IN5:
        enPort = PortA;
        enPin  = Pin05;
        break;

    case ADC12_IN6:
        enPort = PortA;
        enPin  = Pin06;
        break;

    case ADC12_IN7:
        enPort = PortA;
        enPin  = Pin07;
        break;

    case ADC12_IN8:
        enPort = PortB;
        enPin  = Pin00;
        break;

    case ADC12_IN9:
        enPort = PortB;
        enPin  = Pin01;
        break;

    case ADC12_IN10:
        enPort = PortC;
        enPin  = Pin00;
        break;

    case ADC12_IN11:
        enPort = PortC;
        enPin  = Pin01;
        break;

    case ADC1_IN12:
        enPort = PortC;
        enPin  = Pin02;
        break;

    case ADC1_IN13:
        enPort = PortC;
        enPin  = Pin03;
        break;

    case ADC1_IN14:
        enPort = PortC;
        enPin  = Pin04;
        break;

    case ADC1_IN15:
        enPort = PortC;
        enPin  = Pin05;
        break;

    default:
        bFlag = false;
        break;
    }

    if (true == bFlag)
    {
        PORT_Init(enPort, enPin, &stcPortInit);
    }
}

/**
 *******************************************************************************
 ** \brief  DMA configuration for ADC1 and ADC2.
 **
 ******************************************************************************/
static void AdcDmaConfig(void)
{
    stc_dma_config_t stcDmaCfg;

    MEM_ZERO_STRUCT(stcDmaCfg);

		//AOS
	  PWC_Fcg0PeriphClockCmd(PWC_FCG0_PERIPH_AOS, Enable);
		//DMA1
    stcDmaCfg.u16BlockSize   = ADC1_CH_COUNT;
    stcDmaCfg.u16TransferCnt = 0u;
    stcDmaCfg.u32SrcAddr     = (uint32_t)(&M4_ADC1->DR0);
    stcDmaCfg.u32DesAddr     = (uint32_t)(&m_au16Adc1SaValue[0]);
    stcDmaCfg.u16DesRptSize  = ADC1_CH_COUNT;
    stcDmaCfg.u16SrcRptSize  = ADC1_CH_COUNT;
    stcDmaCfg.u32DmaLlp      = 0u;
    stcDmaCfg.stcSrcNseqCfg.u16Cnt    = 0u;
    stcDmaCfg.stcSrcNseqCfg.u32Offset = 0u;
    stcDmaCfg.stcDesNseqCfg.u16Cnt    = 0u;
    stcDmaCfg.stcDesNseqCfg.u32Offset = 0u;
    stcDmaCfg.stcDmaChCfg.enSrcInc    = AddressIncrease;
    stcDmaCfg.stcDmaChCfg.enDesInc    = AddressIncrease;
    stcDmaCfg.stcDmaChCfg.enSrcRptEn  = Enable;
    stcDmaCfg.stcDmaChCfg.enDesRptEn  = Enable;
    stcDmaCfg.stcDmaChCfg.enSrcNseqEn = Disable;
    stcDmaCfg.stcDmaChCfg.enDesNseqEn = Disable;
    stcDmaCfg.stcDmaChCfg.enTrnWidth  = Dma16Bit;
    stcDmaCfg.stcDmaChCfg.enLlpEn     = Disable;
    stcDmaCfg.stcDmaChCfg.enIntEn     = Disable;

    PWC_Fcg0PeriphClockCmd(ADC1_SA_DMA_PWC, Enable);
    DMA_InitChannel(ADC1_SA_DMA_UNIT, ADC1_SA_DMA_CH, &stcDmaCfg);
    DMA_Cmd(ADC1_SA_DMA_UNIT, Enable);
    DMA_ChannelCmd(ADC1_SA_DMA_UNIT, ADC1_SA_DMA_CH, Enable);
    DMA_ClearIrqFlag(ADC1_SA_DMA_UNIT, ADC1_SA_DMA_CH, TrnCpltIrq);
    DMA_SetTriggerSrc(ADC1_SA_DMA_UNIT, ADC1_SA_DMA_CH, ADC1_SA_DMA_TRGSRC);
		
		//DMA2
    stcDmaCfg.u16BlockSize   = ADC2_CH_COUNT;
    stcDmaCfg.u16TransferCnt = 0u;
    stcDmaCfg.u32SrcAddr     = (uint32_t)(&M4_ADC2->DR0);
    stcDmaCfg.u32DesAddr     = (uint32_t)(&m_au16Adc2SaValue[0]);
    stcDmaCfg.u16DesRptSize  = ADC2_CH_COUNT;
    stcDmaCfg.u16SrcRptSize  = ADC2_CH_COUNT;
    stcDmaCfg.u32DmaLlp      = 0u;
    stcDmaCfg.stcSrcNseqCfg.u16Cnt    = 0u;
    stcDmaCfg.stcSrcNseqCfg.u32Offset = 0u;
    stcDmaCfg.stcDesNseqCfg.u16Cnt    = 0u;
    stcDmaCfg.stcDesNseqCfg.u32Offset = 0u;
    stcDmaCfg.stcDmaChCfg.enSrcInc    = AddressIncrease;
    stcDmaCfg.stcDmaChCfg.enDesInc    = AddressIncrease;
    stcDmaCfg.stcDmaChCfg.enSrcRptEn  = Enable;
    stcDmaCfg.stcDmaChCfg.enDesRptEn  = Enable;
    stcDmaCfg.stcDmaChCfg.enSrcNseqEn = Disable;
    stcDmaCfg.stcDmaChCfg.enDesNseqEn = Disable;
    stcDmaCfg.stcDmaChCfg.enTrnWidth  = Dma16Bit;
    stcDmaCfg.stcDmaChCfg.enLlpEn     = Disable;
    stcDmaCfg.stcDmaChCfg.enIntEn     = Disable;

    PWC_Fcg0PeriphClockCmd(ADC2_SA_DMA_PWC, Enable);
    DMA_InitChannel(ADC2_SA_DMA_UNIT, ADC2_SA_DMA_CH, &stcDmaCfg);
    DMA_Cmd(ADC2_SA_DMA_UNIT, Enable);
    DMA_ChannelCmd(ADC2_SA_DMA_UNIT, ADC2_SA_DMA_CH, Enable);
    DMA_ClearIrqFlag(ADC2_SA_DMA_UNIT, ADC2_SA_DMA_CH, TrnCpltIrq);
    DMA_SetTriggerSrc(ADC2_SA_DMA_UNIT, ADC2_SA_DMA_CH, ADC2_SA_DMA_TRGSRC);
}

void AdcConfig(void)
{
    AdcClockConfig();
    AdcInitConfig();
    AdcChannelConfig();
    AdcDmaConfig();
}


