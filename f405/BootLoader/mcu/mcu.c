/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : mcu.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : mcu peripherals config.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/
#include <stdio.h>

/* Public Includes -----------------------------------------------------------*/

/* Private Includes ----------------------------------------------------------*/
#include "mcu.h"
#include "mcu_if.h"
#include "uart_if.h"

/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* Global functions ----------------------------------------------------------*/
/**
  * @brief  peripherals initialization.This function is responisble for setting up 
  *         the mcu after Reset.
  * @param  None
  * @retval None
  */
void mcu_init(void)
{  
    /* Initialize all configured peripherals */
    uart5_init(921600);
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  mcu system clock configure.
  * @param  None
  * @retval None
  */
void mcu_system_clk_config(void) 
{

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage */
    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = MCU_PLL_M;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
          _Error_Handler(__FILE__, __LINE__);
    }

    /**Initializes the CPU, AHB and APB busses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
          _Error_Handler(__FILE__, __LINE__);
    }

    /**Enables the Clock Security System */
    //HAL_RCC_EnableCSS();

    /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
    if (HAL_GetREVID() == 0x1001)
    {
          /* Enable the Flash prefetch */
          __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    }

    /**Configure the Systick interrupt time */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


/**
  * @brief  mcu gpio init for reset state.
  * @param  None
  * @retval None
  */
void mcu_io_init_reset(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, PA4_BLE_MODE_Pin|PA5_BLE_WAKEUP_Pin|PA7_BT_POW_CN_Pin|PA8_STATLED_CONT_Pin|PA15_LED_CONT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, PB14_CAN2_STB_Pin|PB15_RMOTOR_POWER_CTR_Pin|PB6_LMOTOR_POWER_CTR_Pin|PB7_CAN1_STB_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins :  */
  GPIO_InitStruct.Pin = PA4_BLE_MODE_Pin|PA5_BLE_WAKEUP_Pin|PA7_BT_POW_CN_Pin|PA8_STATLED_CONT_Pin|PA15_LED_CONT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB1_IMU_INT_Pin PWR_CHARGE_DET_Pin */
  GPIO_InitStruct.Pin = PB1_IMU_INT_Pin|PWR_CHARGE_DET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB14_CAN2_STB_Pin PB15_RMOTOR_POWER_CTR_Pin PB6_LMOTOR_POWER_CTR_Pin PB7_CAN1_STB_Pin */
  GPIO_InitStruct.Pin = PB14_CAN2_STB_Pin|PB15_RMOTOR_POWER_CTR_Pin|PB6_LMOTOR_POWER_CTR_Pin|PB7_CAN1_STB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC8_SYN6658_BUSY_Pin */
  GPIO_InitStruct.Pin = PC8_SYN6658_BUSY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PC8_SYN6658_BUSY_GPIO_Port, &GPIO_InitStruct);

}

/**
  * @brief  Enables IRQ interrupts by setting the I-bit in the CPSR.Can only be executed in Privileged modes.
  * @param  None
  * @retval None
  */
void mcu_enable_interrupt(void)
{
    __enable_fault_irq();
}

/**
  * @brief  Disables IRQ interrupts by setting the I-bit in the CPSR.Can only be executed in Privileged modes.
  * @param  None
  * @retval None
  */
void mcu_disable_interrupt(void)
{
    __disable_fault_irq();
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
}


/**
  * @brief  mcu reset by software.
  * @param  None
  * @retval None
  */
void mcu_sw_reset(void)
{
    NVIC_SystemReset();
}



/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
    while(1);
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
    while(1);
}

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
    while(1);
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
    while(1);
}

/**
* @brief This function handles Pre-fetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
    while(1);
}


