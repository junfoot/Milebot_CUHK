/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : MitProtocol_if.h
* Author      : Hugo
* Version     : v0.01
* Date        : 2020/08/06
* Description : mit protocol
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date        Name        Changes and comments
* v0.01     2020/08/06  Hugo        initial version
*=============================================================================*/

#ifndef __MITPROTOCOL_IF_H__
#define __MITPROTOCOL_IF_H__

#include "can_if.h"
#include "mitprotocol.h"

//#define Int16FromBuf(_buf,_idx)   ((((int16_t)_buf[_idx+1]) << 8) | _buf[_idx]) 
//
extern CanMessage* Mit_getMessage(unsigned char joint_id);
extern void actuator_init(ACTUATOR_STATE* pactuator);
extern void send_cmd_to_actuator(ACTUATOR_STATE* cmd);
extern void get_state_from_actuator(ACTUATOR_INFO *info, ACTUATOR_STATE *actuator);
extern void NMT_send(ACTUATOR_STATE* actuator, uint8_t cmd);
extern void SDO_master_send(ACTUATOR_STATE* actuator, uint8_t dataType, uint16_t index, uint8_t subIndex, uint32_t value);
extern void PDO_mapping(ACTUATOR_STATE* actuator);
extern void RPDO1_send(ACTUATOR_STATE* actuator, int16_t ctrl_word, int16_t mode, int16_t target_current);


void MIT_CAN_RxCpltCallback(CAN_TypeDef* CANx, CanRxMsgTypeDef* pCANRxMsg);

	
#endif


