#include "mytimer.h"
#include "myadc.h"
#include "myusart.h"
#include "hc32_ddl.h"
#include "arm_math.h" 
#include "sys.h"

/* Define Timer Unit for example */
#define TMR_UNIT            (M4_TMR02)
#define TMR_INI_GCMA        (INT_TMR02_GCMA)
#define TMR_INI_GCMB        (INT_TMR02_GCMB)

#define ENABLE_TMR0()      (PWC_Fcg2PeriphClockCmd(PWC_FCG2_PERIPH_TIM02, Enable))
#define  DLY_MS           (500ul)
#define TIMEOUT_MS        (10u)

#define  LED0_PORT        (PortB)
#define  LED0_PIN         (Pin06)

u8 change_flag = 0;
u16  adc_data = 0,Timer_Count = 0, Timer_Count1 = 0,Timer_Count2 = 0;//Òç³ö´ÎÊý;
u16  AD_Value[512] = {0};

static uint16_t m_au16Adc1Value[ADC1_CH_COUNT];
void Timer0A_CallBack(void);

void timer0_init(void)
{
	stc_tim0_base_init_t stcTimerCfg;
    stc_irq_regi_conf_t stcIrqRegiConf;
    stc_port_init_t stcPortInit;
	
    stc_clk_freq_t stcClkTmp;

    MEM_ZERO_STRUCT(stcTimerCfg);
    MEM_ZERO_STRUCT(stcIrqRegiConf);
    MEM_ZERO_STRUCT(stcPortInit);
	
	    /* Get pclk1 */
    CLK_GetClockFreq(&stcClkTmp);

    /* Enable XTAL32 */
    CLK_Xtal32Cmd(Enable);

    /* Timer0 peripheral enable */
    ENABLE_TMR0();
    /*config register for channel A */
    stcTimerCfg.Tim0_CounterMode = Tim0_Async;
    stcTimerCfg.Tim0_AsyncClockSource = Tim0_XTAL32;
    stcTimerCfg.Tim0_ClockDivision = Tim0_ClkDiv0;
    stcTimerCfg.Tim0_CmpValue = (uint16_t)(15);
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
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_15);
    /* Enable NVIC */
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
	
	/*start timer0*/
    TIMER0_Cmd(TMR_UNIT,Tim0_ChannelA,Enable);
}

void Timer0A_CallBack(void)
{
//	PORT_Toggle(LED0_PORT, LED0_PIN);
	ADC_PollingSa(M4_ADC1, m_au16Adc1Value, ADC1_CH_COUNT, TIMEOUT_MS);
	adc_data =  m_au16Adc1Value[6u];
	AD_Value[Timer_Count] = adc_data;
	Timer_Count++;
		           
	Timer_Count1++;
	Timer_Count2++;
/********************* TEST *****************************************/			
			  
//	if(Timer_Count2==1 || (Timer_Count2-1)%256 == 0)
//	{
//		if((Timer_Count2-1)%2048 == 0)
//		{
//			printf("BegiN ");
//		}
//		else
//		{
//			printf("Begin ");
//		}
//					
//	}
//	printf("%04d ",adc_data);
//	if( Timer_Count1==256 )
//	{
//		Timer_Count1 = 0;
//		if(Timer_Count2==2048)
//		{
//			Timer_Count2 = 0;
//			printf("End\r\n");
//		}
//		else
//		{
//			printf("End\r\n");
//		}
//					
//					
//	}	
/********************* TEST *****************************************/			
//	printf("DC12_IN6 value %d \r\n",adc_data);
	if(Timer_Count == 512)
	{
		Timer_Count = 0;
		change_flag = 1;
	}
	else
	{
		change_flag = 0;
	}
	UsartRxErrProcess();
}

