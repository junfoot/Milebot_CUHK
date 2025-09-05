/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : dma.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : dma interface layer driver.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/

/* Public Includes -----------------------------------------------------------*/
#include "mcu_if.h"

/* Private Includes ----------------------------------------------------------*/
#include "dma.h"
#include "dma_if.h"

#include "uart_if.h"
#include "adc_if.h"


/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* Global functions ----------------------------------------------------------*/
/** 
  * Enable DMA controller clock
  */
void dma_init(void) 
{
    /* DMA controller clock enable */
    __HAL_RCC_DMA2_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();
    
    /* DMA interrupt init */

    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 4, 0);  //ADC
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
    
    HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 5, 0);    //UART1 TX
    HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
    HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 5, 0);    //UART1 RX
    HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

    HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 6, 0);    //UART2 RX   
    HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
    HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 6, 0);    //UART2 TX   
    HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

    HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 8, 0);    //UART5 RX   
    HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
    HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 8, 0);    //UART5 TX   
    HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);

    HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 7, 0);    //UART6 RX
    HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 7, 0);    //UART6 TX
    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

}

void dma_stop_disable(void)
{
    //DMA stop
    HAL_UART_DMAStop(&huart1);
    HAL_UART_DMAStop(&huart2);
    HAL_UART_DMAStop(&huart6);

    HAL_ADC_Stop_DMA(&hadc1);
    
    /* DMA controller clock disable */
    __HAL_RCC_DMA2_CLK_DISABLE();
    __HAL_RCC_DMA1_CLK_DISABLE();
    
}


/* Private functions ---------------------------------------------------------*/
/**
  * @brief  mcu xx func config.
  * @param  None
  * @retval None
  */

/****************************** END OF FILE ***********************************/
