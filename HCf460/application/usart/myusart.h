#ifndef __MYUSART_H
#define __MYUSART_H

/* USART channel definition */
#define USART_CH                        (M4_USART3)

/* USART baudrate definition */
#define USART_BAUDRATE                  (115200ul)

/* USART RX Port/Pin definition */
#define USART_RX_PORT                   (PortC)
#define USART_RX_PIN                    (Pin13)
#define USART_RX_FUNC                   (Func_Usart3_Rx)

/* USART TX Port/Pin definition */
#define USART_TX_PORT                   (PortH)
#define USART_TX_PIN                    (Pin02)
#define USART_TX_FUNC                   (Func_Usart3_Tx)

extern void usart_init(void);
extern void UsartRxErrProcess(void);



#endif

