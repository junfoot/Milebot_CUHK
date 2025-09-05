/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*				  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : can_app.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : private header file of can_app.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CAN_APP_H_
#define _CAN_APP_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include ".\canopen\CANopen.h"
     
/**** Definition of constants ****/

/**** Definition of types ****/ 


/**** Definition of macros ****/
#define CO_TMR_TASK_INTERVAL   500U          /* Interval of tmrTask thread in microseconds */
#define CAN_NODE_RATE		1000		/* 875kbps */
//#define CAN_NODE_C1_ID		0x09	/* left leg ID,connect main board CAN1->0x09 */
//#define CAN_NODE_C2_ID 	0x0A	/* right leg ID,connect main board CAN2->0x0A */

#define CAN_TASK_SLEEEP_TIME	10
/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
void can_app_program_start(void);
void can_app_comm_init(void);
CO_NMT_reset_cmd_t can_app_program_async(uint16_t timer_1ms_diff);

void can_app_start_node1(void);
void can_app_start_node2(void);
void can_app_sdo_comm_polling(uint16_t diff_ms);
#ifdef __cplusplus
}
#endif

#endif /*_CAN_APP_H_*/

/****************************** END OF FILE ***********************************/
