/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : bat_if.h
* Author      : wb
* Version     : v0.01
* Date        : 2018/3/7
* Description : extern header file of smbus.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2018/3/7  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _BAT_IF_H_
#define _BAT_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include <stdint.h>
     
/**** Definition of constants ****/


/**** Definition of macros ****/

//#define USE_OLD_BAT             //是否使用旧的电池


#ifdef USE_OLD_BAT
#define BAT_SMBUS_ADDR        0x0B
#else
#define BAT_SMBUS_ADDR        0x55
#endif

/**** Definition of types ****/ 
enum BQ78350_SBS_CMD
{
    SBS_CMD_REMAINING_CAPACITY_ALARM = 0x01,
    SBS_CMD_TEMP = 0x08,
    SBS_CMD_VOLT = 0x09,
    SBS_CMD_CURRENT = 0x0A,
    SBS_CMD_AVR_CURR = 0x0B,
    SBS_CMD_RELATIVE_PERCENT = 0x0D,
    SBS_CMD_ABS_PERCENT = 0x0E,
    SBS_CMD_REMAINING_CAPACITY = 0x0F,
    SBS_CMD_FULL_CHARGE_CAPACITY = 0x10,
    SBS_CMD_CHARGING_CURRENT = 0x14,
    SBS_CMD_CHARGING_VOLT = 0x15,
    SBS_CMD_BAT_STATUS = 0x16,
};


typedef struct
{
    uint8_t status;
    uint16_t bat_status;
    uint8_t relative_percent;
    uint8_t abs_percent;
    uint16_t remaining;
    uint16_t fullcharge;
    uint16_t volt;
    int16_t curr;
    int16_t avrcurr;
    uint16_t temp;
    uint16_t remaining_capacity_alarm;
    uint16_t error;
    uint16_t vcc;
    uint16_t vc[7];
    
}BAT_INFO;

enum BAT_CMD_STATUS_IDX
{
    BAT_CMD_INIT,
    BAT_CMD_VOLT,
//    BAT_CMD_ABS_PERCENT,
    BAT_CMD_DONE,
    BAT_CMD_REMAINING_CAPACITY,
    BAT_CMD_FULL_CHARGE_CAPACITY,
    BAT_CMD_TEMP,
    BAT_CMD_CURRENT,
    BAT_CMD_AVR_CURR,
    BAT_CMD_STATUS,
    BAT_CMD_RELATIVE_PERCENT,
    
    //BAT_CMD_DONE,
};

/**** Declaration of constants ****/


/**** Declaration of variables ****/

/**** Declaration of functions ****/
extern void bat_init(void);
extern void bat_polling(void);
extern uint16_t return_bat_volt(void);
extern uint16_t return_bat_remaining_capacity(void);
extern uint16_t return_bat_fullcharge_capacity(void);
extern uint8_t return_bat_abs_percent(void);

#ifdef __cplusplus
}
#endif

#endif /*_BAT_IF_H_*/

/****************************** END OF FILE ***********************************/

