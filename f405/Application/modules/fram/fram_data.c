/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : fram_data.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/7/26
* Description : fram data init define.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/7/26  wb    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/

/* Public Includes -----------------------------------------------------------*/

/* Private Includes ----------------------------------------------------------*/
#include "fram_data.h"

/* Global variables ----------------------------------------------------------*/
/*this array hasn't put into the fram,just used in the DIA module,*/
const uint8_t project_number_array[] =
{
    "EXO_BEAR1_10_" 
};

/* software version */
const uint8_t cl_software_version_array[SW_VERSION_LONGTH+1] =
{
    "22.01_170109_D  "  //for B2
};

/* update flag flag *///CL_SOFTWARE_UPDATE_FLAG
const uint8_t cl_software_update_flag_array[] =
{
    0xFF,0xFF,0xFF,0xFF,  //normal run
    0xFF,0xFF,0xFF,0xFF,  //normal run
    0xFF,0xFF,0xFF,0xFF,  //normal run
    0xFF,0xFF,0xFF,0xFF,  //normal run
//  0xAA,0x55,0xA5,0x5A   //need update
};

const uint8_t cl_fram_init_flag_array[] = 
{
    //0x55,0xAA,0x5A,0xA5//need init the whole fram
    0xFF,0xFF,0xFF,0xFF
};

const uint8_t cl_no_init_data_array[1] =//Just no need to change it after to user
{
    0x00,
};

const uint8_t cl_mb_serial_number_array[] =
{
    "MB1620000006"
};

const uint8_t cl_mb_hw_version_array[] =
{
    22,
};
/* Private variables ---------------------------------------------------------*/


/* Global functions ----------------------------------------------------------*/


/****************************** END OF FILE ***********************************/
