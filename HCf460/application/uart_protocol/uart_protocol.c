#include "uart_protocol.h"

#ifdef SYSTEM_GLOBAL
  #define SYSTEM_EXTERN
#else
  #define SYSTEM_EXTERN   extern
#endif

SYSTEM_EXTERN unsigned char  volatile  stream_queue_buf[PROTOCOL_HEAD + STREAM_UART_QUEUE_LMT];
SYSTEM_EXTERN unsigned char  stream_uart_rx_buf[PROTOCOL_HEAD + STREAM_UART_RECV_BUF_LMT];
SYSTEM_EXTERN unsigned char  stream_uart_tx_buf[PROTOCOL_HEAD + STREAM_UART_SEND_BUF_LMT];
//
SYSTEM_EXTERN volatile unsigned char *queue_in;
SYSTEM_EXTERN volatile unsigned char *queue_out;

void uart_protocol_init(void)
{
  queue_in = (unsigned char *)stream_queue_buf;
  queue_out = (unsigned char *)stream_queue_buf;
}

void uart_service(void)
{
	//
}


