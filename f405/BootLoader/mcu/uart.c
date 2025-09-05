/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : uart.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : UART interface layer driver.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/* Public Includes -----------------------------------------------------------*/
#include "mcu_if.h"
#include "main.h"

/* Private Includes ----------------------------------------------------------*/
#include "uart.h"
#include "uart_if.h"

//串口使用说明
//UART5——蓝牙ble 模块


UART_HandleTypeDef huart5;

//接收buf
uint8_t uart5_dma_rx_buf[UART5_DMA_BUF_LEN]={0};
uint8_t uart5_dma_rx_buf_raw[UART5_DMA_BUF_LEN]={0};
uint16_t uart5_dma_rx_buf_len = 0;
uint32_t uart5_rx_data_tick = 0;


/* Private function prototypes -----------------------------------------------*/



/* Global functions ----------------------------------------------------------*/

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
void uart5_init(uint32_t BaudRate)
{
  huart5.Instance = UART5;
  huart5.Init.BaudRate = BaudRate;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  //串口初始化完成后，马上进入接收状态
  uart5_rx_data_config_dma(uart5_dma_rx_buf, UART5_DMA_BUF_LEN);
}

/**
  * @brief  uart dma send data.
  * @param  pbuf:pointer for input data; size:data length to send.
  * @retval None
  */
HAL_StatusTypeDef uart5_dma_send_data(uint8_t *pbuf, uint16_t size)
{
    return(HAL_UART_Transmit_IT(&huart5, (uint8_t*)pbuf, size));
}

/**
  * @brief  config uart3 dma for receive data.
  * @param  pbuf:pointer for store received data; data:assign dma receive data size.
  * @retval None
  */
HAL_StatusTypeDef uart5_rx_data_config_dma(uint8_t *pbuf, uint16_t size)
{
    //暂停DMA接收
    HAL_UART_AbortReceive_IT(&huart5);

    //空闲中断打开/* 2018-8-7 */
    __HAL_UART_CLEAR_IDLEFLAG(&huart5);
    __HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);

    //重新调用DMA串口接收函数, 串口进入接收状态
    return ( HAL_UART_Receive_IT(&huart5, (uint8_t *)pbuf, size) ); 
}


/**
  * @brief  Tx Transfer completed callback
  * @param  huart: UART handle. 
  * @note   This example shows a simple way to report end of DMA Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart5)
    {
    }
}

/**
  * @brief  Rx Transfer completed callback
  * @param  huart: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uint16_t buf_len = 0;
    
    if (huart == &huart5)
    {
        //取出串口接收到的数据
        buf_len = (UART5_DMA_BUF_LEN - (huart->RxXferCount));    
        if(buf_len > 0)
        {
            if((uart5_dma_rx_buf_len + buf_len) >= UART5_DMA_BUF_LEN)
            {
                uart5_dma_rx_buf_len = 0;
            }
            
            memcpy(&uart5_dma_rx_buf_raw[uart5_dma_rx_buf_len], uart5_dma_rx_buf, buf_len);
            uart5_dma_rx_buf_len += buf_len;
            uart5_rx_data_tick = HAL_GetTick();
        }

        //重新进入接收数据的状态
        uart5_rx_data_config_dma(uart5_dma_rx_buf, UART5_DMA_BUF_LEN);
        
    }
}


/**
  * @brief This function handles USART2 global interrupt.
  */
void UART5_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart5);
}



