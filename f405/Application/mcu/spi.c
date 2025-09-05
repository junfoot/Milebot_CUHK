/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : spi.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : spi interface layer driver.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/

/* Public Includes -----------------------------------------------------------*/
#include "mcu_if.h"

/* Private Includes ----------------------------------------------------------*/
#include "spi.h"
#include "spi_if.h"

/* Global variables ----------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi2_rx;
DMA_HandleTypeDef hdma_spi2_tx;

/* Private variables ---------------------------------------------------------*/


/* Buffer used for reception */


/* Global functions ----------------------------------------------------------*/
/**
  * @brief  spi1 init.
  * @param  None
  * @retval None
  */
void spi1_init(void)
{
#if 0   /* Begin if 0 by lison on 2020-6-17 */
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;    /* spi1 21MHz clock for fram */
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
#endif  /* End if 0 by lison on 2020-6-17 */
}

/* SPI2 init function */
void spi2_init(void)
{
//  hspi2.Instance = SPI2;
//  hspi2.Init.Mode = SPI_MODE_MASTER;
//  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
//  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
//  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
//  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
//  hspi2.Init.NSS = SPI_NSS_SOFT;
//  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;    /*84HMz / 16 = 5MHz*/
//  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
//  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
//  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//  hspi2.Init.CRCPolynomial = 10;
//  if (HAL_SPI_Init(&hspi2) != HAL_OK)
//  {
//    _Error_Handler(__FILE__, __LINE__);
//  }
}

/**
  * @brief  spi transmit data by dma.
  * @param  p:pointer to SPI transaction structure
  * @retval TRUE:success; FALSE:fail
  */
uint8_t spi_transmit(struct spi_transaction *p)
{
#if 0   /* Begin if 0 by lison on 2020-6-17 */
    spi_slave_select(p->slave_idx);
    if( HAL_OK == HAL_SPI_Transmit_DMA(p->hspi, p->tx_buf, p->len_w) )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }    
#endif  /* End if 0 by lison on 2020-6-17 */
}

/**
  * @brief  transmit and receive data.
  * @param  p:Pointer to SPI transaction structure
  * @retval TRUE:success; FALSE:fail
  */
uint8_t spi_transceive(struct spi_transaction *p)
{
#if 0   /* Begin if 0 by lison on 2020-6-17 */
    spi_slave_select(p->slave_idx);
    if(HAL_OK == HAL_SPI_TransmitReceive_DMA(p->hspi, p->tx_buf, p->rx_buf, p->len_w + p->len_r))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif  /* End if 0 by lison on 2020-6-17 */
}

/**
  * @brief  configure dma rx,then wait dma rx interrupt.
  * @param  p: pointer to SPI transaction structure
  * @retval TRUE:success; FALSE:fail
  */
uint8_t spi_receive(struct spi_transaction *p)
{
#if 0   /* Begin if 0 by lison on 2020-6-17 */
    spi_slave_select(p->slave_idx);
    if(HAL_OK == HAL_SPI_Receive_DMA(p->hspi, p->rx_buf, p->len_r))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif  /* End if 0 by lison on 2020-6-17 */
}

static inline void _spi_slave_select(uint8_t slave)
{
#if 0   /* Begin if 0 by lison on 2020-6-17 */
    switch (slave) 
    {
        #ifdef USE_SPI_SLAVE0
        case 0:
            HAL_GPIO_WritePin(SPI_SELECT_SLAVE0_PORT, SPI_SELECT_SLAVE0_PIN, GPIO_PIN_RESET);
            break;
        #endif /* USE_SPI_SLAVE0 */
    
        #ifdef USE_SPI_SLAVE1
        case 1:
            HAL_GPIO_WritePin(SPI_SELECT_SLAVE1_PORT, SPI_SELECT_SLAVE1_PIN, GPIO_PIN_RESET);
            break;
        #endif /* USE_SPI_SLAVE1 */
    
        default:
            break;
  }
#endif  /* End if 0 by lison on 2020-6-17 */
}

static inline void _spi_slave_unselect(uint8_t slave)
{
#if 0   /* Begin if 0 by lison on 2020-6-17 */
    switch (slave) 
    {
        #ifdef USE_SPI_SLAVE0
        case 0:
            HAL_GPIO_WritePin(SPI_SELECT_SLAVE0_PORT, SPI_SELECT_SLAVE0_PIN, GPIO_PIN_SET);
            break;
        #endif /* USE_SPI_SLAVE0 */
    
        #ifdef USE_SPI_SLAVE1
        case 1:
            HAL_GPIO_WritePin(SPI_SELECT_SLAVE1_PORT, SPI_SELECT_SLAVE1_PIN, GPIO_PIN_SET);
            break;
        #endif /* USE_SPI_SLAVE1 */
    
        default:
            break;
  }
#endif  /* End if 0 by lison on 2020-6-17 */
}

void spi_slave_select(uint8_t slave)
{
//    _spi_slave_select(slave);
}

void spi_slave_unselect(uint8_t slave)
{
//    _spi_slave_unselect(slave);
}

void spi_init_slaves(void)
{
#if 0   /* Begin if 0 by lison on 2020-6-17 */
#ifdef USE_SPI_SLAVE0
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = SPI_SELECT_SLAVE0_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SPI_SELECT_SLAVE0_PORT, &GPIO_InitStruct);
    _spi_slave_unselect(0);
#endif

#ifdef USE_SPI_SLAVE1
    GPIO_InitStruct.Pin = SPI_SELECT_SLAVE1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SPI_SELECT_SLAVE1_PORT, &GPIO_InitStruct);
    _spi_slave_unselect(1);
#endif
#endif  /* End if 0 by lison on 2020-6-17 */
}

/*
@brief This function handles DMA1 stream3 global interrupt.
*/
//void DMA1_Stream3_IRQHandler(void)
//{
//    HAL_DMA_IRQHandler(&hdma_spi2_rx);
//}

/*
@brief This function handles DMA1 stream4 global interrupt.
*/
//void DMA1_Stream4_IRQHandler(void)
//{
//    HAL_DMA_IRQHandler(&hdma_spi2_tx);
//}

#if 0   /* Begin if 0 by lison on 2020-6-16 */
/*
* @brief This function handles DMA2 stream0 global interrupt.
*/
void DMA2_Stream0_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_spi1_rx);
}

/*
* @brief This function handles DMA2 stream5 global interrupt.
*/
void DMA2_Stream5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_spi1_tx);
}

/**
* @brief This function handles SPI1 global interrupt.
*/
void SPI1_IRQHandler(void)
{
    HAL_SPI_IRQHandler(&hspi1);
}

/**
* @brief This function handles SPI2 global interrupt.
*/
void SPI2_IRQHandler(void)
{
    HAL_SPI_IRQHandler(&hspi2);
}
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  mcu xx func config.
  * @param  None
  * @retval None
  */
#endif  /* End if 0 by lison on 2020-6-16 */


