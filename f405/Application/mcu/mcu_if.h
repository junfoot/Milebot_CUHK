/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : mcu_if.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : extern header file of mcu.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MCU_IF_H_
#define _MCU_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include "stm32f4xx_hal.h"
#include "..\system\std.h"   
#include "..\system\msg_pragmas.h"
/**** Definition of constants ****/


/**** Definition of types ****/ 
typedef void *regarm_t;

struct extctx 
{
  regarm_t      r0;
  regarm_t      r1;
  regarm_t      r2;
  regarm_t      r3;
  regarm_t      r12;
  regarm_t      lr_thd;
  regarm_t      pc;
  regarm_t      xpsr;
};

struct intctx 
{
  regarm_t      r8;
  regarm_t      r9;
  regarm_t      r10;
  regarm_t      r11;
  regarm_t      r4;
  regarm_t      r5;
  regarm_t      r6;
  regarm_t      r7;
  regarm_t      lr;
};

struct MCU_FAULT_INFO
{
    //uint32_t hfsr;
    //uint32_t bfar;
    //uint32_t mmfar;
    //uint32_t cfsr;
    uint32_t stack_data[8];
    uint32_t reset_src;
    uint8_t reset_type;
};

typedef enum
{
     MCU_RESET_BY_SW = 0x00,    /* application software manual reset */
     MCU_RESET_BY_PWR = 0x01,
     MCU_RESET_BY_PIN = 0x02,
     MCU_RESET_BY_ERR = 0x03,    /* watchdog issue a reset */
     MCU_RESET_BY_CSVR  /* Clock failure detection */
}MCU_RESET_TYPE;

/**** Definition of macros ****/


//蓝牙模块
#define PA4_BLE_MODE_Pin GPIO_PIN_11
#define PA4_BLE_MODE_GPIO_Port GPIOA
#define PA5_BLE_WAKEUP_Pin GPIO_PIN_12
#define PA5_BLE_WAKEUP_GPIO_Port GPIOA
#define PA7_BT_POW_CN_Pin GPIO_PIN_7
#define PA7_BT_POW_CN_GPIO_Port GPIOA
//低电平： 配置模式
#define BLE_MODE_CONFIG() HAL_GPIO_WritePin(PA4_BLE_MODE_GPIO_Port, PA4_BLE_MODE_Pin, GPIO_PIN_RESET)
//高电平： 透传模式
#define BLE_MODE_TRANSMIT() HAL_GPIO_WritePin(PA4_BLE_MODE_GPIO_Port, PA4_BLE_MODE_Pin, GPIO_PIN_SET)
#define BT_WAKEUP_enable() HAL_GPIO_WritePin(PA5_BLE_WAKEUP_GPIO_Port, PA5_BLE_WAKEUP_Pin, GPIO_PIN_SET)
#define BT_WAKEUP_disable() HAL_GPIO_WritePin(PA5_BLE_WAKEUP_GPIO_Port, PA5_BLE_WAKEUP_Pin, GPIO_PIN_RESET)
#define BT_POW_enable() HAL_GPIO_WritePin(PA7_BT_POW_CN_GPIO_Port, PA7_BT_POW_CN_Pin, GPIO_PIN_RESET)
#define BT_POW_disable() HAL_GPIO_WritePin(PA7_BT_POW_CN_GPIO_Port, PA7_BT_POW_CN_Pin, GPIO_PIN_SET)

//CAN 总线
#define PB7_CAN1_STB_Pin GPIO_PIN_7
#define PB7_CAN1_STB_GPIO_Port GPIOB
#define PB14_CAN2_STB_Pin GPIO_PIN_14
#define PB14_CAN2_STB_GPIO_Port GPIOB
#define CAN1_receive_enable() HAL_GPIO_WritePin(PB7_CAN1_STB_GPIO_Port, PB7_CAN1_STB_Pin, GPIO_PIN_RESET)
#define CAN1_receive_disable() HAL_GPIO_WritePin(PB7_CAN1_STB_GPIO_Port, PB7_CAN1_STB_Pin, GPIO_PIN_SET)
#define CAN2_receive_enable() HAL_GPIO_WritePin(PB14_CAN2_STB_GPIO_Port, PB14_CAN2_STB_Pin, GPIO_PIN_RESET)
#define CAN2_receive_disable() HAL_GPIO_WritePin(PB14_CAN2_STB_GPIO_Port, PB14_CAN2_STB_Pin, GPIO_PIN_SET)

//电机
#define PB6_LMOTOR_POWER_CTR_Pin GPIO_PIN_6
#define PB6_LMOTOR_POWER_CTR_GPIO_Port GPIOB
#define PB15_RMOTOR_POWER_CTR_Pin GPIO_PIN_15
#define PB15_RMOTOR_POWER_CTR_GPIO_Port GPIOB
#define LMOTOR_POWER_enable() HAL_GPIO_WritePin(PB6_LMOTOR_POWER_CTR_GPIO_Port, PB6_LMOTOR_POWER_CTR_Pin, GPIO_PIN_SET)
#define LMOTOR_POWER_disable() HAL_GPIO_WritePin(PB6_LMOTOR_POWER_CTR_GPIO_Port, PB6_LMOTOR_POWER_CTR_Pin, GPIO_PIN_RESET)
#define RMOTOR_POWER_enable() HAL_GPIO_WritePin(PB15_RMOTOR_POWER_CTR_GPIO_Port, PB15_RMOTOR_POWER_CTR_Pin, GPIO_PIN_SET)
#define RMOTOR_POWER_disable() HAL_GPIO_WritePin(PB15_RMOTOR_POWER_CTR_GPIO_Port, PB15_RMOTOR_POWER_CTR_Pin, GPIO_PIN_RESET)

//开机状态灯(背包上)
#define PA8_STATLED_CONT_Pin GPIO_PIN_8
#define PA8_STATLED_CONT_GPIO_Port GPIOA
#define STATLED_enable() HAL_GPIO_WritePin(PA8_STATLED_CONT_GPIO_Port, PA8_STATLED_CONT_Pin, GPIO_PIN_SET)
#define STATLED_disable() HAL_GPIO_WritePin(PA8_STATLED_CONT_GPIO_Port, PA8_STATLED_CONT_Pin, GPIO_PIN_RESET)
#define STATLED_Toggle() HAL_GPIO_TogglePin(PA8_STATLED_CONT_GPIO_Port, PA8_STATLED_CONT_Pin)

//主板3.3V 上电指示灯(主板上)
#define PA15_LED_CONT_Pin GPIO_PIN_15
#define PA15_LED_CONT_GPIO_Port GPIOA
#define LED_HEARTBEAT_Toggle() HAL_GPIO_TogglePin(PA15_LED_CONT_GPIO_Port, PA15_LED_CONT_Pin)

//充电检测, 输入IO 
#define PWR_CHARGE_DET_Pin 	GPIO_PIN_3
#define PWR_CHARGE_DET_GPIO_Port GPIOB

//IMU, 输入IO 
#define PB1_IMU_INT_Pin GPIO_PIN_1
#define PB1_IMU_INT_GPIO_Port GPIOB

//SYN6658 是否忙标志, 输入IO 
#define PC8_SYN6658_BUSY_Pin GPIO_PIN_8
#define PC8_SYN6658_BUSY_GPIO_Port GPIOC

//功能切换按键
#define FUNCTION_SWITCH_Pin GPIO_PIN_13
#define FUNCTION_SWITCH_GPIO_Port GPIOC


#if 0   /* Begin if 0 by lison on 2020-6-17 */
#define GPIO_SET(param)   HAL_GPIO_WritePin(param##_GPIO_Port,param##_Pin,GPIO_PIN_SET)
#define GPIO_RESET(param)   HAL_GPIO_WritePin(param##_GPIO_Port,param##_Pin,GPIO_PIN_RESET)
#define GPIO_TOGGLE(param)   HAL_GPIO_TogglePin(param##_GPIO_Port,param##_Pin)
#endif  /* End if 0 by lison on 2020-6-17 */

/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
extern void mcu_init(void);
extern void mcu_system_clk_config(void);
extern void mcu_io_init_reset(void);
extern void mcu_io_init_standby(void);
extern void _Error_Handler(char * file, int line);
extern void mcu_disable_interrupt(void);
extern void mcu_enable_interrupt(void);
extern void UsageFault_Handler(void);
extern void HardFault_Handler(void);
extern void NMI_Handler(void);
extern void MemManage_Handler(void);
extern void BusFault_Handler(void);
extern void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef* hwwdg);
extern MCU_RESET_TYPE mcu_get_reset_reason(void);
extern void mcu_sw_reset(void);
extern void mcu_get_fault_stack_data(uint32_t *buf);
extern void mcu_read_fault_stack_data(uint16_t offset);

#ifdef __cplusplus
}
#endif

#endif /*_MCU_IF_H_*/

/****************************** END OF FILE ***********************************/

