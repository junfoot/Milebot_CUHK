/**
 ****************************************************************************************************
 * @file        dma.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-20
 * @brief       DMA ��������
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
 * V1.0 20211020
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/DMA/dma.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/USART2/usart2.h"
#include "./BSP/USART3/usart3.h"
#include "./BSP/UART5/uart5.h"
#include "./BSP/USART6/usart6.h"

DMA_HandleTypeDef  g_dma_handle;                  /* DMA��� */
DMA_HandleTypeDef  g_dma_handle_usart2;
DMA_HandleTypeDef  g_dma_handle_usart3;
DMA_HandleTypeDef  g_dma_handle_uart5;
DMA_HandleTypeDef  g_dma_handle_usart6;


void dma_init(void)
{ 
    __HAL_RCC_DMA2_CLK_ENABLE();                        /* DMA2ʱ��ʹ�� */
    __HAL_RCC_DMA1_CLK_ENABLE();                        /* DMA1ʱ��ʹ�� */
    

    __HAL_LINKDMA(&g_uart1_handle, hdmarx, g_dma_handle);   /* ��DMA��USART1��ϵ����������DMA) */

    /* Tx DMA���� */
    g_dma_handle.Instance = DMA2_Stream2;                    /* ������ѡ�� */
    g_dma_handle.Init.Channel = DMA_CHANNEL_4;                               /* DMAͨ��ѡ�� */
    g_dma_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;           /* �洢�������� */
    g_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;               /* ���������ģʽ */
    g_dma_handle.Init.MemInc = DMA_MINC_ENABLE;                   /* �洢������ģʽ */
    g_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  /* �������ݳ���:8λ */
    g_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     /* �洢�����ݳ���:8λ */
    g_dma_handle.Init.Mode = DMA_NORMAL;                          /* ��������ģʽ */
    g_dma_handle.Init.Priority = DMA_PRIORITY_VERY_HIGH;             /* ���ȼ� */
    g_dma_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;            /* �ر�FIFOģʽ */
    g_dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;    /* FIFO��ֵ���� */
    g_dma_handle.Init.MemBurst = DMA_MBURST_SINGLE;               /* �洢��ͻ�����δ��� */
    g_dma_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;            /* ����ͻ�����δ��� */

    /* DMA interrupt init */
    /* DMA2_Stream2_IRQn interrupt configuration */
//    HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 5, 0);
//    HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

//    HAL_DMA_DeInit(&g_dma_handle);
    HAL_DMA_Init(&g_dma_handle);
    HAL_UART_Receive_DMA(&g_uart1_handle, g_usart_rx_buf, USART_REC_LEN);
    
    /* ---------------------------------------------------------------- */
    g_dma_handle_usart2.Instance = DMA1_Stream6;
    g_dma_handle_usart2.Init.Channel = DMA_CHANNEL_4;
    g_dma_handle_usart2.Init.Direction = DMA_MEMORY_TO_PERIPH;
    g_dma_handle_usart2.Init.PeriphInc = DMA_PINC_DISABLE;
    g_dma_handle_usart2.Init.MemInc = DMA_MINC_ENABLE;
    g_dma_handle_usart2.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    g_dma_handle_usart2.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    g_dma_handle_usart2.Init.Mode = DMA_NORMAL;
    g_dma_handle_usart2.Init.Priority = DMA_PRIORITY_HIGH;
    /* interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
    HAL_DMA_Init(&g_dma_handle_usart2);
    __HAL_LINKDMA(&g_usart2_handler,hdmatx,g_dma_handle_usart2);
        /* ---------------------------------------------------------------- */
    g_dma_handle_usart3.Instance = DMA1_Stream3;
    g_dma_handle_usart3.Init.Channel = DMA_CHANNEL_4;
    g_dma_handle_usart3.Init.Direction = DMA_MEMORY_TO_PERIPH;
    g_dma_handle_usart3.Init.PeriphInc = DMA_PINC_DISABLE;
    g_dma_handle_usart3.Init.MemInc = DMA_MINC_ENABLE;
    g_dma_handle_usart3.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    g_dma_handle_usart3.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    g_dma_handle_usart3.Init.Mode = DMA_NORMAL;
    g_dma_handle_usart3.Init.Priority = DMA_PRIORITY_HIGH;
    /* interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
    HAL_DMA_Init(&g_dma_handle_usart3);
    __HAL_LINKDMA(&g_usart3_handler,hdmatx,g_dma_handle_usart3);
        /* ---------------------------------------------------------------- */
    g_dma_handle_uart5.Instance = DMA1_Stream7;
    g_dma_handle_uart5.Init.Channel = DMA_CHANNEL_4;
    g_dma_handle_uart5.Init.Direction = DMA_MEMORY_TO_PERIPH;
    g_dma_handle_uart5.Init.PeriphInc = DMA_PINC_DISABLE;
    g_dma_handle_uart5.Init.MemInc = DMA_MINC_ENABLE;
    g_dma_handle_uart5.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    g_dma_handle_uart5.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    g_dma_handle_uart5.Init.Mode = DMA_NORMAL;
    g_dma_handle_uart5.Init.Priority = DMA_PRIORITY_HIGH;
    /* interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);
    HAL_DMA_Init(&g_dma_handle_uart5);
    __HAL_LINKDMA(&g_uart5_handler,hdmatx,g_dma_handle_uart5);
        /* ---------------------------------------------------------------- */
    g_dma_handle_usart6.Instance = DMA2_Stream7;
    g_dma_handle_usart6.Init.Channel = DMA_CHANNEL_5;
    g_dma_handle_usart6.Init.Direction = DMA_MEMORY_TO_PERIPH;
    g_dma_handle_usart6.Init.PeriphInc = DMA_PINC_DISABLE;
    g_dma_handle_usart6.Init.MemInc = DMA_MINC_ENABLE;
    g_dma_handle_usart6.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    g_dma_handle_usart6.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    g_dma_handle_usart6.Init.Mode = DMA_NORMAL;
    g_dma_handle_usart6.Init.Priority = DMA_PRIORITY_HIGH;
    /* interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
    HAL_DMA_Init(&g_dma_handle_usart6);
    __HAL_LINKDMA(&g_usart6_handler,hdmatx,g_dma_handle_usart6);
    
}


