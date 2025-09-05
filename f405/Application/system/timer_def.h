/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : timer_def.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : timer macros define.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/

BEGIN_TIMERS

TIMER(TIMER_LED_TEST,                         timer_led_test,                   TIMER_TASK_MMI)
#ifdef POWER_WATCHDOG_OPTION
TIMER(TIMER_PW_WATCHDOG,                     pw_task_wdg_polling_callback,         TIMER_TASK_POWER)
#endif/* POWER_WATCHDOG_OPTION */
//TIMER(TIMER_SD_DELAY_INIT,                  sd_delay_init_callback,                   TIMER_TASK_LOG)
END_TIMERS


/****************************** END OF FILE ***************************/

