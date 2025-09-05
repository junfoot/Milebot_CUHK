/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : task_if.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : extern header file of task.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TASK_IF_H_
#define _TASK_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include <cmsis_os2.h>
     
/**** Definition of constants ****/


/**** Definition of types ****/ 


/**** Definition of macros ****/
#define MMI_THREAD_FLAG_IMU	0x00000001U

/**** Declaration of constants ****/


/**** Declaration of variables ****/
extern osMutexId_t mutex_fram;
extern osMutexId_t mutex_comm;


/**** Declaration of functions ****/
extern void task_init_power(void *argument);
extern void task_create_all(void);
extern void task_kill_all(void);

#ifdef __cplusplus
}
#endif

#endif /*_TASK_IF_H_*/

/****************************** END OF FILE ***********************************/
