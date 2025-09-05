/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*				  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : can_app_msg.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : can_app message define.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CAN_APP_MSG_H_
#define _CAN_APP_MSG_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include "..\..\Mcu\mcu_if.h"
/**** Definition of constants ****/

/**** Definition of types ****/ 

typedef struct
{
	uint16_t *c1_imu_adc;
	uint16_t *c1_enc_inc;
	uint16_t *c1_enc_abs1;
	uint16_t *c1_enc_abs2;
	uint16_t *c2_imu_adc;
	uint16_t *c2_enc_inc;
	uint16_t *c2_enc_abs1;
	uint16_t *c2_enc_abs2;
}CIA402_SENSOR_ABI;


typedef struct
{
	uint16_t status_word; //6041
	int8_t mode_display; //6061
	int32_t postion_actual_value; //

}CIA402_DRIVER2MB_ABI;



typedef struct
{
	uint16_t control_word;
	int8_t mode;
	int32_t position_demand_value;
}CIA402_MB2DRIVER_ABI;


typedef enum {
    
    CIA_MASTER_Init = 0,
    CIA_MASTER_PreOperation,
    CIA_MASTER_Operation,
    CIA_MASTER_ERROR,
}CIA402_MASTER_FSM;


/**** Definition of macros ****/
#define CIA_NODE_MB_ID		0x05
#define CIA_NODE_MB_RATE	1000		/* 875kbps */

#define CIA_NODE_C1_ID		0x06	/* CAN1->0x09 */
#define CIA_NODE_C2_ID 		0x0A	/* CAN2->0x0A */


/**** Declaration of constants ****/


/**** Declaration of variables ****/

extern CIA402_DRIVER2MB_ABI cia_driver2mb;
/**** Declaration of functions ****/



#ifdef __cplusplus
}
#endif

#endif /*_CAN_APP_MSG_H_*/

/****************************** END OF FILE ***********************************/
