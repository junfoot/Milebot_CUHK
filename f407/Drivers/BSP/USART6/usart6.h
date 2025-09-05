/**
 ****************************************************************************************************
 * @file        usart2.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-23
 * @brief       USART2 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211023
 * 第一次发布
 *
 ****************************************************************************************************
 */

#ifndef __USART6_H
#define __USART6_H

#include "./SYSTEM/sys/sys.h"

/*******************************************************************************************************/
#define USART6_TX_GPIO_PORT              GPIOC
#define USART6_TX_GPIO_PIN               GPIO_PIN_6
#define USART6_TX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* 发送引脚时钟使能 */

#define USART6_RX_GPIO_PORT              GPIOC
#define USART6_RX_GPIO_PIN               GPIO_PIN_7
#define USART6_RX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* 接收引脚时钟使能 */

#define USART6_UX                        USART6
#define USART6_UX_IRQn                   USART6_IRQn
#define USART6_UX_IRQHandler             USART6_IRQHandler
#define USART6_UX_CLK_ENABLE()           do{ __HAL_RCC_USART6_CLK_ENABLE(); }while(0)  /* USART1 时钟使能 */
/*******************************************************************************************************/

#define USART6_REC_LEN               64                      
#define USART6_EN_RX                 1

extern UART_HandleTypeDef g_usart6_handler;

extern uint8_t g_USART6_rx_buf[USART6_REC_LEN];   /* 接收缓冲,最大USART2_REC_LEN个字节 */
extern uint8_t g_USART6_rx_cnt;                  /* 接收数据长度 */

void usart6_init( uint32_t baudrate);  /* USART2初始化 */
void usart6_send_data(uint8_t *buf, uint8_t len);    /* USART2发送数据 */
void usart6_receive_data(uint8_t *buf, uint8_t *len);/* USART2接收数据 */
                      
#endif







