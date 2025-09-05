/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : sys_task_if.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : extern header file of sys_task interface.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SYS_TASK_IF_H_
#define _SYS_TASK_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/

     
/**** Definition of constants ****/


/**** Definition of types ****/ 


/**** Definition of macros ****/


/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
extern void sys_task(void *argument);
extern void sys_task_reset_create(void);
extern void sys_timer_isr_handler(void);

#ifdef __cplusplus
}
#endif

#endif /*_SYS_TASK_IF_H_*/

/****************************** END OF FILE ***********************************/
