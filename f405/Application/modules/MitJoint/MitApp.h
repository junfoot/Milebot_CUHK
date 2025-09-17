/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : MitApp.h
* Author      : Hugo
* Version     : v0.01
* Date        : 2020/08/06
* Description : mit app task
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date        Name        Changes and comments
* v0.01     2020/08/06  Hugo        initial version
*=============================================================================*/

#ifndef __MITAPP_H__
#define __MITAPP_H__

#include "can_if.h"
#include "common.h"

typedef enum{
    NMT_CMD_OPERATIONAL		= 0x01,
    NMT_CMD_STOPPED			= 0x02,
    NMT_CMD_PRE_OPERATIONAL	= 0x80,
    NMT_CMD_RESET_NODE		= 0x81,
    NMT_CMD_RESET_COM			= 0x82   
} NMT_CMD;

typedef enum
{
    RPDO_COM_1 = 0x1400,
    RPDO_COM_2 = 0x1401,
    RPDO_COM_3 = 0x1402,
    RPDO_COM_4 = 0x1403,         
} RPDO_COM_INDEX;

typedef enum
{
    RPDO_MAP_1 = 0x1600,
    RPDO_MAP_2 = 0x1601,
    RPDO_MAP_3 = 0x1602,
    RPDO_MAP_4 = 0x1603,
} RPDO_MAP_INDEX;

typedef enum
{
    TPDO_COM_1 = 0x1800,
    TPDO_COM_2 = 0x1801,
    TPDO_COM_3 = 0x1802,
    TPDO_COM_4 = 0x1803,         
} TPDO_COM_INDEX;

typedef enum
{
    TPDO_MAP_1 = 0x1A00,
    TPDO_MAP_2 = 0x1A01,
    TPDO_MAP_3 = 0x1A02,
    TPDO_MAP_4 = 0x1A03,
} TPDO_MAP_INDEX;

typedef enum
{
    RPDO_COB_ID_1 = 0x200,
    RPDO_COB_ID_2 = 0x300,
    RPDO_COB_ID_3 = 0x400,
    RPDO_COB_ID_4 = 0x500,         
} RPDO_COB_ID_INDEX;

typedef enum
{
    TPDO_COB_ID_1 = 0x180,
    TPDO_COB_ID_2 = 0x280,
    TPDO_COB_ID_3 = 0x380,
    TPDO_COB_ID_4 = 0x480,
} TPDO_COB_ID_INDEX;

typedef enum
{
    RSDO_COB_ID_1 = 0x600,     
} RSDO_COB_ID_INDEX;

typedef enum
{
    TSDO_COB_ID_1 = 0x580,
} TSDO_COB_ID_INDEX;


typedef enum
{
    DRIVER_MODE_POSITION = 1,
    DRIVER_MODE_VELOCITY = 3,
    DRIVER_MODE_CURRENT = 4,
} DRIVER_MODE_INDEX;


typedef enum
{
    MAPPING_control_word		= 0x60400010,
    MAPPING_status_word		= 0x60410010,
    MAPPING_control_mode	= 0x60600010,
    MAPPING_position_value	= 0x60630020,
    MAPPING_target_velocity	= 0x60FF0010,
    MAPPING_velocity_value	= 0x606C0010,
    MAPPING_target_current	= 0x60710010,
    MAPPING_current_value	= 0x60780010,
} MAPPING_INDEX;


typedef enum
{
    CTRL_WORD_Enable_Voltage = 0x06,		
    CTRL_WORD_Switch_On = 0x07,			
    CTRL_WORD_Enable_Operation = 0x0F,	
} CTRL_WORD_INDEX;


//广州视源电机的减速比
#define GEAR_RATIO		243.3


typedef enum _mit_mode
{
    MIT_REST_MODE = 0,
    MIT_CALIBRATION_MODE = 1,
    MIT_MOTOR_MODE = 2,
    MIT_SETUP_MODE = 4,
    MIT_ENCODER_MODE = 5,
}MIT_MODE;

typedef enum _can_num
{
    CAN_1 = 0x01,
    CAN_2 = 0x02,
}CAN_NUM;

typedef enum _can_node_id
{
    NODE_ID_1 = 0x01,
//  NODE_ID_2 = 0x02,
    NODE_ID_2 = 0x01,
}CAN_NODE_ID;

#define DIR_FORWARD (1)
#define DIR_REVERSE (-1)

typedef struct
{
    CAN_NUM can_num;
    CAN_NODE_ID node_id;
    JOINT_ID joint_id;
    MIT_MODE motor_mode;
    int8_t dir;
    ACTUATOR_CMD actuator_cmd;
}ACTUATOR_STATE;


extern uint8_t can_flag;

void mit_task(void *argument);
void actuator_cmd_cb(uint8_t __attribute__((unused)) sender_id, ACTUATOR_CMD *cmddata);
//uint8_t walk_detect_right_lift_state(ACTUATOR_INFO *right_walk_info);
//uint8_t walk_detect_left_lift_state(ACTUATOR_INFO *left_walk_info) ;

#endif

