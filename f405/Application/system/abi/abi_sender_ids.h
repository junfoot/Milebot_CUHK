/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : abi_sender_ids.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : Convenience defines for ABI sender IDs.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _ABI_SENDER_IDS_H_
#define _ABI_SENDER_IDS_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/

     
/**** Definition of constants ****/


/**** Definition of types ****/ 


/**** Definition of macros ****/
#define CAN_SENSOR_ID        1
#define IMU_LSM6D_ID         2
#define WALK_ID              3
#define MMI_ID               4
#define POWER_ID             5     
#define BODY_ATTI_ID         6
#define REMOTE_ID            7
#define ACTUATOR_CMD_ID      8
#define ACTUATOR_STATE_ID    9
#define NRF_WIRELESS_DATA_ID 10
#define IMU_CALI_PROMPT_ID   11
/*
 * IDs of encoder sensors
 */


/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/


#ifdef __cplusplus
}
#endif

#endif /*_ABI_SENDER_IDS_H_*/

/****************************** END OF FILE ***********************************/
