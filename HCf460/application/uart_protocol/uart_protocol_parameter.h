#ifndef __UART_PROTOCOL_PARAMETER_H_
#define __UART_PROTOCOL_PARAMETER_H_

#ifdef SYSTEM_GLOBAL
  #define SYSTEM_EXTERN
#else
  #define SYSTEM_EXTERN   extern
#endif

#define         HEAD_FIRST                      0x00
#define         HEAD_SECOND                     0x01        
#define         FRAME_TYPE                      0x02
#define         LENGTH_HIGH                     0x03
#define         LENGTH_LOW                      0x04
#define         DATA_START                      0x05

#define         READ_PARAMETER_CMD              0x00
#define         WRITE_PARAMETER_CMD             0x01

#define         PROTOCOL_HEAD           		0x06
#define         FRAME_FIRST             		0x55
#define         FRAME_SECOND            		0xAA

#define			STREAM_UART_QUEUE_LMT           24
#define			STREAM_UART_RECV_BUF_LMT        24
#define			STREAM_UART_SEND_BUF_LMT        24

#endif


