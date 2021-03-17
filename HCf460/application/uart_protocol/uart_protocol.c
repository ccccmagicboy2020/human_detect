#include "uart_protocol.h"

#ifdef SYSTEM_GLOBAL
  #define SYSTEM_EXTERN
#else
  #define SYSTEM_EXTERN   extern
#endif

#ifndef NULL
#define         NULL                ((void *) 0)
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

unsigned char get_queue_total_data(void)
{
  if(queue_in != queue_out)
    return 1;
  else
    return 0;
}

unsigned char Queue_Read_Byte(void)
{
  unsigned char value;
  
  if(queue_out != queue_in)
  {
    //有数据
    if(queue_out >= (unsigned char *)(stream_queue_buf + sizeof(stream_queue_buf)))
    {
      //数据已经到末尾
      queue_out = (unsigned char *)(stream_queue_buf);
    }
    
    value = *queue_out ++;   
  }
  
  return value;
}

unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len)
{
  unsigned short i;
  unsigned char check_sum = 0;
  
  for(i = 0; i < pack_len; i ++)
  {
    check_sum += *pack ++;
  }
  
  return check_sum;
}

void *my_memcpy(void *dest, const void *src, unsigned short count)  
{  
  unsigned char *pdest = (unsigned char *)dest;  
  const unsigned char *psrc  = (const unsigned char *)src;  
  unsigned short i;
  
  if(dest == NULL || src == NULL)
  { 
    return NULL;
  }
  
  if((pdest <= psrc) || (pdest > psrc + count))
  {  
    for(i = 0; i < count; i ++)
    {  
      pdest[i] = psrc[i];  
    }  
  }
  else
  {
    for(i = count; i > 0; i --)
    {  
      pdest[i - 1] = psrc[i - 1];  
    }  
  }  
  
  return dest;  
}

void data_handle(unsigned short offset)
{
  unsigned char cmd_type = stream_uart_rx_buf[offset + FRAME_TYPE];
  
  switch(cmd_type)
  {
  case READ_PARAMETER_CMD:                                 //读参数
    //read some data
    read_data_handle();
    break;
  case WRITE_PARAMETER_CMD:                                //写参数
    //write some data
    write_data_handle();
    break;
  default:
    break;
  }
}

void uart_service(void)
{
  static unsigned short rx_in = 0;
  unsigned short offset = 0;
  unsigned short rx_value_len = 0;             //数据帧长度
  
  while((rx_in < sizeof(stream_uart_rx_buf)) && get_queue_total_data() > 0)
  {
    stream_uart_rx_buf[rx_in ++] = Queue_Read_Byte();
  }
  
  if(rx_in < PROTOCOL_HEAD)
    return;
  
  while((rx_in - offset) >= PROTOCOL_HEAD)
  {
    if(stream_uart_rx_buf[offset + HEAD_FIRST] != FRAME_FIRST)
    {
      offset ++;
      continue;
    }
    
    if(stream_uart_rx_buf[offset + HEAD_SECOND] != FRAME_SECOND)
    {
      offset ++;
      continue;
    }
    
    rx_value_len = stream_uart_rx_buf[offset + LENGTH_HIGH] * 0x100 + stream_uart_rx_buf[offset + LENGTH_LOW] + PROTOCOL_HEAD;
    if(rx_value_len > sizeof(stream_uart_rx_buf) + PROTOCOL_HEAD)
    {
      offset += 2;
      continue;
    }
    
    if((rx_in - offset) < rx_value_len)
    {
      break;
    }
    
    //数据接收完成
    if(get_check_sum((unsigned char *)stream_uart_rx_buf + offset,rx_value_len - 1) != stream_uart_rx_buf[offset + rx_value_len - 1])
    {
      //校验出错
      offset += 2;
      continue;
    }
    
    data_handle(offset);
    offset += rx_value_len;
  }//end while

  rx_in -= offset;
  if(rx_in > 0)
  {
    my_memcpy(stream_uart_rx_buf,stream_uart_rx_buf + offset,rx_in);
  }
}

void read_data_handle(void)
{
  //
}

void write_data_handle(void)
{
  //
}

