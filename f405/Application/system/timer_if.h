/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : timer_if.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : extern header file of timer.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TIMER_IF_H_
#define _TIMER_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include <stdint.h>  
#include "..\mcu\tim_if.h"

/**** Definition of constants ****/


/**** Definition of types ****/ 
typedef enum
{
    TIMER_TASK_POWER,
    TIMER_TASK_CAN,
    TIMER_TASK_ENG,
    TIMER_TASK_SYS,
    TIMER_TASK_MMI,
    TIMER_TASK_LOG,
    TIMER_TASK_MAX    
}TIMER_TASK_IDS;

typedef enum
{
    TIMER_PERIODIC,    
    TIMER_ONE_SHOT,
    TIMER_TWO_SHOT,
    TIMER_THREE_SHOT    
}TIMER_TYPES;

typedef struct
{
    uint16_t counter;
    uint16_t backupCounter;
    uint16_t param;
    uint8_t type;
    uint8_t rep_cnt;
}TIMER_CLASS;

typedef void (*TIMER_FUNC) (uint16_t param);

typedef struct
{
    TIMER_FUNC action_func;
    uint8_t task_id; 
}TIMER_DESC_TYPE;

/**** Definition of macros ****/
#define TIMER_TIMEOUT               0x0000
#define TIMER_NOT_ACTIVE            0xFFFF
#define TIMER_NO_PARAM              0x00
#define TIMER_NO_BACKUP_COUNTER     0xFFFF
#define TIMER_MAX_SYS_TIMER_TICK    0xFFFF

/**** Declaration of constants ****/
#define HW_TIMER_TICK                   1    /* 1ms */
#define SYS_TIMER_COUNTER               5
#define SYS_TIMER_TICK                  (SYS_TIMER_COUNTER)   /* SYS_TIMER_COUNTER*HW_TIMER_TICK */

#define MSECOND     / SYS_TIMER_TICK
#define SECOND      *1000L/ SYS_TIMER_TICK
#define MSECONDS    / SYS_TIMER_TICK
#define SECONDS     *1000L/ SYS_TIMER_TICK
#define MINUTE      *60000L/ SYS_TIMER_TICK
#define MINUTES     *60000L/ SYS_TIMER_TICK



#define USEC_OF_SEC(sec) ((sec) * 1e6)
/**** Declaration of variables ****/


/**** Declaration of functions ****/
extern void tm_reset_create(void);
extern void tm_create_timer(uint8_t timer_id, uint16_t period, uint8_t type, uint16_t param);
extern void tm_kill_timer(uint8_t timer_id);
extern void tm_msg_handler(uint32_t *mptr);
extern void tm_kill_msg_handler(uint32_t *pTimer_id);
extern void tm_stimulate(uint8_t task_id);
extern void tm_task_tick_polling(void);
extern uint16_t tm_get_system_timer_tick(void);
extern uint8_t tm_is_time_passed(uint16_t tick, uint16_t duration);
extern uint8_t tm_is_timer_active(uint8_t timer_id);
//extern void delay_us(uint16_t us);
//extern void delay_ms(uint16_t ms);
//extern void delay_100ns(uint16_t ns);


/**
 * Get the time in seconds since startup.
 * @return current system time as float with sys_time.resolution
 */
static inline float get_sys_time_float(void)
{
  return (float)(sys_time.nb_sec + (float)(sys_time.nb_sec_rem) / sys_time.cpu_ticks_per_sec);
}


/*
 * Convenience functions to convert between seconds and sys_time ticks.
 */
static inline uint32_t sys_time_ticks_of_sec(float seconds)
{
  return (uint32_t)(seconds * sys_time.ticks_per_sec + 0.5f);
}

static inline uint32_t sys_time_ticks_of_msec(uint32_t msec)
{
  return msec * sys_time.ticks_per_sec / 1000;
}

static inline uint32_t sys_time_ticks_of_usec(uint32_t usec)
{
  return usec * sys_time.ticks_per_sec / 1000000;
}

static inline float sec_of_sys_time_ticks(uint32_t ticks)
{
  return (float)ticks * sys_time.resolution;
}

static inline uint32_t msec_of_sys_time_ticks(uint32_t ticks)
{
  return ticks * 1000 / sys_time.ticks_per_sec;
}

static inline uint32_t usec_of_sys_time_ticks(uint32_t ticks)
{
  return ticks * 1000 / sys_time.ticks_per_sec * 1000;
}



/*
 * Convenience functions to convert between seconds and CPU ticks.
 */
static inline uint32_t cpu_ticks_of_sec(float seconds)
{
  return (uint32_t)(seconds * sys_time.cpu_ticks_per_sec + 0.5f);
}

static inline uint32_t cpu_ticks_of_usec(uint32_t usec)
{
  return usec * (sys_time.cpu_ticks_per_sec / 1000000);
}

static inline int32_t signed_cpu_ticks_of_usec(int32_t usec)
{
  return usec * ((int32_t)sys_time.cpu_ticks_per_sec / 1000000);
}

static inline uint32_t cpu_ticks_of_nsec(uint32_t nsec)
{
  return nsec * (sys_time.cpu_ticks_per_sec / 1000000) / 1000;
}

static inline uint32_t msec_of_cpu_ticks(uint32_t cpu_ticks)
{
  return cpu_ticks / (sys_time.cpu_ticks_per_sec / 1000);
}

static inline uint32_t usec_of_cpu_ticks(uint32_t cpu_ticks)
{
  return cpu_ticks / (sys_time.cpu_ticks_per_sec / 1000000);
}

static inline uint32_t nsec_of_cpu_ticks(uint32_t cpu_ticks)
{
  return cpu_ticks / (sys_time.cpu_ticks_per_sec / 1000000) / 1000;
}

static inline void sys_time_usec_ctl_overflow(int32_t *dt, uint32_t diff)
{
    if(*dt < 0)
    {
        *dt += 0xFFFFFFFF;
    }

    if(*dt > diff)
    {
        *dt = diff;
    }
}

/**
 * Get the time in microseconds since startup.
 * WARNING: overflows after 70min!
 * @return microseconds since startup as uint32_t
 */
static inline uint32_t get_sys_time_usec(void)
{
  return sys_time.nb_sec * 1000000 +
         usec_of_cpu_ticks(sys_time.nb_sec_rem) +
         usec_of_cpu_ticks(systick_get_value());
}

/**
 * Get the time in milliseconds since startup.
 * @return milliseconds since startup as uint32_t
 */
static inline uint32_t get_sys_time_msec(void)
{
  return sys_time.nb_sec * 1000 +
         msec_of_cpu_ticks(sys_time.nb_sec_rem) +
         msec_of_cpu_ticks(systick_get_value());
}

/* Generic timer macros */
#define SysTimeTimerStart(_t) { _t = get_sys_time_usec(); }
#define SysTimeTimer(_t) ( get_sys_time_usec() - (_t))
#define SysTimeTimerStop(_t) { _t = ( get_sys_time_usec() - (_t)); }

#ifdef __cplusplus
}
#endif

#endif /*_TIMER_IF_H_*/

/****************************** END OF FILE ***********************************/
