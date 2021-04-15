#include "hc32_ddl.h"
#include "myled.h"
#include "myusart.h"
#include "myadc.h"
#include "mytimer.h"
#include "sys.h"
#include "matlab.h"
#include "math.h" 
#include "arm_math.h" 
#include "hamming.h"
#include "cfar_ca_emxAPI.h"
#include "time_detection.h"
#include "std_cv.h"
#include "freq_detection.h"
#include "stdio.h"
#include "fifo.h"
#include "bluetooth.h"

#define		N						300         //CFAR窗口大小
#define		pro_N					200         //CFAR保护单元大小
#define		PAD						0.00000001f  //虚警概率
  
#define		rr_threshold			0.5f    //呼吸频率截取范围
#define		secnum					16

#define		FEEDRATE				2048u			//采样率，单位为samples/s

#define		FAST_CHECK_SAMPLES		512				//快检测样本数
#define		FAST_CHECK_DUTY			8u				//快检测周期，单位为秒s
#define		FAST_MAX_DATA_POOL		(FEEDRATE	*	FAST_CHECK_DUTY)
#define		FAST_CHECK_TIMES		(FAST_MAX_DATA_POOL	>> 9)

#define		SLOW_CHECK_SAMPLES		2048			//慢检测样本数
#define		SLOW_CHECK_OVER_SAMPLE	3				//慢检测超采样冥
#define		SLOW_CHECK_DUTY			16				//慢检测周期，单位为秒s
#define		SLOW_CHECK_USE_SAMPLES	(SLOW_CHECK_SAMPLES >> SLOW_CHECK_OVER_SAMPLE)	//慢检测实际用的样本数
#define		SLOW_MAX_DATA_POOL		((FEEDRATE >> SLOW_CHECK_OVER_SAMPLE)	*	SLOW_CHECK_DUTY)
#define		SLOW_CHECK_TIMES		(SLOW_MAX_DATA_POOL	>>	11)

#define		MAX_DATA_POOL			FAST_MAX_DATA_POOL

extern const float hamming_TAB2[4096];
extern FIFO_DataType FIFO_DataBuffer[FIFO_DATA_NUM][FIFO_DATA_SIZE];

//FIFO_DataType Fast_detection_data[MAX_DATA_POOL] __attribute__((section(".ARM.__at_0x1FFF8000"))) = {0};//big raw data pool
FIFO_DataType Fast_detection_data[MAX_DATA_POOL] = {0};//big raw data pool

volatile int state = FAST_CHECK_DATA_PREPARE;	//状态机变量
volatile int next_state = FAST_CHECK_DATA_PREPARE;	//状态机变量的下一个状态

volatile int slow_s0_result = SLOW_OTHERS;
volatile int slow_s0_result_last = SLOW_OTHERS;

char fast_retry_flag = 1;
char slow_retry_flag = 1;

int run_mode = 0;
int slow_only_flag = 0;

volatile int check_status = TUYA_OTHER;
volatile int person_in_range_flag = 0;
volatile int light_status_flag = 0;

////////////////////////////////////////////////////////////
char JS_RTT_UpBuffer[1024];
Val_t adc_value;
////////////////////////////////////////////////////////////
float quick_time_times = 4;			//4
float quick_time_add = 32;			//32
float quick_freq_times = 3;			//3
////////////////////////////////////////////////////////////
float	slow_time_times = 5;			//5
float slow_time_add = 40;				//40
float	slow_freq_times = 6;			//6
float	res_times = 60.0f;				//17.5
float	offsetmin = 0.6f;					//0.6
////////////////////////////////////////////////////////////
void check_status_upload(unsigned int aaaa);
void person_in_range_upload(unsigned int aaaa);
void slow_check_result_upload(unsigned int aaaa);
void clear_buffer(void);
void light_status_upload(unsigned int aaaa);
void stop_sample(unsigned char flag);
void memory_init(void);
void get_mcu_bt_mode(void);
void bt_hand_up(void);
void gpio_output(unsigned char res);
void set_samplerate(unsigned int speed);
////////////////////////////////////////////////////////////
unsigned char upload_disable = 1;
unsigned char g_work_mode = ALL_CHECK;
////////////////////////////////////////////////////////////
unsigned char find_me_flag = 0;
unsigned char find_me_counter = 0;
////////////////////////////////////////////////////////////
unsigned char light_sensor_upload_flag = 0;
extern unsigned short  light_sensor_adc_data;
extern unsigned short  light_sensor2_adc_data;
extern unsigned short  switch_dist;	// 距离设置
extern unsigned short  switch_delay;	// 延时设置
extern unsigned short  switch_light;	// 光敏门限3
////////////////////////////////////////////////////////////
unsigned short Light_threshold1 = 0;
unsigned short Light_threshold2 = 0;
unsigned short Light_threshold3 = 4000;		//up limit
unsigned short Light_threshold4 = 3800;		//down limit
////////////////////////////////////////////////////////////
unsigned int delay_time_num = 0;
unsigned int delay_s_num = 24;
////////////////////////////////////////////////////////////
volatile unsigned int  person_meter = 0;
volatile unsigned int  person_meter_last = 0;

float breathe_freq = 0;
volatile unsigned int slow_check_result = 1;//no person
int study_flag = 0;
int study_mode = 0;

float max_pp1_rt = 0;
float max_pp1_rt_last = 0;
float max_pp2_rt = 0;
float max_pp2_rt_last = 0;

//unsigned int fast_samplerate = 12499;	//4K-250us-128ms
//unsigned int fast_samplerate = 9999;	//5K
unsigned int fast_samplerate = 7499;		//6.7K-150us-76.8ms
//unsigned int fast_samplerate = 4999;	//10K
//unsigned int fast_samplerate = 2999;	//15K
//unsigned int fast_samplerate = 2499;	//20K
//unsigned int fast_samplerate = 1349;	//37K
////////////////////////////////////////////////////////////
//unsigned int slow_samplerate = 12499;	//4K
unsigned int slow_samplerate = 9999;		//5K
//unsigned int slow_samplerate = 7499;	//6.7K
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
void clear_buffer(void)
{
	fast_retry_flag = 1;
	slow_retry_flag = 1;
}

void gpio_output(unsigned char res)
{
	//
	if (res)
	{
		led_red(1);
		
		if (light_sensor2_adc_data < Light_threshold4)//门限3
		//if (light_sensor_adc_data < Light_threshold3)//门限3
		{
			GPIO1_HIGH();
		}
		else if (light_sensor2_adc_data > Light_threshold3)
		{
			GPIO1_LOW();
		}
		else
		{
			//do nothing
		}
		light_status_upload(1);
	}
	else
	{
		led_red(0);
		GPIO1_LOW();
		light_status_upload(0);
	}
}

void fast_output_result(char quick_detection_result)
{
	static char quick_detection_result_last = 0;
	SEGGER_RTT_printf(0, "%squick: %d%s\r\n", RTT_CTRL_BG_GREEN, quick_detection_result, RTT_CTRL_RESET);
	
	if (quick_detection_result != quick_detection_result_last)
	{
		quick_detection_result_last = quick_detection_result;
		
		if (quick_detection_result)//有人
		{
			person_in_range_upload(TUYA_PERSON_STATUS_HAVE_PERSON);
			gpio_output(1);
		}
		else						//无人
		{
			person_in_range_upload(TUYA_PERSON_STATUS_NO_PERSON);
			if (1)		//有继电器
			{
				stop_sample(1);
				gpio_output(0);
				Delay_ms(ALL_UPLOAD_DELAY);
				Delay_ms(ALL_UPLOAD_DELAY);
				Delay_ms(ALL_UPLOAD_DELAY);
				Delay_ms(ALL_UPLOAD_DELAY);
				Delay_ms(ALL_UPLOAD_DELAY);		

				Delay_ms(ALL_UPLOAD_DELAY);
				Delay_ms(ALL_UPLOAD_DELAY);
				Delay_ms(ALL_UPLOAD_DELAY);
				Delay_ms(ALL_UPLOAD_DELAY);
				Delay_ms(ALL_UPLOAD_DELAY);		

				Delay_ms(ALL_UPLOAD_DELAY);
				Delay_ms(ALL_UPLOAD_DELAY);
				Delay_ms(ALL_UPLOAD_DELAY);
				Delay_ms(ALL_UPLOAD_DELAY);
				Delay_ms(ALL_UPLOAD_DELAY);		
				
				stop_sample(0);
				clear_buffer();
			}
			else
			{
				gpio_output(0);
			}
		}
	}
}

static void enable_flash_cache(en_functional_state_t state)
{
	EFM_Unlock();
	EFM_FlashCmd(Enable);
	while(Set !=EFM_GetFlagStatus(EFM_FLAG_RDY))
	{
		;
	}
	EFM_InstructionCacheCmd(state);
	EFM_Lock();
}

void slow_output_result(char slow_s0_result)
{
	//
	Delay_ms(ALL_UPLOAD_DELAY);
	//
	switch (slow_s0_result)
	{
	case BIG_MOTION:	
		if (slow_only_flag == 1)
		{
			person_in_range_upload(TUYA_PERSON_STATUS_HAVE_PERSON);
			gpio_output(1);
			slow_check_result_upload(slow_s0_result);
		}
		else
		{
			slow_check_result_upload(slow_s0_result);
		}
		break;
	case BREATHE:
		if (slow_only_flag == 1)
		{
			person_in_range_upload(TUYA_PERSON_STATUS_HAVE_PERSON);
			gpio_output(1);
			slow_check_result_upload(slow_s0_result);
		}
		else
		{
			slow_check_result_upload(slow_s0_result);
		}
		break;
	case BREATHE_NOT_SURE:
		if (slow_only_flag == 1)
		{
			person_in_range_upload(TUYA_PERSON_STATUS_HAVE_PERSON);
			gpio_output(1);
			slow_check_result_upload(slow_s0_result);
		}
		else
		{
			slow_check_result_upload(slow_s0_result);
		}
		break;
	case NO_PERSON_NOT_SURE:
		slow_check_result_upload(slow_s0_result);
		break;
	case NO_PERSON:
		if (slow_only_flag == 1)
		{
			person_in_range_upload(TUYA_PERSON_STATUS_NO_PERSON);
			gpio_output(0);
			//slow_check_result_upload(slow_s0_result);
		}
		else
		{
			SEGGER_RTT_printf(0, "slow check no person go fast check\r\n");
		}
		break;
	default:
		break;
	}	
}
#pragma arm section code = "RAMCODE"
void fast_check_data_prepare(void)
{
	static	int i = 0;	//index, fullfill the tank first
	int k = 0;	//index
	
	if (fast_retry_flag)
	{
		fast_retry_flag = 0;		
		set_samplerate(fast_samplerate);
		slow_samplerate = 12499;
		i = 0;
		memory_init();
	}
	
	if (FAST_CHECK_TIMES > i)
	{
		//
	}
	
	if (FAST_CHECK_SAMPLES < FIFO_GetDataCount(&FIFO_Data[0]))
	{
		if (FAST_CHECK_TIMES == i)	// the tank is full
		{			
			for(k=0;k<MAX_DATA_POOL - FAST_CHECK_SAMPLES;k++)   //滑窗
			{
				Fast_detection_data[k] =Fast_detection_data[k + FAST_CHECK_SAMPLES];		
			}			
			SEGGER_RTT_printf(0, "fifo0 number: (i=%d) - %d\r\n", i, FIFO_GetDataCount(&FIFO_Data[0]));
			FIFO_ReadData(&FIFO_Data[0], &Fast_detection_data[FAST_CHECK_SAMPLES*(i-1)], FAST_CHECK_SAMPLES);
			SEGGER_RTT_printf(0, "fifo0 number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[0]));
			state = IDLE;
			next_state = FAST_CHECK;			//bingo to check
		}
		else		// fullfill the tank
		{
			SEGGER_RTT_printf(0, "fifo0 number: (i=%d) - %d\r\n", i, FIFO_GetDataCount(&FIFO_Data[0]));
			FIFO_ReadData(&FIFO_Data[0], &Fast_detection_data[FAST_CHECK_SAMPLES*i], FAST_CHECK_SAMPLES);
			SEGGER_RTT_printf(0, "fifo0 number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[0]));
			i++;

			if (i == FAST_CHECK_TIMES)
			{
				state = FAST_CHECK;		//bingo to check
			}
			else
			{
				state = IDLE;
				next_state = FAST_CHECK_DATA_PREPARE;
			}
		}
	}
	else
	{
		state = IDLE;
		next_state = FAST_CHECK_DATA_PREPARE;
	}
}
#pragma arm section
void fast_check_process(void)
{
	int i = 0;	//index
	int adc_sum = 0;
	int	adc_temp = 0;
	int	adc_average= 0;
	int quick_detection_result = 0;
	
	static uint32_t last_tick = 0;
	uint32_t now_tick = 0;
	uint32_t diff = 0;
	
	char float_str[64];	
	
	now_tick = SysTick_GetTick();
	diff = now_tick - last_tick;
	if ((0 != last_tick) && (0 != diff))
	{
		SEGGER_RTT_printf(0, "fast check duty: %dms\r\n", diff);
	}
	last_tick = now_tick;		

	check_status_upload(TUYA_FAST_CHECK);

	for(i=0; i<FAST_MAX_DATA_POOL; i++)
	{
		adc_temp = Fast_detection_data[i];
		//SEGGER_RTT_printf(0, "%d, ", Fast_detection_data[i]);			
		adc_sum += adc_temp;
	}

	adc_average = adc_sum/FAST_MAX_DATA_POOL;
	
	sprintf(float_str, "\r\n%s%sfast check adc_avg: %d(%.3lfV) = %d / %d%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, RTT_CTRL_TEXT_BLACK, adc_average, adc_average*3.3f/4096, adc_sum, FAST_MAX_DATA_POOL, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%s", float_str);
	
	for(i=0; i<FAST_MAX_DATA_POOL; i++)
	{
		Fast_detection_data[i] -= adc_average;
	}

	quick_detection_result = quick_detection(							Fast_detection_data, 
											/* win_size_time =  */		2048, 
											/* stride_time =  */		1024, 
											/* time_times =  */			quick_time_times,		//4
											/* time_add =  */			quick_time_add, 		//32
											/* win_size_freq =  */		256, 
									    /* stride_freq =  */		102, 
											/* time_accum =  */			8, 
											/* xhz1 =  */				2, 
											/* freq_times =  */			quick_freq_times, //3
											/* respiration_times =  */	17.5
											);

	if (quick_detection_result && (run_mode == 0))
	{
		state = SLOW_CHECK_DATA_PREPARE_S0;	//bingo to next
		slow_s0_result_last = SLOW_OTHERS;	//reset the slow check result
		memory_init();
	}
	else
	{
		for(i=0; i<FAST_MAX_DATA_POOL; i++)
		{
			Fast_detection_data[i] += adc_average;
		}
		state = IDLE;
		next_state = FAST_CHECK_DATA_PREPARE;		//loopback
	}

	fast_output_result(quick_detection_result);
}

#pragma arm section code = "RAMCODE"
void slow_check_data_prepare_s0(void)
{
	int i = 0;	//index
	FIFO_DataType  temp[2048] = {0};//temp data
	
	set_samplerate(slow_samplerate);
	
	if (SLOW_CHECK_SAMPLES < FIFO_GetDataCount(&FIFO_Data[0]))
	{
		SEGGER_RTT_printf(0, "fifo0 number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[0]));
		FIFO_ReadData(&FIFO_Data[0], temp, SLOW_CHECK_SAMPLES);
		SEGGER_RTT_printf(0, "fifo0 number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[0]));

		for(i=0;i<SLOW_CHECK_USE_SAMPLES;i++)
		{
			FIFO_WriteOneData(&FIFO_Data[1], temp[i*SLOW_CHECK_OVER_SAMPLE]);
		}
		state = SLOW_CHECK_DATA_PREPARE_S1;		//bingo to next
	}
	else
	{
		state = IDLE;
		next_state = SLOW_CHECK_DATA_PREPARE_S0;	//not enough so loopback
	}
}
#pragma arm section

#pragma arm section code = "RAMCODE"
void slow_check_data_prepare_s1(void)
{
	static	int i = 0;	//index for fullfill the tank
	int k = 0;	//index
	
	if (slow_retry_flag)
	{
		slow_retry_flag = 0;
		i = 0;
	}	
	
	if (SLOW_CHECK_SAMPLES < FIFO_GetDataCount(&FIFO_Data[1]))
	{
		if (SLOW_CHECK_TIMES == i)	//the tank is full
		{			
			for(k=0;k<SLOW_MAX_DATA_POOL - SLOW_CHECK_SAMPLES;k++)   //滑窗
			{
				Fast_detection_data[k] =Fast_detection_data[k + SLOW_CHECK_SAMPLES];		
			}			
			SEGGER_RTT_printf(0, "fifo1 number: (i=%d) - %d\r\n", i, FIFO_GetDataCount(&FIFO_Data[1]));
			FIFO_ReadData(&FIFO_Data[1], &Fast_detection_data[SLOW_CHECK_SAMPLES*(i-1)], SLOW_CHECK_SAMPLES);
			SEGGER_RTT_printf(0, "fifo1 number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[1]));
			state = SLOW_CHECK_S0;		//bingo to check
		}
		else		//the tank is not full
		{
			SEGGER_RTT_printf(0, "fifo1 number: (i=%d) - %d\r\n", i, FIFO_GetDataCount(&FIFO_Data[1]));
			FIFO_ReadData(&FIFO_Data[1], &Fast_detection_data[SLOW_CHECK_SAMPLES*i], SLOW_CHECK_SAMPLES);
			SEGGER_RTT_printf(0, "fifo1 number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[1]));
			i++;

			if (i == SLOW_CHECK_TIMES)
			{
				state = SLOW_CHECK_S0;		//bingo to check
			}
			else
			{
				state = IDLE;
				next_state = SLOW_CHECK_DATA_PREPARE_S0;	//not enough so loopback
			}
		}
	}
	else
	{
		state = IDLE;
		next_state = SLOW_CHECK_DATA_PREPARE_S0;	//not enough so loopback
	}
}
#pragma arm section

void slow_check_process_s0(void)
{
	int i = 0;	//index
	int adc_sum = 0;
	int	adc_temp = 0;
	int	adc_average= 0;
	int bigmotion_time_vote = 0;
	int bigmotion_freq_vote  = 0;
	int respirationfreq_vote[2] = {0};
	int	micromotion_detection_result = 0;
	float offset = 0;
	
	static uint32_t last_tick = 0;
	uint32_t now_tick = 0;
	uint32_t diff = 0;
	
	char float_str[64];		
	
	now_tick = SysTick_GetTick();
	diff = now_tick - last_tick;
	if ((0 != last_tick) && (0 != diff))
	{
		SEGGER_RTT_printf(0, "slow check duty: %dms\r\n", diff);
		
		if (diff > 4800)
		{
			slow_samplerate -= 50;
		}
		else
		{
			slow_samplerate += 50;
		}
		//
	}
	last_tick = now_tick;	

	check_status_upload(TUYA_SLOW_CHECK);
	
	respirationfreq_vote[0] = 0;
	respirationfreq_vote[1] = 0;	

	for(i=0; i<SLOW_MAX_DATA_POOL; i++)
	{
		adc_temp = Fast_detection_data[i];
		adc_sum += adc_temp;
	}
	
	adc_average = adc_sum/SLOW_MAX_DATA_POOL;
	
	sprintf(float_str, "\r\n%s%sslow check adc_avg: %d(%.3lfV) = %d / %d%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, RTT_CTRL_TEXT_BLACK, adc_average, adc_average*3.3f/4096, adc_sum, SLOW_MAX_DATA_POOL, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%s", float_str);	

	for(i=0; i<SLOW_MAX_DATA_POOL; i++)
	{
		Fast_detection_data[i] -= adc_average;
	}

	bigmotion_time_vote = time_detection(	/*in_data*/			Fast_detection_data, 
											/*data_size*/		4096,
											/*win_size_time*/	256,
											/*stride_time*/		128,
											/*time_times*/		slow_time_times,			//5
											/*time_add*/		slow_time_add				//40
											);
									 
	bigmotion_freq_vote  = freq_detection(	/*in_data_freq*/			Fast_detection_data,
											/*hamming_TAB2*/			hamming_TAB2,
											/*data_size*/					4096,
											/*win_size_freq*/			128,
											/*stride_freq*/				64,
											/*time_accum*/				16,
											/*xhz1*/							2,
											/*freq_times*/				slow_freq_times,		//6
											/*respiration_times*/		res_times,		//res_times
											/*respirationfreq_vote*/	respirationfreq_vote
											);
										
	if( bigmotion_freq_vote == 1 )
	{
		offset = offsetmin + 0.05f;
	}
	else
	{
		offset = offsetmin;
	}
	
  switch (mcu_get_bt_work_state())
  {
      case	BT_UN_BIND:
      case    BT_NOT_CONNECTED:
      case    BT_SATE_UNKNOW:
          break;
      case    BT_CONNECTED:
					//
					//
					//offset = 1.2f;
          break;
      default:
          break;
  }	
	
	micromotion_detection_result = Fretting_detection(
											Fast_detection_data, 
											N, 
											pro_N, 
											PAD, 
											offset, 
											rr_threshold
											);
	
	SEGGER_RTT_printf(0, "slow result: %d - %d - %d - %d\r\n", bigmotion_time_vote, bigmotion_freq_vote, respirationfreq_vote[0], micromotion_detection_result);
	
	for(i=0; i<SLOW_MAX_DATA_POOL; i++)
	{
		Fast_detection_data[i] += adc_average;
	}
	
	///////////////////////////////////////////

	if (1 == bigmotion_time_vote)
	{
		slow_s0_result = BIG_MOTION;
	}
	else if (1 == micromotion_detection_result)
	{
		slow_s0_result = BREATHE;
	}
	else if ((0 == bigmotion_freq_vote) && (1 == respirationfreq_vote[0]))
	{
		slow_s0_result = BREATHE;
	}
	else if ((1 == bigmotion_freq_vote) && (1 == respirationfreq_vote[0]))
	{
		slow_s0_result = BREATHE_NOT_SURE;
	}
	else
	{
		slow_s0_result = NO_PERSON_NOT_SURE;
	}

	slow_output_result(slow_s0_result);
	
	switch (slow_s0_result)
	{
	case BIG_MOTION:
	case BREATHE:
		state = IDLE;
		next_state = SLOW_CHECK_S1;
		slow_s0_result_last = slow_s0_result;
		break;
	case BREATHE_NOT_SURE:
	case NO_PERSON_NOT_SURE:
		state = SLOW_CHECK_S1;
		break;
	default:
		state = ERROR_ERROR;
		break;
	}
}

void slow_check_process_s1(void)
{
	static int breathe_timer = 0;
	static int no_person_timer = 0;
	static int no_person_start_tick = 0;
	int no_person_diff = 0;
	int no_person_check_tick = 0;

	if (slow_s0_result_last != slow_s0_result)
	{
		slow_s0_result_last = slow_s0_result;
		state = IDLE;
		next_state = SLOW_CHECK_DATA_PREPARE_S0;
		switch (slow_s0_result)
		{
		case BREATHE_NOT_SURE:
			breathe_timer = 1;		
			break;
		case NO_PERSON_NOT_SURE:
			no_person_timer = 1;
			no_person_start_tick = SysTick_GetTick();
			break;
		default:
			break;
		}
	}
	else
	{
		switch (slow_s0_result)
		{
		case BREATHE_NOT_SURE:
			breathe_timer++;
			if (breathe_timer >= (2 + 1))	//respirationfreq_num
			{
				breathe_timer = 0;
				//do some thing
				slow_s0_result = BREATHE;
				slow_output_result(slow_s0_result);
			}
			state = IDLE;
			next_state = SLOW_CHECK_DATA_PREPARE_S0;		
			break;
		case NO_PERSON_NOT_SURE:
			no_person_timer++;
			if (no_person_timer >= (2 + delay_time_num))		//delay_time_num
			{
				no_person_check_tick = SysTick_GetTick();
				no_person_diff = no_person_check_tick - no_person_start_tick;
				if (no_person_diff > (1000u*delay_s_num - 2*4096))
				{
					SEGGER_RTT_printf(0, "%sno person status: delay_timer=%d, diff=%d%s\r\n", RTT_CTRL_TEXT_BRIGHT_YELLOW, no_person_timer, no_person_diff, RTT_CTRL_RESET);
					no_person_timer = 0;
					state = IDLE;

					if (slow_only_flag == 0)
					{
						next_state = FAST_CHECK_DATA_PREPARE;	//no person so all loopback	to fast check
						//do some thing
						clear_buffer();
					}
					else
					{
						next_state = SLOW_CHECK_DATA_PREPARE_S0;
					}				

					slow_s0_result = NO_PERSON;
					slow_output_result(slow_s0_result);
				}
				else
				{
					state = IDLE;
					next_state = SLOW_CHECK_DATA_PREPARE_S0;	//keep slow check again
				}
			}
			else
			{
				state = IDLE;
				next_state = SLOW_CHECK_DATA_PREPARE_S0;	//keep slow check again
			}
			break;
		default:
			state = IDLE;
			next_state = SLOW_CHECK_DATA_PREPARE_S0;			
			break;
		}
	}
}

void bt_hand_up(void)
{
	bt_uart_write_frame(BT_HAND_UP, 0);
}

void idle_process(void)
{
	static uint32_t last_tick = 0;
	uint32_t now_tick = 0;
	uint32_t diff = 0;
	static uint32_t study_start = 0;
	static uint32_t study_counter = 0;
	uint32_t study_diff = 0;
	static uint32_t study_time_ms = 0;
    
	static unsigned short light_sensor_adc_data_last = 0;
	char float_str[64];	
	
	float switch_dist_f = 0;
	float switch_delay_f = 0;
	float switch_light_f = 0;
	
	static unsigned int free_runner = 0;
	
	free_runner++;
	
	now_tick = SysTick_GetTick();
	diff = now_tick - last_tick;
	if ((0 != last_tick) && (0 != diff))
	{
		//SEGGER_RTT_printf(0, "idle tick max duty: %d\r\n", diff);
	}
	last_tick = now_tick;
	
	//do some thing here global
	//study process
	if (study_flag == 1)
	{
		if (study_counter == 0)
		{
			study_start = SysTick_GetTick();
			SEGGER_RTT_printf(0, "study mode%d begin:\r\n", study_mode);
			study_time_ms = 60u*1000u + study_mode*60u*1000u;
			if (upload_disable == 0)
			{
				mcu_dp_enum_update(DPID_STUDY_PROCESS_UPLOAD, 0);	//start
			}
			study_counter++;
			//change to slow only
			run_mode = 0;//进慢
      slow_only_flag = 1;//不回快
			
			if (upload_disable == 0)
			{
				mcu_dp_enum_update(DPID_WORK_MODE, 2);
			}			

			if (next_state == FAST_CHECK_DATA_PREPARE)
			{
				state = IDLE;
				next_state = SLOW_CHECK_DATA_PREPARE_S0;
				clear_buffer();
			}
			else if(next_state == FAST_CHECK)
			{
				state = IDLE;
				next_state = SLOW_CHECK_DATA_PREPARE_S0;
				clear_buffer();
			}
			//load a low pp1 pp2 value
			res_times = 10.f;
			offsetmin = 0.5f;
			if (upload_disable == 0)
			{
				mcu_dp_value_update(DPID_FREQ_PARAMETER1, (int)((res_times*100.0f)+0.5f));
				Delay_ms(ALL_UPLOAD_DELAY);
				mcu_dp_value_update(DPID_FREQ_PARAMETER2, (int)((offsetmin*1000.0f)+0.5f));
			}
		}
		
		//make sure in the slow check process
		if (TUYA_SLOW_CHECK == check_status)
		{
			if ((max_pp1_rt != max_pp1_rt_last)  &&  (max_pp1_rt != 0))
			{
				switch (study_mode)
				{
					case 0:
						res_times = max_pp1_rt*1.1f;
						break;
					case 1:
						res_times = max_pp1_rt*1.07f;
						break;
					case 2:
						res_times = max_pp1_rt*1.03f;
						break;
					case 3:
						res_times = max_pp1_rt*1.0f;
						break;
					default:
						res_times = max_pp1_rt*1.0f;
						break;
				}
				
				sprintf(float_str, "study new pp1 values: %.3lf\r\n", res_times);
				SEGGER_RTT_printf(0, "%s", float_str);
				
				if (upload_disable == 0)
				{			
					mcu_dp_value_update(DPID_FREQ_PARAMETER1, (int)((res_times*100.0f)+0.5f));
				}
				
				max_pp1_rt_last = max_pp1_rt;
			}			
			if ((max_pp2_rt != max_pp2_rt_last)  &&  (max_pp2_rt != 0))
			{
				switch (study_mode)
				{
					case 0:
						offsetmin = max_pp2_rt*1.1f;
						break;
					case 1:
						offsetmin = max_pp2_rt*1.07f;
						break;
					case 2:
						offsetmin = max_pp2_rt*1.03f;
						break;
					case 3:
						offsetmin = max_pp2_rt*1.0f;
						break;
					default:
						offsetmin = max_pp2_rt*1.0f;
						break;
				}
				
				sprintf(float_str, "study new pp2 values: %.3lf\r\n", offsetmin);
				SEGGER_RTT_printf(0, "%s", float_str);
				
				if (upload_disable == 0)
				{
					mcu_dp_value_update(DPID_FREQ_PARAMETER2, (int)((offsetmin*1000.0f)+0.5f));
				}				
				
				max_pp2_rt_last = max_pp2_rt;
			}
			study_diff = SysTick_GetTick() - study_start;
			if (study_diff > study_time_ms)
			{
				study_flag = 0;
				SEGGER_RTT_printf(0, "study mode%d finished in %dms\r\n", study_mode, study_diff);
				if (upload_disable == 0)
				{			
					mcu_dp_enum_update(DPID_STUDY_PROCESS_UPLOAD, 1);	//end
				}
				//change to auto-check-mode
				run_mode = 0;//进慢
        slow_only_flag = 0;	//回快
				if (upload_disable == 0)
				{
					mcu_dp_enum_update(DPID_WORK_MODE, 0);
				}
				//counter to 0
				study_counter = 0;
			}
		}
	}
	//人表状态更新
	if ((person_meter != person_meter_last) && (person_meter != 0))
	{
		if (upload_disable == 0)
		{		
			mcu_dp_value_update(DPID_PERSON_METER,person_meter);
		}
		SEGGER_RTT_printf(0, "%sperson meter: %d%s\r\n", RTT_CTRL_TEXT_BRIGHT_RED, person_meter, RTT_CTRL_RESET);
		person_meter_last = person_meter;
	}	
	//找我闪动
	if (find_me_flag)
	{		
		led_green(1);
		Delay_ms(100);
		led_green(0);
		Delay_ms(100);
		find_me_counter++;
		if (3 <= find_me_counter)
		{
			find_me_flag = 0;
			find_me_counter = 0;
			if (BT_CONNECTED == mcu_get_bt_work_state())
			{
				led_green(1);
			}
			else
			{
				led_green(0);
			}
		}
	}
	
	if (free_runner%3000 == 0)
	{
		//光敏一直控制继电器
		if (light_sensor2_adc_data < Light_threshold4)//门限3
		//if (light_sensor_adc_data < Light_threshold3)//门限3
		{
			if (person_in_range_flag == 1)
			{
				GPIO1_HIGH();
			}
			else
			{
				GPIO1_LOW();
			}
		}
		else if (light_sensor2_adc_data > Light_threshold3)
		{
			GPIO1_LOW();
		}
		else
		{
			//do nothing
		}	
	}
	
	//光敏控制及上报，gpio状态上报，bt连接情况上报
	if (light_sensor_upload_flag)
	{
		light_sensor_upload_flag = 0;//clear flag
		//
		//light sensor as bt heartbeat
		if (1)
		{
			if (light_sensor_adc_data != light_sensor_adc_data_last)
			{
				mcu_dp_value_update(DPID_LIGHT_SENSOR_RAW, light_sensor_adc_data);
				light_sensor_adc_data_last = light_sensor_adc_data;			
			}
			
			if (1)
			{
				sprintf(float_str, "%slight sensor: %d(%.3lfV)%s\r\n", RTT_CTRL_TEXT_BRIGHT_MAGENTA, light_sensor_adc_data, light_sensor_adc_data*3.3f/4096, RTT_CTRL_RESET);
				SEGGER_RTT_printf(0, "%s", float_str);				
			}
			
			if (1)
			{
				sprintf(float_str, "%slight sensor2: %d(%.3lfV)%s\r\n", RTT_CTRL_TEXT_BRIGHT_MAGENTA, light_sensor2_adc_data, light_sensor2_adc_data*3.3f/4096, RTT_CTRL_RESET);
				SEGGER_RTT_printf(0, "%s", float_str);
			}
			
			if (1)
			{
				switch_dist_f = switch_dist*3.3f/4096;
				sprintf(float_str, "%sswitch distance: %d(%.3lfV)%s\r\n", RTT_CTRL_TEXT_BRIGHT_MAGENTA, switch_dist, switch_dist_f, RTT_CTRL_RESET);
				SEGGER_RTT_printf(0, "%s", float_str);

				if (switch_dist_f > 1.5f)
				{
					//1: on 2: on
					//2米
					quick_time_times = 2048;			//4
					quick_time_add = 135;			//32
					quick_freq_times = 12;			//3
					//////////////////////////////////////////////////////
					slow_time_times = 2048;			//5
					slow_time_add = 135;				//40
					slow_freq_times = 6;			//6
					res_times = 78.0f;				//17.5
					offsetmin = 1.23f;					//0.6					
				}
				else if (switch_dist_f > 1.1f)
				{
					//1: off 2: on
					//3米
					quick_time_times = 400;			//4
					quick_time_add = 120;			//32
					quick_freq_times = 10;			//3
					//////////////////////////////////////////////////////
					slow_time_times = 400;			//5
					slow_time_add = 120;				//40
					slow_freq_times = 10;			//6
					res_times = 65.0f;				//17.5
					offsetmin = 1.1f;					//0.6							
				}
				else if (switch_dist_f > 0.5f)
				{
					//1: on 2: off
					//4米
					quick_time_times = 30;			//4
					quick_time_add = 80;			//32
					quick_freq_times = 6;			//3
					//////////////////////////////////////////////////////
					slow_time_times = 30;			//5
					slow_time_add = 80;				//40
					slow_freq_times = 6;			//6
					res_times = 50.0f;				//17.5
					offsetmin = 0.8f;					//0.6		
				}
				else
				{
					//1: off 2: off
					//最远
					quick_time_times = 4;			//4
					quick_time_add = 32;			//32
					quick_freq_times = 3;			//3
					//////////////////////////////////////////////////////
					slow_time_times = 5;			//5
					slow_time_add = 40;				//40
					slow_freq_times = 6;			//6
					res_times = 17.5f;				//17.5
					offsetmin = 0.6f;					//0.6
				}
			}
			
			if (1)
			{
				switch_delay_f = switch_delay*3.3f/4096;
					
				if (switch_delay_f > 1.7f)
				{
					//3: on 4: on 5: on 
					delay_s_num = 240;
				}
				else if (switch_delay_f > 1.6f)
				{
					//3: off 4: on 5: on 
					delay_s_num = 180;
				}
				else if (switch_delay_f > 1.4f)
				{
					//3: on 4: off 5: on 
					delay_s_num = 120;
				}
				else if (switch_delay_f > 1.2f)
				{
					//3: off 4: off 5: on
					delay_s_num = 60;
				}
				else if (switch_delay_f > 1.0f)
				{
					//3: on 4: on 5: off
					delay_s_num = 50;
				}
				else if (switch_delay_f > 0.7f)
				{
					//3: off 4: on 5: off
					delay_s_num = 40;
				}	
				else if (switch_delay_f > 0.4f)
				{
					//3: on 4: off 5: off
					delay_s_num = 30;
				}
				else
				{
					//3: off 4: off 5: off
					delay_s_num = 24;
				}
				sprintf(float_str, "%sswitch delay: %d(%.3lfV)-%ds%s\r\n", RTT_CTRL_TEXT_BRIGHT_MAGENTA, switch_delay, switch_delay_f, delay_s_num, RTT_CTRL_RESET);
				SEGGER_RTT_printf(0, "%s", float_str);				
			}

			if (1)
			{
				switch_light_f = switch_light*3.3f/4096;
				
				if (switch_light_f > 1.7f)
				{
					//6: on 7: on 8: on 
					Light_threshold3 = 500;
					Light_threshold4 = Light_threshold3 - 200;
				}
				else if (switch_light_f > 1.6f)
				{
					//6: off 7: on 8: on 
					Light_threshold3 = 1000;
					Light_threshold4 = Light_threshold3 - 200;
				}
				else if (switch_light_f > 1.4f)
				{
					//6: on 7: off 8: on 
					Light_threshold3 = 1500;
					Light_threshold4 = Light_threshold3 - 200;
				}
				else if (switch_light_f > 1.2f)
				{
					//6: off 7: off 8: on 
					Light_threshold3 = 2000;
					Light_threshold4 = Light_threshold3 - 200;
				}
				else if (switch_light_f > 1.0f)
				{
					//6: on 7: on 8: off
					Light_threshold3 = 2500;
					Light_threshold4 = Light_threshold3 - 200;
				}
				else if (switch_light_f > 0.7f)
				{
					//6: off 7: on 8: off
					Light_threshold3 = 3000;
					Light_threshold4 = Light_threshold3 - 200;
				}	
				else if (switch_light_f > 0.4f)
				{
					//6: on 7: off 8: off
					Light_threshold3 = 3500;
					Light_threshold4 = Light_threshold3 - 200;
				}
				else
				{
					//6: off 7: off 8: off 
					Light_threshold3 = 4000;
					Light_threshold4 = Light_threshold3 - 200;
				}
				sprintf(float_str, "%sswitch light: %d(%.3lfV)-(u%d)(d%d)%s\r\n", RTT_CTRL_TEXT_BRIGHT_MAGENTA, switch_light, switch_light_f, Light_threshold3, Light_threshold4, RTT_CTRL_RESET);
				SEGGER_RTT_printf(0, "%s", float_str);				
			}			
		}
			
		if (light_sensor_adc_data > Light_threshold1)//门限1
		{
//			GPIO2_HIGH();
		}
		else
		{
//			GPIO2_LOW();
		}

		if (light_sensor_adc_data < Light_threshold2)//门限2
		{
//			GPIO3_HIGH();
		}
		else
		{
//			GPIO3_LOW();
		}		
		
		if (light_sensor_adc_data > Light_threshold4)//门限4
		{
//			GPIO5_HIGH();
		}
		else
		{
//			GPIO5_LOW();
		}
		
		SEGGER_RTT_printf(0, 
										"\r\n%sgpio0~gpio6 read bit:%d-%d-%d-%d-%d-%d-%d%s\r\n", 
										RTT_CTRL_TEXT_BRIGHT_MAGENTA,
										PORT_GetBit(PortA, Pin07),
										PORT_GetBit(PortA, Pin08),
										PORT_GetBit(PortB, Pin06),
										PORT_GetBit(PortB, Pin05),
										PORT_GetBit(PortA, Pin00),
										PORT_GetBit(PortA, Pin04),
										PORT_GetBit(PortB, Pin00),
										RTT_CTRL_RESET
										);
                                        
        switch (mcu_get_bt_work_state())
        {
            case	BT_UN_BIND:
                SEGGER_RTT_printf(0,"bt unbind\r\n");
                break;
            case    BT_NOT_CONNECTED:
                SEGGER_RTT_printf(0,"bt binded but not connect\r\n");
								get_mcu_bt_mode();
                bt_hand_up();
                break;
            case    BT_CONNECTED:
                SEGGER_RTT_printf(0,"bt binded and connected\r\n");
                break;
            case    BT_SATE_UNKNOW:
                SEGGER_RTT_printf(0,"bt unknow status\r\n");
                //get_mcu_bt_mode();
                break;
            default:
                break;
        }
				SEGGER_RTT_printf(0,"\r\n");
	}
	//串口错误处理
	UsartRxErrProcess();
	tuya_UsartRxErrProcess();
	//
	state = UART_PROTOCOL;
}

void error_process(void)
{
	//do some print
	SEGGER_RTT_WriteString(0, "error!!!\r\n");
}

void uart_post_process()
{
	//
	state = next_state;
}

void person_in_range_upload(unsigned int aaaa)
{
	person_in_range_flag = aaaa;
	static int person_in_range_flag_last = 0;	
	
	if (upload_disable == 0)
	{
		if (person_in_range_flag != person_in_range_flag_last)
		{
			mcu_dp_enum_update(DPID_PERSON_IN_RANGE, aaaa);
			
			person_in_range_flag_last = person_in_range_flag;
			if (aaaa == 1)
			{
				SEGGER_RTT_printf(0, "%s有人!%s\r\n", RTT_CTRL_TEXT_BRIGHT_RED, RTT_CTRL_RESET);
				person_meter++;
			}
			else
			{
				SEGGER_RTT_printf(0, "%s无人!%s\r\n", RTT_CTRL_TEXT_BRIGHT_GREEN, RTT_CTRL_RESET);
			}
		}
	}
	else
	{
		if (aaaa == 1)
		{
			SEGGER_RTT_printf(0, "%s有人!%s\r\n", RTT_CTRL_TEXT_BRIGHT_RED, RTT_CTRL_RESET);
			person_meter++;
		}
		else
		{
			SEGGER_RTT_printf(0, "%s无人!%s\r\n", RTT_CTRL_TEXT_BRIGHT_GREEN, RTT_CTRL_RESET);
		}
	}
}

void light_status_upload(unsigned int aaaa)
{
	light_status_flag = aaaa;
	static int light_status_flag_last = 0;
	
	if (upload_disable == 0)
	{
		if (light_status_flag != light_status_flag_last)
		{
			mcu_dp_enum_update(DPID_LIGHT_STATUS, aaaa);
			light_status_flag_last = light_status_flag;
		}	
	}
}

void check_status_upload(unsigned int aaaa)
{
	check_status = aaaa;
	static unsigned int check_status_last = TUYA_OTHER;
	
	if (upload_disable == 0)
	{
		if (check_status != check_status_last)
		{
			mcu_dp_enum_update(DPID_CHECK_PROCESS, aaaa);
			check_status_last = check_status;
		}	
	}
}

void slow_check_result_upload(unsigned int aaaa)
{
	static unsigned int slow_check_result_last = 1;//no person
	
	slow_check_result = aaaa;
	
	if (upload_disable == 0)
	{
		if (slow_check_result != slow_check_result_last)
		{
			mcu_dp_enum_update(DPID_SLOW_CHECK_RESULT, aaaa);
			slow_check_result_last = slow_check_result;
		}	
	}
}

void app(void)
{
	static uint32_t finish_tick = 0;
	static uint32_t start_tick = 0;
	uint32_t diff = 0;
	
	switch (state)
	{
		case	FAST_CHECK_DATA_PREPARE:
			fast_check_data_prepare();
			break;
		case	FAST_CHECK:
			start_tick = SysTick_GetTick();
			fast_check_process();
			finish_tick = SysTick_GetTick();
			diff = finish_tick - start_tick;
			SEGGER_RTT_printf(0, "fast check last: %dms\r\n", diff);
			break;
		case	SLOW_CHECK_DATA_PREPARE_S0:
			slow_check_data_prepare_s0();
			break;
		case	SLOW_CHECK_DATA_PREPARE_S1:
			slow_check_data_prepare_s1();
			break;			
		case	SLOW_CHECK_S0:
			start_tick = SysTick_GetTick();
			slow_check_process_s0();
			finish_tick = SysTick_GetTick();
			diff = finish_tick - start_tick;
			SEGGER_RTT_printf(0, "slow check last: %dms\r\n", diff);		
			break;
		case	SLOW_CHECK_S1:
			slow_check_process_s1();
			break;
		case	UART_PROTOCOL:
			bt_uart_service();
			uart_post_process();
			break;
		case	IDLE:
			idle_process();
			break;
		case	ERROR_ERROR:
			error_process();
			break;
		default:
			error_process();
			break;
	}
}

void gpio_init(void)
{
	stc_port_init_t stcPortInit;
	/* configuration structure initialization */
	MEM_ZERO_STRUCT(stcPortInit);

	stcPortInit.enPinMode = Pin_Mode_Out;
	stcPortInit.enExInt = Disable;
	stcPortInit.enPullUp = Disable;
	/* LED0 Port/Pin initialization */

	//PORT_Init(PortA, Pin07, &stcPortInit);   //P1-4	//gpio0
	PORT_Init(PortA, Pin08, &stcPortInit);   //P1-3	//gpio1
	//PORT_Init(PortB, Pin06, &stcPortInit);   //P5-1 //gpio2
	//PORT_Init(PortB, Pin05, &stcPortInit);   //P5-2	//gpio3
	//PORT_Init(PortA, Pin00, &stcPortInit);   //P5-3 //gpio4
	//PORT_Init(PortA, Pin04, &stcPortInit);   //P5-4	//gpio5		
	//PORT_Init(PortB, Pin00, &stcPortInit);   //P5-5 //gpio6
		
//	GPIO0_LOW();
	GPIO1_LOW();
//	GPIO2_LOW();
//	GPIO3_LOW();
//	GPIO4_LOW();
//	GPIO5_LOW();
//	GPIO6_LOW();
}

void segger_init(void)
{
	//
	SEGGER_RTT_ConfigUpBuffer(1, "JScope_U2U2U2U2U2U2", &JS_RTT_UpBuffer[0], sizeof(JS_RTT_UpBuffer), SEGGER_RTT_MODE_NO_BLOCK_SKIP);
	
	SEGGER_RTT_Init();
	SEGGER_RTT_printf(0, "%sphosense radar chip: XBR816C DEMO%s\r\n", RTT_CTRL_BG_BRIGHT_RED, RTT_CTRL_RESET);
	
	SEGGER_SYSVIEW_Conf();
}

void read_uid(void)
{
		char	i = 0;
    char data[12] = {0};
		
		SEGGER_RTT_printf(0, "%smcu chip uid: \r\n", RTT_CTRL_TEXT_BRIGHT_GREEN);
		for(i = 0; i < 12; i++) 
		{
				data[i] = *((unsigned char *)(FEM_UQID1 + i));
				SEGGER_RTT_printf(0, "%02X ", data[i]);
		}
		SEGGER_RTT_printf(0, "%s\r\n", RTT_CTRL_RESET);
}

void SysTick_IrqHandler(void)
{
		SEGGER_SYSVIEW_RecordEnterISR();	
    SysTick_IncTick();
		SEGGER_SYSVIEW_RecordExitISR();	
}

void tick_init(void)
{
	SysTick_Init(1000u);//1ms
}

void set_samplerate(unsigned int speed)
{
	*((unsigned int *)(TMR02_CMPBR)) = speed;
}

void memory_init(void)
{
	stop_sample(1);
	
	while (1 != FIFO_IsDataEmpty(&FIFO_Data[0]))
	{
		SEGGER_RTT_printf(0, "fifo0 number useless: %d\r\n", FIFO_GetDataCount(&FIFO_Data[0]));	
		FIFO_ReadData(&FIFO_Data[0], &Fast_detection_data[0], 2000);
		SEGGER_RTT_printf(0, "fifo0 number useless: %d\r\n", FIFO_GetDataCount(&FIFO_Data[0]));
	}
		
	while (1 != FIFO_IsDataEmpty(&FIFO_Data[1]))
	{
		SEGGER_RTT_printf(0, "fifo1 number useless: %d\r\n", FIFO_GetDataCount(&FIFO_Data[1]));	
		FIFO_ReadData(&FIFO_Data[1], &Fast_detection_data[0], 2000);
		SEGGER_RTT_printf(0, "fifo1 number useless: %d\r\n", FIFO_GetDataCount(&FIFO_Data[1]));
	}	
	
	FIFO_Init(&FIFO_Data[0]);
	FIFO_Init(&FIFO_Data[1]);	
	memset(&Fast_detection_data[0], 0, MAX_DATA_POOL * 2);
	memset(&FIFO_DataBuffer[0], 0, FIFO_DATA_NUM * FIFO_DATA_SIZE * 2);
	stop_sample(0);	
}

void stop_sample(unsigned char flag)
{
	if (flag)
	{
		TIMER0_Cmd(TMR_UNIT,Tim0_ChannelB,Disable);
	}
	else
	{
		TIMER0_Cmd(TMR_UNIT,Tim0_ChannelB,Enable);
	}
}

int main(void)
{

	memory_init();
	SysClkIni();
	usart_init();//both debug and tuya
	led_init();	
	AdcConfig();
	timer0_init();
	ADC_StartConvert(M4_ADC1);
	ADC_StartConvert(M4_ADC2);
	bt_protocol_init();
	gpio_init();
  segger_init();		
	get_mcu_bt_mode();
	read_uid();
	tick_init();
	enable_flash_cache(Enable);
	
	SysTick_GetTick();
	GPIO1_HIGH();
	Delay_ms(ALL_UPLOAD_DELAY);
	GPIO1_LOW();
	SysTick_GetTick();
	
	while(1)
	{
		app();
	}
}
