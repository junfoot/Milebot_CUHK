/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : power_drv_if.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : extern header file of power_drv.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _POWER_DRV_IF_H_
#define _POWER_DRV_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include "..\mcu\mcu_if.h"
     
/**** Definition of constants ****/


/**** Definition of types ****/ 


/**** Definition of macros ****/
#if 0   /* Begin if 0 by lison on 2020-6-17 */
//#define PW_LLEG_MOTOR_POWER_ON()   HAL_GPIO_WritePin(LLEG_MOTOR_PWR_EN_GPIO_Port,LLEG_MOTOR_PWR_EN_Pin,GPIO_PIN_SET)
//#define PW_LLEG_MOTOR_POWER_OFF()  HAL_GPIO_WritePin(LLEG_MOTOR_PWR_EN_GPIO_Port,LLEG_MOTOR_PWR_EN_Pin,GPIO_PIN_RESET)

#define PW_LCD_POWER_ON()     GPIO_SET(LCD_PWR_EN)
#define PW_LCD_POWER_OFF()     GPIO_RESET(LCD_PWR_EN)

#define PW_COLLECTION_POWER_ON()     GPIO_SET(COLLECTION_PWR_EN)
#define PW_COLLECTION_POWER_OFF()     GPIO_RESET(COLLECTION_PWR_EN)


#define PW_CAN1_POWER_ON()         GPIO_SET(CAN1_PWR_EN)
#define PW_CAN1_POWER_OFF()     GPIO_RESET(CAN1_PWR_EN)

#define PW_CAN2_POWER_ON()         GPIO_SET(CAN2_PWR_EN)
#define PW_CAN2_POWER_OFF()     GPIO_RESET(CAN2_PWR_EN)

#define PW_SD_POWER_ON()         GPIO_RESET(SDCARD_PWR_EN)
#define PW_SD_POWER_OFF()     GPIO_SET(SDCARD_PWR_EN)

//#define PW_FAN_POWER_ON()         GPIO_SET(FAN_EN)
//#define PW_FAN_POWER_OFF()     GPIO_RESET(FAN_EN)

#define PW_SYSTEM_POWER_ON()         GPIO_RESET(PWR_OFF_CTL)
#define PW_SYSTEM_POWER_OFF()     GPIO_SET(PWR_OFF_CTL)

#define PW_BLE_POWER_ON()     GPIO_RESET(BLE_3V3_EN)
#define PW_BLE_POWER_OFF()     GPIO_SET(BLE_3V3_EN)

#define PW_LED_ON()     GPIO_RESET(LED_EN)
#define PW_LED_OFF()     GPIO_SET(LED_EN)
#define PW_LED_TOGGLE()    GPIO_TOGGLE(LED_EN)

#define PW_CHARGE_POWER_ON()     GPIO_SET(CHARGE_EN)
#define PW_CHARGE_POWER_OFF()     GPIO_RESET(CHARGE_EN)

#define PW_LLEG_LED_ON()     GPIO_SET(LLEG_BLED)
#define PW_LLEG_LED_OFF()     GPIO_RESET(LLEG_BLED)

#define PW_RLEG_LED_ON()     GPIO_SET(RLEG_BLED)
#define PW_RLEG_LED_OFF()     GPIO_RESET(RLEG_BLED)
#define PW_STARTUP_LED_GREEN_ON()     GPIO_SET(LED_GREEN)
#define PW_STARTUP_LED_GREEN_OFF()     GPIO_RESET(LED_GREEN)
#define PW_STARTUP_LED_RED_ON()     GPIO_SET(LED_RED)
#define PW_STARTUP_LED_RED_OFF()     GPIO_RESET(LED_RED)


#define PW_ALARM_LED_ON()     GPIO_SET(ALARM_LED)
#define PW_ALARM_LED_OFF()     GPIO_RESET(ALARM_LED)

//#define PW_STARTUP_LED_ON()     GPIO_SET(LED_STARTUP)
//#define PW_STARTUP_LED_OFF()     GPIO_RESET(LED_STARTUP)

#define PW_MOTOR_POWER_ON()     GPIO_SET(MOTOR_PWR_CTL)
#define PW_MOTOR_POWER_OFF()     GPIO_RESET(MOTOR_PWR_CTL)

#define PW_MOTOR_UPPWR_ON()     GPIO_SET(MOTOR_UPPWR_EN)
#define PW_MOTOR_UPPWR_OFF()     GPIO_RESET(MOTOR_UPPWR_EN)

#define PW_BEEP_ON()         GPIO_SET(BEEP_CTL)
#define PW_BEEP_OFF()         GPIO_RESET(BEEP_CTL)

#define PW_IMU_POWER_ON()         GPIO_RESET(IMU_PWR_EN)
#define PW_IMU_POWER_OFF()         GPIO_SET(IMU_PWR_EN)
#endif  /* End if 0 by lison on 2020-6-17 */


/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
extern void pw_power_on(void);
extern void pw_check_mcu_reset_source(void);
extern void pw_reset_cause_handle(void);

#ifdef __cplusplus
}
#endif

#endif /*_POWER_DRV_IF_H_*/

/****************************** END OF FILE ***********************************/
