/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : timer.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : timer function
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/
#include "timer_if.h"
/* Public Includes -----------------------------------------------------------*/

/* Private Includes ----------------------------------------------------------*/
#include "timer.h"   
#include "timer_if.h" 

#include "timer_class.h"
#include "timer_def.h"
#include "timer_class.h"
#include "timer_def.h"
#include "timer_class.h"
#include "timer_def.h"

/* Global variables ----------------------------------------------------------*/
struct SYS_TIME sys_time;
uint16_t sys_timer_tick_counter;  

/* Private variables ---------------------------------------------------------*/
TIMER_CLASS timer_status[TIMER_MAX];
uint8_t timer_tick_poll_counter;


/* Global functions ----------------------------------------------------------*/
/**
  * @brief  config system timer interrupt.
  * @param  None
  * @retval None
  */
void sys_time_init(void)
{
    sys_time.nb_sec     = 0;
    sys_time.nb_sec_rem = 0;
    sys_time.nb_tick    = 0;

    sys_time.ticks_per_sec = SYS_TIME_FREQUENCY;
    sys_time.resolution = 1.0 / sys_time.ticks_per_sec;

    sys_time.cpu_ticks_per_sec = SYS_TIME_CLK / SYS_TIME_PRESCALER;
    /* cpu ticks per desired sys_time timer step */
    sys_time.resolution_cpu_ticks = (uint32_t)(sys_time.resolution * sys_time.cpu_ticks_per_sec + 0.5f);

    tim7_init(SYS_TIME_PRESCALER, sys_time.resolution_cpu_ticks);
}

void sys_time_polling(void)
{
    sys_time.nb_tick++;
    sys_time.nb_sec_rem += sys_time.resolution_cpu_ticks;
    if (sys_time.nb_sec_rem >= sys_time.cpu_ticks_per_sec) 
    {
        sys_time.nb_sec_rem -= sys_time.cpu_ticks_per_sec;
        sys_time.nb_sec++;
      }
}

/**
  * @brief  this function init all the timers.
  * @param  None
  * @retval None
  */
void tm_reset_create(void)
{
    uint8_t i;
    for(i = 0x00; i < TIMER_MAX; i++)
    {
        timer_status[i].counter = TIMER_NOT_ACTIVE;
        timer_status[i].backupCounter = TIMER_NOT_ACTIVE;
        timer_status[i].param = TIMER_NO_PARAM;
    }
    timer_tick_poll_counter = 0;
    sys_timer_tick_counter = 0;

    sys_time_init();
}

/**
  * @brief  this function create timer which is defined by the parameters.
  * @param  timer_id : the No. of the timer defined in enum TIMER_IDS
            period : time, the unit is 1 mseconds. 
            type : Once or periodic timer.
            param : timeout param.
  * @retval None
  */
void tm_create_timer(uint8_t timer_id, uint16_t period, uint8_t type, uint16_t param)
{
    if(timer_id < TIMER_MAX)
    {
        if(period == TIMER_NOT_ACTIVE)
        {
            period = TIMER_NOT_ACTIVE - 1;
        }
        timer_status[timer_id].counter = period;
        timer_status[timer_id].type = type;
        timer_status[timer_id].rep_cnt = type;
        if(type == TIMER_ONE_SHOT)
        {
            timer_status[timer_id].backupCounter = TIMER_NO_BACKUP_COUNTER;
        }
        else
        {
            timer_status[timer_id].backupCounter = period;
        }

        timer_status[timer_id].param = param;
    }
}

/**
  * @brief  this function kill timer which is defined by the parameter timer_id.
  * @param  timer_id : the No. of the timer defined in enum TIMER_IDS
  * @retval None
  */
void tm_kill_timer(uint8_t timer_id)
{
    if(timer_id < TIMER_MAX)
    {
        timer_status[timer_id].counter = TIMER_NOT_ACTIVE;
        timer_status[timer_id].backupCounter = TIMER_NO_BACKUP_COUNTER;
        timer_status[timer_id].param = TIMER_NO_PARAM;
    }
}

/**
  * @brief  tm_stimulate.
  * @param  None
  * @retval None
  */
void tm_stimulate(uint8_t task_id)
{
    uint8_t i;
    for(i = 0x00; i < TIMER_MAX; i++)
    {
        if((timer_array[i].task_id == task_id)
            && (timer_status[i].counter == TIMER_TIMEOUT))
        {
            if(timer_status[i].backupCounter != TIMER_NO_BACKUP_COUNTER)
            {
                if(timer_status[i].type == TIMER_PERIODIC)
                {
                    timer_status[i].counter = timer_status[i].backupCounter;
                }
                else
                {
                    if(--timer_status[i].rep_cnt)
                    {
                        timer_status[i].counter = timer_status[i].backupCounter;
                    }
                    else
                    {
                        timer_status[i].counter = TIMER_NOT_ACTIVE;
                    }
                }
            }
            else    //TIMER_ONE_SHOT,end timer
            {
                timer_status[i].counter = TIMER_NOT_ACTIVE;
            }

            timer_array[i].action_func(timer_status[i].param);
        }
    }
}

/**
  * @brief  time task pollings.
  * @param  None
  * @retval None
  */
void tm_task_tick_polling(void)  
{
    uint8_t i;

    sys_time_polling();

    sys_timer_tick_counter++;
    timer_tick_poll_counter++;
    /* the minimum timeout timer is limit to 5ms. */
    if(timer_tick_poll_counter >= SYS_TIMER_COUNTER)
    {
        timer_tick_poll_counter = 0;               
        for(i = 0x00; i < TIMER_MAX; i++)
        {
            if(timer_status[i].counter != TIMER_NOT_ACTIVE
                && timer_status[i].counter != TIMER_TIMEOUT)
            {
                timer_status[i].counter--;
            }
        }    
    }
}

/**
  * @brief  return current system tick, in millisecond.
  * @param  None
  * @retval current system tick, in millisecond.
  */
uint16_t tm_get_system_timer_tick(void)
{
    return( sys_timer_tick_counter );
}

/**
  * @brief  check if specific duration passed..
  * @param  tick : last tick returned by "tm_get_system_timer_tick"
*           duration: duration of time, in millisecond
  * @retval TRUE/FALSE
  */
uint8_t tm_is_time_passed(uint16_t tick, uint16_t duration)
{
    uint16_t temp = tm_get_system_timer_tick();
    
    if( tick > temp )
    {
        return( ((TIMER_MAX_SYS_TIMER_TICK - tick) + temp) > duration );
    }

    return( (temp - tick) > duration );
}

/**
  * @brief  check if specific timer is active or not.
  * @param  None
  * @retval None
  */
uint8_t tm_is_timer_active(uint8_t timer_id)
{
    return (uint8_t)(timer_status[timer_id].counter != TIMER_NOT_ACTIVE);
}

/**
  * @brief  delay us.
  * @param  None
  * @retval None
  */
//void delay_us(uint16_t us)
//{
//    uint32_t i = 30*us;
//    while(i--)
//    {
//        __asm__("nop");
//    }
//}

/**
  * @brief  delay ms.
  * @param  None
  * @retval None
  */
//void delay_ms(uint16_t ms)
//{
//    while(ms--)
//    {
//        delay_us(1000);
//    }
//}

void timer_led_test(uint16_t test)
{
}


/**************** END OF FILE *****************************************/
