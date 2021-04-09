#ifndef _SYS_H
#define _SYS_H

#include "stdint.h"
#include "fifo.h"
#include "bluetooth.h"
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"

typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  
typedef const int16_t sc16;  
typedef const int8_t sc8;

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  
typedef const uint16_t uc16;  
typedef const uint8_t uc8;

#define ALL_UPLOAD_DELAY 100
#define SEGGER_RTT_IN_RAM 1

#define EFM_BASE_ADDR		0x40010400
#define FEM_UQID1				(0x40010400 + 0x50)
#define FEM_UQID2				(0x40010400 + 0x54)
#define FEM_UQID3				(0x40010400 + 0x58)

#define GPIO0_HIGH()		(PORT_SetBits(PortA, Pin07))
#define GPIO0_LOW()			(PORT_ResetBits(PortA, Pin07))

#define GPIO1_HIGH()		(PORT_SetBits(PortA, Pin08))
#define GPIO1_LOW()			(PORT_ResetBits(PortA, Pin08))

#define GPIO2_HIGH()		(PORT_SetBits(PortB, Pin06))
#define GPIO2_LOW()			(PORT_ResetBits(PortB, Pin06))

#define GPIO3_HIGH()		(PORT_SetBits(PortB, Pin05))
#define GPIO3_LOW()			(PORT_ResetBits(PortB, Pin05))

#define GPIO4_HIGH()		(PORT_SetBits(PortA, Pin00))
#define GPIO4_LOW()			(PORT_ResetBits(PortA, Pin00))

#define GPIO5_HIGH()		(PORT_SetBits(PortA, Pin04))
#define GPIO5_LOW()			(PORT_ResetBits(PortA, Pin04))

#define GPIO6_HIGH()		(PORT_SetBits(PortB, Pin00))
#define GPIO6_LOW()			(PORT_ResetBits(PortB, Pin00))

typedef struct Val
{
	unsigned short Val1;
	unsigned short Val2;
} Val_t;

enum work_mode
{
	ALL_CHECK=0,			//default
	FAST_CHECK_ONLY,	//只查快检测
	SLOW_CHECK_ONLY,	//只查慢检测
	MODE_OTHER
};

enum app_state
{
	FAST_CHECK_DATA_PREPARE=0,
	FAST_CHECK,
	SLOW_CHECK_DATA_PREPARE_S0,
	SLOW_CHECK_DATA_PREPARE_S1,
	SLOW_CHECK_S0,
	SLOW_CHECK_S1,
	IDLE,
	UART_PROTOCOL,
	ERROR_ERROR,
};

enum tuya_upload_check_status
{
	TUYA_FAST_CHECK=0,
	TUYA_SLOW_CHECK=1,
	TUYA_INIT=2,
	TUYA_OTHER=3
};

enum tuya_upload_person_in_range_status
{
	TUYA_PERSON_STATUS_NO_PERSON=0,
	TUYA_PERSON_STATUS_HAVE_PERSON=1,
	TUYA_PERSON_STATUS_HAVE_PERSON_FROM_GROUP=2,
	TUYA_PERSON_STATUS_OTHER=3
};

enum slow_s0_result
{
	NO_PERSON_NOT_SURE=0,			//no person but not sure
	NO_PERSON,
	BIG_MOTION,
	BREATHE,
	BREATHE_NOT_SURE,				//breathe but not sure
};

#endif


