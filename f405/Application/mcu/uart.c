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
//#include "..\modules\sensor\encoder_if.h"
//#include "..\power\power_mana_if.h"
//#include "..\mmi\mmi_if.h"
//#include "..\bus\bus_if.h"
#include "..\mcu\main.h"

/* Private Includes ----------------------------------------------------------*/
#include "uart.h"
#include "uart_if.h"
#include "syn6658.h"

//串口使用说明
//USART1——调试debug使用(RS232或TTL电平)
//USART2——4G 模块
//USART5——蓝牙ble 模块
//USART6——SYN6658 语音芯片


UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_uart5_rx;
DMA_HandleTypeDef hdma_uart5_tx;
DMA_HandleTypeDef hdma_usart6_rx;
DMA_HandleTypeDef hdma_usart6_tx;

//接收buf
uint8_t uart1_dma_rx_buf[UART1_DMA_BUF_LEN]={0};
uint8_t uart1_dma_rx_buf_raw[UART1_DMA_BUF_LEN]={0};//给上层调用
uint8_t uart1_dma_rx_buf_len = 0;

uint8_t uart2_dma_rx_buf[UART2_DMA_BUF_LEN]={0};
uint8_t uart2_dma_rx_buf_raw[UART2_DMA_BUF_LEN]={0};
uint8_t uart2_dma_rx_buf_len = 0;
uint32_t uart2_rx_data_tick = 0;

uint8_t uart5_dma_rx_buf[UART5_DMA_BUF_LEN]={0};
uint8_t uart5_dma_rx_buf_raw[UART5_DMA_BUF_LEN]={0};
uint8_t uart5_dma_rx_buf_len = 0;
uint32_t uart5_rx_data_tick = 0;

uint8_t uart6_dma_rx_buf[UART6_DMA_BUF_LEN]={0};
uint8_t uart6_dma_rx_buf_raw[UART6_DMA_BUF_LEN]={0};



/* Private function prototypes -----------------------------------------------*/
HAL_StatusTypeDef uart1_rx_data_config_dma(uint8_t *pbuf, uint16_t size);



/* Global functions ----------------------------------------------------------*/

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
void uart1_init(void)
{
  huart1.Instance = USART1;
//	huart1.Init.BaudRate = 115200;
  huart1.Init.BaudRate = 460800;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  //串口初始化完成后，马上进入接收状态
  uart1_rx_data_config_dma(uart1_dma_rx_buf, UART1_DMA_BUF_LEN);
}

void uart1_send_data(uint8_t *buf, uint16_t len)
{
    HAL_UART_Transmit_DMA(&huart1, buf, len);
}


#ifdef PRINTF_ENABLE
static uint8_t printf_buf[1024];
#endif
void mprintf(const char * format, ...)
{
#ifdef PRINTF_ENABLE

    va_list ap;
    int n;

    va_start(ap, format);
    n = vsnprintf ((char*)printf_buf, 1024, format, ap);
    va_end(ap);

    HAL_UART_Transmit_DMA(&huart1, (uint8_t*)printf_buf, n);
    
#endif
}

/**
  * @brief  config uart3 dma for receive data.
  * @param  pbuf:pointer for store received data; data:assign dma receive data size.
  * @retval None
  */
HAL_StatusTypeDef uart1_rx_data_config_dma(uint8_t *pbuf, uint16_t size)
{
    //暂停DMA接收
    HAL_UART_DMAStop(&huart1);

    //空闲中断打开/* 2018-8-7 */
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);

    //重新调用DMA串口接收函数, 串口进入接收状态
    return ( HAL_UART_Receive_DMA(&huart1, (uint8_t *)pbuf, size) ); 
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
void uart2_init(uint32_t BaudRate)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = BaudRate;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  //串口初始化完成后，马上进入接收状态
  uart2_rx_data_config_dma(uart2_dma_rx_buf, UART2_DMA_BUF_LEN);
}

/**
  * @brief  uart dma send data.
  * @param  pbuf:pointer for input data; size:data length to send.
  * @retval None
  */
HAL_StatusTypeDef uart2_dma_send_data(uint8_t *pbuf, uint16_t size)
{
    return(HAL_UART_Transmit_DMA(&huart2, (uint8_t*)pbuf, size));
}

/**
  * @brief  config uart3 dma for receive data.
  * @param  pbuf:pointer for store received data; data:assign dma receive data size.
  * @retval None
  */
HAL_StatusTypeDef uart2_rx_data_config_dma(uint8_t *pbuf, uint16_t size)
{
    //暂停DMA接收
    HAL_UART_DMAStop(&huart2);

    //空闲中断打开/* 2018-8-7 */
    __HAL_UART_CLEAR_IDLEFLAG(&huart2);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);

    //重新调用DMA串口接收函数, 串口进入接收状态
    return ( HAL_UART_Receive_DMA(&huart2, (uint8_t *)pbuf, size) ); 
}

/**
  * @brief USART5 Initialization Function
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
    return(HAL_UART_Transmit_DMA(&huart5, (uint8_t*)pbuf, size));
}

/**
  * @brief  config uart3 dma for receive data.
  * @param  pbuf:pointer for store received data; data:assign dma receive data size.
  * @retval None
  */
HAL_StatusTypeDef uart5_rx_data_config_dma(uint8_t *pbuf, uint16_t size)
{
    //暂停DMA接收
    HAL_UART_DMAStop(&huart5);

    //空闲中断打开/* 2018-8-7 */
    __HAL_UART_CLEAR_IDLEFLAG(&huart5);
    __HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);

    //重新调用DMA串口接收函数, 串口进入接收状态
    return ( HAL_UART_Receive_DMA(&huart5, (uint8_t *)pbuf, size) ); 
}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
void uart6_init(void)
{
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  //串口初始化完成后，马上进入接收状态
  uart6_rx_data_config_dma(uart6_dma_rx_buf, UART6_DMA_BUF_LEN);
}

/**
  * @brief  uart dma send data.
  * @param  pbuf:pointer for input data; size:data length to send.
  * @retval None
  */
HAL_StatusTypeDef uart6_dma_send_data(uint8_t *pbuf, uint16_t size)
{
    return(HAL_UART_Transmit_DMA(&huart6, (uint8_t*)pbuf, size));
}

/**
  * @brief  config uart3 dma for receive data.
  * @param  pbuf:pointer for store received data; data:assign dma receive data size.
  * @retval None
  */
HAL_StatusTypeDef uart6_rx_data_config_dma(uint8_t *pbuf, uint16_t size)
{
    //暂停DMA接收
    HAL_UART_DMAStop(&huart6);

    //空闲中断打开/* 2018-8-7 */
    __HAL_UART_CLEAR_IDLEFLAG(&huart6);
    __HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE);

    //重新调用DMA串口接收函数, 串口进入接收状态
    return ( HAL_UART_Receive_DMA(&huart6, (uint8_t *)pbuf, size) ); 
}



/**
  * @brief  Tx Transfer completed callback
  * @param  huart: UART handle. 
  * @note   This example shows a simple way to report end of DMA Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
unsigned int uart1_send_counter=0;
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
//    if(huart == &huart1)
//    {
//    }
//    else if(huart == &huart2)
//    {
//    }
//    else if (huart == &huart6)
//    {
//    }
    uart1_send_counter++;
}

/**
  * @brief  Rx Transfer completed callback
  * @param  huart: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */

//extern  unsigned int  data_buf;
//extern int unpack(unsigned char *buff,unsigned int lenght);
//unsigned int counter=0; 
//static  unsigned  char *    pDMA_buf=uart1_dma_rx_buf;
//unsigned  char *    ppDMA_buf = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uint16_t buf_len = 0;
   
    
    if(huart == &huart1)
    {
//        buf_len = (UART1_DMA_BUF_LEN - (huart->hdmarx->Instance->NDTR));    
//        if(buf_len > 0)
//        {
//            counter++;
//            ppDMA_buf = pDMA_buf;
//            
//             if(pDMA_buf==uart1_dma_rx_buf)
//                pDMA_buf=uart1_dma_rx_buf_raw;
//            else
//                pDMA_buf=uart1_dma_rx_buf;
//            
//            uart1_rx_data_config_dma(pDMA_buf, UART1_DMA_BUF_LEN);
//            
//            data_buf = unpack(ppDMA_buf,buf_len);
//            
//            memset(ppDMA_buf,0,buf_len);
//		}
//        else
//          uart1_rx_data_config_dma(pDMA_buf, UART1_DMA_BUF_LEN); 
		
			
		//取出串口接收到的数据
        buf_len = (UART1_DMA_BUF_LEN - (huart->hdmarx->Instance->NDTR));    
        if(buf_len > 0)
        {
            memcpy(&uart1_dma_rx_buf_raw[uart1_dma_rx_buf_len], uart1_dma_rx_buf, buf_len);
            uart1_dma_rx_buf_len += buf_len;
        }

        //重新进入接收数据的状态
        uart1_rx_data_config_dma(uart1_dma_rx_buf, UART1_DMA_BUF_LEN);
            
		
        
    }
    else if (huart == &huart2)
    {
        //取出串口接收到的数据
        buf_len = (UART2_DMA_BUF_LEN - (huart->hdmarx->Instance->NDTR));    
        if(buf_len > 0)
        {
            memcpy(&uart2_dma_rx_buf_raw[uart2_dma_rx_buf_len], uart2_dma_rx_buf, buf_len);
            uart2_dma_rx_buf_len += buf_len;
            uart2_rx_data_tick = HAL_GetTick();
        }

        //重新进入接收数据的状态
        uart2_rx_data_config_dma(uart2_dma_rx_buf, UART2_DMA_BUF_LEN);
        
    }
    else if (huart == &huart5)
    {
        //取出串口接收到的数据
        buf_len = (UART5_DMA_BUF_LEN - (huart->hdmarx->Instance->NDTR));    
        if(buf_len > 0)
        {
            memcpy(&uart5_dma_rx_buf_raw[uart5_dma_rx_buf_len], uart5_dma_rx_buf, buf_len);
            uart5_dma_rx_buf_len += buf_len;
            uart5_rx_data_tick = HAL_GetTick();
        }

        //重新进入接收数据的状态
        uart5_rx_data_config_dma(uart5_dma_rx_buf, UART5_DMA_BUF_LEN);
        
    }
    else if (huart == &huart6)
    {
        //取出串口接收到的数据
        buf_len = (UART6_DMA_BUF_LEN - (huart->hdmarx->Instance->NDTR));    
        if(buf_len > 0)
        {
            if(buf_len == 1)
            {
                syn6658_uart_receive_callback(uart6_dma_rx_buf[0]);
            }
        }

        //重新进入接收数据的状态
        uart6_rx_data_config_dma(uart6_dma_rx_buf, UART6_DMA_BUF_LEN);

    }
}


/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart1);
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart2);
}

/**
  * @brief This function handles UART5 global interrupt.
  */
void UART5_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart5);
}

/**
  * @brief This function handles USART6 global interrupt.
  */
void USART6_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart6);
}


/**
  * @brief This function handles DMA2 stream7 global interrupt.
  */
void DMA2_Stream7_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart1_tx);
}

/**
  * @brief This function handles DMA2 stream2 global interrupt.
  */
void DMA2_Stream2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
}

/**
  * @brief This function handles DMA1 stream6 global interrupt.
  */
void DMA1_Stream6_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart2_tx);
}

/**
  * @brief This function handles DMA1 stream5 global interrupt.
  */
void DMA1_Stream5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart2_rx);
}

/**
  * @brief This function handles DMA1 stream7 global interrupt.
  */
void DMA1_Stream7_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_uart5_tx);
}

/**
  * @brief This function handles DMA1 stream0 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_uart5_rx);
}

/**
  * @brief This function handles DMA2 stream6 global interrupt.
  */
void DMA2_Stream6_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart6_tx);
}

/**
  * @brief This function handles DMA2 stream1 global interrupt.
  */
void DMA2_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart6_rx);
}


