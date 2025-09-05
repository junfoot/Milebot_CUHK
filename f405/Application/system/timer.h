/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : timer.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : private header file of timer.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TIMER_H_
#define _TIMER_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
     
/**** Definition of constants ****/


/**** Definition of types ****/ 


/**** Definition of macros ****/
#define SYS_TIME_CLK    84000000U    /* 84MHz */
#if 0   /* Begin if 0 by lison on 2018-6-28 */
#define SYS_TIME_FREQUENCY    1000U    /* sys_time timer frequency in Hz,1ms interrupt */
#else
#define SYS_TIME_FREQUENCY	2000U	/* sys_time timer frequency in Hz,500us interrupt */
#endif  /* End if 0 by lison on 2018-6-28 */
#define SYS_TIME_PRESCALER    2U
#define SYS_TIME_PERIOD        (SYS_TIME_CLK/SYS_TIME_PRESCALER/SYS_TIME_FREQUENCY)  /* 42000,must less than 0xFFFF */

/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
void sys_time_init(void);
void sys_time_polling(void);


#ifdef __cplusplus
}
#endif

#endif /*_TIMER_H_*/

/****************************** END OF FILE ***********************************/
