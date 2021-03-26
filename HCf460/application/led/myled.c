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
	  PORT_Init(PortA, Pin05, &stcPortInit);   //Green
	  	 
		led_red(1);
		led_green(1);	//初始时是亮的
}

void led_red(char onoff)
{
    if (onoff)
    {
        PORT_ResetBits(PortA, Pin01);
    }
    else
    {
        PORT_SetBits(PortA, Pin01);
    }
}

void led_green(char onoff)
{
    if (onoff)
    {
        PORT_ResetBits(PortA, Pin05);
    }
    else
    {
        PORT_SetBits(PortA, Pin05);
    }
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
    stc_clk_mpll_cfg_t      stcMpllCfg;

    MEM_ZERO_STRUCT(enSysClkSrc);
    MEM_ZERO_STRUCT(stcSysClkCfg);
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

#ifdef USE_INTERN_HRC
		CLK_HrcCmd(Enable);       //Enable HRC
		stcMpllCfg.pllmDiv = 2u;//使用内部时钟源16M
    CLK_SetPllSource(ClkPllSrcHRC);//使用内部时钟源16M		
#else
		stc_clk_xtal_cfg_t      stcXtalCfg;
		MEM_ZERO_STRUCT(stcXtalCfg);
    stcXtalCfg.enMode = ClkXtalModeOsc;
    stcXtalCfg.enDrv = ClkXtalLowDrv;
    stcXtalCfg.enFastStartup = Enable;
    CLK_XtalConfig(&stcXtalCfg);
    CLK_XtalCmd(Enable);
		stcMpllCfg.pllmDiv = 1u;//使用外部时钟源8M
		CLK_SetPllSource(ClkPllSrcXTAL);//使用外部时钟源8M
#endif

    stcMpllCfg.plln = 42u;
    stcMpllCfg.PllpDiv = 2u;
    stcMpllCfg.PllqDiv = 2u;
    stcMpllCfg.PllrDiv = 2u;

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


