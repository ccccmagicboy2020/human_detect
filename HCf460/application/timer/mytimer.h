#ifndef __MYTIMER_H
#define __MYTIMER_H


/* Define Timer Unit for example */
#define TMR_UNIT            (M4_TMR02)
#define TMR_INI_GCMA        (INT_TMR02_GCMA)
#define TMR_INI_GCMB        (INT_TMR02_GCMB)

#define ENABLE_TMR0()      (PWC_Fcg2PeriphClockCmd(PWC_FCG2_PERIPH_TIM02, Enable))
#define  DLY_MS           (500ul)
#define TIMEOUT_MS        (10u)

#define  LED0_PORT        (PortB)
#define  LED0_PIN         (Pin06)


extern void timer0_init(void);

static void Timer0B_CallBack(void);

#endif


