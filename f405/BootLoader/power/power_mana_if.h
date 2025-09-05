/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : power_mana_if.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : extern header file of power_mana interface.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _POWER_MANA_IF_H_
#define _POWER_MANA_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
     
/**** Definition of constants ****/


/**** Definition of types ****/ 
typedef struct 
{        
    uint16_t task_status;    
    uint16_t volt;
    float filtered_volt;
    uint8_t battery_charge_state[2];
    uint8_t battery_charge_moment;
    uint8_t power_off;
}POWER_INFO;

//电池电量等级
typedef enum
{
    BATTERY_LEVEL_0 = 0,
    BATTERY_LEVEL_1,
    BATTERY_LEVEL_2,
    BATTERY_LEVEL_3,
    BATTERY_LEVEL_4,
    BATTERY_LEVEL_5,
} BATTERY_LEVEL_STATUS; 


/**** Definition of macros ****/


/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
extern void power_task(void *argument);
BATTERY_LEVEL_STATUS get_bat_level(void);


#ifdef __cplusplus
}
#endif

#endif /*_POWER_MANA_IF_H_*/

/****************************** END OF FILE ***********************************/
