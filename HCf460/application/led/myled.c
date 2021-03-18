#include "myled.h"
#include "hc32_ddl.h"
#include "sys.h"

void led_init(void)
{
	  stc_port_init_t stcPortInit;

      /* configuration structure initialization */
      MEM_ZERO_STRUCT(stcPortInit);

      stcPortInit.enPinMode = Pin_Mode_Out;
      stcPortInit.enExInt = Enable;
      stcPortInit.enPullUp = Enable;

      /* LED0 Port/Pin initialization */
      PORT_Init(PortA, Pin01, &stcPortInit);   //Red
	  PORT_Init(PortA, Pin05, &stcPortInit);   //Blue
	  
	  PORT_Init(PortB, Pin06, &stcPortInit);   //Blue
	 
	  PORT_SetBits(PortA, Pin01);
	  PORT_SetBits(PortA, Pin05); 
}

void LED_RED(void)
{
	PORT_ResetBits(PortA, Pin01);
	PORT_SetBits(PortA, Pin05);	
}

void LED_RED_TWO(void)
{
	PORT_ResetBits(PortA, Pin01);
	PORT_SetBits(PortA, Pin05);
	Ddl_Delay1ms(50);
	PORT_SetBits(PortA, Pin01);
	PORT_SetBits(PortA, Pin05);
	Ddl_Delay1ms(50);
	PORT_ResetBits(PortA, Pin01);
	PORT_SetBits(PortA, Pin05);
}

void LED_BLUE_TWO(void)
{
	PORT_SetBits(PortA, Pin01);
	PORT_ResetBits(PortA, Pin05);
	Ddl_Delay1ms(50);
	PORT_SetBits(PortA, Pin01);
	PORT_SetBits(PortA, Pin05);
	Ddl_Delay1ms(50);
	PORT_SetBits(PortA, Pin01);
	PORT_ResetBits(PortA, Pin05);
}

void LED_GREEN(void)
{
	PORT_ResetBits(PortA, Pin01);
	PORT_ResetBits(PortA, Pin05);
}

void LED_GREEN_TWO(void)
{
	
	PORT_ResetBits(PortA, Pin01);
	PORT_ResetBits(PortA, Pin05);
	Ddl_Delay1ms(50);
	PORT_SetBits(PortA, Pin01);
	PORT_SetBits(PortA, Pin05);
	Ddl_Delay1ms(50);
	PORT_ResetBits(PortA, Pin01);
	PORT_ResetBits(PortA, Pin05);
	
}



/**
 ******************************************************************************
 ** \brief  Initialize the system clock for the sample
 **
 ** \param  None
 **
 ** \return None
 ******************************************************************************/
void SysClkIni(void)
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
    stcSysClkCfg.enHclkDiv = ClkSysclkDiv1;   // 168MHz
    stcSysClkCfg.enExclkDiv = ClkSysclkDiv2;  // 84MHz
    stcSysClkCfg.enPclk0Div = ClkSysclkDiv1;  // 168MHz
    stcSysClkCfg.enPclk1Div = ClkSysclkDiv4;  // 42MHz
    stcSysClkCfg.enPclk2Div = ClkSysclkDiv4;  // 42MHz
    stcSysClkCfg.enPclk3Div = ClkSysclkDiv4;  // 42MHz
    stcSysClkCfg.enPclk4Div = ClkSysclkDiv2;  // 84MHz
    CLK_SysClkConfig(&stcSysClkCfg);

    /* Switch system clock source to MPLL. */
    /* Use Xtal as MPLL source. */
    stcXtalCfg.enMode = ClkXtalModeOsc;
    stcXtalCfg.enDrv = ClkXtalLowDrv;
    stcXtalCfg.enFastStartup = Enable;
    CLK_XtalConfig(&stcXtalCfg);
    CLK_XtalCmd(Enable);

    /* MPLL config. */
    /*system clk = 168M, pclk1 = 84M, pclk3 = 42M*/
    stcMpllCfg.pllmDiv = 1u;
    stcMpllCfg.plln = 42u;
    stcMpllCfg.PllpDiv = 2u;
    stcMpllCfg.PllqDiv = 2u;
    stcMpllCfg.PllrDiv = 2u;

    CLK_SetPllSource(ClkPllSrcXTAL);
    CLK_MpllConfig(&stcMpllCfg);

    /* flash read wait cycle setting */
    EFM_Unlock();
    EFM_SetLatency(EFM_LATENCY_4);
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


