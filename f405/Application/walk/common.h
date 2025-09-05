/**
* @file        common.h
* @brief       the head file of common use
* @details     Including definitions
* @author      Huguang, any question please send mail to huguang108@163.com
* @date        2020-6-29
* @version     1.0
* @copyright   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd. All Rights Reserved.
**********************************************************************************
* @attention
* @par         Modify log:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/08/07  <td>1.0      <td>Huguang   <td>initial version
* </table>
*
**********************************************************************************
*/

#include "algorithm.h"
#ifndef _COMMON_H_
#define _COMMON_H_

#define CONTROL_CYCLE 0.001
#define PI 3.14159265354
// Command sent to actuator
typedef struct
{
    float set_torque;
    float out_vel;
}ACTUATOR_CMD;

// Information of actuator
typedef struct
{
    float output_angle;		//Motec驱动器中, 该值是增量位置(还没有除以减速比)
    float output_torque;	//Motec驱动器中, 该值是电流, 单位是毫安mA
    float output_velocity;	//还没有除以减速比, 单位是RPM
}ACTUATOR_INFO;



typedef enum
{
    LEFT_HIP = 0,
    RIGHT_HIP,
    ALL_JOINTS,
}JOINT_ID;
#define FIRST_JOINT LEFT_HIP

#endif /*_WALK_H_*/

/****************************** END OF FILE ***********************************/

