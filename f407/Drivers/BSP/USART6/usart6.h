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

#ifndef __USART6_H
#define __USART6_H

#include "./SYSTEM/sys/sys.h"

/*******************************************************************************************************/
#define USART6_TX_GPIO_PORT              GPIOC
#define USART6_TX_GPIO_PIN               GPIO_PIN_6
#define USART6_TX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* ��������ʱ��ʹ�� */

#define USART6_RX_GPIO_PORT              GPIOC
#define USART6_RX_GPIO_PIN               GPIO_PIN_7
#define USART6_RX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* ��������ʱ��ʹ�� */

#define USART6_UX                        USART6
#define USART6_UX_IRQn                   USART6_IRQn
#define USART6_UX_IRQHandler             USART6_IRQHandler
#define USART6_UX_CLK_ENABLE()           do{ __HAL_RCC_USART6_CLK_ENABLE(); }while(0)  /* USART1 ʱ��ʹ�� */
/*******************************************************************************************************/

#define USART6_REC_LEN               64                      
#define USART6_EN_RX                 1

extern UART_HandleTypeDef g_usart6_handler;

extern uint8_t g_USART6_rx_buf[USART6_REC_LEN];   /* ���ջ���,���USART2_REC_LEN���ֽ� */
extern uint8_t g_USART6_rx_cnt;                  /* �������ݳ��� */

void usart6_init( uint32_t baudrate);  /* USART2��ʼ�� */
void usart6_send_data(uint8_t *buf, uint8_t len);    /* USART2�������� */
void usart6_receive_data(uint8_t *buf, uint8_t *len);/* USART2�������� */
                      
#endif







