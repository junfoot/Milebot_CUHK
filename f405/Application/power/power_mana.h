/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : power_mana.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : private header file of power_mana.c
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _POWER_MANA_H_
#define _POWER_MANA_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include "power_mana_if.h"    
/**** Definition of constants ****/


/**** Definition of types ****/ 


/**** Definition of macros ****/
#define PW_POWER_SUPPLY_STABLE_TIME        50    /* 50ms */

typedef enum
{  
    WDG_TASK_COMM,
    WDG_TASK_WALK,
    WDG_TASK_CAN,
    WDG_TASK_BLE,
    WDG_TASK_SYS,
    WDG_TASK_ENG,
    WDG_TASK_MAX,   

    WDG_TASK_MMI,  /* not used */
}WDG_TASK_IDS;
typedef enum
{  
    PRESS,
    DETACH,      
}POWER_SWITCH;

#define PW_WATCHDOG_RUN_NORMAL         (~(0xFFFF<<WDG_TASK_MAX)) 


//按键的状态(消除抖动后)
typedef enum
{
    KEY_PRESS = 0,
    KEY_RELEASE
} KEY_CURRENT_STATUS;

//每间隔5 ms , 扫描一次按键
#define KEY_SCAN_TIME   5

//按键消抖的次数(连续间隔KEY_SCAN_TIME 毫秒的检测次数)
#define KEY_SCAN_NUM    3

//按键管脚的状态(按键被按下后, 读取到低电平)
typedef enum
{
    KEY_PIN_RELEASE = 0,
    KEY_PIN_PRESS
} KEY_PIN_STATUS;


/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
static void pw_reset_create(void);
void pw_all_modules_reset_create(void);
void bat_status(POWER_INFO* power);
#ifdef __cplusplus
}
#endif

#endif /*_POWER_MANA_H_*/

/****************************** END OF FILE ***********************************/
