#ifndef _3117DC73_499B_4141_97BB_6FAA213755BA_
#define _3117DC73_499B_4141_97BB_6FAA213755BA_

union KKK
{
	int	int_value[13];
	struct
	{
		float	quick_time_times;
		float	quick_time_add;
		float	quick_freq_times;
		float	slow_time_times;
		float	slow_time_add;
		float	slow_freq_times;
		float	res_times;
		float	offsetmin;
		int		Light_threshold1;
		int		Light_threshold2;
		int		Light_threshold3;
		int		Light_threshold4;
		int		delay_time_num;
	}ppp;
};

#define	QUICK_TIME_TIMES_FLASH_INT	(*((volatile int *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x00)))
#define	QUICK_TIME_TIMES_FLASH	(*((volatile float *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x00)))

#define	QUICK_TIME_ADD_FLASH_INT	(*((volatile int *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x04)))
#define	QUICK_TIME_ADD_FLASH	(*((volatile float *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x04)))

#define	QUICK_FREQ_TIMES_FLASH_INT	(*((volatile int *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x08)))
#define	QUICK_FREQ_TIMES_FLASH	(*((volatile float *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x08)))

#define	SLOW_TIME_TIMES_FLASH_INT	(*((volatile int *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x0C)))
#define	SLOW_TIME_TIMES_FLASH	(*((volatile float *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x0C)))

#define	SLOW_TIME_ADD_FLASH_INT	(*((volatile int *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x10)))
#define	SLOW_TIME_ADD_FLASH	(*((volatile float *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x10)))

#define	SLOW_FREQ_TIMES_FLASH_INT	(*((volatile int *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x14)))
#define	SLOW_FREQ_TIMES_FLASH	(*((volatile float *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x14)))

#define	RES_TIMES_FLASH_INT	(*((volatile int *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x18)))
#define	RES_TIMES_FLASH	(*((volatile float *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x18)))

#define	OFFSETMIN_FLASH_INT	(*((volatile int *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x1C)))
#define	OFFSETMIN_FLASH	(*((volatile float *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x1C)))

#define	RESETBTCNT_FLASH	(*((volatile int *)(USER_PARAMETER_START_SECTOR_ADDRESS1 + 0x00)))
#define	BT_JOIN_CNT_FLASH	(*((volatile int *)(USER_PARAMETER_START_SECTOR_ADDRESS1 + 0x04)))

#define LIGHT_THRESHOLD1_FLASH	(*((volatile int *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x20)))
#define LIGHT_THRESHOLD2_FLASH	(*((volatile int *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x24)))
#define LIGHT_THRESHOLD3_FLASH	(*((volatile int *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x28)))
#define LIGHT_THRESHOLD4_FLASH	(*((volatile int *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x2C)))

#define DELAY_TIME_NUM_FLASH (*((volatile int *)(USER_PARAMETER_START_SECTOR_ADDRESS0 + 0x30)))







#endif//_3117DC73_499B_4141_97BB_6FAA213755BA_
