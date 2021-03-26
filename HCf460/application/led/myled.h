#ifndef __MYLED_H
#define __MYLED_H

extern void led_init(void);
extern void SysClkIni(void);

extern void led_red(char onoff);
extern void led_green(char onoff);

extern void led_onboard_status_upload(unsigned char aaaa);
extern void check_status_upload(unsigned char aaaa);

#endif

