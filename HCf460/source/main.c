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
#include "matlab.h"
#include "uart_protocol.h"

#include "cfar_ca_emxAPI.h"
#include "time_detection.h"
#include "std_cv.h"
#include "freq_detection.h"
#include "stdio.h"
#include "fifo.h"

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

float   offsetmax =  0.65;     //门限偏置
float   offsetmin =  0.6;

FIFO_DataType Fast_detection_data[MAX_DATA_POOL] = {0};//big raw data pool

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
	ERROR,
};

enum slow_s0_result
{
	NO_PERSON_NOT_SURE=0,
	BIG_MOTION,
	BREATHE,
	BREATHE_NOT_SURE,
};

int state = FAST_CHECK_DATA_PREPARE;	//状态机变量
int next_state = FAST_CHECK_DATA_PREPARE;

int slow_s0_result = NO_PERSON_NOT_SURE;
int slow_s0_result_last = NO_PERSON_NOT_SURE;


void fast_check_data_prepare(void)
{
	static	int i = 0;	//index
	int k = 0;	//index
	
	if (FAST_CHECK_SAMPLES < FIFO_GetDataCount(&FIFO_Data[0]))
	{
		if (FAST_CHECK_TIMES == i)
		{			
			for(k=0;k<MAX_DATA_POOL - FAST_CHECK_SAMPLES;k++)   //滑窗
			{
				Fast_detection_data[k] =Fast_detection_data[k + FAST_CHECK_SAMPLES];		
			}			
			printf("fifo number: %d - %d\r\n", i, FIFO_GetDataCount(&FIFO_Data[0]));
			FIFO_ReadData(&FIFO_Data[0], &Fast_detection_data[FAST_CHECK_SAMPLES*(i-1)], FAST_CHECK_SAMPLES);
			printf("fifo number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[0]));
			state = FAST_CHECK;
		}
		else
		{
			printf("fifo0 number: %d - %d\r\n", i, FIFO_GetDataCount(&FIFO_Data[0]));
			FIFO_ReadData(&FIFO_Data[0], &Fast_detection_data[FAST_CHECK_SAMPLES*i], FAST_CHECK_SAMPLES);
			printf("fifo0 number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[0]));
			i++;

			if (i == FAST_CHECK_TIMES)
			{
				state = FAST_CHECK;
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
											/* time_times =  */			5.8,
											/* time_add =  */			60, 
											/* win_size_freq =  */		256, 
									        /* stride_freq =  */		102, 
											/* time_accum =  */			8, 
											/* xhz1 =  */				2, 
											/* freq_times =  */			9, 
											/* respiration_times =  */	20.5
											);

	if (quick_detection_result)
	{
		state = SLOW_CHECK_DATA_PREPARE_S0;
	}
	else
	{
		for(i=0; i<FAST_MAX_DATA_POOL; i++)
		{
			Fast_detection_data[i] += adc_average;
		}
		state = IDLE;
		next_state = FAST_CHECK_DATA_PREPARE;
	}
}

void slow_check_data_prepare_s0(void)
{
	int i = 0;	//index
	FIFO_DataType  temp[2048] = {0};//temp data
	
	if (SLOW_CHECK_SAMPLES < FIFO_GetDataCount(&FIFO_Data[0]))
	{
		printf("fifo number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[0]));
		FIFO_ReadData(&FIFO_Data[0], temp, SLOW_CHECK_SAMPLES);
		printf("fifo number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[0]));

		for(i=0;i<SLOW_CHECK_USE_SAMPLES;i++)
		{
			FIFO_WriteOneData(&FIFO_Data[1], temp[i*SLOW_CHECK_OVER_SAMPLE]);
		}
		state = SLOW_CHECK_DATA_PREPARE_S1;
	}
	else
	{
		state = IDLE;
		next_state = SLOW_CHECK_DATA_PREPARE_S0;
	}
}

void slow_check_data_prepare_s1(void)
{
	static	int i = 0;	//index
	int k = 0;	//index
	
	if (SLOW_CHECK_SAMPLES < FIFO_GetDataCount(&FIFO_Data[1]))
	{
		if (SLOW_CHECK_TIMES == i)
		{			
			for(k=0;k<SLOW_MAX_DATA_POOL - SLOW_CHECK_SAMPLES;k++)   //滑窗
			{
				Fast_detection_data[k] =Fast_detection_data[k + SLOW_CHECK_SAMPLES];		
			}			
			printf("fifo1 number: %d - %d\r\n", i, FIFO_GetDataCount(&FIFO_Data[1]));
			FIFO_ReadData(&FIFO_Data[1], &Fast_detection_data[SLOW_CHECK_SAMPLES*(i-1)], SLOW_CHECK_SAMPLES);
			printf("fifo1 number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[1]));
			state = SLOW_CHECK_S0;
		}
		else
		{
			printf("fifo1 number: %d - %d\r\n", i, FIFO_GetDataCount(&FIFO_Data[1]));
			FIFO_ReadData(&FIFO_Data[1], &Fast_detection_data[SLOW_CHECK_SAMPLES*i], SLOW_CHECK_SAMPLES);
			printf("fifo1 number: %d\r\n", FIFO_GetDataCount(&FIFO_Data[1]));
			i++;

			if (i == SLOW_CHECK_TIMES)
			{
				state = SLOW_CHECK_S0;
			}
			else
			{
				state = IDLE;
				next_state = SLOW_CHECK_DATA_PREPARE_S0;
			}
		}
	}
	else
	{
		state = IDLE;
		next_state = SLOW_CHECK_DATA_PREPARE_S0;
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
											/*time_times*/		5,
											/*time_add*/		50
											);
									 
	bigmotion_freq_vote  = freq_detection(	/*in_data_freq*/			Fast_detection_data,
											/*hamming_TAB2*/			hamming_TAB2,
											/*data_size*/				4096,
											/*win_size_freq*/			128,
											/*stride_freq*/				64,
											/*time_accum*/				16,
											/*xhz1*/					2,
											/*freq_times*/				6.5,
											/*respiration_times*/		20.5,
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

	for(i=0; i<SLOW_MAX_DATA_POOL; i++)
	{
		Fast_detection_data[i] += adc_average;
	}

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

	switch (slow_s0_result)
	{
	case BIG_MOTION:
		state = IDLE;
		next_state = SLOW_CHECK_DATA_PREPARE_S0;
		break;
	case BREATHE:
		state = IDLE;
		next_state = SLOW_CHECK_DATA_PREPARE_S0;
		break;
	case BREATHE_NOT_SURE:
		state = SLOW_CHECK_S1;
		break;
	case NO_PERSON_NOT_SURE:
		state = SLOW_CHECK_S1;
		break;
	default:
		state = ERROR;
		break;
	}
	
}

//delay_time_num = (ceil)(delay_time* 2/ secnum); 
//respirationfreq_num=(ceil)(delay_time* 2/ secnum*0.125);

void slow_check_process_s1(void)
{
	static int breathe_timer = 0;
	static int no_person_timer = 0;

	if (slow_s0_result_last != slow_s0_result)
	{
		slow_s0_result_last = slow_s0_result;
		state = IDLE;
		next_state = SLOW_CHECK_DATA_PREPARE_S0;		
	}
	else
	{
		switch (slow_s0_result)
		{
		case BREATHE_NOT_SURE:
			breathe_timer++;
			if (breathe_timer > 3)
			{
				//do some thing
			}
			state = IDLE;
			next_state = SLOW_CHECK_DATA_PREPARE_S0;		
			break;
		case NO_PERSON_NOT_SURE:
			no_person_timer++;
			if (no_person_timer > 5)
			{
				state = IDLE;
				next_state = FAST_CHECK_DATA_PREPARE;				
				//do some thing
			}
			else
			{
				state = IDLE;
				next_state = SLOW_CHECK_DATA_PREPARE_S0;
			}
			break;
		default:
			state = ERROR;
			break;
		}
	}
}

void idle_process(void)
{
	state = UART_PROTOCOL;
}

void error_process(void)
{
	//do some print
}

void app(void)
{
	switch (state)
	{
		case	FAST_CHECK_DATA_PREPARE:
			fast_check_data_prepare();
			break;
		case	FAST_CHECK:
			fast_check_process();
			break;
		case	SLOW_CHECK_DATA_PREPARE_S0:
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
			uart_service();
			break;
		case	IDLE:
			idle_process();
			break;
		case	ERROR:
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
	usart_init();
	AdcConfig();
	timer0_init();
	ADC_StartConvert(M4_ADC1);
	uart_protocol_init();
	
	while(1)
	{				
		app();
	}
}
