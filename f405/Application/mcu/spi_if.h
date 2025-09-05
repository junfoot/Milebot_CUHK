/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : spi_if.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : extern header file of spi.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SPI_IF_H_
#define _SPI_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include <stdint.h>
#include "mcu_if.h" 
     
/**** Definition of constants ****/



/**** Definition of macros ****/


/**** Definition of types ****/ 
/** SPI transaction status.
 */
enum SPITransactionStatus 
{
  SPITransPending,
  SPITransRunning,
  SPITransSuccess,
  SPITransFailed,
  SPITransDone
};

/** I2C transaction structure.
 */
struct spi_transaction
{
    uint16_t len_r;
    uint16_t  len_w;
    uint8_t *tx_buf;  ///< pointer to receive buffer for DMA
    uint8_t *rx_buf; ///< pointer to transmit buffer for DMA
    uint8_t slave_idx;            ///< slave id: #SPI_SLAVE0 to #SPI_SLAVE4
    volatile enum SPITransactionStatus status;
    SPI_HandleTypeDef *hspi;    /* SPI handle Structure definition */
    uint16_t error;
};

/**** Declaration of constants ****/


/**** Declaration of variables ****/
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern DMA_HandleTypeDef hdma_spi2_rx;
extern DMA_HandleTypeDef hdma_spi2_tx;

/**** Declaration of functions ****/
extern void SPI1_IRQHandler(void);
extern void SPI2_IRQHandler(void);
extern void DMA1_Stream3_IRQHandler(void);
extern void DMA1_Stream4_IRQHandler(void);
extern void DMA2_Stream0_IRQHandler(void);
extern void DMA2_Stream5_IRQHandler(void);

extern void spi1_init(void);
extern void spi2_init(void);

extern uint8_t spi_transmit(struct spi_transaction *p);
extern uint8_t spi_transceive(struct spi_transaction *p);
extern uint8_t spi_receive(struct spi_transaction *p);
extern void spi_init_slaves(void);
extern void spi_slave_select(uint8_t slave);
extern void spi_slave_unselect(uint8_t slave);

#ifdef __cplusplus
}
#endif

#endif /*_SPI_IF_H_*/

/****************************** END OF FILE ***********************************/
