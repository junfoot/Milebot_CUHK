/**
 ****************************************************************************************************
 * @file        usart.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       ���ڳ�ʼ������(һ���Ǵ���1)��֧��printf
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
 * V1.0 20211014
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"

#include "./USMART/usmart.h"
#include "./USMART/usmart_port.h"

#include "./BSP/DMA/dma.h"

#include "string.h"
#include <stdio.h>

extern uint8_t mode;

extern float tor_origin[4];
extern short tor_o[4];

extern uint8_t pc_o[20];

/* ���ʹ��os,����������ͷ�ļ����� */
//#if SYS_SUPPORT_OS
//#include "includes.h"                               /* os ʹ�� */
//#endif

/******************************************************************************************/
/* �������´���, ֧��printf����, ������Ҫѡ��use MicroLIB */

#if 1
#if (__ARMCC_VERSION >= 6010050)                    /* ʹ��AC6������ʱ */
__asm(".global __use_no_semihosting\n\t");          /* ������ʹ�ð�����ģʽ */
__asm(".global __ARM_use_no_argv \n\t");            /* AC6����Ҫ����main����Ϊ�޲�����ʽ�����򲿷����̿��ܳ��ְ�����ģʽ */

#else
/* ʹ��AC5������ʱ, Ҫ�����ﶨ��__FILE �� ��ʹ�ð�����ģʽ */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* ��ʹ�ð�����ģʽ��������Ҫ�ض���_ttywrch\_sys_exit\_sys_command_string����,��ͬʱ����AC6��AC5ģʽ */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* ����_sys_exit()�Ա���ʹ�ð�����ģʽ */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}

/* FILE �� stdio.h���涨��. */
FILE __stdout;

/* �ض���fputc����, printf�������ջ�ͨ������fputc����ַ��������� */
int fputc(int ch, FILE *f)
{
    while ((USART1->SR & 0X40) == 0);               /* �ȴ���һ���ַ�������� */

    USART1->DR = (uint8_t)ch;                       /* ��Ҫ���͵��ַ� ch д�뵽DR�Ĵ��� */
    return ch;
}
#endif
/***********************************************END*******************************************/
    
#if USART_EN_RX                                     /* ���ʹ���˽��� */

/* ���ջ���, ���USART_REC_LEN���ֽ�. */
uint8_t g_usart_rx_buf[USART_REC_LEN];

/*  ����״̬
 *  bit15��      ������ɱ�־
 *  bit14��      ���յ�0x0d
 *  bit13~0��    ���յ�����Ч�ֽ���Ŀ
*/
uint16_t g_usart_rx_sta = 0;

uint8_t g_rx_buffer[RXBUFFERSIZE];                  /* HAL��ʹ�õĴ��ڽ��ջ��� */

UART_HandleTypeDef g_usart1_handler;                  /* UART��� */


/**
 * @brief       ����X��ʼ������
 * @param       baudrate: ������, �����Լ���Ҫ���ò�����ֵ
 * @note        ע��: ����������ȷ��ʱ��Դ, ���򴮿ڲ����ʾͻ������쳣.
 *              �����USART��ʱ��Դ��sys_stm32_clock_init()�������Ѿ����ù���.
 * @retval      ��
 */
void usart_init(uint32_t baudrate)
{
    USART_UX_CLK_ENABLE();                                  /* USART1 ʱ��ʹ�� */
    USART_TX_GPIO_CLK_ENABLE();                             /* ��������ʱ��ʹ�� */
    USART_RX_GPIO_CLK_ENABLE();                             /* ��������ʱ��ʹ�� */
    
    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin = USART_TX_GPIO_PIN;               /* TX���� */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* ����������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
    gpio_init_struct.Alternate = USART_TX_GPIO_AF;          /* ����ΪUSART1 */
    HAL_GPIO_Init(USART_TX_GPIO_PORT, &gpio_init_struct);   /* ��ʼ���������� */

    gpio_init_struct.Pin = USART_RX_GPIO_PIN;               /* RX���� */
    gpio_init_struct.Alternate = USART_RX_GPIO_AF;          /* ����ΪUSART1 */
    HAL_GPIO_Init(USART_RX_GPIO_PORT, &gpio_init_struct);   /* ��ʼ���������� */

    g_usart1_handler.Instance = USART_UX;                         /* USART1 */
    g_usart1_handler.Init.BaudRate = baudrate;                    /* ������ */
    g_usart1_handler.Init.WordLength = UART_WORDLENGTH_8B;        /* �ֳ�Ϊ8λ���ݸ�ʽ */
    g_usart1_handler.Init.StopBits = UART_STOPBITS_1;             /* һ��ֹͣλ */
    g_usart1_handler.Init.Parity = UART_PARITY_NONE;              /* ����żУ��λ */
    g_usart1_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;        /* ��Ӳ������ */
    g_usart1_handler.Init.Mode = UART_MODE_TX_RX;                 /* �շ�ģʽ */
    HAL_UART_Init(&g_usart1_handler);                             /* HAL_UART_Init()��ʹ��UART1 */
    __HAL_UART_DISABLE_IT(&g_usart1_handler, UART_IT_TC);
    
    #if USART_EN_RX
//        __HAL_UART_ENABLE_IT(&g_usart1_handler, UART_IT_RXNE);
        __HAL_UART_ENABLE_IT(&g_usart1_handler, UART_IT_IDLE);  // ʹ�ܴ��ڿ����ж�
        HAL_NVIC_EnableIRQ(USART_UX_IRQn);                      /* ʹ��USART1�ж�ͨ�� */
        HAL_NVIC_SetPriority(USART_UX_IRQn, 7, 0);              /* ��ռ���ȼ�3�������ȼ�3 */
    #endif
    
//    /* �ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ��������������� */
//    HAL_UART_Receive_IT(&g_usart1_handler, (uint8_t *)g_rx_buffer, RXBUFFERSIZE);
    
}

void USART_UX_IRQHandler(void)
{ 
    uint8_t res;
    
    if ((__HAL_UART_GET_FLAG(&g_usart1_handler, UART_FLAG_IDLE) != RESET))
    {
        __HAL_UART_CLEAR_IDLEFLAG(&g_usart1_handler);
        
//				HAL_UART_DMAStop(&g_usart1_handler);
				/* ***************Stop UART DMA Rx request if ongoing************** */
				uint32_t dmarequest = 0x00U;
				dmarequest = HAL_IS_BIT_SET(USART_UX->CR3, USART_CR3_DMAR);
				if ((g_usart1_handler.RxState == HAL_UART_STATE_BUSY_RX) && dmarequest)
				{
					CLEAR_BIT(USART_UX->CR3, USART_CR3_DMAR);

					/* Abort the UART DMA Rx stream */
					if (g_usart1_handler.hdmarx != NULL)
					{
						HAL_DMA_Abort(g_usart1_handler.hdmarx);
					}
//					UART_EndRxTransfer(g_usart2_handler);
					/* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
					CLEAR_BIT(USART_UX->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
					CLEAR_BIT(USART_UX->CR3, USART_CR3_EIE);

					/* In case of reception waiting for IDLE event, disable also the IDLE IE interrupt source */
					if (g_usart1_handler.ReceptionType == HAL_UART_RECEPTION_TOIDLE)
					{
						CLEAR_BIT(USART_UX->CR1, USART_CR1_IDLEIE);
					}

					/* At end of Rx process, restore huart->RxState to Ready */
					g_usart1_handler.RxState = HAL_UART_STATE_READY;
					g_usart1_handler.ReceptionType = HAL_UART_RECEPTION_STANDARD;
				}			
				/* *************************************************************** */
        
        // ������
        g_usart_rx_sta = USART_REC_LEN - __HAL_DMA_GET_COUNTER(&g_dma_handle_usart1_rx);
        if(g_usart_rx_buf[g_usart_rx_sta - 1] == 0x0a)
        {
            g_usart_rx_sta |= 0x8000;
            usmart_scan();
        }
        
        memset(g_usart_rx_buf, 0, sizeof(g_usart_rx_buf)); 

        HAL_UART_Receive_DMA(&g_usart1_handler, g_usart_rx_buf, USART_REC_LEN);
    }
}

#endif


 

 




