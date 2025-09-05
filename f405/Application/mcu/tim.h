/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : tim.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : private header file of TIM.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TIM_H_
#define _TIM_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
     
/**** Definition of constants ****/
#ifdef USE_MAXON_DRIVER 
#define PWM_FREQ             1000U    /* 1000MHz */
#endif
#ifdef USE_AMC_DRIVER 
#define PWM_FREQ               15000U    /* 15000Hz */
#endif

/**** Definition of types ****/ 


/**** Definition of macros ****/
//#define TIM1_CLK_FREQ            168000000U    /* 168MHz */
//#define TIM1_PWM_FREQ            1000U    /* 1000Hz */
//#define TIM1_INIT_PERIOD_VAL    999U    /* pwm freq 1000Hz */
//#define TIM1_FRESCALER_VAL         (TIM1_CLK_FREQ/TIM1_PWM_FREQ/(TIM1_INIT_PERIOD_VAL+1) - 1)
//#define TIM1_CH1_INTI_DUTY        0
//
//#define TIM9_CLK_FREQ            168000000U    /* 168MHz */
//#define TIM9_PWM_FREQ            PWM_FREQ    /* 1500Hz */
//#define TIM9_INIT_PERIOD_VAL    999U    /* pwm freq 1000Hz */
//#define TIM9_FRESCALER_VAL         (TIM9_CLK_FREQ/TIM9_PWM_FREQ/(TIM9_INIT_PERIOD_VAL+1) - 1)
//#define TIM9_CH1_INTI_DUTY        0
//
//#define TIM4_CLK_FREQ            84000000U    /* 168MHz */
//#define TIM4_PWM_FREQ            PWM_FREQ    /* 15000Hz */
//#define TIM4_INIT_PERIOD_VAL    999U    /* pwm freq 1000Hz */
//#define TIM4_FRESCALER_VAL         (TIM4_CLK_FREQ/TIM4_PWM_FREQ/(TIM4_INIT_PERIOD_VAL+1) - 1)
//#define TIM4_CH1_INTI_DUTY        0
//
//#define TIM13_CLK_FREQ            84000000U    /* 84MHz */
//#define TIM13_PWM_FREQ            PWM_FREQ    /* 1500Hz */
//#define TIM13_INIT_PERIOD_VAL    999U    /* pwm freq 1000Hz */
//#define TIM13_FRESCALER_VAL         (TIM13_CLK_FREQ/TIM13_PWM_FREQ/(TIM13_INIT_PERIOD_VAL+1) - 1)
//#define TIM13_CH1_INTI_DUTY        0
//
//#define TIM14_CLK_FREQ            84000000U    /* 84MHz */
//#define TIM14_PWM_FREQ            PWM_FREQ    /* 1500Hz */
//#define TIM14_INIT_PERIOD_VAL    999U    /* pwm freq 1000Hz */
//#define TIM14_FRESCALER_VAL         (TIM14_CLK_FREQ/TIM14_PWM_FREQ/(TIM14_INIT_PERIOD_VAL+1) - 1)
//#define TIM14_CH1_INTI_DUTY        0

/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
void MX_TIM3_Init(void);

void MX_TIM10_Init(void);
void MX_TIM11_Init(void);



#ifdef __cplusplus
}
#endif

#endif /*_TIM_H_*/

/****************************** END OF FILE ***********************************/
