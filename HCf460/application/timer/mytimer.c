#include "mytimer.h"
#include "myadc.h"
#include "myusart.h"
#include "hc32_ddl.h"
#include "arm_math.h" 
#include "sys.h"
#include "fifo.h"

static uint16_t m_au16Adc1Value[ADC1_CH_COUNT];

void timer0_init(void)
{
    stc_tim0_base_init_t stcTimerCfg;
    stc_irq_regi_conf_t stcIrqRegiConf;
    stc_port_init_t stcPortInit;

    volatile uint32_t u32Pclk1;
    stc_clk_freq_t stcClkTmp;
    
	

    MEM_ZERO_STRUCT(stcTimerCfg);
    MEM_ZERO_STRUCT(stcIrqRegiConf);
    MEM_ZERO_STRUCT(stcPortInit);

    

  

    /* Get pclk1 */
    CLK_GetClockFreq(&stcClkTmp);
    u32Pclk1 = stcClkTmp.pclk1Freq;
	//u32cpu = stcClkTmp.sysclkFreq;

    /* Enable XTAL32 */
    CLK_Xtal32Cmd(Enable);

    /* Timer0 peripheral enable */
    ENABLE_TMR0();

    /*config register for channel B */
    stcTimerCfg.Tim0_CounterMode = Tim0_Sync;
    stcTimerCfg.Tim0_SyncClockSource = Tim0_Pclk1;
    stcTimerCfg.Tim0_ClockDivision = Tim0_ClkDiv0;
    stcTimerCfg.Tim0_CmpValue = (uint16_t)(25000);
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
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_15);
    /* Enable NVIC */
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /*start timer0*/
//    TIMER0_Cmd(TMR_UNIT,Tim0_ChannelA,Enable);
    TIMER0_Cmd(TMR_UNIT,Tim0_ChannelB,Enable);
}

static void Timer0B_CallBack(void)		// T === 500us
{
    u16  adc_data = 0;		//adcÊý¾Ý

//	PORT_Toggle(LED0_PORT, LED0_PIN);
	ADC_PollingSa(M4_ADC1, m_au16Adc1Value, ADC1_CH_COUNT, TIMEOUT_MS);
	adc_data =  m_au16Adc1Value[6u];
//	printf("DC12_IN6 value %d \r\n",adc_data);
	FIFO_WriteOneData(&FIFO_Data[0], adc_data);
	
//	UsartRxErrProcess();
}

