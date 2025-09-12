/**
 ****************************************************************************************************
 * @file        dma.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-20
 * @brief       DMA 驱动代码
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
 * V1.0 20211020
 * 第一次发布
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

DMA_HandleTypeDef  g_dma_handle_usart1_rx;                  /* DMA句柄 */
DMA_HandleTypeDef  g_dma_handle_usart1_tx;
DMA_HandleTypeDef  g_dma_handle_usart2_rx;
DMA_HandleTypeDef  g_dma_handle_usart2_tx;
//DMA_HandleTypeDef  g_dma_handle_usart3;
//DMA_HandleTypeDef  g_dma_handle_uart5;
//DMA_HandleTypeDef  g_dma_handle_usart6;


void dma_init(void)
{ 
    __HAL_RCC_DMA2_CLK_ENABLE();                        /* DMA2时钟使能 */
    __HAL_RCC_DMA1_CLK_ENABLE();                        /* DMA1时钟使能 */
    

    __HAL_LINKDMA(&g_usart1_handler, hdmarx, g_dma_handle_usart1_rx);   /* 将DMA与USART1联系起来（接收DMA) */

    /* Tx DMA配置 */
    g_dma_handle_usart1_rx.Instance = DMA2_Stream2;                    /* 数据流选择 */
    g_dma_handle_usart1_rx.Init.Channel = DMA_CHANNEL_4;                               /* DMA通道选择 */
    g_dma_handle_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;           /* 存储器到外设 */
    g_dma_handle_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;               /* 外设非增量模式 */
    g_dma_handle_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;                   /* 存储器增量模式 */
    g_dma_handle_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  /* 外设数据长度:8位 */
    g_dma_handle_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     /* 存储器数据长度:8位 */
    g_dma_handle_usart1_rx.Init.Mode = DMA_NORMAL;                          /* 外设流控模式 */
    g_dma_handle_usart1_rx.Init.Priority = DMA_PRIORITY_HIGH;             /* 优先级 */
    g_dma_handle_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;            /* 关闭FIFO模式 */
    g_dma_handle_usart1_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;    /* FIFO阈值配置 */
    g_dma_handle_usart1_rx.Init.MemBurst = DMA_MBURST_SINGLE;               /* 存储器突发单次传输 */
    g_dma_handle_usart1_rx.Init.PeriphBurst = DMA_PBURST_SINGLE;            /* 外设突发单次传输 */

    /* DMA interrupt init */
    /* DMA2_Stream2_IRQn interrupt configuration */
//    HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 5, 0);
//    HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

//    HAL_DMA_DeInit(&g_dma_handle_usart1_rx);
    HAL_DMA_Init(&g_dma_handle_usart1_rx);
    HAL_UART_Receive_DMA(&g_usart1_handler, g_usart_rx_buf, USART_REC_LEN);
		
		/* ---------------------------------------------------------------- */

		g_dma_handle_usart2_rx.Instance = DMA1_Stream5;
    g_dma_handle_usart2_rx.Init.Channel = DMA_CHANNEL_4;
    g_dma_handle_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    g_dma_handle_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    g_dma_handle_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    g_dma_handle_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    g_dma_handle_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    g_dma_handle_usart2_rx.Init.Mode = DMA_NORMAL;
    g_dma_handle_usart2_rx.Init.Priority = DMA_PRIORITY_HIGH;
    g_dma_handle_usart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;            /* 关闭FIFO模式 */
    g_dma_handle_usart2_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;    /* FIFO阈值配置 */
    g_dma_handle_usart2_rx.Init.MemBurst = DMA_MBURST_SINGLE;               /* 存储器突发单次传输 */
    g_dma_handle_usart2_rx.Init.PeriphBurst = DMA_PBURST_SINGLE;            /* 外设突发单次传输 */
    /* interrupt configuration */
		
//		HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 6, 0);
//    HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
    
		HAL_UART_Receive_DMA(&g_usart2_handler, g_USART2_rx_buf, USART2_REC_LEN);
		
		HAL_DMA_Init(&g_dma_handle_usart2_rx);
		__HAL_LINKDMA(&g_usart2_handler, hdmarx, g_dma_handle_usart2_rx);
	
	
		/* ---------------------------------------------------------------- */
    g_dma_handle_usart1_tx.Instance = DMA2_Stream7;
    g_dma_handle_usart1_tx.Init.Channel = DMA_CHANNEL_4;
    g_dma_handle_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    g_dma_handle_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    g_dma_handle_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    g_dma_handle_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    g_dma_handle_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    g_dma_handle_usart1_tx.Init.Mode = DMA_NORMAL;
    g_dma_handle_usart1_tx.Init.Priority = DMA_PRIORITY_HIGH;
    /* interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
    HAL_DMA_Init(&g_dma_handle_usart1_tx);
    __HAL_LINKDMA(&g_usart1_handler,hdmatx,g_dma_handle_usart1_tx);
		
    /* ---------------------------------------------------------------- */
    g_dma_handle_usart2_tx.Instance = DMA1_Stream6;
    g_dma_handle_usart2_tx.Init.Channel = DMA_CHANNEL_4;
    g_dma_handle_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    g_dma_handle_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    g_dma_handle_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
    g_dma_handle_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    g_dma_handle_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    g_dma_handle_usart2_tx.Init.Mode = DMA_NORMAL;
    g_dma_handle_usart2_tx.Init.Priority = DMA_PRIORITY_HIGH;
    /* interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
    HAL_DMA_Init(&g_dma_handle_usart2_tx);
    __HAL_LINKDMA(&g_usart2_handler,hdmatx,g_dma_handle_usart2_tx);
   








/* ---------------------------------------------------------------- */
//    g_dma_handle_usart3.Instance = DMA1_Stream3;
//    g_dma_handle_usart3.Init.Channel = DMA_CHANNEL_4;
//    g_dma_handle_usart3.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    g_dma_handle_usart3.Init.PeriphInc = DMA_PINC_DISABLE;
//    g_dma_handle_usart3.Init.MemInc = DMA_MINC_ENABLE;
//    g_dma_handle_usart3.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//    g_dma_handle_usart3.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//    g_dma_handle_usart3.Init.Mode = DMA_NORMAL;
//    g_dma_handle_usart3.Init.Priority = DMA_PRIORITY_HIGH;
//    /* interrupt configuration */
//    HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 10, 0);
//    HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
//    HAL_DMA_Init(&g_dma_handle_usart3);
//    __HAL_LINKDMA(&g_usart3_handler,hdmatx,g_dma_handle_usart3);
//        /* ---------------------------------------------------------------- */
//    g_dma_handle_uart5.Instance = DMA1_Stream7;
//    g_dma_handle_uart5.Init.Channel = DMA_CHANNEL_4;
//    g_dma_handle_uart5.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    g_dma_handle_uart5.Init.PeriphInc = DMA_PINC_DISABLE;
//    g_dma_handle_uart5.Init.MemInc = DMA_MINC_ENABLE;
//    g_dma_handle_uart5.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//    g_dma_handle_uart5.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//    g_dma_handle_uart5.Init.Mode = DMA_NORMAL;
//    g_dma_handle_uart5.Init.Priority = DMA_PRIORITY_HIGH;
//    /* interrupt configuration */
//    HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 10, 0);
//    HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);
//    HAL_DMA_Init(&g_dma_handle_uart5);
//    __HAL_LINKDMA(&g_uart5_handler,hdmatx,g_dma_handle_uart5);
//        /* ---------------------------------------------------------------- */
//    g_dma_handle_usart6.Instance = DMA2_Stream7;
//    g_dma_handle_usart6.Init.Channel = DMA_CHANNEL_5;
//    g_dma_handle_usart6.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    g_dma_handle_usart6.Init.PeriphInc = DMA_PINC_DISABLE;
//    g_dma_handle_usart6.Init.MemInc = DMA_MINC_ENABLE;
//    g_dma_handle_usart6.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//    g_dma_handle_usart6.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//    g_dma_handle_usart6.Init.Mode = DMA_NORMAL;
//    g_dma_handle_usart6.Init.Priority = DMA_PRIORITY_HIGH;
//    /* interrupt configuration */
//    HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 10, 0);
//    HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
//    HAL_DMA_Init(&g_dma_handle_usart6);
//    __HAL_LINKDMA(&g_usart6_handler,hdmatx,g_dma_handle_usart6);
    
}


