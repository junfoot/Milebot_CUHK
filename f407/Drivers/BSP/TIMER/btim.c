/**
 ****************************************************************************************************
 * @file        btim.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-15
 * @brief       基本定时器 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211015
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "./BSP/LED/led.h"
#include "./BSP/TIMER/btim.h"
#include "./BSP/LCD/lcd.h"
#include "DS_300c.h"
#include "./SYSTEM/delay/delay.h"
#include "gait.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

TIM_HandleTypeDef g_tim6_handler;         /* 定时器参数句柄 */
TIM_HandleTypeDef g_tim7_handler; 

extern QueueHandle_t sem1;
extern QueueHandle_t sem3;

/**
 * @brief       基本定时器TIMX定时中断初始化函数
 * @note
 *              基本定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              基本定时器的时钟为APB1时钟的2倍, 而APB1为42M, 所以定时器时钟 = 84Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr : 自动重装值。
 * @param       psc : 时钟预分频数
 * @retval      无
 */
void btim_tim6_int_init(uint16_t arr, uint16_t psc)
{
	BTIM_TIM6_INT_CLK_ENABLE();                     /* 使能TIMx时钟 */
	
    g_tim6_handler.Instance = BTIM_TIM6_INT;                      /* 定时器x */
    g_tim6_handler.Init.Prescaler = psc;                          /* 分频 */
    g_tim6_handler.Init.CounterMode = TIM_COUNTERMODE_UP;         /* 递增计数模式 */
    g_tim6_handler.Init.Period = arr;                             /* 自动装载值 */
    HAL_TIM_Base_Init(&g_tim6_handler);
	
	HAL_NVIC_SetPriority(BTIM_TIM6_INT_IRQn, 5, 0); /* 抢占1，子优先级4 */
	HAL_NVIC_EnableIRQ(BTIM_TIM6_INT_IRQn);         /* 开启ITMx中断 */
    
    HAL_TIM_Base_Start_IT(&g_tim6_handler);                       /* 使能定时器x和定时器更新中断 */
}

/**
 * @brief       基本定时器TIMX中断服务函数
 * @param       无
 * @retval      无
 */
void BTIM_TIM6_INT_IRQHandler(void)
{
//    HAL_TIM_IRQHandler(&g_timx_handler);  /* 定时器回调函数 */
    BaseType_t err;
    if(__HAL_TIM_GET_FLAG(&g_tim6_handler, TIM_FLAG_UPDATE) != RESET)
    {	
        if(sem1 != NULL)
        {
            err = xSemaphoreGiveFromISR(sem1, NULL);
        }
        
    }
    __HAL_TIM_CLEAR_IT(&g_tim6_handler, TIM_IT_UPDATE);
}



void btim_tim7_int_init(uint16_t arr, uint16_t psc)
{
	BTIM_TIM7_INT_CLK_ENABLE();                     /* 使能TIMx时钟 */
	
    g_tim7_handler.Instance = BTIM_TIM7_INT;                      /* 定时器x */
    g_tim7_handler.Init.Prescaler = psc;                          /* 分频 */
    g_tim7_handler.Init.CounterMode = TIM_COUNTERMODE_UP;         /* 递增计数模式 */
    g_tim7_handler.Init.Period = arr;                             /* 自动装载值 */
    HAL_TIM_Base_Init(&g_tim7_handler);
	
	
	HAL_NVIC_SetPriority(BTIM_TIM7_INT_IRQn, 5, 0); /* 抢占1，子优先级3 */
	HAL_NVIC_EnableIRQ(BTIM_TIM7_INT_IRQn);         /* 开启ITMx中断 */
   
    HAL_TIM_Base_Start_IT(&g_tim7_handler);                       /* 使能定时器x和定时器更新中断 */
}

/**
 * @brief       基本定时器TIMX中断服务函数
 * @param       无
 * @retval      无
 */
void BTIM_TIM7_INT_IRQHandler(void)
{
//    HAL_TIM_IRQHandler(&g_timx_handler);  /* 定时器回调函数 */
    BaseType_t err;
    if(__HAL_TIM_GET_FLAG(&g_tim7_handler, TIM_FLAG_UPDATE) != RESET)
    {
        if(sem3 != NULL)  // 释放信号量sem3
        {
            err = xSemaphoreGiveFromISR(sem3, NULL);
        }
    }
    __HAL_TIM_CLEAR_IT(&g_tim7_handler, TIM_IT_UPDATE);
}

