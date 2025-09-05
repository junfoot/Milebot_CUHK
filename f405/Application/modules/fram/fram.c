/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : fram.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/7/26
* Description : fram interface file.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     201911/26  wb    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/
#include <string.h>
#include <stddef.h>
#include "cmsis_os2.h"  

/* Public Includes -----------------------------------------------------------*/
#include "..\..\mcu\spi_if.h"
#include "..\..\os\task_if.h"

/* Private Includes ----------------------------------------------------------*/
#include "fm25v_if.h"
#include "fram_data.h"
#include "fram.h"
#include "fram_if.h"
#define FRAM_MACROS

#include "fram_class.h"
#include "fram_def.h"
#include "fram_class.h"
#include "fram_def.h"
#include "fram_class.h"
#include "fram_def.h"
#include "fram_class.h"
#include "fram_def.h"
#include "fram_class.h"
#include "fram_def.h"
#include "fram_class.h"
#include "fram_def.h"
#include "fram_class.h"
#include "fram_def.h"
#include "fram_class.h"
#include "fram_def.h"
#include "fram_class.h"
#include "fram_def.h"

/* Global variables ----------------------------------------------------------*/

const uint8_t fram_init_flags[4] =
{0x55,0xAA,0x5A,0xA5};//need init the whole fram 

#ifdef UPGRADE_OPTION
const uint8_t cl_swdl_mask_array[] =
{
   0xAA,0x55,0xA5,0x5A,
   0xAA,0xAA,0xAA,0xAA,
   0x55,0x55,0x55,0x55,
   0xFF,0xFF,0xFF,0xFF,
};

const uint8_t clear_update_flag_array[] =
{
  0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,              
};
#endif    /* UPGRADE_OPTION */

/* Private variables ---------------------------------------------------------*/
const FRAM_DATA_INIT_TYPE fram_data_section[FRAM_DATA_INIT_SECTION_MAX] = 
{
    {FRAM_DATA_INIT_SECTION_ONE, CL_FRAM_RESERVE1, CL_FRAM_RESERVE2},
};

/* Global functions ----------------------------------------------------------*/
/**
  * @brief  fram init
  * @param  None
  * @retval None
  */
void fram_init(void)
{  
    spi1_init();
    fm25v_init(&hspi1, FRAM_SPI_SLAVE_IDX);
}

uint8_t fram_flag = 0;    /* for test */
uint8_t cl_mcu_fault[20];
uint8_t cl_serial_number[20] = "hello,robot!";
/**
  * @brief  fram polling
  * @param  None
  * @retval None
  */
void fram_polling(void)
{
    if(fram_flag == 1)
    {
        fram_flag = 0;
        fram_sn_data_write(cl_serial_number);
    }

    if(fram_flag == 2)
    {
        fram_flag = 0;
        fram_sn_data_read(cl_mcu_fault);
    }

    if(fram_flag == 3)
    {
        fram_flag = 0;
        fram_read_mcu_fault(cl_mcu_fault);
    }

    if(fram_flag == 4)
    {
        fram_flag = 0;
        fram_sn_data_read(cl_mcu_fault);
    }
}

/**
  * @brief  fram write data for id.
  * @param  id:
              writte_buffer
  * @retval 0-success, others-fail 
  */
uint8_t fram_id_write(uint8_t id, const uint8_t *write_buffer)
{
    uint8_t ret_val = 0;
    uint16_t addr = fram_get_address(id, 0);
    uint16_t length = object_quantity[id] * object_size[id];
    osStatus_t status;

    status = osMutexAcquire(mutex_fram, FRAM_MUTEX_TIMEOUT);
    if(osOK == status)
    {
        ret_val = fm25v_write(addr, write_buffer , length);
    }
    osMutexRelease (mutex_fram);
    return ret_val;
}

/**
  * @brief  write whole id data.
  * @param  id:
              writte_buffer:
  * @retval 0-success, others-fail 
  */
uint8_t fram_write(uint8_t id, uint16_t item, const uint8_t *write_buffer)
{
    uint8_t ret_val = 0;
    uint16_t addr = fram_get_address(id, item);
    uint16_t length = object_size[id];
    osStatus_t status;

    status = osMutexAcquire(mutex_fram, FRAM_MUTEX_TIMEOUT);
    if(osOK == status)
    {    
        ret_val = fm25v_write(addr, write_buffer , length);
    }
    osMutexRelease (mutex_fram);
    return ret_val;
}

/**
  * @brief  fram read id data.
  * @param  id:
              writte_buffer:
  * @retval 0-success, others-fail 
  */
uint8_t fram_read(uint8_t id, uint16_t item, uint8_t *read_buffer)
{
    uint8_t ret_val = 0;
    uint16_t addr = fram_get_address(id, item);
    uint16_t length = object_size[id];
    osStatus_t status;

    status = osMutexAcquire(mutex_fram, FRAM_MUTEX_TIMEOUT);
    if(osOK == status)
    {
        ret_val = fm25v_read(addr, read_buffer, length);
    }
    osMutexRelease (mutex_fram);
    return ret_val;
}

/**
  * @brief  fram read id data.
  * @param  id:
              read_buffer:
  * @retval 0-success, others-fail 
  */
uint8_t fram_id_read(uint8_t id, uint8_t *read_buffer)
{
    uint8_t ret_val = 0;
    uint16_t addr = fram_get_address(id, 0);
    uint16_t length = object_quantity[id] * object_size[id];
    osStatus_t status;

    status = osMutexAcquire(mutex_fram, FRAM_MUTEX_TIMEOUT);
    if(osOK == status)
    {
        ret_val = fm25v_read(addr, read_buffer, length);
    }
    osMutexRelease (mutex_fram);
    return ret_val;
}

/**
  * @brief  factory reset fram data.
  * @param  items:
  * @retval none 
  */
void fram_factory_reset_init(uint16_t items)
{
    uint8_t index1,index2;
    const uint8_t* temp_pointer;
    for(index1 = 0; index1 < FRAM_DATA_INIT_SECTION_MAX; index1++)
    {
        if(!(items & (1<<index1)))
        {
            continue;
        }
        for( index2 = fram_data_section[index1].fram_id_start; index2 <= fram_data_section[index1].fram_id_end; index2++)
        {
            temp_pointer = cl_data_array[index2];
            fram_id_write(index2,(uint8_t *)temp_pointer);
        }
    }
    fram_write(CL_FRAM_INIT_FLAG, 0x00, (uint8_t *)cl_fram_init_flag_array);     
}

/**
  * @brief  init all the fram data
  * @param  none
  * @retval none 
  */
void fram_init_all_data(void)
{
    const uint8_t* temp_pointer;
    uint8_t temp_software_version_array[0x11];
    uint8_t temp_fram_init_flags[4];
    
    fram_read(CL_SOFTWARE_VERSION, 0x00, temp_software_version_array);
    temp_software_version_array[0x10] = '\0';
    
    if(!memcmp((const uint8_t *)cl_software_version_array, (const uint8_t *)temp_software_version_array,0x10))
    {
        //current software version is the same as fram.
        fram_read(CL_FRAM_INIT_FLAG, 0x00, temp_fram_init_flags);
        if(memcmp((const uint8_t *)fram_init_flags, (const uint8_t *)temp_fram_init_flags,4) != 0)
        {
            /*TODOM: reset fram data*/
            
            if(memcmp((const uint8_t *)&fram_init_flags[2], (const uint8_t *)&temp_fram_init_flags[2],2) == 0)
            {
                /* need to reset special data section */
                fram_factory_reset_init(*((uint16_t *)temp_fram_init_flags));
            }
         }    
    }
    else
    {
        temp_pointer = cl_data_array[CL_SOFTWARE_VERSION];
        fram_id_write(CL_SOFTWARE_VERSION,(uint8_t *)temp_pointer);

    }
    
}

/**
  * @brief  fram write searial number.
  * @param  none
  * @retval 0-success, others-fail  
  */
uint8_t fram_sn_data_write(uint8_t *buffer)
{
    return fram_write(CL_PRODUCT_SERIES_NUMBER, 0, buffer);
}

/**
  * @brief  fram read searial number.
  * @param  none
  * @retval 0-success, others-fail  
  */
uint8_t fram_sn_data_read(uint8_t *buffer)
{
    return fram_read(CL_PRODUCT_SERIES_NUMBER, 0, buffer);
}

/**
  * @brief  fram write mcu fault data.
  * @param  none
  * @retval 0-success, others-fail  
  */
uint8_t fram_write_mcu_fault(uint8_t *buffer)
{
    return fram_write(CL_MCU_FAULT_DATA, 0, buffer);
}

/**
  * @brief  fram read fault data.
  * @param  none
  * @retval 0-success, others-fail  
  */
uint8_t fram_read_mcu_fault(uint8_t *buffer)
{
    return fram_read(CL_MCU_FAULT_DATA, 0, buffer);
}

/**
  * @brief  fram write system setting data 
  * @param  none
  * @retval 0-success, others-fail  
  */
uint8_t fram_write_system_setting(uint8_t *buffer)
{
    return fram_write(CL_FRAM_SYSTEM_SETTING, 0, buffer);
}

/**
  * @brief  fram read encorder by Bourns 
  * @param  none
  * @retval 0-success, others-fail  
  */
uint8_t fram_read_system_setting(uint8_t *buffer)
{
    return fram_read(CL_FRAM_SYSTEM_SETTING, 0, buffer);
}

/**
  * @brief  fram write imu calibration data 
  * @param  none
  * @retval 0-success, others-fail  
  */
uint8_t fram_write_imu_cali(uint8_t *buffer)
{
    return fram_write(CL_FRAM_IMU_CALIBRATION, 0, buffer);
}

/**
  * @brief  fram read imu calibration data 
  * @param  none
  * @retval 0-success, others-fail  
  */

uint8_t fram_read_imu_cali(uint8_t *buffer)
{
	return fram_read(CL_FRAM_IMU_CALIBRATION, 0, buffer);
}

/**
  * @brief  fram write encorder by Bourns 
  * @param  none
  * @retval 0-success, others-fail  
  */
uint8_t fram_write_enc_bourns(uint8_t *buffer)
{
    return fram_write(CL_FRAM_ENC_BOURNS, 0, buffer);
}

/**
  * @brief  fram read encorder by Bourns 
  * @param  none
  * @retval 0-success, others-fail  
  */
uint8_t fram_read_enc_bourns(uint8_t *buffer)
{
    return fram_read(CL_FRAM_ENC_BOURNS, 0, buffer);
}

/**
  * @brief  fram write encorder by TAMAGAWA 
  * @param  none
  * @retval 0-success, others-fail  
  */
uint8_t fram_write_enc_tawa(uint8_t *buffer)
{
    return fram_write(CL_FRAM_ENC_TAMAGAWA, 0, buffer);
}

/**
  * @brief  fram read encorder by TAMAGAWA 
  * @param  none
  * @retval 0-success, others-fail  
  */
uint8_t fram_read_enc_tawa(uint8_t *buffer)
{
    return fram_read(CL_FRAM_ENC_TAMAGAWA, 0, buffer);
}
#ifdef UPGRADE_OPTION
/**
  * @brief  fram write sw upgrade data.
  * @param  none
  * @retval 0-success, others-fail  
  */
uint8_t fram_write_swdl_mask(void)
{
    uint8_t buf[16]=
    {
       0xAA,0x55,0xA5,0x5A,
       0xAA,0xAA,0xAA,0xAA,
       0x55,0x55,0x55,0x55,
       0xFF,0xFF,0xFF,0xFF,
    };
    
    return ( fram_write(CL_SOFTWARE_UPDATE_FLAG, 0, buf) );
}

/**
  * @brief  fram erase sw upgrade data.
  * @param  none
  * @retval 0-success, others-fail  
  */
uint8_t fram_erase_swdl_mask (void)
{   
    return ( fram_write(CL_SOFTWARE_UPDATE_FLAG, 0, clear_update_flag_array) );
}

/**
  * @brief  fram read sw upgrade data.
  * @param  none
  * @retval 0-success, others-fail  
  */
uint8_t fram_read_swdl_mask (uint8_t* pBlockData)
{
    return ( fram_read(CL_SOFTWARE_UPDATE_FLAG, 0, pBlockData) );
}

/**
  * @brief  check weather sw upgrade is available.
  * @param  none
  * @retval 0-success, others-fail  
  */
bool_t fram_update_is_available(void)
{
    uint8_t swdl_mask[16];
    bool_t retval = FALSE;
    uint8_t i;
    bool_t swdl_get_fram_error = FALSE;

    for(i=0; i<3; i++)
    {
        swdl_get_fram_error = FALSE;               
        if( fram_read_swdl_mask(swdl_mask) == 0)
        {
            break;
        }
        else
        {
            swdl_get_fram_error = TRUE;
            delay_us(200);                 
        }
    }
    
    if(swdl_get_fram_error == FALSE)
    {
        if(memcmp(swdl_mask,cl_swdl_mask_array,16) == 0)  
        {//these two variables are the same
            retval = TRUE;
        }              
    }

    return (retval);
}
#endif    /* UPGRADE_OPTION */

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  get the fram address where the information should be sent.
  * @param  id:
              item:
  * @retval 0-success, others-fail 
  */
static uint16_t fram_get_address (uint8_t id, uint16_t item)
{
    return (object_base[id] + item * object_size[id]);
}

/****************************** END OF FILE ***********************************/