/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : mcu.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : private task header file.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TASK_H_
#define _TASK_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include "cmsis_os2.h"
     
/**** Definition of constants ****/


/**** Definition of types ****/ 
/*      task priority        */
typedef enum 
{
    SOUND_TASK_PRIORITY = osPriorityNormal2,
    COMM_TASK_PRIORITY  = osPriorityNormal3,
    
    SYS_TASK_PRIORITY  = osPriorityHigh2,
    BLE_TASK_PRIORITY  = osPriorityHigh5,
    
    SCA_TASK_PRIORITY   = osPriorityRealtime,
    WALK_TASK_PRIORITY  = osPriorityRealtime1,
    POWER_TASK_PRIORITY = osPriorityRealtime2,     
}TYPE_TASK_PRIORITY;

/**** Definition of macros ****/


/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/


#ifdef __cplusplus
}
#endif

#endif /*_TASK_H_*/

/****************************** END OF FILE ***********************************/
