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

#ifndef __USART2_H
#define __USART2_H

#include "./SYSTEM/sys/sys.h"

extern UART_HandleTypeDef g_usart2_handler;

/******************************************************************************************/
/* USART2 引脚 和 串口 定义 
 * 默认是针对USART22的.
 * 注意: 通过修改这10个宏定义, 可以支持UART1~UART7任意一个串口.
 */
#define USART2_RE_GPIO_PORT                  GPIOG
#define USART2_RE_GPIO_PIN                   GPIO_PIN_8
#define USART2_RE_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   /* PD口时钟使能 */

#define USART2_TX_GPIO_PORT                  GPIOD
#define USART2_TX_GPIO_PIN                   GPIO_PIN_5
#define USART2_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define USART2_RX_GPIO_PORT                  GPIOD
#define USART2_RX_GPIO_PIN                   GPIO_PIN_6
#define USART2_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define USART2_UX                            USART2
#define USART2_UX_IRQn                       USART2_IRQn
#define USART2_UX_IRQHandler                 USART2_IRQHandler
#define USART2_UX_CLK_ENABLE()               do{ __HAL_RCC_USART2_CLK_ENABLE(); }while(0)  /* USART2 时钟使能 */

/******************************************************************************************/

/* 控制USART2_RE脚, 控制USART2发送/接收状态
 * USART2_RE = 0, 进入接收模式
 * USART2_RE = 1, 进入发送模式
 */
//#define USART2_RE(x)   do{ x ? \
//                          HAL_GPIO_WritePin(USART2_RE_GPIO_PORT, USART2_RE_GPIO_PIN, GPIO_PIN_SET) : \
//                          HAL_GPIO_WritePin(USART2_RE_GPIO_PORT, USART2_RE_GPIO_PIN, GPIO_PIN_RESET); \
//                      }while(0)


#define USART2_REC_LEN               128          /* 根据需要修改 num * 4 + 4 */

#define USART2_EN_RX                 1           /* 使能（1）/禁止（0）USART2接收 */


extern uint8_t g_USART2_rx_buf[USART2_REC_LEN];   /* 接收缓冲,最大USART2_REC_LEN个字节 */
extern uint8_t g_USART2_rx_cnt;                  /* 接收数据长度 */


void usart2_init( uint32_t baudrate);  /* USART2初始化 */
void usart2_send_data(uint8_t *buf, uint8_t len);    /* USART2发送数据 */
void usart2_receive_data(uint8_t *buf, uint8_t *len);/* USART2接收数据 */

#endif







