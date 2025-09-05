/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : task.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : task create
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/
#include "cmsis_os2.h"                  // ARM::CMSIS:RTOS2:Keil RTX5

/* Public Includes -----------------------------------------------------------*/
#include "..\power\power_mana_if.h"
#include "ble_if.h"  

/* Private Includes ----------------------------------------------------------*/
#include "..\os\task.h"
#include "..\os\task_if.h"   

/* Global variables ----------------------------------------------------------*/
/* Declare all RTX mailbox              */

/* Private variables ---------------------------------------------------------*/
/* All task ID              */
static osThreadId_t tid_power; 
static osThreadId_t tid_ble;

/* All task stack             */
static uint64_t stk_power[1000/8];    // stack size = 125
static uint64_t stk_ble[1000/8];


/* Attribute of thread power */
osThreadAttr_t tid_power_attr = 
{ 
    .name = "thread_power",
    .attr_bits = osThreadJoinable,
    .cb_mem = NULL,
    .cb_size = 0,
    .stack_mem = stk_power,
    .stack_size = sizeof(stk_power),
    .priority = (osPriority_t)POWER_TASK_PRIORITY,    
    .tz_module = 0,
    .reserved = 0
};


osThreadAttr_t tid_ble_attr = 
{ 
    .name = "thread_ble",
    .attr_bits = osThreadJoinable,
    .cb_mem = NULL,
    .cb_size = 0,
    .stack_mem = stk_ble,
    .stack_size = sizeof(stk_ble),
    .priority = (osPriority_t)BLE_TASK_PRIORITY,    
    .tz_module = 0,
    .reserved = 0
};


/* Global functions ----------------------------------------------------------*/
/**
  * @brief  create power_tast and initialize tasks' variables.
  * @param  None
  * @retval None
  */
void task_init_power(void)
{  
    tid_power = osThreadNew(power_task, NULL, &tid_power_attr);
}

/**
  * @brief  create all tast except power_task.
  * @param  None
  * @retval None
  */
void task_create_all(void)
{
    tid_ble = osThreadNew(ble_task, NULL, &tid_ble_attr);
}

/**
  * @brief  kill all task.
  * @param  None
  * @retval None
  */
void task_kill_all(void)
{  	
    osThreadTerminate(tid_power);
    osThreadTerminate(tid_ble);
}


