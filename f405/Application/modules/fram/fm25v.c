/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : fm25v.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/7/26
* Description : fm25v01 driver.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/7/26  wb          initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/

/* Public Includes -----------------------------------------------------------*/
#include "..\..\mcu\spi_if.h"
#include "stm32f4xx_hal_spi.h"
#include "fram_if.h"
#include "..\..\system\timer_if.h"

/* Private Includes ----------------------------------------------------------*/
#include "fm25v.h"
#include "fm25v_if.h"

/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static struct FM25V_SPI fm25v;

/* Global functions ----------------------------------------------------------*/
/**
  * @brief  fram init.
  * @param  spi_p: pointer to hspi1.
              slave_idx: cs number
  * @retval None
  */
void fm25v_init(SPI_HandleTypeDef *spi_p, const uint8_t slave_idx)
{
    /* Set spi_peripheral and start flash address */
    fm25v.spi_t.hspi = spi_p;
    fm25v.flash_addr = 0x0;

    fm25v.spi_t.len_r = 0;
    fm25v.spi_t.len_w = 0;
    fm25v.spi_t.rx_buf = fm25v.rx_buf;
    fm25v.spi_t.tx_buf = fm25v.tx_buf;
    fm25v.spi_t.slave_idx = slave_idx;
    fm25v.spi_t.status = SPITransDone;
    fm25v.spi_t.hspi->TxCpltCb = fm25v_spi_cb;
    fm25v.spi_t.hspi->RxCpltCb = fm25v_spi_cb;
    fm25v.spi_t.hspi->TxRxCpltCb = fm25v_spi_cb;
    fm25v.spi_t.hspi->ErrorCb = fm25_spi_error_cb;
    /* Update the status and start with enabling writing */
    fm25v.status = FM25V_STATE_IDLE;
    fm25v.valid = FALSE;
    fm25v_read_id();
}

/**
  * @brief  fram read id.
  * @param  None
  * @retval None
  */
void fm25v_read_id(void)
{
    if(fm25v.status != FM25V_STATE_IDLE)
    {
        return;
    }
    // Write the read id command to the chip
    fm25v.spi_t.rx_buf = fm25v.rx_buf;
    fm25v.spi_t.tx_buf = fm25v.tx_buf;
    fm25v.status = FM25V_STATE_READ_ID;
    fm25v.tx_buf[0] = FM25V_RDID;
    fm25v.spi_t.len_w = 1;
    fm25v.spi_t.len_r = 9;
    spi_transceive(&fm25v.spi_t);
}

/**
  * @brief  fm25v spi tx/rx complete callback function.
  * @param  spi_p
  * @retval None
  */
void fm25v_spi_cb(SPI_HandleTypeDef *spi_p)
{
    switch(fm25v.status)
    {
        case FM25V_STATE_READ_ID:
            if( (fm25v.rx_buf[7] == FM25V_DEV_ID_7) && (fm25v.rx_buf[8] == FM25V_DEV_ID_8) )
            {
                fm25v.valid = TRUE;
            }
            
              fm25v.status = FM25V_STATE_IDLE;
            fm25v.spi_t.rx_buf = fm25v.rx_buf;
            spi_slave_unselect(fm25v.spi_t.slave_idx);
            break;
        case FM25V_STATE_WRITE_BYTES:
            switch(fm25v.status_idx)
              {
                // Send the address with 2 or 1 byte(s) of data
                case 0:
                    spi_slave_unselect(fm25v.spi_t.slave_idx);
                    fm25v.status_idx = 1;
                    fm25v.spi_t.tx_buf = fm25v.tx_buf;
                    fm25v.tx_buf[0] = FM25V_WRITE;
                    fm25v.tx_buf[1] = ((fm25v.flash_addr&0x3F) >> 8) & 0xFF;
                    fm25v.tx_buf[2] = fm25v.flash_addr & 0xFF;
                    fm25v.transfer_idx = 0;
                    for(uint16_t i=0; i<fm25v.transfer_length; i++)
                    {
                        fm25v.tx_buf[3+i] = fm25v.transfer_buf[i];
                    }
                    fm25v.spi_t.len_w = 3 + fm25v.transfer_length;
                    fm25v.spi_t.len_r = 0;
                    spi_transmit(&fm25v.spi_t);
                    break;
                case 1:
                    fm25v.writing_flag = FALSE;
                    fm25v.status = FM25V_STATE_IDLE;
                    spi_slave_unselect(fm25v.spi_t.slave_idx);
                    break;
                case 2:    /* read status register */
                    spi_slave_unselect(fm25v.spi_t.slave_idx);
                    fm25v.status_idx = 0;
                    fm25v.spi_t.rx_buf = fm25v.rx_buf;
                    fm25v.spi_t.tx_buf = fm25v.tx_buf;
                    fm25v.tx_buf[0] = FM25V_RDSR;
                    fm25v.spi_t.len_w = 1;
                    fm25v.spi_t.len_r = 1;
                    spi_transceive(&fm25v.spi_t);
                    break;
                default:
                    fm25v.error_cnt++;
                    fm25v.status = FM25V_STATE_IDLE;
                    spi_slave_unselect(fm25v.spi_t.slave_idx);
                  break;
              }
              break;

        case FM25V_STATE_READ_BYTES:
              for(uint16_t idx=0; idx < fm25v.spi_t.len_r; idx++)
              {
                  fm25v.input_temp_buf[idx] = fm25v.rx_buf[idx+3];
              }
              fm25v.status = FM25V_STATE_IDLE;
            fm25v.reading_flag = FALSE;
            spi_slave_unselect(fm25v.spi_t.slave_idx);
            break;

        case FM25V_STATE_READ_RDSR:
            ;
            break;
        default:
            break;
    }
}

/**
  * @brief  fm25v write data,timeout blocking mode.
  * @param  addr:
              buf:
              len:
  * @retval 0:success, others:fail.
  */
uint8_t fm25v_write(uint16_t addr, const uint8_t *buf, uint16_t len)
{
    volatile uint16_t timeout;
    
    if(fm25v.status != FM25V_STATE_IDLE)
    {
        fm25v.error_cnt++;
        return 2;
    }

    if(len >= FM25V_BUF_SIZE)
    {
        fm25v.error_cnt++;
        return 3;
    }

    // Set the transfer buffer
    fm25v.transfer_buf = (uint8_t *)buf; // Not copied so keep buffer available!
    fm25v.transfer_idx = 0;
    fm25v.transfer_length = len;
    fm25v.flash_addr = addr;

    // Enable writing
    fm25v.status = FM25V_STATE_WRITE_BYTES;
    fm25v.status_idx = 0;
    fm25v.tx_buf[0] = FM25V_WREN;
    fm25v.spi_t.len_w = 1;
    fm25v.spi_t.len_r = 0;
    fm25v.writing_flag = TRUE;
    spi_transmit(&fm25v.spi_t);
        
    timeout = (uint16_t)(fm25v.transfer_length*0.4f + FM25V_TIMEOUT_US) * FM25V_CNT_PER_US;
    uint16_t tm_cnt = 0;
    while( fm25v.writing_flag && (++tm_cnt < timeout) );

    if(fm25v.writing_flag)
    {
        fm25v.error_cnt++;
        return 1;    //Fail
    }
    else
    {
        return 0;    //Success
    }
}

/**
  * @brief  fm25v read data,timeout blocking mode.
  * @param  addr: fm25v01 memory address.
              buf: pointer to store read buffer.
              len:
  * @retval 0:success, others:fail.
  */
uint8_t fm25v_read(uint16_t addr, uint8_t *buf, uint16_t len)
{
    volatile uint16_t timeout;

    if(fm25v.status != FM25V_STATE_IDLE)
    {
        fm25v.error_cnt++;
        return 2;
    }

    if(len >= FM25V_BUF_SIZE)
    {
        fm25v.error_cnt++;
        return 3;
    }

    fm25v.input_temp_buf = buf;
    fm25v.spi_t.rx_buf = fm25v.rx_buf;
    fm25v.status = FM25V_STATE_READ_BYTES;
    fm25v.status_idx = 0;
    fm25v.tx_buf[0] = FM25V_READ;
    fm25v.tx_buf[1] = ((addr&0x3F) >> 8) & 0xFF;
    fm25v.tx_buf[2] = addr & 0xFF;
    
    fm25v.spi_t.len_w = 3;
    fm25v.spi_t.len_r = len;
    fm25v.reading_flag = TRUE;
    spi_transceive(&fm25v.spi_t);
    
    timeout = (uint16_t)(fm25v.spi_t.len_r*0.4f + FM25V_TIMEOUT_US) * FM25V_CNT_PER_US;
    
    uint16_t tm_cnt = 0;
    while( fm25v.reading_flag && (++tm_cnt < timeout) );
    
    if(fm25v.reading_flag)
    {
        fm25v.error_cnt++;
        return 1;    //Fail
    }
    else
    {
        return 0;    //Success
    }
    
}

/* Private functions ---------------------------------------------------------*/
static void fm25_spi_error_cb(SPI_HandleTypeDef *object)
{
    spi_slave_unselect(fm25v.spi_t.slave_idx);
    fm25v.spi_t.error++;
    fm25v.status = FM25V_STATE_IDLE;
}


/****************************** END OF FILE ***********************************/
