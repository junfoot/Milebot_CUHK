




/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*				  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : algorithm.h
* Author      : zx
* Version     : v0.01
* Date        : 2017/6/30
* Description : private header file of algorithm.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/30 zx          initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CAN_MOTION_CONTROL_IF_H_
#define _CAN_MOTION_CONTROL_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include "stdint.h"

#include "..\modules\can\can_app_msg.h"
#include "..\walk\walk_if.h"
/**** Definition of constants ****/



#define MAX_F_ERR           8192         /*最大允许误差 */      
#define MIN_P_LIMIT        ((int32_t)(-1000000000))   /*最小位置极限   int32  用户单位*/      
#define MAX_P_LIMIT         ((int32_t)(1000000000))      /*最大位置极限 int32 用户单位*/     
#define MAX_SPEED           ((uint32_t)(409600))          /*最大轮廓速度   0x607f   uint32*/     
#define MAX_QDEC            ((uint32_t)(1000000000))      /*快速停机减速度 0x6085  uint32*/      
#define MAX_ACC             ((uint32_t)(160000))          /*最大轮廓加速度 0x60c5  uint32*/     
#define MAX_DACC             ((uint32_t)(160000))          /*最大轮廓减速度 0x60c6 uint32*/      
#define INTERPOLATE_TIME        ((uint8_t*)(10))
#define INTERPOLATE_FIRST_POS    (uint32_t)(0)    /*第一个插补位置*/

	/**** Definition of types ****/ 

#if 0
	typedef enum
	{
		ServoCommandOFF=0,
		ServoCommandON,
		ServoCommandEnable,
		ServoCommandRunMode,
		ServoCommandDisable,
		ServoCommandEmeStop,
		ServoCommandFaultReset,
		ServoCommandHalt,
		ServoCommandHaltReset,

	}SERVO_CMD;

#endif

	typedef enum 
	{
		SERVO_INIT=0,
		SERVO_READY,
		SERVO_WAIT_FOR_ENABLE,
		SERVO_ENABLE,
		SERVO_RUN,
		SERVO_BREAK,
	}SERVO_CMD;

	typedef enum
	{
		PROFILE_POSTION =1,
		VELOCITY,
		PROFILE_VELOCITY,
		TORQUE,
		NA,
		HOMING_MODE,
		INTERPOLATE_POSITION,
	}MODE_DISPLAY;

	typedef enum
	{
		TPDO1=0,
		TPDO2,
		RPDO1,
	}PDO_NUM;

	typedef enum
	{
		DISABLE_PDO =0,
		CLEAR_MAP,
		SET_WORD_1,
		SET_WORD_2,
		SET_WORD_3,
		MAPPING_TWO,
		SET_TRANSMISSION_TYPE,
		SET_ASYNC_TIME,
		ENABLE_PDO,
		MAP_DONE,
	}PDO_MAPPING_STATE_MACHINE;



	typedef struct 
	{
		uint16_t pdo_comm_param;
		uint16_t pdo_map_param;
		uint16_t word_one;
		uint16_t word_two;
		uint16_t word_three;
		PDO_MAPPING_STATE_MACHINE pdo_fsm;
	}CANOPEN_COM_INFO;
/**** Definition of macros ****/


/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
extern void cia_motion_task(void);
extern uint8_t cia_parameter_init(void);
extern MODE_DISPLAY cia_set_mode(CIA402_MB2DRIVER_ABI*output,uint8_t mode);
extern void cia_motion_run(CIA402_MB2DRIVER_ABI *output,SERVO_CMD *cmd);
extern int CIA_PDO_MAPPING(CANOPEN_COM_INFO command,PDO_MAPPING_STATE_MACHINE *cmd ,int diff_ms,PDO_NUM pdo_num,uint8_t subindex,int word1,int word2,int word3);

extern void cia_motor_init(CIA402_MB2DRIVER_ABI output);
extern void cia_motor_disable(CIA402_MB2DRIVER_ABI output);
extern void cia_motor_enable(CIA402_MB2DRIVER_ABI output);
#ifdef __cplusplus
}
#endif

#endif /*_MCU_H_*/

/****************************** END OF FILE ***********************************/
