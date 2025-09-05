/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : fm25v_if.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/7/26
* Description : extern header file of mcu.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/7/26  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _FM25V_IF_H_
#define _FM25V_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include "..\..\mcu\spi_if.h"
#include "..\..\system\std.h"

/**** Definition of constants ****/

/**** Definition of macros ****/
#define FM25V_BUF_SIZE    256

/**** Definition of types ****/ 
enum FM25V_STATUS
{
    FM25V_STATE_UNINIT,                  /**< The chip isn't initialized */
    FM25V_STATE_IDLE,                    /**< The chip is idle and can be used */
    FM25V_STATE_READ_ID,                 /**< The chip is busy with getting the chip ID */
    FM25V_STATE_CHIP_ERASE,              /**< The chip is busy erasing itself */
    FM25V_STATE_WRITE_BYTES,             /**< The chip is busy writing bytes */
    FM25V_STATE_READ_BYTES,              /**< The chip is busy reading bytes */
    FM25V_STATE_READ_RDSR,               /**< The chip is busy reading status register */
};
     
struct FM25V_SPI
{
    enum FM25V_STATUS status;   /**< The status of the SST25VFxxxx flash chip */
    volatile uint8_t status_idx;                       /**< The counter of substatuses */
    struct spi_transaction spi_t;             /**< The SPI transaction used for the writing and reading of registers */
    uint8_t rx_buf[FM25V_BUF_SIZE];      /**< The input buffer for the SPI transaction */
    uint8_t tx_buf[FM25V_BUF_SIZE];     /**< The output buffer for the SPI transaction */
    volatile uint16_t flash_addr;                      /**< The flash address to write at */

    volatile uint8_t *transfer_buf;                    /**< The transfer buffer */
    volatile uint8_t *input_temp_buf;
    volatile uint16_t transfer_idx;                     /**< The transfer idx is used for counting input/output bytes */
    volatile uint16_t transfer_length;                  /**< The transfer buffer length */

    volatile uint8_t valid;
    uint16_t error_cnt;
    volatile uint8_t reading_flag;                        /* wait read data,1-reading data,0-read finished. */
    volatile uint8_t writing_flag;                        /* wait write data,1-writing data,0-write finished. */
};


/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
extern void fm25v_spi_cb(SPI_HandleTypeDef *spi_p);
extern void fm25v_read_id(void);
extern void fm25v_init(SPI_HandleTypeDef *spi_p, const uint8_t slave_idx);
extern uint8_t fm25v_write(uint16_t addr, const uint8_t *buf, uint16_t len);
extern uint8_t fm25v_read(uint16_t addr, uint8_t *buf, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /*_FM25V_IF_H_*/

/****************************** END OF FILE ***********************************/
