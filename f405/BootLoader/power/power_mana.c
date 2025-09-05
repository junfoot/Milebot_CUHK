/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : power_mana.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : power management.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/
#include "cmsis_os2.h"                  // ARM::CMSIS:RTOS2:Keil RTX5


/* Public Includes -----------------------------------------------------------*/
#include "main.h"
#include "task_if.h"
#include "internal_flash.h"
#include "ble_if.h"  
#include "mcu_if.h"


/* Private Includes ----------------------------------------------------------*/
#include "power_mana.h"
#include "power_mana_if.h"


/* Global variables ----------------------------------------------------------*/


/* Global functions ----------------------------------------------------------*/

/**
  * @brief  task to handle all power function .
  * @param  None
  * @retval None
  */
void power_task(void *argument)
{
    uint32_t led_heartbeat_tick = 0;
    
    pw_reset_create();
    
    while(1)
    {
        if((HAL_GetTick() - led_heartbeat_tick) >= 100)
        {
            led_heartbeat_tick = HAL_GetTick();
            LED_HEARTBEAT_Toggle();
            STATLED_Toggle();
        }
        
        osDelay(20);
    }
}


/**
  * @brief  handle power reset.
  * @param  None
  * @retval None
  */
static void pw_reset_create(void)
{
    BT_WAKEUP_enable();
    BT_POW_enable();

    osDelay(PW_POWER_SUPPLY_STABLE_TIME);
    mcu_init();
    task_create_all();
}


