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

u8 change_flag1 = 0,change_flag2 = 0,change_flag3 = 0,change_flag4 = 0,AD_Zanflag = 0,AD_Zanflag1 = 0;
u16  adc_data = 0,Timer_Count1 = 0, Timer_Count2 = 0,Timer_Count3 = 0,Timer_Count4 = 0;//Òç³ö´ÎÊý;

u16  AD_Value[512] = {0};
u16  AD_Value1[512] = {0};
u16  AD_buffer[4096] = {0};
u16  AD_buffer1[4096] = {0};


static uint16_t m_au16Adc1Value[ADC1_CH_COUNT];
void Timer0B_CallBack(void);
extern u8 Sampling_status;

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

void Timer0B_CallBack(void)
{
//	PORT_Toggle(LED0_PORT, LED0_PIN);
	ADC_PollingSa(M4_ADC1, m_au16Adc1Value, ADC1_CH_COUNT, TIMEOUT_MS);
	adc_data =  m_au16Adc1Value[6u];
/******************************TEST***********************************/	
/******************************TEST***********************************/	
//	Ti_Count++;
//	if(Ti_Count == 1)
//	{
//		printf("B ");
//	}
//	printf("%4d ",adc_data);
//	if(Ti_Count == 512)
//	{
//		Ti_Count = 0;
//		printf("\r\n");
//	}
/******************************TEST***********************************/	
/******************************TEST***********************************/
    if(Sampling_status == 0)
	{
		if(AD_Zanflag == 0)
		{
			AD_Value[Timer_Count1] = adc_data;
			Timer_Count1++;
		}
		else
		{
			AD_Value1[Timer_Count2] =  adc_data;
			Timer_Count2++;
		}
		
		if(Timer_Count1 == 512)
		{
			Timer_Count1 = 0;
			AD_Zanflag = 1;
			change_flag1 = 1;
			
		}
		else if(Timer_Count2 == 512)
		{
			Timer_Count2 = 0;
			AD_Zanflag = 0;
			change_flag2 = 1;
			
		}
			

	}
	
	if(Sampling_status == 1)
	{

		if(AD_Zanflag1 == 0)
		{
			AD_buffer[Timer_Count3] = adc_data;
			Timer_Count3++;
		}
		else
		{
			AD_buffer1[Timer_Count4] =  adc_data;
			Timer_Count4++;
		}
		
		if(Timer_Count3 == 4096)
		{
			Timer_Count3 = 0;
			AD_Zanflag1 = 1;
			change_flag3 = 1;
			
		}
		else if(Timer_Count4 == 4096)
		{
			Timer_Count4 = 0;
			AD_Zanflag1 = 0;
			change_flag4 = 1;
			
		}

	}
	UsartRxErrProcess();
}

