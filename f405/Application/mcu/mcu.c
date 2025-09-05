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
#include "dma_if.h"
#include "can_if.h"
//#include "i2c_if.h"
#include "spi_if.h"
#include "uart_if.h"
#include "wdg_if.h"
#include "uart_if.h"  
#include "at24c256.h"

/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
volatile struct MCU_FAULT_INFO mcu_fault __attribute__((section ("backup_ram_data")));
volatile static uint8_t mcu_fault_read_flag = 0;

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
    AT24CXX_Init();
    dma_init();
    uart1_init();
    uart2_init(115200);
    uart5_init(115200);
    uart6_init();
    CAN_Init(CAN1, 1000);
    CAN_Init(CAN2, 1000);
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

  /*Configure GPIO pin : FUNCTION_SWITCH_Pin */
  GPIO_InitStruct.Pin = FUNCTION_SWITCH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(FUNCTION_SWITCH_GPIO_Port, &GPIO_InitStruct);

}



/**
  * @brief  Enables IRQ interrupts by setting the I-bit in the CPSR.Can only be executed in Privileged modes.
  * @param  None
  * @retval None
  */
void mcu_enable_interrupt(void)
{
    //CPSIE i;
    //BX  lr;
    __enable_fault_irq();
}

/**
  * @brief  Disables IRQ interrupts by setting the I-bit in the CPSR.Can only be executed in Privileged modes.
  * @param  None
  * @retval None
  */
void mcu_disable_interrupt(void)
{
    //CPSID i;
    //BX  lr;
    __disable_fault_irq();
}

uint32_t  fail_line;
char *fail_file;
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
      mcu_disable_interrupt();
      fail_file = file;
      fail_line = line;
    mcu_enable_interrupt();
      
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @brief  mcu fault data reset.
  * @param  None
  * @retval None
  */
void mcu_fault_reset(void)
{
    //mcu_fault.hfsr = 0x00;
    //mcu_fault.bfar = 0x00;
    //mcu_fault.mmfar = 0x00;
    //mcu_fault.cfsr = 0x00;
    for(uint8_t i=0; i<8; i++)
    {
        mcu_fault.stack_data[i] = 0x00;
    }
    mcu_fault_read_flag = 0x00;
    mcu_fault.reset_type = 0x00;
    mcu_fault.reset_src = 0x00;
}

/**
  * @brief  this function return a flag to indicate reset reason
  * @param  None
  * @retval reset reason flag
  */
MCU_RESET_TYPE mcu_get_reset_reason(void)
{
    uint32_t rst_source = RCC->CSR;
    MCU_RESET_TYPE ret_val = MCU_RESET_BY_ERR;
    
    mcu_fault.reset_src = rst_source;
    
    if( (rst_source & RCC_CSR_PORRSTF) == RCC_CSR_PORRSTF )    /* power on */
    {
        mcu_fault_reset();
        ret_val = MCU_RESET_BY_PWR;
    }
    else if( (rst_source & RCC_CSR_WWDGRSTF_Msk) == RCC_CSR_WWDGRSTF_Msk )
    {
        ret_val = MCU_RESET_BY_ERR;
    }
    else if( (rst_source & RCC_CSR_SFTRSTF_Msk) == RCC_CSR_SFTRSTF_Msk )
    {
        ret_val = MCU_RESET_BY_SW;
    }
    
    #if 0
    volatile uint8_t wbuf[20];
    wbuf[0] = (uint8_t)(rst_source&0xFF);
    wbuf[1] = (uint8_t)((rst_source>>8)&0xFF);
    wbuf[2] = (uint8_t)((rst_source>>16)&0xFF);
    wbuf[3] = (uint8_t)((rst_source>>24)&0xFF);
    uart1_send_data((uint8_t*)wbuf, 4);
    #endif

    RCC->CSR |= RCC_CSR_RMVF;
    return ret_val;
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
  * @brief  get mcu fault stack data.
  * @param  buf: LR value.
  * @retval None
  */
void mcu_get_fault_stack_data(uint32_t *buf)
{
    buf[0] = mcu_fault.stack_data[5];
    buf[1] = mcu_fault.stack_data[6];
    buf[2] = mcu_fault.reset_src;
}

/**
  * @brief  read mcu fault stack data.
  * @param  offset: psp or msp offset value
  * @retval None
  */
void mcu_read_fault_stack_data(uint16_t offset)
{
    __disable_fault_irq();
    
    if(mcu_fault_read_flag != TRUE)
    {
        mcu_fault_read_flag = TRUE;
        volatile uint32_t psp = __get_PSP();
        volatile uint32_t *vp = (uint32_t*)(psp + offset);

        for(uint8_t i=0; i<8; i++)
        {
            mcu_fault.stack_data[i] = *(vp+i);
        }
    }
    
    __enable_fault_irq();
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
    mcu_read_fault_stack_data(0x00);
    while(1);
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
    mcu_read_fault_stack_data(0x00);
    while(1);
}

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
    mcu_read_fault_stack_data(0x00);
    HAL_RCC_NMI_IRQHandler();
    while(1);
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
    mcu_read_fault_stack_data(0x00);
    while(1);
}

/**
* @brief This function handles Pre-fetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
    mcu_read_fault_stack_data(0x00);
    while(1);
}

#ifdef POWER_WATCHDOG_OPTION
/**
  * @brief  WWDG Early Wakeup callback, feed time is:5ms ~ 49ms.
  * @param  hwwdg : pointer to a WWDG_HandleTypeDef structure that contains
  *                the configuration information for the specified WWDG module.
  * @retval None
  */
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef* hwwdg)
{
    mcu_read_fault_stack_data(0x00);
    while(1);
}
#endif    /* POWER_WATCHDOG_OPTION */

/****************************** END OF FILE ***********************************/

