/**
* @file  protocol.h
* @brief declaration of fuction in  protocol.c
* @author luchao
* @date 2020.03.13
* @par email:
* luchao@tuya.com
* @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
* @par company
* http://www.tuya.com
*/
#ifndef __PROTOCOL_H_
#define __PROTOCOL_H_

#ifdef __cplusplus
extern "C"
{
#endif

///< product INFORMATION

#define PRODUCT_KEY "kp5q4tff"    //开发平台创建产品后生成的16位字符产品唯一标识


///< mcu version 
#define MCU_VER "1.0.0"                          
 
#define ZIGBEE_UART_QUEUE_LMT             256             // using to save data received from uart
#define ZIGBEE_UART_RECV_BUF_LMT          128             //
#define ZIGBEE_UART_SEND_BUF_LMT          128             //


typedef enum
{
    MCU_TYPE_DC_POWER = 1,
    MCU_TYPE_LOWER_POWER,
    MCU_TYPE_SCENE
}MCU_TYPE_E;

/**
 * if mcu need to support the time function, this macro definition should be opened
 * and need complete mcu_write_rtctime function 
 * 
 */
 
//#define    SUPPORT_MCU_RTC_CHECK             //start time calibration

/**
 * if mcu need to support OTA, this macro definition should be opened
 */
//#define    SUPPORT_MCU_OTA                  //support mcu ota


/**
 * if mcu need to support mcu type checking, this macro definition should be opened
 * 
 */
//#define    CHECK_MCU_TYPE               //support mcu type check 


/**
 * if mcu need to support zigbee network parameter setting, this macro definition should be opened
 * 
 */
//#define  SET_ZIGBEE_NWK_PARAMETER        //support zigbee nwk parameter setting 


/**
 * if mcu need to send a broadcast data, this macro definition should be opened
 * 
 */
//#define  BROADCAST_DATA_SEND           //support broadcast data sending



/**
 * DP data list,this code will be generate by cloud platforms
 */

//感应延时(可下发可上报)
//备注:
#define DPID_PIR_DELAY 102
//覆盖范围(可下发可上报)
//备注:枚举范围：aa,bb,cc,dd,ee,ff,gg,hh,ii
//
#define DPID_LOAD_RADAR_PARAMETER 105
//灯状态(只上报)
//备注:枚举范围：aa,bb,cc
//上报APP关于灯的实时状态
#define DPID_LIGHT_STATUS 118
//有无人状态(只上报)
//备注:枚举范围：aa,bb,cc
//感应到的人的状态
#define DPID_PERSON_IN_RANGE 119
//设备查找(可下发可上报)
//备注:这个是找灯的功能
#define DPID_FIND_ME 122
//人次计数(只上报)
//备注:
#define DPID_PERSON_METER 124
//检测状态(只上报)
//备注:枚举范围：aa,bb,cc,dd
//向APP上报当前的工作新状态
#define DPID_CHECK_PROCESS 140
//人动静状态(只上报)
//备注:枚举范围：aa,bb,cc,dd,ee,ff
//大动作，微动，呼吸，没人等
#define DPID_SLOW_CHECK_RESULT 141
//时域门限0(可下发可上报)
//备注:时域乘法限
#define DPID_TIME_TIMES 143
//时域门限0触发值(只上报)
//备注:时域乘限实时值
#define DPID_TIME_TIMES_RT 144
//时域门限1(可下发可上报)
//备注:时域加法限
#define DPID_TIME_ADD 145
//时域门限1触发值(只上报)
//备注:时域加法限
#define DPID_TIME_ADD_RT 146
//检测模式(可下发可上报)
//备注:枚举范围：aa,bb,cc,dd
//控制工作模式
#define DPID_WORK_MODE 147
//频域门限0(可下发可上报)
//备注:频域乘法限
#define DPID_FREQ_TIMES 148
//频域门限0触发值(只上报)
//备注:频域乘限实时值
#define DPID_FREQ_TIMES_RT 149
//工厂操作(可下发可上报)
//备注:枚举范围：aa,bb,cc,dd,ee,ff,gg,hh
#define DPID_FACTORY_OP 150
//频域门限1(可下发可上报)
//备注:呼吸门限
#define DPID_FREQ_PARAMETER1 160
//频域门限1触发值(只上报)
//备注:呼吸门限实时值
#define DPID_FREQ_PARAMETER1_RT 161
//频域门限2(可下发可上报)
//备注:
#define DPID_FREQ_PARAMETER2 162
//频域门限2触发值(只上报)
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
//备注:枚举范围：aa,bb,cc,dd,ee,ff
#define DPID_COMMON_COMMAND 169
//呼吸频率(只上报)
//备注:
#define DPID_BREATHE_FREQ 170
//背景探测(可下发可上报)
//备注:枚举范围：aa,bb,cc,dd
//设置不同的学习策略
#define DPID_STUDY_CMD 171
//背景探测进度上报(只上报)
//备注:枚举范围：aa,bb
//学习状态上报
#define DPID_STUDY_PROCESS_UPLOAD 172
//心跳频率(只上报)
//备注:
#define DPID_HEART_FREQ 173
//呼吸幅度(只上报)
//备注:
#define DPID_BREATHE_SCALE 174
//通迅周期(可下发可上报)
//备注:
#define DPID_UPLOAD_DUTY 175



/**
* @brief encapsulates a generic send function, developer should use their own function to completing this fuction 
* @param[in] {value} send signle data 
* @return  void
*/
void uart_transmit_output(unsigned char value);

/**
* @brief Upload all dp information of the system, and realize the synchronization of APP and muc data
* @param[in] {void}
* @return  void
*/
void all_data_update(void);

/**
* @brief mcu check local RTC time 
* @param[in] {time} timestamp
* @return  void
*/
void mcu_write_rtctime(unsigned char time[]);

/**
* @brief Zigbee functional test feedback
* @param[in] {void} 
* @return  void
*/
void zigbee_test_result(void);

/**
* @brief this function will handle uart received frame data  
* @param[in] {dpid}   dp id
* @param[in] {value}  dp data 
* @param[in] {length} lenght of dp data 
* @return  handle result 
*/
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length);

/**
* @brief get received cmd total number
* @param[in] {void}   
* @return  received cmd total number
*/
unsigned char get_download_cmd_total(void);

/**
* @brief received zigbee net_work state handle 
* @param[in] {zigbee_work_state}  zigbee current network state
* @return  void 
*/
void zigbee_work_state_event(unsigned char zigbee_work_state);
/**
* @brief received reset zigbee response 
* @param[in] {state} response state 
* @return  void 
*/
void mcu_reset_zigbee_event(unsigned char state);

/**
* @brief check mcu version response
* @param[in] {void}
* @return  void 
*/
void response_mcu_ota_version_event(void);


#ifdef SUPPORT_MCU_OTA 
/**
* @brief mcu ota update notify response
* @param[in] {offset} offset of file 
* @return  void 
*/
void response_mcu_ota_notify_event(unsigned char offset);
/**
* @brief received mcu ota data request response
* @param[in] {fw_offset}  offset of file 
* @param[in] {data}  received data  
* @return  void 
*/
void reveived_mcu_ota_data_handle(unsigned int fw_offset,char *data);

/**
* @brief mcu send ota data request 
* @param[in] {offset} offset of file 
* @return  void 
*/
void mcu_ota_fw_request_event(unsigned char offset);

/**
* @brief mcu ota data result notify
* @param[in] {offset} offset of file 
* @return  void 
*/
void mcu_ota_result_event(unsigned char offset);


/**
* @brief mcu ota data handle 
* @param[in] {fw_offset} frame offset 
* @param[in] {data} received data
* @return  void 
*/
void ota_fw_data_handle(unsigned int fw_offset,char *data);
#endif

#ifdef __cplusplus
}
#endif
#endif
