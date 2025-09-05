/**
 ****************************************************************************************************
 * @file        usart2.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-23
 * @brief       USART2 ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211023
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __UART5_H
#define __UART5_H

#include "./SYSTEM/sys/sys.h"

/*******************************************************************************************************/
#define UART5_TX_GPIO_PORT              GPIOC
#define UART5_TX_GPIO_PIN               GPIO_PIN_12
#define UART5_TX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* ��������ʱ��ʹ�� */

#define UART5_RX_GPIO_PORT              GPIOD
#define UART5_RX_GPIO_PIN               GPIO_PIN_2
#define UART5_RX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* ��������ʱ��ʹ�� */

#define UART5_UX                        UART5
#define UART5_UX_IRQn                   UART5_IRQn
#define UART5_UX_IRQHandler             UART5_IRQHandler
#define UART5_UX_CLK_ENABLE()           do{ __HAL_RCC_UART5_CLK_ENABLE(); }while(0)  /* USART1 ʱ��ʹ�� */
/*******************************************************************************************************/

#define UART5_REC_LEN               64
#define UART5_EN_RX                 1

extern UART_HandleTypeDef g_uart5_handler;

extern uint8_t g_UART5_rx_buf[UART5_REC_LEN];   /* ���ջ���,���USART2_REC_LEN���ֽ� */
extern uint8_t g_UART5_rx_cnt;                  /* �������ݳ��� */

void uart5_init( uint32_t baudrate);  /* USART2��ʼ�� */
void uart5_send_data(uint8_t *buf, uint8_t len);    /* USART2�������� */
void uart5_receive_data(uint8_t *buf, uint8_t *len);/* USART2�������� */

#endif







