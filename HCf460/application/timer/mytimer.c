#include "mytimer.h"
#include "myadc.h"
#include "myusart.h"
#include "hc32_ddl.h"
#include "arm_math.h" 
#include "sys.h"
#include "fifo.h"

extern uint16_t m_au16Adc1SaValue[ADC1_CH_COUNT];
volatile uint32_t Timer_Counter = 0;

void Delay_ms(unsigned int t)
{
	Timer_Counter = 0;
	while (Timer_Counter < t)
	{
		//清看门狗
	}
}

void timer0_init(void)
{
    stc_tim0_base_init_t stcTimerCfg;
    stc_irq_regi_conf_t stcIrqRegiConf;
    stc_port_init_t stcPortInit;

    volatile uint32_t u32Pclk1;
		volatile uint32_t u32cpu;
    stc_clk_freq_t stcClkTmp;
    
    MEM_ZERO_STRUCT(stcTimerCfg);
    MEM_ZERO_STRUCT(stcIrqRegiConf);
    MEM_ZERO_STRUCT(stcPortInit);

    /* Get pclk1 */
    CLK_GetClockFreq(&stcClkTmp);
    u32Pclk1 = stcClkTmp.pclk1Freq;
		u32cpu = stcClkTmp.sysclkFreq;

    /* Timer0 peripheral enable */
    ENABLE_TMR0();

    /*config register for channel A */
    stcTimerCfg.Tim0_CounterMode = Tim0_Sync;
    stcTimerCfg.Tim0_SyncClockSource = Tim0_Pclk1;//50MHz
    stcTimerCfg.Tim0_ClockDivision = Tim0_ClkDiv0;
    stcTimerCfg.Tim0_CmpValue = (uint16_t)(50000 - 1);//1ms
    TIMER0_BaseInit(TMR_UNIT,Tim0_ChannelA,&stcTimerCfg);

    /* Enable channel A interrupt */
    TIMER0_IntCmd(TMR_UNIT,Tim0_ChannelA,Enable);
    /* Register TMR_INI_GCMA Int to Vect.No.001 */
    stcIrqRegiConf.enIRQn = Int001_IRQn;
    /* Select I2C Error or Event interrupt function */
    stcIrqRegiConf.enIntSrc = TMR_INI_GCMA;
    /* Callback function */
    stcIrqRegiConf.pfnCallback =&Timer0A_CallBack;
    /* Registration IRQ */
    enIrqRegistration(&stcIrqRegiConf);
    /* Clear Pending */
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    /* Set priority */
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_14);
    /* Enable NVIC */
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);    

    /*config register for channel B */
    stcTimerCfg.Tim0_CounterMode = Tim0_Sync;
    stcTimerCfg.Tim0_SyncClockSource = Tim0_Pclk1;//50MHz
    stcTimerCfg.Tim0_ClockDivision = Tim0_ClkDiv0;
    stcTimerCfg.Tim0_CmpValue = (uint16_t)(25000 - 1);//500us
    TIMER0_BaseInit(TMR_UNIT,Tim0_ChannelB,&stcTimerCfg);

    /* Enable channel B interrupt */
    TIMER0_IntCmd(TMR_UNIT,Tim0_ChannelB,Enable);
    /* Register TMR_INI_GCMB Int to Vect.No.002 */
    stcIrqRegiConf.enIRQn = Int002_IRQn;
    /* Select I2C Error or Event interrupt function */
    stcIrqRegiConf.enIntSrc = TMR_INI_GCMB;
    /* Callback function */
    stcIrqRegiConf.pfnCallback = &Timer0B_CallBack;
    /* Registration IRQ */
    enIrqRegistration(&stcIrqRegiConf);
    /* Clear Pending */
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    /* Set priority */
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_00);
    /* Enable NVIC */
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /*start timer0*/
    TIMER0_Cmd(TMR_UNIT,Tim0_ChannelA,Enable);
    TIMER0_Cmd(TMR_UNIT,Tim0_ChannelB,Enable);
}

static void Timer0B_CallBack(void)		// T === 500us
{
  u16  if_adc_data = 0;		//IF adc数据
  //u16  light_sensor_adc_data = 0;	//光敏数据

	if (Set == DMA_GetIrqFlag(ADC1_SA_DMA_UNIT, ADC1_SA_DMA_CH, BlkTrnCpltIrq))
	{
		DMA_ClearIrqFlag(ADC1_SA_DMA_UNIT, ADC1_SA_DMA_CH, BlkTrnCpltIrq);
		if_adc_data =  m_au16Adc1SaValue[6u];
		//light_sensor_adc_data =  m_au16Adc1SaValue[9u];
		
		FIFO_WriteOneData(&FIFO_Data[0], if_adc_data);		
	}
}

static void Timer0A_CallBack(void)      //  T = 1ms
{
	Timer_Counter++;
}

