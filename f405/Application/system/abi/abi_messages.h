/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : abi_messages.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : middleware library ABI,send and receive messages of class sw
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _ABI_MESSAGES_H_
#define _ABI_MESSAGES_H_
#include "abi_common.h"


/* Messages IDs */
#define ABI_CAN_SENSOR_ID 0
#define ABI_IMU_GYRO_FLOAT_ID 1
#define ABI_IMU_ACCEL_FLOAT_ID 2
#define ABI_IMU_MAG_INT32_ID 3
#define ABI_IMU_LOWPASSED_ID 4
#define ABI_IMU_CALI_PROMPT_ID 5
#define ABI_NRF_WIRELESS_DATA_ID 6
#define ABI_BODY_ATTI_ID 7
#define ABI_MMI_COMMAND_ID 8
#define ABI_WALK_DATA_ID 9
#define ABI_POWER_INFO_ID 10
#define ABI_ACTUATOR_CMD_ID     11
#define ABI_ACTUATOR_STATE_ID   12
/* Array and linked list structure */
#define ABI_MESSAGE_NB 13

ABI_EXTERN abi_event* abi_queues[ABI_MESSAGE_NB];

/* Callbacks */
typedef void (*abi_callbackPOWER_INFO)(uint8_t sender_id, POWER_INFO * info);
typedef void (*abi_callbackACTUATOR_CMD)(uint8_t sender_id,   ACTUATOR_CMD * actuator_cmd);
typedef void (*abi_callbackACTUATOR_STATE)(uint8_t sender__id,ACTUATOR_INFO * actuator_info);
/* Bind and Send functions */


/****************************************** MsgPOWER_INFO *********************************************/
/*
@brief POWER_INFO publicated by power_task
*/

static inline void AbiBindMsgPOWER_INFO(uint8_t sender_id, abi_event * ev, abi_callbackPOWER_INFO cb) {
  ev->id = sender_id;
  ev->cb = (abi_callback)cb;
  ABI_PREPEND(abi_queues[ABI_POWER_INFO_ID],ev);
}

static inline void AbiSendMsgPOWER_INFO(uint8_t sender_id, POWER_INFO * msg) {
  abi_event* e;
  ABI_FOREACH(abi_queues[ABI_POWER_INFO_ID],e) {
    if (e->id == ABI_BROADCAST || e->id == sender_id) {
      abi_callbackPOWER_INFO cb = (abi_callbackPOWER_INFO)(e->cb);
      cb(sender_id, msg);
    }
  }
}



/****************************************** MsgACTUATOR_CMD **********************************************/
/*
@brief ACTUATOR_CMD publicated by walk_task, transmit actuator command to actuator_task 
*/

static inline void AbiBindMsgACTUATOR_CMD(uint8_t sender_id ,abi_event *ev, abi_callbackACTUATOR_CMD cb)
{
    ev->id = sender_id;
    ev->cb = (abi_callback)cb;
    ABI_PREPEND(abi_queues[ABI_ACTUATOR_CMD_ID],ev);
}


static inline void AbiSendMsgACTUATOR_CMD(uint8_t sender_id, ACTUATOR_CMD* msg)
{
    abi_event *e ;
    ABI_FOREACH(abi_queues[ABI_ACTUATOR_CMD_ID],e)
        {
            if (e->id == ABI_BROADCAST || e->id == sender_id)
            {
                abi_callbackACTUATOR_CMD cb = (abi_callbackACTUATOR_CMD)(e->cb);
                cb(sender_id,msg);	
            }
        }
}

/****************************************** MsgACTUATOR_STATE *********************************************/
/*
@brief ACTUATOR_STATE publicated by actuator_task, transmit all actuator state to other task 
*/

static inline void AbiBindMsgACTUATOR_STATE(uint8_t sender_id, abi_event * ev, abi_callbackACTUATOR_STATE cb)
{	
	ev->id = sender_id;
	ev->cb = (abi_callback)cb;
	ABI_PREPEND(abi_queues[ABI_ACTUATOR_STATE_ID],ev);
}

static inline void AbiSendMsgACTUATOR_STATE(uint8_t sender_id, ACTUATOR_INFO * msg)
{
	abi_event* e;
	 ABI_FOREACH(abi_queues[ABI_ACTUATOR_STATE_ID],e)
	 {
	   if (e->id == ABI_BROADCAST || e->id == sender_id) 
	   	{
		 abi_callbackACTUATOR_STATE cb = (abi_callbackACTUATOR_STATE)(e->cb);
		 cb(sender_id, msg);
	   }
	 }
}


#endif /* _ABI_MESSAGES_H_ */
/****************************** END OF FILE ***********************************/
