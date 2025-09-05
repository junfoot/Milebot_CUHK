/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : internal_flash.c
* Author      : LisonLiu
* Version     : v0.01
* Date        : 2020/6/30
* Description : 
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2020/6/30  LisonLiu    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "mcu_if.h"
#include "stm32f4xx_hal.h"
#include "internal_flash.h"


/* Global variables ----------------------------------------------------------*/

//存储的参数值的buf 
uint8_t param_buf[PARAM_BUF_LEN] = {0};

//设备SN 号
uint8_t device_SN_buf[DEVICE_SN_LEN + 1] = {0};


/* Private function prototypes -----------------------------------------------*/
static uint32_t GetSector(uint32_t Address);


/* Private functions ---------------------------------------------------------*/

//擦除指定扇区
void erase_sector(uint32_t sector_addr, uint32_t sector_num)
{
    uint32_t SectorError = 0;
    uint32_t first_sector = 0;

    /*Variable used for Erase procedure*/
    static FLASH_EraseInitTypeDef EraseInitStruct;

    /* Unlock the Flash to enable the flash control register access *************/ 
    HAL_FLASH_Unlock();

    first_sector = GetSector(sector_addr);

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = first_sector;
    EraseInitStruct.NbSectors = sector_num;
    if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
    { 
        /* 
        Error occurred while sector erase. 
        User can add here some code to deal with this error. 
        SectorError will contain the faulty sector and then to know the code error on this sector,
        user can call function 'HAL_FLASH_GetError()'
        */
        /*
        FLASH_ErrorTypeDef errorcode = HAL_FLASH_GetError();
        */
        _Error_Handler(__FILE__, __LINE__);
    }

    /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
    you have to make sure that these data are rewritten before they are accessed during code
    execution. If this cannot be done safely, it is recommended to flush the caches by setting the
    DCRST and ICRST bits in the FLASH_CR register. */
    __HAL_FLASH_DATA_CACHE_DISABLE();
    __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();

    __HAL_FLASH_DATA_CACHE_RESET();
    __HAL_FLASH_INSTRUCTION_CACHE_RESET();

    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
    __HAL_FLASH_DATA_CACHE_ENABLE();

    /* Lock the Flash to disable the flash control register access (recommended
    to protect the FLASH memory against possible unwanted operation) *********/
    HAL_FLASH_Lock(); 

}

//不擦除扇区，直接写
void write_internal_flash_without_erase(uint32_t addr, uint8_t* buf, uint16_t len)
{
    uint32_t Address = 0;
    uint32_t temp_write_data = 0;
    uint8_t i = 0;

    /* Unlock the Flash to enable the flash control register access *************/ 
    HAL_FLASH_Unlock();

    /* Program the user Flash area word by word, 4 个字节*/

    Address = addr;
    for(i = 0; i < (len+3)/4; i++)
    {
        temp_write_data = buf[i*4 + 3];
        temp_write_data = (temp_write_data << 8) + buf[i*4 + 2];
        temp_write_data = (temp_write_data << 8) + buf[i*4 + 1];
        temp_write_data = (temp_write_data << 8) + buf[i*4 + 0];
        
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, temp_write_data) == HAL_OK)
        {
            Address = Address + 4;
        }
        else
        { 
            /* Error occurred while writing data in Flash memory. 
            User can add here some code to deal with this error */
            /*
            FLASH_ErrorTypeDef errorcode = HAL_FLASH_GetError();
            */
            _Error_Handler(__FILE__, __LINE__);
        }
    }

    /* Lock the Flash to disable the flash control register access (recommended
    to protect the FLASH memory against possible unwanted operation) *********/
    HAL_FLASH_Lock(); 

}


/*
    往固定的扇区里写入数据
    注意: 由于写flash 前要把整个扇区擦除, 所以一定要注意, 
    写flash 前要把之前的flash 读到全局buf 中, 然后修改buf 的某些数据, 
    修改完成后, 再把全局buf 写进flash
*/
void write_internal_flash(uint32_t addr, uint8_t* buf, uint8_t len)
{
    uint32_t FirstSector = 0;
    uint32_t NbOfSectors = 1;  //只操作一个扇区
    uint32_t SectorError = 0;

    uint32_t Address = 0;
    uint32_t temp_write_data = 0;
    uint8_t i = 0;
    
    /*Variable used for Erase procedure*/
    static FLASH_EraseInitTypeDef EraseInitStruct;

    /* Unlock the Flash to enable the flash control register access *************/ 
    HAL_FLASH_Unlock();

    FirstSector = GetSector(addr);

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = FirstSector;
    EraseInitStruct.NbSectors = NbOfSectors;
    if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
    { 
        /* 
        Error occurred while sector erase. 
        User can add here some code to deal with this error. 
        SectorError will contain the faulty sector and then to know the code error on this sector,
        user can call function 'HAL_FLASH_GetError()'
        */
        /*
        FLASH_ErrorTypeDef errorcode = HAL_FLASH_GetError();
        */
        _Error_Handler(__FILE__, __LINE__);
    }

    /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
    you have to make sure that these data are rewritten before they are accessed during code
    execution. If this cannot be done safely, it is recommended to flush the caches by setting the
    DCRST and ICRST bits in the FLASH_CR register. */
    __HAL_FLASH_DATA_CACHE_DISABLE();
    __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();

    __HAL_FLASH_DATA_CACHE_RESET();
    __HAL_FLASH_INSTRUCTION_CACHE_RESET();

    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
    __HAL_FLASH_DATA_CACHE_ENABLE();

    /* Program the user Flash area word by word, 4 个字节*/
    Address = addr;
    for(i = 0; i < (len+3)/4; i++)
    {
        temp_write_data = buf[i*4 + 3];
        temp_write_data = (temp_write_data << 8) + buf[i*4 + 2];
        temp_write_data = (temp_write_data << 8) + buf[i*4 + 1];
        temp_write_data = (temp_write_data << 8) + buf[i*4 + 0];
        
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, temp_write_data) == HAL_OK)
        {
            Address = Address + 4;
        }
        else
        { 
            /* Error occurred while writing data in Flash memory. 
            User can add here some code to deal with this error */
            /*
            FLASH_ErrorTypeDef errorcode = HAL_FLASH_GetError();
            */
            _Error_Handler(__FILE__, __LINE__);
        }
    }

    /* Lock the Flash to disable the flash control register access (recommended
    to protect the FLASH memory against possible unwanted operation) *********/
    HAL_FLASH_Lock(); 

}


//读取固定扇区里的数据
void read_internal_flash(uint32_t addr, uint8_t* buf, uint8_t len)
{
    uint32_t Address = 0;
    uint32_t temp_read_data = 0;
    uint8_t i = 0;
    
    Address = addr;

    /* read the user Flash area word by word, 4 个字节*/
    for(i = 0; i < (len+3)/4; i++)
    {
        temp_read_data = *(__IO uint32_t*)Address;
        buf[i*4 + 0] = (uint8_t)(temp_read_data >> 0);
        buf[i*4 + 1] = (uint8_t)(temp_read_data >> 8);
        buf[i*4 + 2] = (uint8_t)(temp_read_data >> 16);
        buf[i*4 + 3] = (uint8_t)(temp_read_data >> 24);

        Address = Address + 4;
    }
    
}


/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_SECTOR_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_SECTOR_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_SECTOR_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_SECTOR_10;  
  }
  else /* (Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11) */
  {
    sector = FLASH_SECTOR_11;
  }

  return sector;
}



