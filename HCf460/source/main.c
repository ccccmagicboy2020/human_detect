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

#define		FEEDRATE				2048			//采样率，单位为samples/s

#define		FAST_CHECK_SAMPLES		512				//快检测样本数
#define		FAST_CHECK_DUTY			8				//快检测周期，单位为秒s
#define		FAST_MAX_DATA_POOL		FEEDRATE	*	FAST_CHECK_DUTY
#define		FAST_CHECK_TIMES		FAST_MAX_DATA_POOL	>> 9

#define		SLOW_CHECK_SAMPLES		2048			//慢检测样本数
#define		SLOW_CHECK_OVER_SAMPLE	3				//慢检测超采样冥
#define		SLOW_CHECK_DUTY			16				//慢检测周期，单位为秒s
#define		SLOW_CHECK_USE_SAMPLES	SLOW_CHECK_SAMPLES >> SLOW_CHECK_OVER_SAMPLE	//慢检测实际用的样本数
#define		SLOW_MAX_DATA_POOL		(FEEDRATE >> SLOW_CHECK_OVER_SAMPLE)	*	SLOW_CHECK_DUTY
#define		SLOW_CHECK_TIMES		SLOW_MAX_DATA_POOL	>>	11

#define		MAX_DATA_POOL			FAST_MAX_DATA_POOL

extern const float hamming_TAB2[4096];

float		slow_time_times = 30;
int 		slow_time_add = 600;
float		slow_freq_times = 1800;
float		res_times = 50000;		//17.5
float   offsetmax = 3;     		//门限偏置  0.65
float   offsetmin = 3;				//		0.6

FIFO_DataType Fast_detection_data[MAX_DATA_POOL] = {0};//big raw data pool

enum work_mode
{
	ALL_CHECK=0,			//default
	FAST_CHECK_ONLY,
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

enum slow_s0_result
{
	NO_PERSON_NOT_SURE=0,			//no person but not sure
	NO_PERSON,
	BIG_MOTION,
	BREATHE,
	BREATHE_NOT_SURE,				//breathe but not sure
};

int state = FAST_CHECK_DATA_PREPARE;	//状态机变量
int next_state = FAST_CHECK_DATA_PREPARE;	//状态机变量的下一个状态

int slow_s0_result = NO_PERSON_NOT_SURE;
int slow_s0_result_last = NO_PERSON_NOT_SURE;

char fast_retry_flag = 0;
char slow_retry_flag = 0;

int run_mode = ALL_CHECK;
//int run_mode = FAST_CHECK_ONLY;

void clear_buffer(void)
{
	fast_retry_flag = 1;
	slow_retry_flag = 1;
}

void fast_output_result(char quick_detection_result)
{
	printf("quick: %d \r\n", quick_detection_result);
	if (quick_detection_result)//有人
	{
		led_red(1);			
		led_green(0);
	}
	else						//无人
	{
		led_red(0);			
		led_green(0);		
	}
}

void slow_output_result(char slow_s0_result)
{
	switch (slow_s0_result)
	{
	case BIG_MOTION:
		printf("big: 1 \r\n");
		led_red(1);			
		led_green(0);		
		break;
	case BREATHE:
		printf("micro: 1 \r\n");
		led_red(0);			
		led_green(1);
		break;
	case BREATHE_NOT_SURE:
		printf("micro: 0.3 \r\n");
		led_red(0);			
		led_green(1);		
		break;
	case NO_PERSON_NOT_SURE:
		printf("big: 0.5 \r\n");
		led_red(0);			
		led_green(1);		
		break;
	case NO_PERSON:
		printf("slow check no person go fast check\r\n");
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
		led_red(1);
		led_green(0);	
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
			state = FAST_CHECK;			//bingo to check
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

	for(i=0; i<FAST_MAX_DATA_POOL; i++)
	{
		adc_temp = Fast_detection_data[i];
		adc_sum += adc_temp;
	}
	adc_average = (int)(adc_sum/FAST_MAX_DATA_POOL + 0.5);

	for(i=0; i<FAST_MAX_DATA_POOL; i++)
	{
		Fast_detection_data[i] -= adc_average;
	}

	quick_detection_result = quick_detection(							Fast_detection_data, 
											/* win_size_time =  */		2048, 
											/* stride_time =  */		1024, 
											/* time_times =  */			4,		//4
											/* time_add =  */			32, 		//32
											/* win_size_freq =  */		256, 
									        /* stride_freq =  */		102, 
											/* time_accum =  */			8, 
											/* xhz1 =  */				2, 
											/* freq_times =  */			3, 
											/* respiration_times =  */	17.5
											);

	if (quick_detection_result && (run_mode == ALL_CHECK))
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

	for(i=0; i<SLOW_MAX_DATA_POOL; i++)
	{
		adc_temp = Fast_detection_data[i];
		adc_sum += adc_temp;
	}
	adc_average = (int)(adc_sum/SLOW_MAX_DATA_POOL + 0.5);

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
											/*data_size*/				4096,
											/*win_size_freq*/			128,
											/*stride_freq*/				64,
											/*time_accum*/				16,
											/*xhz1*/					2,
											/*freq_times*/				slow_freq_times,		//6
											/*respiration_times*/		res_times,		//res_times
											/*respirationfreq_vote*/	respirationfreq_vote
											);
										
	if( bigmotion_freq_vote == 1 )
	{
		offset = offsetmax;
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
	
	printf("result: %d - %d - %d - %d\r\n", bigmotion_time_vote, bigmotion_freq_vote, respirationfreq_vote[0], micromotion_detection_result);	

	for(i=0; i<SLOW_MAX_DATA_POOL; i++)
	{
		Fast_detection_data[i] += adc_average;
	}
	
	///////////////////////////////////////////
	//micromotion_detection_result = 0;
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
			if (no_person_timer >= 2)		//delay_time_num
			{
				no_person_timer = 0;
				state = IDLE;
				next_state = FAST_CHECK_DATA_PREPARE;	//no person so all loopback	to fast check
				//do some thing
				clear_buffer();
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

void idle_process(void)
{
	//do some thing here global
	//
	//
	state = UART_PROTOCOL;
}

void error_process(void)
{
	//do some print
	printf("error!!! \r\n");
}

void uart_post_process()
{
	//
	state = next_state;
}

void app(void)
{
	switch (state)
	{
		case	FAST_CHECK_DATA_PREPARE:
			mcu_dp_enum_update(DPID_CHECK_PROCESS, 0);
			fast_check_data_prepare();
			break;
		case	FAST_CHECK:
			fast_check_process();
			break;
		case	SLOW_CHECK_DATA_PREPARE_S0:
			mcu_dp_enum_update(DPID_CHECK_PROCESS, 1);
			slow_check_data_prepare_s0();
			break;
		case	SLOW_CHECK_DATA_PREPARE_S1:
			slow_check_data_prepare_s1();
			break;			
		case	SLOW_CHECK_S0:
			slow_check_process_s0();
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

int main(void)
{
	FIFO_Init(&FIFO_Data[0]);
	FIFO_Init(&FIFO_Data[1]);
	SysClkIni();
	led_init();
	usart_init();//both debug and tuya
	AdcConfig();
	timer0_init();
	ADC_StartConvert(M4_ADC1);
	bt_protocol_init();
	
	while(1)
	{				
		app();
	}
}
