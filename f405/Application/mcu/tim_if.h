/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : tim_if.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : extern header file of TIM.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TIM_IF_H_
#define _TIM_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include "stm32f4xx_hal.h"

     
/**** Definition of constants ****/


/**** Definition of types ****/ 
typedef enum 
{
  TIM_NUM4 = 4U, 
  TIM_NUM9 = 9U,
  TIM_NUM1 = 1U,
  TIM_NUM13 = 13U,
  TIM_NUM14 = 14U,
} TIM_NUM_PARAM;

struct SYS_TIME 
{
  volatile uint32_t nb_sec;       ///< full seconds since startup
  volatile uint32_t nb_sec_rem;   ///< remainder of seconds since startup in CPU_TICKS
  volatile uint32_t nb_tick;      ///< SYS_TIME_TICKS since startup

  float resolution;               ///< sys_time_timer resolution in seconds
  uint32_t ticks_per_sec;         ///< sys_time ticks per second (SYS_TIME_FREQUENCY)
  uint32_t resolution_cpu_ticks;  ///< sys_time_timer resolution in cpu ticks
  uint32_t cpu_ticks_per_sec;     ///< cpu ticks per second
};
/**** Definition of macros ****/
#define TIM_CHANNEL_NUM1        TIM_CHANNEL_1
#define TIM_CHANNEL_NUM2        TIM_CHANNEL_2
#define TIM_CHANNEL_NUM3        TIM_CHANNEL_3
#define TIM_CHANNEL_NUM4        TIM_CHANNEL_4

#define TIM_ENC_PERIOD_VAL        65536
#define TIM_ENC_FILTER_VAL        6
/**** Declaration of constants ****/


/**** Declaration of variables ****/
extern struct SYS_TIME sys_time;

/**** Declaration of functions ****/
extern void sys_time_init(void);

extern void TIM7_IRQHandler(void);
extern void tim_sys_enable_irq(void);
extern void tim_sys_disable_irq(void);
extern void tim7_init(uint32_t prescaler, uint32_t period);
extern uint32_t systick_get_value(void);


#ifdef __cplusplus
}
#endif

#endif /*_TIM_IF_H_*/

/****************************** END OF FILE ***********************************/
