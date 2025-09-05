/**
 */

#ifndef __USART3_H
#define __USART3_H

#include "./SYSTEM/sys/sys.h"

/*******************************************************************************************************/
#define USART3_TX_GPIO_PORT              GPIOB
#define USART3_TX_GPIO_PIN               GPIO_PIN_10
#define USART3_TX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* 发送引脚时钟使能 */

#define USART3_RX_GPIO_PORT              GPIOB
#define USART3_RX_GPIO_PIN               GPIO_PIN_11
#define USART3_RX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* 接收引脚时钟使能 */

#define USART3_UX                        USART3
#define USART3_UX_IRQn                   USART3_IRQn
#define USART3_UX_IRQHandler             USART3_IRQHandler
#define USART3_UX_CLK_ENABLE()           do{ __HAL_RCC_USART3_CLK_ENABLE(); }while(0)  /* USART3 时钟使能 */
/*******************************************************************************************************/


#define USART3_REC_LEN               32
#define USART3_EN_RX                 1

extern UART_HandleTypeDef g_usart3_handler;

extern uint8_t g_USART3_rx_buf[USART3_REC_LEN];   /* 接收缓冲,最大RS485_REC_LEN个字节 */
extern uint8_t g_USART3_rx_cnt;                  /* 接收数据长度 */

void usart3_init( uint32_t baudrate);  /* RS485初始化 */
void usart3_send_data(uint8_t *buf, uint8_t len);    /* RS485发送数据 */
void usart3_receive_data(uint8_t *buf, uint8_t *len);/* RS485接收数据 */
                      
#endif







