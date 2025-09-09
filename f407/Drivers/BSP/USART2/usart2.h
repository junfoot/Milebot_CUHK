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

#ifndef __USART2_H
#define __USART2_H

#include "./SYSTEM/sys/sys.h"

extern UART_HandleTypeDef g_usart2_handler;

/******************************************************************************************/
/* USART2 ���� �� ���� ���� 
 * Ĭ�������USART22��.
 * ע��: ͨ���޸���10���궨��, ����֧��UART1~UART7����һ������.
 */
#define USART2_RE_GPIO_PORT                  GPIOG
#define USART2_RE_GPIO_PIN                   GPIO_PIN_8
#define USART2_RE_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   /* PD��ʱ��ʹ�� */

#define USART2_TX_GPIO_PORT                  GPIOD
#define USART2_TX_GPIO_PIN                   GPIO_PIN_5
#define USART2_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

#define USART2_RX_GPIO_PORT                  GPIOD
#define USART2_RX_GPIO_PIN                   GPIO_PIN_6
#define USART2_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

#define USART2_UX                            USART2
#define USART2_UX_IRQn                       USART2_IRQn
#define USART2_UX_IRQHandler                 USART2_IRQHandler
#define USART2_UX_CLK_ENABLE()               do{ __HAL_RCC_USART2_CLK_ENABLE(); }while(0)  /* USART2 ʱ��ʹ�� */

/******************************************************************************************/

/* ����USART2_RE��, ����USART2����/����״̬
 * USART2_RE = 0, �������ģʽ
 * USART2_RE = 1, ���뷢��ģʽ
 */
//#define USART2_RE(x)   do{ x ? \
//                          HAL_GPIO_WritePin(USART2_RE_GPIO_PORT, USART2_RE_GPIO_PIN, GPIO_PIN_SET) : \
//                          HAL_GPIO_WritePin(USART2_RE_GPIO_PORT, USART2_RE_GPIO_PIN, GPIO_PIN_RESET); \
//                      }while(0)


#define USART2_REC_LEN               128          /* ������Ҫ�޸� num * 4 + 4 */

#define USART2_EN_RX                 1           /* ʹ�ܣ�1��/��ֹ��0��USART2���� */


extern uint8_t g_USART2_rx_buf[USART2_REC_LEN];   /* ���ջ���,���USART2_REC_LEN���ֽ� */
extern uint8_t g_USART2_rx_cnt;                  /* �������ݳ��� */


void usart2_init( uint32_t baudrate);  /* USART2��ʼ�� */
void usart2_send_data(uint8_t *buf, uint8_t len);    /* USART2�������� */
void usart2_receive_data(uint8_t *buf, uint8_t *len);/* USART2�������� */

#endif







