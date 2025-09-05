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
#include "..\walk\walk.h"
#include "..\system\sys_task_if.h"
#include "..\modules\can\can_app_if.h"
#include "..\modules\mitjoint\mitapp.h"
#include "syn6658.h"
#include "..\comm\comm.h"
#include "ble_if.h"  

/* Private Includes ----------------------------------------------------------*/
#include "..\os\task.h"
#include "..\os\task_if.h"   

/* Global variables ----------------------------------------------------------*/
/* Declare all RTX mailbox              */

/* All mutex             */
osMutexId_t mutex_fram;
osMutexId_t mutex_comm;

/* Private variables ---------------------------------------------------------*/
/* All task ID              */
static osThreadId_t tid_power; 
static osThreadId_t tid_mit;
static osThreadId_t tid_walk;
static osThreadId_t tid_comm;
static osThreadId_t tid_sound;
static osThreadId_t tid_ble;
static osThreadId_t tid_sys;

/* All task stack             */
static uint64_t stk_power[1000/8];    // stack size = 125
static uint64_t stk_mit[2000/8];
static uint64_t stk_walk[4000/8];    // stack size = 250
static uint64_t stk_comm[1000/8];      // stack size = 125
static uint64_t stk_sound[1000/8];
static uint64_t stk_ble[1000/8];
static uint64_t stk_sys[1000/8];


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

osThreadAttr_t tid_mit_attr = 
{ 
    .name = "thread_mit",
    .attr_bits = osThreadJoinable,
    .cb_mem = NULL,
    .cb_size = 0,
    .stack_mem = stk_mit,
    .stack_size = sizeof(stk_mit),
    .priority = (osPriority_t)SCA_TASK_PRIORITY,    
    .tz_module = 0,
    .reserved = 0
};

osThreadAttr_t tid_walk_attr = 
{ 
    .name = "thread_walk",
    .attr_bits = osThreadJoinable,
    .cb_mem = NULL,
    .cb_size = 0,
    .stack_mem = stk_walk,
    .stack_size = sizeof(stk_walk),
    .priority = (osPriority_t)WALK_TASK_PRIORITY,    
    .tz_module = 0,
    .reserved = 0
};

osThreadAttr_t tid_comm_attr = 
{ 
    .name = "thread_comm",
    .attr_bits = osThreadJoinable,
    .cb_mem = NULL,
    .cb_size = 0,
    .stack_mem = stk_comm,
    .stack_size = sizeof(stk_comm),
    .priority = (osPriority_t)COMM_TASK_PRIORITY,    
    .tz_module = 0,
    .reserved = 0
};

osThreadAttr_t tid_sound_attr = 
{ 
    .name = "thread_sound",
    .attr_bits = osThreadJoinable,
    .cb_mem = NULL,
    .cb_size = 0,
    .stack_mem = stk_sound,
    .stack_size = sizeof(stk_sound),
    .priority = (osPriority_t)SOUND_TASK_PRIORITY,    
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

osThreadAttr_t tid_sys_attr = 
{ 
    .name = "thread_sys",
    .attr_bits = osThreadJoinable,
    .cb_mem = NULL,
    .cb_size = 0,
    .stack_mem = stk_sys,
    .stack_size = sizeof(stk_sys),
    .priority = (osPriority_t)SYS_TASK_PRIORITY,    
    .tz_module = 0,
    .reserved = 0
};



/* Global functions ----------------------------------------------------------*/
/**
  * @brief  create power_tast and initialize tasks' variables.
  * @param  None
  * @retval None
  */
void task_init_power(void *argument)
{  
    mutex_fram = osMutexNew(NULL);
    mutex_comm = osMutexNew(NULL);     
    tid_power = osThreadNew(power_task, NULL, &tid_power_attr);
}

/**
  * @brief  create all tast except power_task.
  * @param  None
  * @retval None
  */
void task_create_all(void)
{
    tid_walk = osThreadNew(walk_task, NULL, &tid_walk_attr);
    tid_mit = osThreadNew(mit_task, NULL, &tid_mit_attr);
    tid_comm = osThreadNew(comm_task, NULL, &tid_comm_attr);
    tid_sound = osThreadNew(sound_task, NULL, &tid_sound_attr);
    tid_ble = osThreadNew(ble_task, NULL, &tid_ble_attr);
    tid_sys = osThreadNew(sys_task, NULL, &tid_sys_attr);
}

/**
  * @brief  kill all task.
  * @param  None
  * @retval None
  */
void task_kill_all(void)
{  	
    osThreadTerminate(tid_mit);
    osThreadTerminate(tid_walk);
    osThreadTerminate(tid_power);
    osThreadTerminate(tid_comm);
    osThreadTerminate(tid_sound);
    osThreadTerminate(tid_ble);
    osThreadTerminate(tid_sys);
}
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  mcu xx func config.
  * @param  None
  * @retval None
  */


/****************************** END OF FILE ***********************************/

