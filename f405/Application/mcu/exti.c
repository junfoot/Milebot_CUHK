/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : exti.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : exti hal interface driver.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/

/* Public Includes -----------------------------------------------------------*/
#include "mcu_if.h"

/* Private Includes ----------------------------------------------------------*/
#include "exti.h"
#include "exti_if.h"

/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* Global functions ----------------------------------------------------------*/
/**
  * @brief  peripherals initialization.This function is responisble for setting up 
  *         the mcu after Reset.
  * @param  None
  * @retval None
  */
/**
* @brief This function handles EXTI line0 interrupt.
*/
void EXTI0_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
* @brief This function handles EXTI line3 interrupt.
*/
void EXTI3_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

/**
* @brief This function handles EXTI line[9:5] interrupts.
*/
void EXTI9_5_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
}

/**
* @brief This function handles EXTI line[15:10] interrupts.
*/
void EXTI15_10_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	#if 0
    if(GPIO_Pin == GPIO_PIN_3)  /* SD detect exti */
      {
          if(HAL_GPIO_ReadPin(SDCARD_DET_GPIO_Port, SDCARD_DET_Pin) == GPIO_PIN_SET)
          {
            sd_inserted_update(0);
        }
        else    /* 0-inserted. */
        {
            sd_inserted_update(1);
        }
      }
	#endif
}
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  mcu xx func config.
  * @param  None
  * @retval None
  */


/****************************** END OF FILE ***********************************/
