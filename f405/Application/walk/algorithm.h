/**
* @file        algorithm.h
* @brief       algorithms header file
* @details     Including some algorithms definitions
* @author      Huguang, any question please send mail to huguang108@163.com
* @date        2020-6-30
* @version     1.0
* @copyright   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd. All Rights Reserved.
**********************************************************************************
* @attention
* @par         Modify log:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/06/30  <td>1.0      <td>Huguang   <td>initial version
* </table>
*
**********************************************************************************
*/

#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#define PI 3.14159265354

typedef struct 
{
  float ratio;
  float out;
}filter_info;

extern float update_1th_filter(filter_info *filter, float value);
extern void init_1th_filter(filter_info *filter, float cut_off, float value);
extern float im_control(float pos_err, float vel_current, float k, float b);
void update_phase(float* phase_pointer, float frequency);

#endif /*_MCU_H_*/

/****************************** END OF FILE ***********************************/

