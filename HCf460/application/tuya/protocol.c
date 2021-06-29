/*************************************    ***Copyright (c)*************************
**                               版权所有 (C), 2015-2020, 涂鸦科技
**
**                                 http://www.tuya.com
**
**--------------文件信息-------------------------------------------------------
**文   件   名: protocol.c
**描        述: 下发/上报数据处理函数
**使 用 说 明 :

                  *******非常重要，一定要看哦！！！********

** 1、用户在此文件中实现数据下发/上报功能
** 2、DP的ID/TYPE及数据处理函数都需要用户按照实际定义实现
** 3、当开始某些宏定义后需要用户实现代码的函数内部有#err提示,完成函数后请删除该#err
**
**--------------版本修订记录---------------------------------------------------

** 版  本:v2.0
** 日　期: 2020年3月23日
** 描　述: 
1.	增加cmd 0x09模块解绑接口支持
2.	增加cmd 0x0e rf射频测试接口支持
3.	增加cmd 0xe0 记录型数据上报接口支持
4.	增加cmd 0xE1 获取实时时间接口支持
5.	增加 cmd 0xe2 修改休眠模式状态广播间隔支持
6.	增加 cmd 0xe4 关闭系统时钟功能支持
7.	增加 cmd 0xe5 低功耗使能支持
8.	增加 cmd 0xe6 获取一次性动态密码支持
9.	增加 cmd 0xe7断开蓝牙连接支持
10.	增加 cmd 0xe8 查询MCU版本号支持
11.	增加 cmd 0xe9 MCU主动发送版本号支持
12.	增加 cmd 0xea OTA升级请求支持
13.	增加 cmd 0xeb OTA升级文件信息支持
14.	增加 cmd 0xec OTA升级文件偏移请求支持
15.	增加 cmd 0xed OTA升级数据支持
16.	增加 cmd 0xee OTA升级结束支持
17.	增加 cmd 0xa0 MCU 获取模块版本信息支持
18.	增加 cmd 0xa1 恢复出厂设置通知支持
19.  增加MCU OTA demo
20. 优化串口解析器

******************************************************************************/
#include "sys.h"
#include "bluetooth.h"
#include "myusart.h"
#include "hc32f46x_usart.h"
#include "hc32_ddl.h"

extern int check_status;
extern int person_in_range_flag;
extern int run_mode;
extern int slow_only_flag;
extern int state;
extern int next_state;

extern unsigned char g_work_mode;
extern unsigned char find_me_flag;
extern unsigned short  light_sensor_adc_data;

extern unsigned int  person_meter;

extern float breathe_freq;
extern int light_status_flag;
extern unsigned int slow_check_result;

extern int study_flag;
extern int study_mode;

extern	union KKK upssa0;
extern	char quick_detection_result_last;
extern	char slow_check_result_last;
extern	unsigned char data_report_upload_enable;
extern	int breathe_upload_en;
///////////////////////////////////////////////////////////////////////////////
void Delay_ms(unsigned int t);
void update_check_parameter(void);
void save_upssa0(void);

/******************************************************************************
                                移植须知:
1:MCU必须在while中直接调用mcu_api.c内的bt_uart_service()函数
2:程序正常初始化完成后,建议不进行关串口中断,如必须关中断,关中断时间必须短,关中断会引起串口数据包丢失
3:请勿在中断/定时器中断内调用上报函数
******************************************************************************/

         
/******************************************************************************
                              第一步:初始化
1:在需要使用到bt相关文件的文件中include "bt.h"
2:在MCU初始化中调用mcu_api.c文件中的bt_protocol_init()函数
3:将MCU串口单字节发送函数填入protocol.c文件中uart_transmit_output函数内,并删除#error
4:在MCU串口接收函数中调用mcu_api.c文件内的uart_receive_input函数,并将接收到的字节作为参数传入
5:单片机进入while循环后调用mcu_api.c文件内的bt_uart_service()函数
******************************************************************************/

/******************************************************************************
                        1:dp数据点序列类型对照表
          **此为自动生成代码,如在开发平台有相关修改请重新下载MCU_SDK**         
******************************************************************************/
const DOWNLOAD_CMD_S download_cmd[] =
{
  {DPID_PIR_DELAY, DP_TYPE_VALUE},
  {DPID_LOAD_RADAR_PARAMETER, DP_TYPE_ENUM},
  {DPID_LIGHT_STATUS, DP_TYPE_ENUM},
  {DPID_PERSON_IN_RANGE, DP_TYPE_ENUM},
  {DPID_FIND_ME, DP_TYPE_BOOL},
  {DPID_PERSON_METER, DP_TYPE_VALUE},
  {DPID_CHECK_PROCESS, DP_TYPE_ENUM},
  {DPID_SLOW_CHECK_RESULT, DP_TYPE_ENUM},
  {DPID_TIME_TIMES, DP_TYPE_VALUE},
  {DPID_TIME_TIMES_RT, DP_TYPE_VALUE},
  {DPID_TIME_ADD, DP_TYPE_VALUE},
  {DPID_TIME_ADD_RT, DP_TYPE_VALUE},
  {DPID_WORK_MODE, DP_TYPE_ENUM},
  {DPID_FREQ_TIMES, DP_TYPE_VALUE},
  {DPID_FREQ_TIMES_RT, DP_TYPE_VALUE},
  {DPID_FACTORY_OP, DP_TYPE_ENUM},
  {DPID_FREQ_PARAMETER1, DP_TYPE_VALUE},
  {DPID_FREQ_PARAMETER1_RT, DP_TYPE_VALUE},
  {DPID_FREQ_PARAMETER2, DP_TYPE_VALUE},
  {DPID_FREQ_PARAMETER2_RT, DP_TYPE_VALUE},
  {DPID_LIGHT_SENSOR_RAW, DP_TYPE_VALUE},
  {DPID_LIGHT_THRESHOLD1, DP_TYPE_VALUE},
  {DPID_LIGHT_THRESHOLD2, DP_TYPE_VALUE},
  {DPID_LIGHT_THRESHOLD3, DP_TYPE_VALUE},
  {DPID_LIGHT_THRESHOLD4, DP_TYPE_VALUE},
  {DPID_COMMON_COMMAND, DP_TYPE_ENUM},
  {DPID_BREATHE_FREQ, DP_TYPE_VALUE},
  {DPID_STUDY_CMD, DP_TYPE_ENUM},
  {DPID_STUDY_PROCESS_UPLOAD, DP_TYPE_ENUM},
	{DPID_UPLOAD_DUTY, DP_TYPE_VALUE},
};

/******************************************************************************
                           2:串口单字节发送函数
请将MCU串口发送函数填入该函数内,并将接收到的数据作为参数传入串口发送函数
******************************************************************************/

/*****************************************************************************
函数名称 : uart_transmit_output
功能描述 : 发数据处理
输入参数 : value:串口收到字节数据
返回参数 : 无
使用说明 : 请将MCU串口发送函数填入该函数内,并将接收到的数据作为参数传入串口发送函数
*****************************************************************************/
void uart_transmit_output(unsigned char value)
{
#ifndef TUYA_ENABLE
    USART_CH->DR = value;
    while (0ul == USART_CH->SR_f.TC)
    {
        ;
    }	
#else
    USART_TUYA_CH->DR = value;
    while (0ul == USART_TUYA_CH->SR_f.TC)
    {
        ;
    }	
#endif		
}
/******************************************************************************
                           第二步:实现具体用户函数
1:APP下发数据处理
2:数据上报处理
******************************************************************************/

/******************************************************************************
                            1:所有数据上报处理
当前函数处理全部数据上报(包括可下发/可上报和只上报)
  需要用户按照实际情况实现:
  1:需要实现可下发/可上报数据点上报
  2:需要实现只上报数据点上报
此函数为MCU内部必须调用
用户也可调用此函数实现全部数据上报
******************************************************************************/

//自动化生成数据上报函数

/*****************************************************************************
函数名称 : all_data_update
功能描述 : 系统所有dp点信息上传,实现APP和muc数据同步
输入参数 : 无
返回参数 : 无
使用说明 : 此函数SDK内部需调用;
           MCU必须实现该函数内数据上报功能;包括只上报和可上报可下发型数据
*****************************************************************************/
void all_data_update(void)
{
  //此代码为平台自动生成，请按照实际数据修改每个可下发可上报函数和只上报函数
    //mcu_dp_value_update(DPID_PIR_DELAY, 0); //枚举型数据上报;
		//Delay_ms(ALL_UPLOAD_DELAY);
    //mcu_dp_enum_update(DPID_LOAD_RADAR_PARAMETER, 0); //枚举型数据上报;
	//Delay_ms(ALL_UPLOAD_DELAY);
	mcu_dp_enum_update(DPID_LIGHT_STATUS, light_status_flag);
	Delay_ms(ALL_UPLOAD_DELAY);
	//////////////////////////////////////////////////////////////////////////

    mcu_dp_enum_update(DPID_PERSON_IN_RANGE, person_in_range_flag); //枚举型数据上报;
		Delay_ms(ALL_UPLOAD_DELAY);

		if (person_meter != 0)
		{
			if (0)	//bypass this
			{
				mcu_dp_value_update(DPID_PERSON_METER,person_meter); //VALUE型数据上报;
			}
			Delay_ms(ALL_UPLOAD_DELAY);
		}
    
	mcu_dp_enum_update(DPID_CHECK_PROCESS, check_status); //枚举型数据上报;
    Delay_ms(ALL_UPLOAD_DELAY);
	mcu_dp_enum_update(DPID_SLOW_CHECK_RESULT, slow_check_result);
	Delay_ms(ALL_UPLOAD_DELAY);
    mcu_dp_enum_update(DPID_WORK_MODE, g_work_mode); //枚举型数据上报;
		Delay_ms(ALL_UPLOAD_DELAY);
		mcu_dp_bool_update(DPID_FIND_ME,1);
		Delay_ms(ALL_UPLOAD_DELAY);
		mcu_dp_value_update(DPID_LIGHT_THRESHOLD1, upssa0.ppp.Light_threshold1);
		Delay_ms(ALL_UPLOAD_DELAY);
		mcu_dp_value_update(DPID_LIGHT_THRESHOLD2, upssa0.ppp.Light_threshold2);
		Delay_ms(ALL_UPLOAD_DELAY);
		mcu_dp_value_update(DPID_LIGHT_THRESHOLD3, upssa0.ppp.Light_threshold3);
		Delay_ms(ALL_UPLOAD_DELAY);
		mcu_dp_value_update(DPID_LIGHT_THRESHOLD4, upssa0.ppp.Light_threshold4);
		Delay_ms(ALL_UPLOAD_DELAY);
		
		if (breathe_freq != 0)
		{
			if (breathe_upload_en)	//
			{
				mcu_dp_value_update(DPID_BREATHE_FREQ, (int)((breathe_freq*10.0f)+0.5f));
				Delay_ms(ALL_UPLOAD_DELAY);
			}		
		}
		
		update_check_parameter();
		
}


/******************************************************************************
                                WARNING!!!    
                            2:所有数据上报处理
自动化代码模板函数,具体请用户自行实现数据处理
******************************************************************************/

/*****************************************************************************
函数名称 : dp_download_pir_delay_handle
功能描述 : 针对DPID_PIR_DELAY的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_pir_delay_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为ENUM
    unsigned char ret;
    unsigned char pir_delay;
    
    pir_delay = mcu_get_dp_download_value(value,length);
	
	upssa0.ppp.delay_time_num = pir_delay;
	
	SEGGER_RTT_printf(0, "%s%snew delay_time_num: %d%s\r\n", RTT_CTRL_BG_BRIGHT_YELLOW, RTT_CTRL_TEXT_BLACK, upssa0.ppp.delay_time_num, RTT_CTRL_RESET);

    //处理完DP数据后应有反馈
    ret = mcu_dp_value_update(DPID_PIR_DELAY, upssa0.ppp.delay_time_num);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_load_radar_parameter_handle
功能描述 : 针对DPID_LOAD_RADAR_PARAMETER的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
void load_ceiling_setup(int mode)
{
	if (mode == 0)		//1M
	{
		upssa0.ppp.quick_time_times = 3.05f;
		upssa0.ppp.quick_time_add = 172.79f;
		upssa0.ppp.quick_freq_times = 58.96f;
		upssa0.ppp.slow_time_times = 3.05f*1.3f;
		upssa0.ppp.slow_time_add = 172.79f;
		upssa0.ppp.slow_freq_times = 58.96f;
		upssa0.ppp.res_times = 202.17f;
		upssa0.ppp.offsetmin = 1.22f*1.1f;
	}
	else if (mode == 1)		//1.5M
	{
		upssa0.ppp.quick_time_times = 2.53f;
		upssa0.ppp.quick_time_add = 106.50f;
		upssa0.ppp.quick_freq_times = 44.52f;
		upssa0.ppp.slow_time_times = 2.53f*1.3f;
		upssa0.ppp.slow_time_add = 106.50f;
		upssa0.ppp.slow_freq_times = 44.52f;
		upssa0.ppp.res_times = 183.92f;
		upssa0.ppp.offsetmin = 1.27f;
	}
	else if (mode == 2)		//2.0M
	{
		upssa0.ppp.quick_time_times = 2.42f;
		upssa0.ppp.quick_time_add = 78.02f;
		upssa0.ppp.quick_freq_times = 37.03f;
		upssa0.ppp.slow_time_times = 2.42f*1.3f;
		upssa0.ppp.slow_time_add = 78.02f;
		upssa0.ppp.slow_freq_times = 37.03f;
		upssa0.ppp.res_times = 164.03f;
		upssa0.ppp.offsetmin = 1.16f*1.1f;
	}
	else if (mode == 3)		//2.5M
	{
		upssa0.ppp.quick_time_times = 2.39f;
		upssa0.ppp.quick_time_add = 59.78f;
		upssa0.ppp.quick_freq_times = 30.22f;
		upssa0.ppp.slow_time_times = 2.39f*1.3f;
		upssa0.ppp.slow_time_add = 59.78f;
		upssa0.ppp.slow_freq_times = 30.22f;
		upssa0.ppp.res_times = 139.01f;
		upssa0.ppp.offsetmin = 1.27f*1.1f;
	}
	else if (mode == 4)		//3.0M
	{
		upssa0.ppp.quick_time_times = 2.37f;
		upssa0.ppp.quick_time_add = 51.72f;
		upssa0.ppp.quick_freq_times = 27.72f;
		upssa0.ppp.slow_time_times = 2.37f*1.3f;
		upssa0.ppp.slow_time_add = 51.72f;
		upssa0.ppp.slow_freq_times = 27.72f;
		upssa0.ppp.res_times = 116.44f;
		upssa0.ppp.offsetmin = 1.07f*1.1f;
	}
	else if (mode == 5)		//3.5M
	{
		upssa0.ppp.quick_time_times = 2.51f;
		upssa0.ppp.quick_time_add = 40.12f;
		upssa0.ppp.quick_freq_times = 20.24f;
		upssa0.ppp.slow_time_times = 2.51f;
		upssa0.ppp.slow_time_add = 40.12f;
		upssa0.ppp.slow_freq_times = 20.24f;
		upssa0.ppp.res_times = 109.41f;
		upssa0.ppp.offsetmin = 1.08f;
	}	
	else if (mode == 6)		//4.0M
	{
		upssa0.ppp.quick_time_times = 2.31f;
		upssa0.ppp.quick_time_add = 34.52f;
		upssa0.ppp.quick_freq_times = 21.14f;
		upssa0.ppp.slow_time_times = 2.31f;
		upssa0.ppp.slow_time_add = 34.52f;
		upssa0.ppp.slow_freq_times = 21.14f;
		upssa0.ppp.res_times = 94.20f;
		upssa0.ppp.offsetmin = 1.05f;
	}
	else if (mode == 7)		//4.5M
	{
		upssa0.ppp.quick_time_times = 2.12f;
		upssa0.ppp.quick_time_add = 27.67f;
		upssa0.ppp.quick_freq_times = 19.83f;
		upssa0.ppp.slow_time_times = 2.12f*1.2f;
		upssa0.ppp.slow_time_add = 27.67f;
		upssa0.ppp.slow_freq_times = 19.83f;
		upssa0.ppp.res_times = 93.56f;
		upssa0.ppp.offsetmin = 1.15f*1.05f;
	}
	else if (mode == 8)		//5.0M
	{
		upssa0.ppp.quick_time_times = 2.05f;
		upssa0.ppp.quick_time_add = 21.50f;
		upssa0.ppp.quick_freq_times = 17.54f;
		upssa0.ppp.slow_time_times = 2.05f*1.2f;
		upssa0.ppp.slow_time_add = 21.50f;
		upssa0.ppp.slow_freq_times = 17.54f;
		upssa0.ppp.res_times = 85.85f;
		upssa0.ppp.offsetmin = 1.08f*0.95f;
	}
	else if (mode == 101)	//台灯
	{
		upssa0.ppp.quick_time_times = 3.05f;
		upssa0.ppp.quick_time_add = 495.13f;
		upssa0.ppp.quick_freq_times = 297.25f;
		upssa0.ppp.slow_time_times = 24.55f;
		upssa0.ppp.slow_time_add = 495.13f;
		upssa0.ppp.slow_freq_times = 56.29f;
		upssa0.ppp.res_times = 470.40f;
		upssa0.ppp.offsetmin = 2.33f;
	}
}

void load_wall_setup(int mode)
{
	if (mode == 0)
	{
		upssa0.ppp.quick_time_times = 2048.0f;
		upssa0.ppp.quick_time_add = 135.0f;
		upssa0.ppp.quick_freq_times = 12.0f;
		upssa0.ppp.slow_time_times = 2048.0f;
		upssa0.ppp.slow_time_add = 135.0f;
		upssa0.ppp.slow_freq_times = 12.0f;
		upssa0.ppp.res_times = 78.0f;
		upssa0.ppp.offsetmin = 1.23f;
	}
	else if (mode == 1)
	{
		upssa0.ppp.quick_time_times = 3.8f;
		upssa0.ppp.quick_time_add = 35.0f;
		upssa0.ppp.quick_freq_times = 4.0f;
		upssa0.ppp.slow_time_times = 3.8f;
		upssa0.ppp.slow_time_add = 35.0f;
		upssa0.ppp.slow_freq_times = 4.0f;
		upssa0.ppp.res_times = 16.5f;
		upssa0.ppp.offsetmin = 0.33f;
	}
}

static unsigned char dp_download_load_radar_parameter_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为ENUM
    unsigned char ret;
    unsigned char load_radar_parameter;
    
    load_radar_parameter = mcu_get_dp_download_enum(value,length);
    switch(load_radar_parameter) {
        case 0:
			//load_ceiling_setup(0);
			//SEGGER_RTT_printf(0, "%s%sload ceiling setup %d%s\r\n", RTT_CTRL_BG_BRIGHT_YELLOW, RTT_CTRL_TEXT_BLACK, 0, RTT_CTRL_RESET);
        break;
     
        case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
					load_ceiling_setup(load_radar_parameter - 1);
					SEGGER_RTT_printf(0, "%s%sload ceiling setup %d%s\r\n", RTT_CTRL_BG_BRIGHT_YELLOW, RTT_CTRL_TEXT_BLACK, load_radar_parameter - 1, RTT_CTRL_RESET);   
        break;
        
        
        case 0x60:
        case 0x61:
        case 0x62:
        case 0x63:
        case 0x64:					
            SEGGER_RTT_printf(0, "load user parameter%d\r\n", load_radar_parameter - 0x60);
        break;
        
        default:
					//
        break;
    }
    
    //处理完DP数据后应有反馈
    ret = mcu_dp_enum_update(DPID_LOAD_RADAR_PARAMETER, load_radar_parameter);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_find_me_handle
功能描述 : 针对DPID_FIND_ME的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_find_me_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为BOOL
    unsigned char ret;
    //0:关/1:开

		find_me_flag = 1;
  
    //处理完DP数据后应有反馈
    ret = mcu_dp_bool_update(DPID_FIND_ME,1);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_time_times_handle
功能描述 : 针对DPID_TIME_TIMES的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_time_times_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为VALUE
    unsigned char ret;
    unsigned long time_times_x;
    
    time_times_x = mcu_get_dp_download_value(value,length);

		if (g_work_mode == FAST_CHECK_ONLY)
		{
			//
			upssa0.ppp.quick_time_times = time_times_x/100.0f;
		}
		else if (g_work_mode == SLOW_CHECK_ONLY)
		{
			//
			upssa0.ppp.slow_time_times = time_times_x/100.0f;
		}
    
    //处理完DP数据后应有反馈
    ret = mcu_dp_value_update(DPID_TIME_TIMES, time_times_x);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_time_add_handle
功能描述 : 针对DPID_TIME_ADD的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_time_add_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为VALUE
    unsigned char ret;
    unsigned long time_add_x;
    
    time_add_x = mcu_get_dp_download_value(value,length);
	
		if (g_work_mode == FAST_CHECK_ONLY)
		{
			//
			upssa0.ppp.quick_time_add = time_add_x/100.0f;
		}
		else if (g_work_mode == SLOW_CHECK_ONLY)
		{
			upssa0.ppp.slow_time_add = time_add_x/100.0f;
		}
    
    //处理完DP数据后应有反馈
    ret = mcu_dp_value_update(DPID_TIME_ADD,time_add_x);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_work_mode_handle
功能描述 : 针对DPID_WORK_MODE的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_work_mode_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为ENUM
    unsigned char ret;
    unsigned char work_mode;
    
    work_mode = mcu_get_dp_download_enum(value,length);
    switch(work_mode) {
        case 0:
					run_mode = 0;//进慢
          slow_only_flag = 0;	//回快	
        break;
        
        case 1:
					run_mode = 1;//不进慢
          slow_only_flag = 0;//回快
				
					if (next_state == FAST_CHECK_DATA_PREPARE)
					{
						//
					}
					else if(next_state == FAST_CHECK)
					{
						//
					}
					else if(next_state == SLOW_CHECK_DATA_PREPARE_S0)
					{
						state = IDLE;
						next_state = FAST_CHECK_DATA_PREPARE;
						clear_buffer();
					}
					else if(next_state == SLOW_CHECK_DATA_PREPARE_S1)
					{
						state = IDLE;
						next_state = FAST_CHECK_DATA_PREPARE;
						clear_buffer();
					}
					else if(next_state == SLOW_CHECK_S0)
					{
						state = IDLE;
						next_state = FAST_CHECK_DATA_PREPARE;
						clear_buffer();
					}	
					else if(next_state == SLOW_CHECK_S1)
					{
						state = IDLE;
						next_state = FAST_CHECK_DATA_PREPARE;
						clear_buffer();
					}
        break;
        
        case 2:
					run_mode = 0;//进慢
          slow_only_flag = 1;//不回快

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
        break;
        
        case 3:
        break;
        
        default:
    
        break;
    }
    
		g_work_mode = work_mode;
    //处理完DP数据后应有反馈
    ret = mcu_dp_enum_update(DPID_WORK_MODE, g_work_mode);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_freq_times_handle
功能描述 : 针对DPID_FREQ_TIMES的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_freq_times_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为VALUE
    unsigned char ret;
    unsigned long freq_times_x;
		char float_str[64];		
    
    freq_times_x = mcu_get_dp_download_value(value,length);

		if (g_work_mode == FAST_CHECK_ONLY)
		{
			//
			upssa0.ppp.quick_freq_times = freq_times_x/100.0f;
			sprintf(float_str, "quick_freq_times value: %.2lf\r\n", upssa0.ppp.quick_freq_times);
			SEGGER_RTT_printf(0, "%s", float_str);			
		}
		else if (g_work_mode == SLOW_CHECK_ONLY)
		{
			//
			upssa0.ppp.slow_freq_times = freq_times_x/100.0f;
			sprintf(float_str, "slow_freq_times value: %.2lf\r\n", upssa0.ppp.slow_freq_times);
			SEGGER_RTT_printf(0, "%s", float_str);			
		}
    
		
    //处理完DP数据后应有反馈
    ret = mcu_dp_value_update(DPID_FREQ_TIMES,freq_times_x);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_factory_op_handle
功能描述 : 针对DPID_FACTORY_OP的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_factory_op_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为ENUM
    unsigned char ret;
    unsigned char factory_op;
    
    factory_op = mcu_get_dp_download_enum(value,length);
    switch(factory_op) {
        case 0:// soft reset the mcu
			soft_reset_mcu();
        break;
        
        case 1:// go bootloader and fw ota
			go_bootloader_ota();
        break;
        
        case 2:// tuya re-config the network
			tuya_re_config_network();
        break;
        
        case 3:// tuya reset module
			tuya_reset_module();
        break;
        
        case 4:// tuya retry the ota (send fail)
			tuya_retry_ota();
        break;
        
        case 5:// reset_default_parameter
			reset_default_parameter();				
        break;
        case 6:// do nothing
					//
        break;
        case 7:// tuya 产测
			//
        break;
        
        default:
    
        break;
    }
    
    //处理完DP数据后应有反馈
    ret = mcu_dp_enum_update(DPID_FACTORY_OP, factory_op);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_freq_parameter1_handle
功能描述 : 针对DPID_FREQ_PARAMETER1的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_freq_parameter1_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为VALUE
    unsigned char ret;
    unsigned long freq_parameter1;
		char float_str[64];		
    
    freq_parameter1 = mcu_get_dp_download_value(value,length);

		if (g_work_mode == SLOW_CHECK_ONLY)
		{
			//
			upssa0.ppp.res_times = freq_parameter1/100.0f;
			sprintf(float_str, "res_times value: %.2lf\r\n", upssa0.ppp.res_times);
			SEGGER_RTT_printf(0, "%s", float_str);			
		}
    
    //处理完DP数据后应有反馈
    ret = mcu_dp_value_update(DPID_FREQ_PARAMETER1,freq_parameter1);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_freq_parameter2_handle
功能描述 : 针对DPID_FREQ_PARAMETER2的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_freq_parameter2_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为VALUE
    unsigned char ret;
    unsigned long freq_parameter2;
		char float_str[64];		
    
    freq_parameter2 = mcu_get_dp_download_value(value,length);

		if (g_work_mode == SLOW_CHECK_ONLY)
		{
			//
			upssa0.ppp.offsetmin = freq_parameter2/1000.0f;
			sprintf(float_str, "offsetmin value: %.3lf\r\n", upssa0.ppp.offsetmin);
			SEGGER_RTT_printf(0, "%s", float_str);		
		}
    
    //处理完DP数据后应有反馈
    ret = mcu_dp_value_update(DPID_FREQ_PARAMETER2,freq_parameter2);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_light_threshold1_handle
功能描述 : 针对DPID_LIGHT_THRESHOLD1的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_light_threshold1_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为VALUE
    unsigned char ret;
    
    upssa0.ppp.Light_threshold1 = mcu_get_dp_download_value(value,length);
 
		SEGGER_RTT_printf(0, "%s%sload light sensor threshold1: %d%s\r\n", RTT_CTRL_BG_BRIGHT_YELLOW, RTT_CTRL_TEXT_BLACK, upssa0.ppp.Light_threshold1, RTT_CTRL_RESET);	

    //处理完DP数据后应有反馈
    ret = mcu_dp_value_update(DPID_LIGHT_THRESHOLD1, upssa0.ppp.Light_threshold1);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_light_threshold2_handle
功能描述 : 针对DPID_LIGHT_THRESHOLD2的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_light_threshold2_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为VALUE
    unsigned char ret;
    
    upssa0.ppp.Light_threshold2 = mcu_get_dp_download_value(value,length);
    
		SEGGER_RTT_printf(0, "%s%sload light sensor threshold2: %d%s\r\n", RTT_CTRL_BG_BRIGHT_YELLOW, RTT_CTRL_TEXT_BLACK, upssa0.ppp.Light_threshold2, RTT_CTRL_RESET);	
    //处理完DP数据后应有反馈
    ret = mcu_dp_value_update(DPID_LIGHT_THRESHOLD2, upssa0.ppp.Light_threshold2);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_light_threshold3_handle
功能描述 : 针对DPID_LIGHT_THRESHOLD3的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_light_threshold3_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为VALUE
    unsigned char ret;
    
    upssa0.ppp.Light_threshold3 = mcu_get_dp_download_value(value,length);
    
		SEGGER_RTT_printf(0, "%s%sload light sensor threshold3: %d%s\r\n", RTT_CTRL_BG_BRIGHT_YELLOW, RTT_CTRL_TEXT_BLACK, upssa0.ppp.Light_threshold3, RTT_CTRL_RESET);
    //处理完DP数据后应有反馈
    ret = mcu_dp_value_update(DPID_LIGHT_THRESHOLD3, upssa0.ppp.Light_threshold3);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_light_threshold4_handle
功能描述 : 针对DPID_LIGHT_THRESHOLD4的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_light_threshold4_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为VALUE
    unsigned char ret;
    
    upssa0.ppp.Light_threshold4 = mcu_get_dp_download_value(value,length);
    
		SEGGER_RTT_printf(0, "%s%sload light sensor threshold4: %d%s\r\n", RTT_CTRL_BG_BRIGHT_YELLOW, RTT_CTRL_TEXT_BLACK, upssa0.ppp.Light_threshold4, RTT_CTRL_RESET);	
    //处理完DP数据后应有反馈
    ret = mcu_dp_value_update(DPID_LIGHT_THRESHOLD4, upssa0.ppp.Light_threshold4);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}

static unsigned char dp_download_upload_duty_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为VALUE
    unsigned char ret;
    
    upssa0.ppp.upload_duty = mcu_get_dp_download_value(value,length);
    
		SEGGER_RTT_printf(0, "%s%sload upload_duty: %dms%s\r\n", RTT_CTRL_BG_BRIGHT_YELLOW, RTT_CTRL_TEXT_BLACK, upssa0.ppp.upload_duty, RTT_CTRL_RESET);		
    //处理完DP数据后应有反馈
    ret = mcu_dp_value_update(DPID_UPLOAD_DUTY, upssa0.ppp.upload_duty);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}

void update_check_parameter(void)
{
		if (g_work_mode == FAST_CHECK_ONLY)
		{
			//
			mcu_dp_value_update(DPID_TIME_TIMES, (int)((upssa0.ppp.quick_time_times*100.0f)+0.5f));
			Delay_ms(ALL_UPLOAD_DELAY);
			mcu_dp_value_update(DPID_TIME_ADD, (int)((upssa0.ppp.quick_time_add*100.0f)+0.5f));
			Delay_ms(ALL_UPLOAD_DELAY);
			mcu_dp_value_update(DPID_FREQ_TIMES, (int)((upssa0.ppp.quick_freq_times*100.0f)+0.5f));			
		}
		else if (g_work_mode == SLOW_CHECK_ONLY)
		{
			//
			mcu_dp_value_update(DPID_TIME_TIMES, (int)((upssa0.ppp.slow_time_times*100.0f)+0.5f));	
			Delay_ms(ALL_UPLOAD_DELAY);
			mcu_dp_value_update(DPID_TIME_ADD, (int)((upssa0.ppp.slow_time_add*100.0f)+0.5f)); 
			Delay_ms(ALL_UPLOAD_DELAY);
			mcu_dp_value_update(DPID_FREQ_TIMES, (int)((upssa0.ppp.slow_freq_times*100.0f)+0.5f));
			Delay_ms(ALL_UPLOAD_DELAY);
			mcu_dp_value_update(DPID_FREQ_PARAMETER1, (int)((upssa0.ppp.res_times*100.0f)+0.5f));
			Delay_ms(ALL_UPLOAD_DELAY);
			mcu_dp_value_update(DPID_FREQ_PARAMETER2, (int)((upssa0.ppp.offsetmin*1000.0f)+0.5f));
		}
		else
		{
			//
		}
}
/*****************************************************************************
函数名称 : dp_download_common_command_handle
功能描述 : 针对DPID_COMMON_COMMAND的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 只下发类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_common_command_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为ENUM
    unsigned char ret;
    unsigned char common_command;
    
    common_command = mcu_get_dp_download_enum(value,length);
    switch(common_command) {
        case 0://上传更新检测参数
			update_check_parameter();
        break;
        
        case 1://保存参数到flash
			save_upssa0();
        break;
        
        case 2://上传人状态
			mcu_dp_enum_update(DPID_PERSON_IN_RANGE, quick_detection_result_last);
        break;
        
        case 3://上传慢检测结果
			mcu_dp_enum_update(DPID_SLOW_CHECK_RESULT, slow_check_result_last);
        break;
        
        case 4://enable the data upload every 5s
			data_report_upload_enable = 1;
        break;
        
        case 5://disalbe the data upload every 5s
			data_report_upload_enable = 0;
        break;
        
				case 0x14://enable
					breathe_upload_en = 1;
					break;
				case 0x15://disable
					breathe_upload_en = 0;
					break;
        default:
					break;
    }
    
    //处理完DP数据后应有反馈
    ret = mcu_dp_enum_update(DPID_COMMON_COMMAND, common_command);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_study_cmd_handle
功能描述 : 针对DPID_STUDY_CMD的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_study_cmd_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为ENUM
    unsigned char ret;
    unsigned char study_cmd;
    
    study_cmd = mcu_get_dp_download_enum(value,length);
    switch(study_cmd) {
        case 0:
					study_flag = 1;
					study_mode = 0;
        break;
        
        case 1:
					study_flag = 1;
					study_mode = 1;					
        break;
        
        case 2:
					study_flag = 1;
					study_mode = 2;					
        break;
        
        case 3:
					study_flag = 1;
					study_mode = 3;					
        break;
        
        default:
    
        break;
    }
    
    //处理完DP数据后应有反馈
    ret = mcu_dp_enum_update(DPID_STUDY_CMD, study_cmd);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}


/******************************************************************************
                                WARNING!!!                     
此代码为SDK内部调用,请按照实际dp数据实现函数内部数据
******************************************************************************/

/******************************************************************************
                                WARNING!!!                     
以下函数用户请勿修改!!
******************************************************************************/

/*****************************************************************************
函数名称 : dp_download_handle
功能描述 : dp下发处理函数
输入参数 : dpid:DP序号
           value:dp数据缓冲区地址
           length:dp数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERRO
使用说明 : 该函数用户不能修改
*****************************************************************************/
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length)
{
  /*********************************
  当前函数处理可下发/可上报数据调用                    
  具体函数内需要实现下发数据处理
  完成用需要将处理结果反馈至APP端,否则APP会认为下发失败
  ***********************************/
  unsigned char ret;
  switch(dpid)
  {
        case DPID_PIR_DELAY:
            //感应延时处理函数
            ret = dp_download_pir_delay_handle(value,length);
        break;
        case DPID_LOAD_RADAR_PARAMETER:
            //加载预设参数处理函数
            ret = dp_download_load_radar_parameter_handle(value,length);
        break;
        case DPID_FIND_ME:
            //找我处理函数
            ret = dp_download_find_me_handle(value,length);
        break;
        case DPID_TIME_TIMES:
            //时域门限0处理函数
            ret = dp_download_time_times_handle(value,length);
        break;
        case DPID_TIME_ADD:
            //时域门限1处理函数
            ret = dp_download_time_add_handle(value,length);
        break;
        case DPID_WORK_MODE:
            //检测模式处理函数
            ret = dp_download_work_mode_handle(value,length);
        break;
        case DPID_FREQ_TIMES:
            //频域门限0处理函数
            ret = dp_download_freq_times_handle(value,length);
        break;
        case DPID_FACTORY_OP:
            //工厂操作处理函数
            ret = dp_download_factory_op_handle(value,length);
        break;
        case DPID_FREQ_PARAMETER1:
            //频域门限1处理函数
            ret = dp_download_freq_parameter1_handle(value,length);
        break;
        case DPID_FREQ_PARAMETER2:
            //频域门限2处理函数
            ret = dp_download_freq_parameter2_handle(value,length);
        break;
        case DPID_LIGHT_THRESHOLD1:
            //光敏门限1处理函数
            ret = dp_download_light_threshold1_handle(value,length);
        break;
        case DPID_LIGHT_THRESHOLD2:
            //光敏门限2处理函数
            ret = dp_download_light_threshold2_handle(value,length);
        break;
        case DPID_LIGHT_THRESHOLD3:
            //光敏门限3处理函数
            ret = dp_download_light_threshold3_handle(value,length);
        break;
        case DPID_LIGHT_THRESHOLD4:
            //光敏门限4处理函数
            ret = dp_download_light_threshold4_handle(value,length);
        break;
        case DPID_COMMON_COMMAND:
            //一般命令处理函数
            ret = dp_download_common_command_handle(value,length);
        break;
        case DPID_STUDY_CMD:
            //学习命令处理函数
            ret = dp_download_study_cmd_handle(value,length);
        break;
				case DPID_UPLOAD_DUTY:
						//通迅周期
						ret = dp_download_upload_duty_handle(value,length);
				break;

  default:
    break;
  }
  return ret;
}
/*****************************************************************************
函数名称 : get_download_cmd_total
功能描述 : 获取所有dp命令总和
输入参数 : 无
返回参数 : 下发命令总和
使用说明 : 该函数用户不能修改
*****************************************************************************/
unsigned char get_download_cmd_total(void)
{
  return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}

//////////////////////////////////当前MCU SDK版本较上一版本新增支持协议接口////////////////////
#ifdef TUYA_BCI_UART_COMMON_UNBOUND_REQ 
/*****************************************************************************
函数名称 :  bt_unbound_req
功能描述 : 向模块发送解绑请求，模块收到该指令后会解除蓝牙绑定关系
输入参数 : 无
返回参数 : 无
使用说明 : MCU主动解绑调用
*****************************************************************************/
void bt_unbound_req(void)
{
	bt_uart_write_frame(TUYA_BCI_UART_COMMON_UNBOUND_REQ,0);
}
#endif

#ifdef TUYA_BCI_UART_COMMON_RF_TEST 
/*****************************************************************************
函数名称 :  bt_rf_test_req
功能描述 : 向模块发射频测试请求
输入参数 : 无
返回参数 : 无
使用说明 : 
*****************************************************************************/
void bt_rf_test_req(void)
{
	bt_uart_write_frame(TUYA_BCI_UART_COMMON_RF_TEST,0);
}
/*****************************************************************************
函数名称 : bt_rf_test_result
功能描述 : 蓝牙rf测试反馈
输入参数 : result:蓝牙rf测试结果;0:失败/1:成功
           rssi:测试成功表示蓝牙信号强度/测试失败值无意义
返回参数 : 无
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_rf_test_result(unsigned char result,signed char rssi)
{
  //#error "请自行完善该功能,完成后请删除该行"
  if(result == 0)
  {
    //测试失败
  }
  else
  {
    //测试成功
    //rssi为信号强度，一般大于-70dbm 蓝牙信号都在正常范围内
  }
  
}
#endif

#ifdef TUYA_BCI_UART_COMMON_SEND_STORAGE_TYPE 
/*****************************************************************************
函数名称 : bt_send_recordable_dp_data
功能描述 : 记录型数据上报
输入参数 : type -1： 蓝牙模块自带时间上报 -2： 只上报原始数据，无时间-3： MCU自带时间上报
		dpid:  datapoint 序号
		dptype:对应开放平台上某datapoint 具体的数据类型
		value:具体数据
		len:数据的长度
返回参数 : 无
使用说明 : MCU需要自行完善该函数功能
	建议使用缓存队列，将要发给模块的所有数据放到MCU缓存队列中，上报成功一条后再上报下一条，记录型数据要确保每条数据都上报成功
*****************************************************************************/
void bt_send_recordable_dp_data(unsigned char snedType,unsigned char dpid,unsigned char dpType, unsigned char value[],unsigned short len)
{
	//#error "请自行完善该功能,完成后请删除该行"
	if(snedType==0x01)//格式1，蓝牙模块自带时间上报
	{

	}
	else if(snedType==0x02)//格式2，只上报原始数据，无时间  （备注:telink对接平台不支持该格式）
	{

	}
	else if(snedType==0x03)//格式3，MCU自带时间上报
	{

	}
}
/*****************************************************************************
函数名称 : bt_send_recordable_dp_data_result
功能描述 : 记录型数据上报
输入参数 : result ：0存储成功，1存储失败
返回参数 : 无
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_send_recordable_dp_data_result(unsigned char result)
{
	//#error "请自行完善该功能,完成后请删除该行"
}
#ifdef TUYA_BCI_UART_COMMON_SEND_TIME_SYNC_TYPE 
/*****************************************************************************
函数名称 : bt_send_time_sync_type
功能描述 : 向模块发送时间同步请求
输入参数 :sync_time_type
0x00- 获取7字节时间时间类型+2
字节时区信息
0x01- 获取13字节ms级unix时间
+2字节时区信息
0x02- 获取7字节时间时间类型+2
字节时区信息

返回参数 : 无
使用说明 :
*****************************************************************************/
void bt_send_time_sync_req(unsigned char sync_time_type)
{
	unsigned short length = 0;
  
  	length = set_bt_uart_byte(length,sync_time_type);
  	
	bt_uart_write_frame(TUYA_BCI_UART_COMMON_SEND_TIME_SYNC_TYPE,length);
}
/*****************************************************************************
函数名称 : bt_time_sync_result
功能描述 : 向模块发送时间同步的结果
输入参数 :result同步结果 0成功，其他失败
		sync_time_type时间格式
		bt_time自定义时间（如果是时间格式0或者1该值有效）
		time_zone_100 时区
		time_stamp_ms 时间戳（如果是时间格式1，该值有效）
返回参数 : 无
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_time_sync_result(unsigned char result,unsigned char sync_time_type,bt_time_struct_data_t bt_time,unsigned short time_zone_100,long long time_stamp_ms)
{
	//#error "请自行完善该功能,完成后请删除该行"
	if(result == 0x00)
	{
		//同步时间成功
		if(sync_time_type==0x00||sync_time_type==0x02)
		{
			//将bt_time中的自定义时间格式的数据填充到mcu时钟系统中
			//time_zone_100时区
		}
		else if(sync_time_type==0x01)
		{
			//将time_stamp_ms中的时间戳填充到mcu时钟系统中
			//time_zone_100时区
		}
	}
	else
	{
		//同步时间失败
	}
}
#endif
#endif

#ifdef TUYA_BCI_UART_COMMON_MODIFY_ADV_INTERVAL
/*****************************************************************************
函数名称 : bt_modify_adv_interval_req
功能描述 : 向模块发送修改低功耗下模块广播间隔的请求
输入参数 :value * 100ms等于要修改的广播间隔，value（0-20）
返回参数 : 无
使用说明 : 
*****************************************************************************/
void bt_modify_adv_interval_req(unsigned char value)
{
	unsigned short length = 0;
  
  	length = set_bt_uart_byte(length,value);
  	
	bt_uart_write_frame(TUYA_BCI_UART_COMMON_MODIFY_ADV_INTERVAL,length);
}
/*****************************************************************************
函数名称 : bt_modify_adv_interval_result
功能描述 :向模块发送修改低功耗下模块广播间隔的结果
输入参数 :result同步结果 0成功，其他失败

返回参数 : 无
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_modify_adv_interval_result(unsigned char result)
{
	//#error "请自行完善该功能,完成后请删除该行"
	if(result == 0x00)
	{
		//成功

	}
	else
	{
		//失败
	}
}
#endif

#ifdef TUYA_BCI_UART_COMMON_TURNOFF_SYSTEM_TIME
/*****************************************************************************
函数名称 : bt_close_timer_req
功能描述 : 向模块发送关闭系统时钟的请求（目前仅适用telink平台）
输入参数 :value 0关闭，1打开
返回参数 : 无
使用说明 : 
*****************************************************************************/
void bt_close_timer_req(unsigned char value)
{
	unsigned short length = 0;
  
  	length = set_bt_uart_byte(length,value);
  	
	bt_uart_write_frame(TUYA_BCI_UART_COMMON_TURNOFF_SYSTEM_TIME,length);
}
/*****************************************************************************
函数名称 : bt_close_timer_result
功能描述 :向模块发送关闭系统时钟的结果
输入参数 :result同步结果 0成功，其他失败

返回参数 : 无
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_close_timer_result(unsigned char result)
{
	//#error "请自行完善该功能,完成后请删除该行"
	if(result == 0x00)
	{
		//成功

	}
	else
	{
		//失败
	}
}
#endif

#ifdef TUYA_BCI_UART_COMMON_ENANBLE_LOWER_POWER
/*****************************************************************************
函数名称 : bt_enable_lowpoer_req
功能描述 : 向模块发送使能低功耗的请求（目前仅适用telink平台）
输入参数 :value 0关闭，1打开
返回参数 : 无
使用说明 :
*****************************************************************************/
void bt_enable_lowpoer_req(unsigned char value)
{
	unsigned short length = 0;
  
  	length = set_bt_uart_byte(length,value);
  	
	bt_uart_write_frame(TUYA_BCI_UART_COMMON_ENANBLE_LOWER_POWER,length);
}
/*****************************************************************************
函数名称 : bt_enable_lowpoer_result
功能描述 :向模块发送使能低功耗的结果
输入参数 :result同步结果 0成功，其他失败

返回参数 : 无
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_enable_lowpoer_result(unsigned char result)
{
	//#error "请自行完善该功能,完成后请删除该行"
	if(result == 0x00)
	{
		//成功

	}
	else
	{
		//失败
	}
}
#endif

#ifdef TUYA_BCI_UART_COMMON_SEND_ONE_TIME_PASSWORD_TOKEN
/*****************************************************************************
函数名称 : bt_send_one_time_password_token
功能描述 : 向模块发送获取一次性动态密码匹配结果的请求
输入参数 :value cmcu端输入的动态密码，len 8
返回参数 : 无
使用说明 :用于锁通用串口对接动态密码功能
*****************************************************************************/
unsigned char bt_send_one_time_password_token(unsigned char value[],unsigned char len)
{
	unsigned short length = 0;
 	if(len!=8)return 0;
 	
  	length = set_bt_uart_buffer(length,value,8);
  	
	bt_uart_write_frame(TUYA_BCI_UART_COMMON_SEND_ONE_TIME_PASSWORD_TOKEN,length);
	
	return 0;
}
/*****************************************************************************
函数名称 : bt_send_one_time_password_token_result
功能描述 :向模块获取一次性动态密码匹配的结果
输入参数 :result同步结果 0x00密码核对通过，0x01密码核对失败

返回参数 : 无
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_send_one_time_password_token_result(unsigned char result)
{
	//#error "请自行完善该功能,完成后请删除该行"
	if(result == 0x00)
	{
		//密码核对通过

	}
	else
	{
		//密码核对失败
	}
}
#endif

#ifdef TUYA_BCI_UART_COMMON_ACTIVE_DISCONNECT
/*****************************************************************************
函数名称 : bt_enable_lowpoer_req
功能描述 : 向模块发送断开蓝牙连接的请求
输入参数 :value 0关闭，1打开
返回参数 : 无
使用说明 :
*****************************************************************************/
void bt_disconnect_req(void)
{
	bt_uart_write_frame(TUYA_BCI_UART_COMMON_ACTIVE_DISCONNECT,0);
}
/*****************************************************************************
函数名称 : bt_enable_lowpoer_result
功能描述 :向模块发送断开蓝牙连接的结果
输入参数 :result结果 0成功，其他失败

返回参数 : 无
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_disconnect_result(unsigned char result)
{
	//#error "请自行完善该功能,完成后请删除该行"
	if(result == 0x00)
	{
		//成功

	}
	else
	{
		//失败
	}
}
#endif

#ifdef TUYA_BCI_UART_COMMON_MCU_SEND_VERSION
/*****************************************************************************
函数名称 : bt_send_mcu_ver
功能描述 :向模块主动发送MCU版本号，主要是为了模块能更及时获取到MCU的版本信息
输入参数 :

返回参数 : 无
使用说明 : MCU可以在串口初始化后调用一次
*****************************************************************************/
void bt_send_mcu_ver(void)
{
	unsigned short length = 0;
	length = set_bt_uart_buffer(length,(unsigned char *)MCU_APP_VER_NUM,3);
	length = set_bt_uart_buffer(length,(unsigned char *)MCU_HARD_VER_NUM,3);
	bt_uart_write_frame(TUYA_BCI_UART_COMMON_MCU_SEND_VERSION,length);
}
#endif
#ifdef TUYA_BCI_UART_COMMON_FACTOR_RESET_NOTIFY
/*****************************************************************************
函数名称 : bt_factor_reset_notify
功能描述 :模块恢复出厂设置后向mcu发送的通知
输入参数 :

返回参数 : 无
使用说明 : MCU可以在此处完成MCU恢复出厂设置的操作
*****************************************************************************/
void bt_factor_reset_notify(void)
{
	//#error "请自行完善该功能,完成后请删除该行"
}
#endif


void soft_reset_mcu(void)
{
	NVIC_SystemReset();
}
	
void go_bootloader_ota(void)
{	
	//goto bootloader
	//
}
		
void tuya_re_config_network(void)
{
	//
	mcu_reset_bt();
}
			
void tuya_reset_module(void)
{
	//
	bt_unbound_req();
}
				
void tuya_retry_ota(void)
{
	//
}
					
void reset_default_parameter(void)
{
	char float_str[64];
	
	//load factory parameter here!
	load_ceiling_setup(0);
	sprintf(float_str, "%s%sload quick_time_times: %.3lf%s\r\n", RTT_CTRL_BG_BRIGHT_BLUE, RTT_CTRL_TEXT_WHITE, upssa0.ppp.quick_time_times, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%s", float_str);
	sprintf(float_str, "%s%sload quick_time_add: %.3lf%s\r\n", RTT_CTRL_BG_BRIGHT_BLUE, RTT_CTRL_TEXT_WHITE, upssa0.ppp.quick_time_add, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%s", float_str);
	sprintf(float_str, "%s%sload quick_freq_times: %.3lf%s\r\n", RTT_CTRL_BG_BRIGHT_BLUE, RTT_CTRL_TEXT_WHITE, upssa0.ppp.quick_freq_times, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%s", float_str);	
	sprintf(float_str, "%s%sload slow_time_times: %.3lf%s\r\n", RTT_CTRL_BG_BRIGHT_BLUE, RTT_CTRL_TEXT_WHITE, upssa0.ppp.slow_time_times, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%s", float_str);
	sprintf(float_str, "%s%sload slow_time_add: %.3lf%s\r\n", RTT_CTRL_BG_BRIGHT_BLUE, RTT_CTRL_TEXT_WHITE, upssa0.ppp.slow_time_add, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%s", float_str);
	sprintf(float_str, "%s%sload slow_freq_times: %.3lf%s\r\n", RTT_CTRL_BG_BRIGHT_BLUE, RTT_CTRL_TEXT_WHITE, upssa0.ppp.slow_freq_times, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%s", float_str);	
	sprintf(float_str, "%s%sload res_times: %.3lf%s\r\n", RTT_CTRL_BG_BRIGHT_BLUE, RTT_CTRL_TEXT_WHITE, upssa0.ppp.res_times, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%s", float_str);	
	sprintf(float_str, "%s%sload offsetmin: %.3lf%s\r\n", RTT_CTRL_BG_BRIGHT_BLUE, RTT_CTRL_TEXT_WHITE, upssa0.ppp.offsetmin, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%s", float_str);	
	
	upssa0.ppp.Light_threshold1 = 0;
	SEGGER_RTT_printf(0, "%s%sload Light_threshold1: %d%s\r\n", RTT_CTRL_BG_BRIGHT_BLUE, RTT_CTRL_TEXT_WHITE, upssa0.ppp.Light_threshold1, RTT_CTRL_RESET);
	
	upssa0.ppp.Light_threshold2 = 0;
	SEGGER_RTT_printf(0, "%s%sload Light_threshold2: %d%s\r\n", RTT_CTRL_BG_BRIGHT_BLUE, RTT_CTRL_TEXT_WHITE, upssa0.ppp.Light_threshold2, RTT_CTRL_RESET);
	
	upssa0.ppp.Light_threshold3 = 4000;
	SEGGER_RTT_printf(0, "%s%sload Light_threshold3: %d%s\r\n", RTT_CTRL_BG_BRIGHT_BLUE, RTT_CTRL_TEXT_WHITE, upssa0.ppp.Light_threshold3, RTT_CTRL_RESET);
	
	upssa0.ppp.Light_threshold4 = 3800;
	SEGGER_RTT_printf(0, "%s%sload Light_threshold4: %d%s\r\n", RTT_CTRL_BG_BRIGHT_BLUE, RTT_CTRL_TEXT_WHITE, upssa0.ppp.Light_threshold4, RTT_CTRL_RESET);
	
	upssa0.ppp.delay_time_num = 32;
	SEGGER_RTT_printf(0, "%s%sload delay_time_num: %ds%s\r\n", RTT_CTRL_BG_BRIGHT_BLUE, RTT_CTRL_TEXT_WHITE, upssa0.ppp.delay_time_num, RTT_CTRL_RESET);
	
	upssa0.ppp.upload_duty = 8000;
	SEGGER_RTT_printf(0, "%s%sload upload_duty: %dms%s\r\n", RTT_CTRL_BG_BRIGHT_BLUE, RTT_CTRL_TEXT_WHITE, upssa0.ppp.upload_duty, RTT_CTRL_RESET);		

	//
}

