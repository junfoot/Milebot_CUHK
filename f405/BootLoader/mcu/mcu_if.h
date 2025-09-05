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
/**** Definition of constants ****/


/**** Definition of types ****/ 

/**** Definition of macros ****/

//����ģ��
#define PA4_BLE_MODE_Pin GPIO_PIN_4
#define PA4_BLE_MODE_GPIO_Port GPIOA
#define PA5_BLE_WAKEUP_Pin GPIO_PIN_5
#define PA5_BLE_WAKEUP_GPIO_Port GPIOA
#define PA7_BT_POW_CN_Pin GPIO_PIN_7
#define PA7_BT_POW_CN_GPIO_Port GPIOA
//�͵�ƽ�� ����ģʽ
#define BLE_MODE_CONFIG() HAL_GPIO_WritePin(PA4_BLE_MODE_GPIO_Port, PA4_BLE_MODE_Pin, GPIO_PIN_RESET)
//�ߵ�ƽ�� ͸��ģʽ
#define BLE_MODE_TRANSMIT() HAL_GPIO_WritePin(PA4_BLE_MODE_GPIO_Port, PA4_BLE_MODE_Pin, GPIO_PIN_SET)
#define BT_WAKEUP_enable() HAL_GPIO_WritePin(PA5_BLE_WAKEUP_GPIO_Port, PA5_BLE_WAKEUP_Pin, GPIO_PIN_SET)
#define BT_WAKEUP_disable() HAL_GPIO_WritePin(PA5_BLE_WAKEUP_GPIO_Port, PA5_BLE_WAKEUP_Pin, GPIO_PIN_RESET)
#define BT_POW_enable() HAL_GPIO_WritePin(PA7_BT_POW_CN_GPIO_Port, PA7_BT_POW_CN_Pin, GPIO_PIN_RESET)
#define BT_POW_disable() HAL_GPIO_WritePin(PA7_BT_POW_CN_GPIO_Port, PA7_BT_POW_CN_Pin, GPIO_PIN_SET)

//CAN ����
#define PB7_CAN1_STB_Pin GPIO_PIN_7
#define PB7_CAN1_STB_GPIO_Port GPIOB
#define PB14_CAN2_STB_Pin GPIO_PIN_14
#define PB14_CAN2_STB_GPIO_Port GPIOB
#define CAN1_receive_enable() HAL_GPIO_WritePin(PB7_CAN1_STB_GPIO_Port, PB7_CAN1_STB_Pin, GPIO_PIN_RESET)
#define CAN1_receive_disable() HAL_GPIO_WritePin(PB7_CAN1_STB_GPIO_Port, PB7_CAN1_STB_Pin, GPIO_PIN_SET)
#define CAN2_receive_enable() HAL_GPIO_WritePin(PB14_CAN2_STB_GPIO_Port, PB14_CAN2_STB_Pin, GPIO_PIN_RESET)
#define CAN2_receive_disable() HAL_GPIO_WritePin(PB14_CAN2_STB_GPIO_Port, PB14_CAN2_STB_Pin, GPIO_PIN_SET)

//���
#define PB6_LMOTOR_POWER_CTR_Pin GPIO_PIN_6
#define PB6_LMOTOR_POWER_CTR_GPIO_Port GPIOB
#define PB15_RMOTOR_POWER_CTR_Pin GPIO_PIN_15
#define PB15_RMOTOR_POWER_CTR_GPIO_Port GPIOB
#define LMOTOR_POWER_enable() HAL_GPIO_WritePin(PB6_LMOTOR_POWER_CTR_GPIO_Port, PB6_LMOTOR_POWER_CTR_Pin, GPIO_PIN_SET)
#define LMOTOR_POWER_disable() HAL_GPIO_WritePin(PB6_LMOTOR_POWER_CTR_GPIO_Port, PB6_LMOTOR_POWER_CTR_Pin, GPIO_PIN_RESET)
#define RMOTOR_POWER_enable() HAL_GPIO_WritePin(PB15_RMOTOR_POWER_CTR_GPIO_Port, PB15_RMOTOR_POWER_CTR_Pin, GPIO_PIN_SET)
#define RMOTOR_POWER_disable() HAL_GPIO_WritePin(PB15_RMOTOR_POWER_CTR_GPIO_Port, PB15_RMOTOR_POWER_CTR_Pin, GPIO_PIN_RESET)

//����״̬��(������)
#define PA8_STATLED_CONT_Pin GPIO_PIN_8
#define PA8_STATLED_CONT_GPIO_Port GPIOA
#define STATLED_enable() HAL_GPIO_WritePin(PA8_STATLED_CONT_GPIO_Port, PA8_STATLED_CONT_Pin, GPIO_PIN_SET)
#define STATLED_disable() HAL_GPIO_WritePin(PA8_STATLED_CONT_GPIO_Port, PA8_STATLED_CONT_Pin, GPIO_PIN_RESET)
#define STATLED_Toggle() HAL_GPIO_TogglePin(PA8_STATLED_CONT_GPIO_Port, PA8_STATLED_CONT_Pin)

//����3.3V �ϵ�ָʾ��(������)
#define PA15_LED_CONT_Pin GPIO_PIN_15
#define PA15_LED_CONT_GPIO_Port GPIOA
#define LED_HEARTBEAT_Toggle() HAL_GPIO_TogglePin(PA15_LED_CONT_GPIO_Port, PA15_LED_CONT_Pin)

//�����, ����IO 
#define PWR_CHARGE_DET_Pin 	GPIO_PIN_3
#define PWR_CHARGE_DET_GPIO_Port GPIOB

//IMU, ����IO 
#define PB1_IMU_INT_Pin GPIO_PIN_1
#define PB1_IMU_INT_GPIO_Port GPIOB

//SYN6658 �Ƿ�æ��־, ����IO 
#define PC8_SYN6658_BUSY_Pin GPIO_PIN_8
#define PC8_SYN6658_BUSY_GPIO_Port GPIOC


/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
extern void mcu_init(void);
extern void mcu_system_clk_config(void);
extern void mcu_io_init_reset(void);
extern void _Error_Handler(char * file, int line);
extern void mcu_disable_interrupt(void);
extern void mcu_enable_interrupt(void);
extern void UsageFault_Handler(void);
extern void HardFault_Handler(void);
extern void NMI_Handler(void);
extern void MemManage_Handler(void);
extern void BusFault_Handler(void);
extern void mcu_sw_reset(void);

#ifdef __cplusplus
}
#endif

#endif /*_MCU_IF_H_*/

/****************************** END OF FILE ***********************************/

