#ifndef __MYLED_H
#define __MYLED_H

#define   KEY1    PORT_GetBit(PortB,Pin06) 
#define   KEY7    PORT_GetBit(PortB,Pin14) 
#define   KEY8    PORT_GetBit(PortB,Pin15)
#define   KEY5    PORT_GetBit(PortA,Pin08) 
#define   KEY6    PORT_GetBit(PortB,Pin13)

//extern void key_init(void);
extern void led_init(void);
extern void SysClkIni(void);
//extern void KEY_Init(void);

extern void led_red(char onoff);
extern void led_green(char onoff);

#endif

