/****************************************Copyright (c)*************************
**                               版权所有 (C), 2015-2020, 涂鸦科技
**
**                                 http://www.tuya.com
**
**--------------文件信息-------------------------------------------------------
**文   件   名: protocol.h
**描        述: 下发/上报数据处理函数
**使 用 说 明 :

                  *******非常重要，一定要看哦！！！********

** 1、用户在此文件中实现数据下发/上报功能
** 2、DP的ID/TYPE及数据处理函数都需要用户按照实际定义实现
** 3、当开始某些宏定义后需要用户实现代码的函数内部有#err提示,完成函数后请删除该#err
**
**--------------历史版本修订---------------------------------------------------
** 版  本: v1.0
** 日　期: 2017年5月3日
** 描　述: 1:创建涂鸦bluetooth对接MCU_SDK
**
**-----------------------------------------------------------------------------
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
#ifndef __PROTOCOL_H_
#define __PROTOCOL_H_


#include "sys.h"
#include "bluetooth.h"
/******************************************************************************
                            用户相关信息配置
******************************************************************************/
/******************************************************************************
                            1:修改产品信息                
******************************************************************************/
#define PRODUCT_KEY "ve6mrgpb"    //开发平台创建产品后生成的16位字符产品唯一标识



#define MCU_VER "1.0.0"                                 //用户的软件版本,用于MCU固件升级,MCU升级版本需修改
#define MCU_APP_VER_NUM      (0x010000UL)				//用户的软件版本,用于MCU固件升级,MCU升级版本需修改     				1.0.0
#define MCU_HARD_VER_NUM     (0x010000UL)					//用户的硬件版本,当前没有实际用处


/******************************************************************************
                          2:MCU是否需要支固件升级                  
如需要支持MCU固件升级,请开启该宏
MCU可调用mcu_api.c文件内的mcu_firm_update_query()函数获取当前MCU固件更新情况
                        ********WARNING!!!**********
当前接收缓冲区为关闭固件更新功能的大小,固件升级包为256字节
如需要开启该功能,串口接收缓冲区会变大
******************************************************************************/
#define         SUPPORT_MCU_FIRM_UPDATE                 //开启MCU固件升级功能(默认开启)
/******************************************************************************
                         3:定义收发缓存:
                    如当前使用MCU的RAM不够,可修改为24
******************************************************************************/
#ifndef SUPPORT_MCU_FIRM_UPDATE
#define BT_UART_QUEUE_LMT             512              //数据接收队列大小,如MCU的RAM不够,可缩小
#define BT_UART_RECV_BUF_LMT          512              //根据用户DP数据大小量定,必须大于32
#else
#define BT_UART_QUEUE_LMT             512             //数据接收队列大小,如MCU的RAM不够,可缩小
#define BT_UART_RECV_BUF_LMT          256             //固件升级缓冲区,需大缓存,必须大于260
#endif

#define BT_UART_SEND_BUF_LMT         512              //根据用户DP数据大小量定,必须大于32
/******************************************************************************
                        4:定义模块工作方式
模块自处理:
          bt指示灯和bt复位按钮接在bt模块上(开启BT_CONTROL_SELF_MODE宏)
          并正确定义BT_STATE_KEY和BT_RESET_KEY
MCU自处理:
          bt指示灯和bt复位按钮接在MCU上(关闭BT_CONTROL_SELF_MODE宏)
          MCU在需要处理复位bt的地方调用mcu_api.c文件内的mcu_reset_bt()函数,并可调用mcu_get_reset_bt_flag()函数返回复位bt结果
          或调用设置bt模式mcu_api.c文件内的mcu_set_bt_mode(BT_CONFIG_E mode)函数,并可调用mcu_get_bt_work_state()函数返回设置bt结果
******************************************************************************/
//#define         BT_CONTROL_SELF_MODE                       //bt自处理按键及LED指示灯;如为MCU外界按键/LED指示灯请关闭该宏
#ifdef          BT_CONTROL_SELF_MODE                      //模块自处理
  #define     BT_STATE_KEY            14                    //bt模块状态指示按键，请根据实际GPIO管脚设置
  #define     BT_RESERT_KEY           0                     //bt模块重置按键，请根据实际GPIO管脚设置
#endif

/******************************************************************************
                      5:MCU是否需要支持校时功能                     
如需要请开启该宏,并在Protocol.c文件内mcu_write_rtctime实现代码
mcu_write_rtctime内部有#err提示,完成函数后请删除该#err
mcu在bt模块正确联网后可调用mcu_get_system_time()函数发起校时功能
******************************************************************************/
#define         SUPPORT_MCU_RTC_CHECK                //开启校时功能

/******************************************************************************
                        1:dp数据点序列号重新定义
          **此为自动生成代码,如在开发平台有相关修改请重新下载MCU_SDK**         
******************************************************************************/
//感应延时(可下发可上报)
//备注:
#define DPID_PIR_DELAY 102
//加载预设参数(可下发可上报)
//备注:
#define DPID_LOAD_RADAR_PARAMETER 105
//灯状态(只上报)
//备注:上报APP关于灯的实时状态
#define DPID_LIGHT_STATUS 118
//人状态(只上报)
//备注:感应到的人的状态
#define DPID_PERSON_IN_RANGE 119
//找我(可下发可上报)
//备注:这个是找灯的功能
#define DPID_FIND_ME 122
//人次计数(只上报)
//备注:
#define DPID_PERSON_METER 124
//检测状态(只上报)
//备注:向APP上报当前的工作新状态
#define DPID_CHECK_PROCESS 140
//慢检测结果(只上报)
//备注:大动作，微动，呼吸，没人等
#define DPID_SLOW_CHECK_RESULT 141
//时域门限0(可下发可上报)
//备注:时域乘法限
#define DPID_TIME_TIMES 143
//时域门限0实时值(只上报)
//备注:时域乘限实时值
#define DPID_TIME_TIMES_RT 144
//时域门限1(可下发可上报)
//备注:时域加法限
#define DPID_TIME_ADD 145
//时域门限1实时值(只上报)
//备注:时域加法限
#define DPID_TIME_ADD_RT 146
//检测模式(可下发可上报)
//备注:控制工作模式
#define DPID_WORK_MODE 147
//频域门限0(可下发可上报)
//备注:频域乘法限
#define DPID_FREQ_TIMES 148
//频域门限0实时值(只上报)
//备注:频域乘限实时值
#define DPID_FREQ_TIMES_RT 149
//工厂操作(可下发可上报)
//备注:枚举范围：aa,bb,cc,dd,ee,ff,gg,hh
#define DPID_FACTORY_OP 150
//频域门限1(可下发可上报)
//备注:呼吸门限
#define DPID_FREQ_PARAMETER1 160
//频域门限1实时值(只上报)
//备注:呼吸门限实时值
#define DPID_FREQ_PARAMETER1_RT 161
//频域门限2(可下发可上报)
//备注:
#define DPID_FREQ_PARAMETER2 162
//频域门限2实时值(只上报)
//备注:
#define DPID_FREQ_PARAMETER2_RT 163
//光敏值(只上报)
//备注:光敏值
#define DPID_LIGHT_SENSOR_RAW 164
//光敏门限1(可下发可上报)
//备注:
#define DPID_LIGHT_THRESHOLD1 165
//光敏门限2(可下发可上报)
//备注:
#define DPID_LIGHT_THRESHOLD2 166
//光敏门限3(可下发可上报)
//备注:
#define DPID_LIGHT_THRESHOLD3 167
//光敏门限4(可下发可上报)
//备注:
#define DPID_LIGHT_THRESHOLD4 168
//一般命令(只下发)
//备注:
#define DPID_COMMON_COMMAND 169
//呼吸频率(只上报)
//备注:
#define DPID_BREATHE_FREQ 170
//学习命令(可下发可上报)
//备注:设置不同的学习策略
#define DPID_STUDY_CMD 171
//学习进度上报(只上报)
//备注:学习状态上报
#define DPID_STUDY_PROCESS_UPLOAD 172
//CCCC ADD HERE!
#define DPID_UPLOAD_DUTY	175



/*****************************************************************************
函数名称 : all_data_update
功能描述 : 系统所有dp点信息上传
输入参数 : 无
返回参数 : 无
使用说明 : MCU必须实现该函数内数据上报功能
*****************************************************************************/
void all_data_update(void);



/*****************************************************************************
函数名称 : dp_download_handle
功能描述 : dp下发处理函数
输入参数 : dpid:DP序号
value:dp数据缓冲区地址
length:dp数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERRO
使用说明 : 该函数用户不能修改
*****************************************************************************/
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length);
/*****************************************************************************
函数名称 : get_download_cmd_total
功能描述 : 获取所有dp命令总和
输入参数 : 无
返回参数 : 下发命令总和
使用说明 : 该函数用户不能修改
*****************************************************************************/
unsigned char get_download_cmd_total(void);


//////////////////////////////////当前MCU SDK版本较上一版本新增支持协议接口////////////////////
/*****************************************************************************
函数名称 :  bt_unbound_req
功能描述 : 向模块发送解绑请求，模块收到该指令后会解除蓝牙绑定关系
输入参数 : 无
返回参数 : 无
使用说明 : MCU主动解绑调用
*****************************************************************************/
void bt_unbound_req(void);
/*****************************************************************************
函数名称 :  bt_unbound_req
功能描述 : 向模块发射频测试请求
输入参数 : 无
返回参数 : 无
使用说明 : 
*****************************************************************************/
void bt_rf_test_req(void);
/*****************************************************************************
函数名称 : bt_rf_test_result
功能描述 : 蓝牙rf测试反馈
输入参数 : result:蓝牙rf测试结果;0:失败/1:成功
           rssi:测试成功表示蓝牙信号强度/测试失败值无意义
返回参数 : 无
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_rf_test_result(unsigned char result,signed char rssi);
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
void bt_send_recordable_dp_data(unsigned char snedType,unsigned char dpid,unsigned char dpType, unsigned char value[],unsigned short len);
/*****************************************************************************
函数名称 : bt_send_recordable_dp_data_result
功能描述 : 记录型数据上报
输入参数 : result ：0存储成功，1存储失败
返回参数 : 无
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_send_recordable_dp_data_result(unsigned char result);
/*****************************************************************************
函数名称 : bt_modify_adv_interval_req
功能描述 : 向模块发送修改低功耗下模块广播间隔的请求
输入参数 :value * 100ms等于要修改的广播间隔，value（0-20）
返回参数 : 无
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_modify_adv_interval_req(unsigned char value);
/*****************************************************************************
函数名称 : bt_modify_adv_interval_result
功能描述 :向模块发送修改低功耗下模块广播间隔的结果
输入参数 :result同步结果 0成功，其他失败

返回参数 : 无
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_modify_adv_interval_result(unsigned char result);
/*****************************************************************************
函数名称 : bt_close_timer_req
功能描述 : 向模块发送关闭系统时钟的请求（目前仅适用telink平台）
输入参数 :value 0关闭，1打开
返回参数 : 无
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_close_timer_req(unsigned char value);
/*****************************************************************************
函数名称 : bt_close_timer_result
功能描述 :向模块发送关闭系统时钟的结果
输入参数 :result同步结果 0成功，其他失败

返回参数 : 无
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_close_timer_result(unsigned char result);
/*****************************************************************************
函数名称 : bt_enable_lowpoer_req
功能描述 : 向模块发送使能低功耗的请求（目前仅适用telink平台）
输入参数 :value 0关闭，1打开
返回参数 : 无
使用说明 :
*****************************************************************************/
void bt_enable_lowpoer_req(unsigned char value);
/*****************************************************************************
函数名称 : bt_enable_lowpoer_result
功能描述 :向模块发送使能低功耗的结果
输入参数 :result同步结果 0成功，其他失败

返回参数 : 无
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_enable_lowpoer_result(unsigned char result);
/*****************************************************************************
函数名称 : bt_send_one_time_password_token
功能描述 : 向模块发送获取一次性动态密码匹配结果的请求
输入参数 :value cmcu端输入的动态密码，len 8
返回参数 : 无
使用说明 :用于锁通用串口对接动态密码功能
*****************************************************************************/
unsigned char bt_send_one_time_password_token(unsigned char value[],unsigned char len);
/*****************************************************************************
函数名称 : bt_send_one_time_password_token_result
功能描述 :向模块获取一次性动态密码匹配的结果
输入参数 :result同步结果 0x00密码核对通过，0x01密码核对失败

返回参数 : 无
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_send_one_time_password_token_result(unsigned char result);
/*****************************************************************************
函数名称 : bt_enable_lowpoer_req
功能描述 : 向模块发送断开蓝牙连接的请求
输入参数 :value 0关闭，1打开
返回参数 : 无
使用说明 :
*****************************************************************************/
void bt_disconnect_req(void);
/*****************************************************************************
函数名称 : bt_enable_lowpoer_result
功能描述 :向模块发送断开蓝牙连接的结果
输入参数 :result结果 0成功，其他失败

返回参数 : 无
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_disconnect_result(unsigned char result);
/*****************************************************************************
函数名称 : bt_factor_reset_notify
功能描述 :模块恢复出厂设置后向mcu发送的通知
输入参数 :

返回参数 : 无
使用说明 : MCU可以在此处完成MCU恢复出厂设置的操作
*****************************************************************************/
void bt_factor_reset_notify(void);

void clear_buffer(void);
void Delayms(unsigned short ms);

void soft_reset_mcu(void);
void go_bootloader_ota(void);
void tuya_re_config_network(void);
void tuya_reset_module(void);
void tuya_retry_ota(void);
void reset_default_parameter(void);



#endif

