/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : uart_if.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : extern header file of uart.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _UART_IF_H_
#define _UART_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include <stdint.h>
#include "main.h"
#include "mcu_if.h"

/**** Definition of constants ****/


/**** Definition of types ****/ 


/**** Definition of macros ****/
#define UART5_DMA_BUF_LEN       512


/**** Declaration of variables ****/
extern uint8_t uart5_dma_rx_buf_raw[UART5_DMA_BUF_LEN];
extern uint16_t uart5_dma_rx_buf_len;
extern uint32_t uart5_rx_data_tick;

extern UART_HandleTypeDef huart5;

extern void uart5_init(uint32_t BaudRate);
extern HAL_StatusTypeDef uart5_dma_send_data(uint8_t *pbuf, uint16_t size);
extern HAL_StatusTypeDef uart5_rx_data_config_dma(uint8_t *pbuf, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif /*_UART_IF_H_*/

/****************************** END OF FILE ***********************************/
