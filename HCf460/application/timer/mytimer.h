#ifndef __MYTIMER_H
#define __MYTIMER_H

/* Define Timer Unit for example */
#define TMR_UNIT            (M4_TMR02)
#define TMR_INI_GCMA        (INT_TMR02_GCMA)
#define TMR_INI_GCMB        (INT_TMR02_GCMB)

#define ENABLE_TMR0()      (PWC_Fcg2PeriphClockCmd(PWC_FCG2_PERIPH_TIM02, Enable))

extern void timer0_init(void);
extern void Delay_ms(unsigned int t);
static void Timer0A_CallBack(void);
static void Timer0B_CallBack(void);

#endif


