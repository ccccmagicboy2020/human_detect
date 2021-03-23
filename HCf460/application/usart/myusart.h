#ifndef __MYUSART_H
#define __MYUSART_H

/* USART channel definition */
#define USART_CH                        (M4_USART3)
#define USART_TUYA_CH                   (M4_USART1)

/* USART baudrate definition */
#define USART_BAUDRATE                  (115200ul)

/* USART RX Port/Pin definition */
#define USART_RX_PORT                   (PortC)
#define USART_RX_PIN                    (Pin13)
#define USART_RX_FUNC                   (Func_Usart3_Rx)

#define USART_TUYA_RX_PORT                   (PortA)
#define USART_TUYA_RX_PIN                    (Pin03)
#define USART_TUYA_RX_FUNC                   (Func_Usart1_Rx)
#define USART_TUYA_RI_NUM                        (INT_USART1_RI)

/* USART TX Port/Pin definition */
#define USART_TX_PORT                   (PortH)
#define USART_TX_PIN                    (Pin02)
#define USART_TX_FUNC                   (Func_Usart3_Tx)

#define USART_TUYA_TX_PORT                   (PortA)
#define USART_TUYA_TX_PIN                    (Pin02)
#define USART_TUYA_TX_FUNC                   (Func_Usart1_Tx)

extern void usart_init(void);
extern void UsartRxErrProcess(void);



#endif

