/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : tim.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : tim interface layer driver.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date        Name        Changes and comments
* v0.01     2019/10/10  cz    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/

/* Public Includes -----------------------------------------------------------*/
#include "..\system\std.h"
#include "mcu_if.h"
#include "..\power\power_drv_if.h"
#include "..\system\sys_task_if.h"

/* Private Includes ----------------------------------------------------------*/
#include "tim.h"
#include "tim_if.h"

static TIM_HandleTypeDef htim7;

/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);


/* Global functions ----------------------------------------------------------*/


/**
  * @brief  enable interrupt.
  * @param  None
  * @retval None
  */
void tim_sys_enable_irq(void)
{
    HAL_NVIC_EnableIRQ(TIM7_IRQn);
}

/**
  * @brief  disable interrupt.
  * @param  None
  * @retval None
  */
void tim_sys_disable_irq(void)
{
    HAL_NVIC_DisableIRQ(TIM7_IRQn);
}



/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim: TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim7)
    {
        /* 1ms interrupt handle function */
        sys_timer_isr_handler();
    }
}

/**
* @brief This function handles TIM7 global interrupt.
*/
void TIM7_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim7);
}

/**
  * @brief  PWM Pulse finished callback in non blocking mode 
  * @param  htim: pointer to a TIM_HandleTypeDef structure that contains
  *                the configuration information for TIM module.
  * @retval None
  */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{

    
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  TIM7 init.
  * @param  None
  * @retval None
  */
void tim7_init(uint32_t prescaler, uint32_t period)
{
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = prescaler-1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = period-1;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  /* Start Channel1 */
  if(HAL_TIM_Base_Start_IT(&htim7) != HAL_OK)
  {
    /* Starting Error */
    _Error_Handler(__FILE__, __LINE__);
  }

}

/**
  * @brief  get system tim current counter.
  * @param  None
  * @retval None
  */
uint32_t systick_get_value(void)
{
    return __HAL_TIM_GET_COUNTER(&htim7);
}
/****************************** END OF FILE ***********************************/
