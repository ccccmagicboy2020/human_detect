#ifndef __UART_PROTOCOL_H_
#define __UART_PROTOCOL_H_

#include "uart_protocol_parameter.h"

void uart_protocol_init(void);
void uart_service(void);
unsigned char get_queue_total_data(void);
unsigned char Queue_Read_Byte(void);
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len);
void *my_memcpy(void *dest, const void *src, unsigned short count);
void data_handle(unsigned short offset);

#endif
