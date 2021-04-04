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
#define		PAD						0.00000001  //虚警概率
  
#define		rr_threshold			0.6    //呼吸频率截取范围
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

FIFO_DataType Fast_detection_data[MAX_DATA_POOL] = {0};//big raw data pool

int state = FAST_CHECK_DATA_PREPARE;	//状态机变量
int next_state = FAST_CHECK_DATA_PREPARE;	//状态机变量的下一个状态

int slow_s0_result = NO_PERSON_NOT_SURE;
int slow_s0_result_last = NO_PERSON_NOT_SURE;

char fast_retry_flag = 0;
char slow_retry_flag = 0;

int run_mode = 0;
int slow_only_flag = 0;

int check_status = TUYA_OTHER;
int check_status_last = TUYA_OTHER;

int person_in_range_flag = TUYA_PERSON_STATUS_OTHER;
int person_in_range_flag_last = TUYA_PERSON_STATUS_OTHER;

int led_onboard_status = TUYA_LED_ONBOARD_OTHER;
int led_onboard_status_last = TUYA_LED_ONBOARD_OTHER;

int light_status_flag = 0;
int light_status_flag_last = 0;

////////////////////////////////////////////////////////////
float quick_time_times = 4;
float quick_time_add = 32;
float quick_freq_times = 3;
////////////////////////////////////////////////////////////
unsigned char abDataIn[100];
unsigned char abDataOut[100];
////////////////////////////////////////////////////////////
float	slow_time_times = 5;
float slow_time_add = 40;
float	slow_freq_times = 6;
float	res_times = 17.5;		//17.5
float	offsetmin = 0.6;			//		0.6
////////////////////////////////////////////////////////////
void check_status_upload(unsigned char aaaa);
void person_in_range_upload(unsigned char aaaa);
void led_onboard_status_upload(unsigned char aaaa);
void clear_buffer(void);
void light_status_upload(unsigned char aaaa);
////////////////////////////////////////////////////////////
unsigned char upload_disable = 1;
unsigned char g_work_mode = ALL_CHECK;
////////////////////////////////////////////////////////////
unsigned char find_me_flag = 0;
unsigned char find_me_counter = 0;
////////////////////////////////////////////////////////////
unsigned char light_sensor_upload_flag = 0;
extern unsigned short  light_sensor_adc_data;
////////////////////////////////////////////////////////////
unsigned short Light_threshold1 = 0;
unsigned short Light_threshold2 = 0;
unsigned short Light_threshold3 = 0;
unsigned short Light_threshold4 = 0;
////////////////////////////////////////////////////////////
unsigned int delay_time_num = 0;
////////////////////////////////////////////////////////////
unsigned int  person_meter = 0;
unsigned int  person_meter_last = 0;
////////////////////////////////////////////////////////////
void get_mcu_bt_mode(void);
void bt_hand_up(void);
void clear_buffer(void);
void gpio_output(unsigned char res);

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
		led_green(0);
		
		GPIO0_HIGH();
		GPIO1_LOW();
		
		if (light_sensor_adc_data < Light_threshold3)//门限3
		{
			GPIO4_HIGH();
		}
		else
		{
			GPIO4_LOW();
		}
		light_status_upload(0);
	}
	else
	{
		led_red(0);			
		led_green(0);
		GPIO0_LOW();
		GPIO1_HIGH();
		GPIO4_LOW();
		light_status_upload(1);
	}
}

void fast_output_result(char quick_detection_result)
{
	//printf("quick: %d \r\n", quick_detection_result);
	if (quick_detection_result)//有人
	{
		led_onboard_status_upload(TUYA_LED_ONBOARD_RED_ON_GREEN_OFF);
		person_in_range_upload(TUYA_PERSON_STATUS_HAVE_PERSON);
		gpio_output(1);		
	}
	else						//无人
	{
		led_onboard_status_upload(TUYA_LED_ONBOARD_RED_OFF_GREEN_OFF);
		person_in_range_upload(TUYA_PERSON_STATUS_NO_PERSON);
		gpio_output(0);
	}
}

void slow_output_result(char slow_s0_result)
{
	switch (slow_s0_result)
	{
	case BIG_MOTION:	
		if (slow_only_flag == 1)
		{
			fast_output_result(1);
		}
		else
		{
			led_red(1);			
			led_green(0);
			led_onboard_status_upload(TUYA_LED_ONBOARD_RED_ON_GREEN_OFF);
		}
		break;
	case BREATHE:
		if (slow_only_flag == 1)
		{
			fast_output_result(1);
		}
		else
		{
			led_red(0);			
			led_green(1);
			led_onboard_status_upload(TUYA_LED_ONBOARD_RED_OFF_GREEN_ON);
		}
		break;
	case BREATHE_NOT_SURE:
		if (slow_only_flag == 1)
		{
			fast_output_result(1);
		}
		else
		{
			led_red(0);			
			led_green(1);
			led_onboard_status_upload(TUYA_LED_ONBOARD_RED_OFF_GREEN_ON);
		}
		break;
	case NO_PERSON_NOT_SURE:
		if (slow_only_flag == 1)
		{
			fast_output_result(0);
		}
		else
		{
			led_red(1);			
			led_green(1);
			led_onboard_status_upload(TUYA_LED_ONBOARD_RED_ON_GREEN_ON);
		}
		break;
	case NO_PERSON:
		if (slow_only_flag == 1)
		{
			fast_output_result(0);
		}
		else
		{
			//printf("slow check no person go fast check\r\n");
		}
		break;
	default:
		break;
	}	
}

void fast_check_data_prepare(void)
{
	static	int i = 0;	//index, fullfill the tank first
	int k = 0;	//index
	
	if (fast_retry_flag)
	{
		fast_retry_flag = 0;
		i = 0;
	}
	
	if (FAST_CHECK_TIMES > i)
	{
		//led_red(1);
		//led_green(0);
		//led_onboard_status_upload(TUYA_LED_ONBOARD_RED_ON_GREEN_OFF);
		//person_in_range_upload(TUYA_PERSON_STATUS_HAVE_PERSON);

		//gpio output 
		//
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
			//printf("fifo0 number: %d - %d\r\n", i, FIFO_GetDataCount(&FIFO_Data[0]));
			FIFO_ReadData(&FIFO_Data[0], &Fast_detection_data[FAST_CHECK_SAMPLES*(i-1)], FAST_CHECK_SAMPLES);
			//printf("fifo0 number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[0]));
			state = IDLE;
			next_state = FAST_CHECK;			//bingo to check
		}
		else		// fullfill the tank
		{
			//printf("fifo0 number: %d - %d\r\n", i, FIFO_GetDataCount(&FIFO_Data[0]));
			FIFO_ReadData(&FIFO_Data[0], &Fast_detection_data[FAST_CHECK_SAMPLES*i], FAST_CHECK_SAMPLES);
			//printf("fifo0 number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[0]));
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
	
	sprintf(float_str, "\r\nfast check adc_avg: %d(%d) = %d / %d\r\n", adc_average, adc_sum/FAST_MAX_DATA_POOL, adc_sum, FAST_MAX_DATA_POOL);
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

void slow_check_data_prepare_s0(void)
{
	int i = 0;	//index
	FIFO_DataType  temp[2048] = {0};//temp data
	
	if (SLOW_CHECK_SAMPLES < FIFO_GetDataCount(&FIFO_Data[0]))
	{
		//printf("fifo0 number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[0]));
		FIFO_ReadData(&FIFO_Data[0], temp, SLOW_CHECK_SAMPLES);
		//printf("fifo0 number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[0]));

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
			//printf("fifo1 number: %d - %d\r\n", i, FIFO_GetDataCount(&FIFO_Data[1]));
			FIFO_ReadData(&FIFO_Data[1], &Fast_detection_data[SLOW_CHECK_SAMPLES*(i-1)], SLOW_CHECK_SAMPLES);
			//printf("fifo1 number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[1]));
			state = SLOW_CHECK_S0;		//bingo to check
		}
		else		//the tank is not full
		{
			//printf("fifo1 number: %d - %d\r\n", i, FIFO_GetDataCount(&FIFO_Data[1]));
			FIFO_ReadData(&FIFO_Data[1], &Fast_detection_data[SLOW_CHECK_SAMPLES*i], SLOW_CHECK_SAMPLES);
			//printf("fifo1 number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[1]));
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
	
	sprintf(float_str, "\r\nslow check adc_avg: %d(%d) = %d / %d\r\n", adc_average, adc_sum/SLOW_MAX_DATA_POOL, adc_sum, SLOW_MAX_DATA_POOL);
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
		next_state = SLOW_CHECK_DATA_PREPARE_S0;
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
			break;
		default:
			state = ERROR_ERROR;
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
			break;
		default:
			state = ERROR_ERROR;
			break;
		}
	}
}

void bt_hand_up(void)
{
	//
	bt_uart_write_frame(BT_HAND_UP, 0);
}

void idle_process(void)
{
	static uint32_t last_tick = 0;
	uint32_t now_tick = 0;
	uint32_t diff = 0;
	
	now_tick = SysTick_GetTick();
	diff = now_tick - last_tick;
	if ((0 != last_tick) && (0 != diff))
	{
		//SEGGER_RTT_printf(0, "idle tick max duty: %d\r\n", diff);
	}
	last_tick = now_tick;
	
	//do some thing here global
	//人表状态更新
	if (person_meter != person_meter_last)
	{
		mcu_dp_value_update(DPID_PERSON_METER,person_meter);
		SEGGER_RTT_printf(0, "person meter: %d\r\n", person_meter);
		person_meter_last = person_meter;
	}	
	//
	if (find_me_flag)
	{
		led_red(1);			
		led_green(1);
		Delay_ms(100);
		led_red(0);
		led_green(0);
		Delay_ms(100);
		find_me_counter++;
		if (3 <= find_me_counter)
		{
			find_me_flag = 0;
			find_me_counter = 0;
		}
	}
	//
	if (light_sensor_upload_flag)
	{
		light_sensor_upload_flag = 0;//clear flag
		//
		mcu_dp_value_update(DPID_LIGHT_SENSOR_RAW, light_sensor_adc_data);

		if (light_sensor_adc_data > Light_threshold1)//门限1
		{
			GPIO2_HIGH();
		}
		else
		{
			GPIO2_LOW();
		}

		if (light_sensor_adc_data < Light_threshold2)//门限2
		{
			GPIO3_HIGH();
		}
		else
		{
			GPIO3_LOW();
		}		
		
		if (light_sensor_adc_data > Light_threshold4)//门限4
		{
			GPIO5_HIGH();
		}
		else
		{
			GPIO5_LOW();
		}
		
		SEGGER_RTT_printf(0, 
										"\r\ngpio0~gpio6 read bit:%d-%d-%d-%d-%d-%d-%d\r\n", 
										PORT_GetBit(PortA, Pin07),
										PORT_GetBit(PortA, Pin08),
										PORT_GetBit(PortB, Pin06),
										PORT_GetBit(PortB, Pin05),
										PORT_GetBit(PortA, Pin00),
										PORT_GetBit(PortA, Pin04),
										PORT_GetBit(PortB, Pin00)
										);
                                        
        switch (mcu_get_bt_work_state())
        {
            case	BT_UN_BIND:
                SEGGER_RTT_printf(0,"bt unbind\r\n");
                break;
            case    BT_NOT_CONNECTED:
                SEGGER_RTT_printf(0,"bt binded but not connect\r\n");
                bt_hand_up();
                break;
            case    BT_CONNECTED:
                SEGGER_RTT_printf(0,"bt binded and connected\r\n");
                break;
            case    BT_SATE_UNKNOW:
                SEGGER_RTT_printf(0,"bt unknow status\r\n");
								get_mcu_bt_mode();
                break;
            default:
                break;
        }
	}
	//
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

void person_in_range_upload(unsigned char aaaa)
{
	person_in_range_flag = aaaa;
	
	if (upload_disable == 0)
	{
		if (person_in_range_flag != person_in_range_flag_last)
		{
			mcu_dp_enum_update(DPID_PERSON_IN_RANGE, aaaa);
			person_in_range_flag_last = person_in_range_flag;
			if (aaaa == 1)
			{
				SEGGER_RTT_printf(0, "%s有人!\r\n", RTT_CTRL_TEXT_BRIGHT_RED);
			}
			else
			{
				SEGGER_RTT_printf(0, "%s无人!\r\n", RTT_CTRL_TEXT_BRIGHT_GREEN);
			}
			
			if (person_in_range_flag == TUYA_PERSON_STATUS_HAVE_PERSON)
			{
				person_meter++;
			}			
		}
	}
}

void light_status_upload(unsigned char aaaa)
{
	light_status_flag = aaaa;
	
	if (upload_disable == 0)
	{
		if (light_status_flag != light_status_flag_last)
		{
			mcu_dp_enum_update(DPID_LIGHT_STATUS, aaaa);
			light_status_flag_last = light_status_flag;
		}	
	}
}

void check_status_upload(unsigned char aaaa)
{
	check_status = aaaa;
	
	if (upload_disable == 0)
	{
		if (check_status != check_status_last)
		{
			mcu_dp_enum_update(DPID_CHECK_PROCESS, aaaa);
			check_status_last = check_status;
		}	
	}
}

void led_onboard_status_upload(unsigned char aaaa)
{
	led_onboard_status = aaaa;
	
	if (upload_disable == 0)
	{
		if (led_onboard_status != led_onboard_status_last)
		{
			mcu_dp_enum_update(DPID_LED_ON_BOARD_STATUS, aaaa);
			led_onboard_status_last = led_onboard_status;
		}	
	}
}

void app(void)
{
	uint32_t finish_tick = 0;
	uint32_t start_tick = 0;
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

	PORT_Init(PortA, Pin07, &stcPortInit);   //P1-4	//gpio0
	PORT_Init(PortA, Pin08, &stcPortInit);   //P1-3	//gpio1
	PORT_Init(PortB, Pin06, &stcPortInit);   //P5-1 //gpio2
	PORT_Init(PortB, Pin05, &stcPortInit);   //P5-2	//gpio3
	PORT_Init(PortA, Pin00, &stcPortInit);   //P5-3 //gpio4
	PORT_Init(PortA, Pin04, &stcPortInit);   //P5-4	//gpio5		
	PORT_Init(PortB, Pin00, &stcPortInit);   //P5-5 //gpio6
		
	GPIO0_LOW();
	GPIO1_LOW();
	GPIO2_LOW();
	GPIO3_LOW();
	GPIO4_LOW();
	GPIO5_LOW();
	GPIO6_LOW();
}

void segger_init(void)
{
	SEGGER_RTT_ConfigDownBuffer(0, "DataIn", &abDataIn[0], sizeof(abDataIn), SEGGER_RTT_MODE_NO_BLOCK_SKIP);
	SEGGER_RTT_ConfigUpBuffer(0, "DataOut", &abDataOut[0], sizeof(abDataOut), SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
	SEGGER_RTT_Init();
	SEGGER_RTT_WriteString(0, "phosense radar chip: XBR816C DEMO\r\n");	
}

void read_uid(void)
{
		char	i = 0;
    char data[10] = {0};
		
		SEGGER_RTT_WriteString(0, "mcu chip uid: \r\n");
		for(i = 0; i < 10; i++) 
		{
				data[i] = *((unsigned char *)(MCU_UID_ADDR_S + i));
				SEGGER_RTT_printf(0, "%02X ", data[i]);
		}
		SEGGER_RTT_WriteString(0, "\r\n");
}

void SysTick_IrqHandler(void)
{
    SysTick_IncTick();
}

void tick_init(void)
{
	SysTick_Init(1000u);//1ms
}

int main(void)
{
	FIFO_Init(&FIFO_Data[0]);
	FIFO_Init(&FIFO_Data[1]);
	SysClkIni();
	usart_init();//both debug and tuya
	led_init();	
	AdcConfig();
	timer0_init();
	ADC_StartConvert(M4_ADC1);
	bt_protocol_init();
	gpio_init();
  segger_init();		
	get_mcu_bt_mode();
	bt_hand_up();
	read_uid();
	tick_init();
	
	while(1)
	{
		app();
	}
}
