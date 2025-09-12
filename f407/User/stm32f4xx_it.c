/**
  ******************************************************************************
  * @file    Templates/Src/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal.h"

#include "./SYSTEM/SYS/sys.h"
#include "./BSP/CAN/can.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/DMA/dma.h"

#include "./BSP/USART2/usart2.h"
#include "./BSP/USART3/usart3.h"
#include "./BSP/UART5/uart5.h"
#include "./BSP/USART6/usart6.h"
/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

extern CAN_HandleTypeDef   g_canx_handler;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
#if (!SYS_SUPPORT_OS)
void SVC_Handler(void)
{
}
#endif

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
#if (!SYS_SUPPORT_OS)
void PendSV_Handler(void)
{
}
#endif

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
#if (!SYS_SUPPORT_OS)
void SysTick_Handler(void)
{
  HAL_IncTick();
}
#endif

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @brief This function handles CAN1 RX0 interrupts.
  */
//void CAN1_RX0_IRQHandler(void)
//{
//  HAL_CAN_IRQHandler(&g_canx_handler);

//}

void DMA2_Stream7_IRQHandler(void)
{
    if (__HAL_DMA_GET_FLAG(&g_dma_handle_usart1_tx, DMA_FLAG_TCIF3_7))
    {
        __HAL_DMA_CLEAR_FLAG(&g_dma_handle_usart1_tx, DMA_FLAG_TCIF3_7);
        __HAL_UART_CLEAR_FLAG(&g_usart1_handler, UART_FLAG_TC);
//				HAL_UART_DMAStop(&g_usart1_handler);
				/* *************************************************************** */
				uint32_t dmarequest = 0x00U;
				dmarequest = HAL_IS_BIT_SET(g_usart1_handler.Instance->CR3, USART_CR3_DMAT);
				if ((g_usart1_handler.gState == HAL_UART_STATE_BUSY_TX) && dmarequest)
				{
					CLEAR_BIT(g_usart1_handler.Instance->CR3, USART_CR3_DMAT);

					/* Abort the UART DMA Tx stream */
					if (g_usart1_handler.hdmatx != NULL)
					{
						HAL_DMA_Abort(g_usart1_handler.hdmatx);
					}
					/* Disable TXEIE and TCIE interrupts */
					CLEAR_BIT(g_usart1_handler.Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));
					/* At end of Tx process, restore huart->gState to Ready */
					g_usart1_handler.gState = HAL_UART_STATE_READY;
				}
				/* *************************************************************** */
			
    }
}

void DMA1_Stream6_IRQHandler(void)
{
    if (__HAL_DMA_GET_FLAG(&g_dma_handle_usart2_tx, DMA_FLAG_TCIF2_6))
    {
        __HAL_DMA_CLEAR_FLAG(&g_dma_handle_usart2_tx, DMA_FLAG_TCIF2_6);
        __HAL_UART_CLEAR_FLAG(&g_usart2_handler, UART_FLAG_TC);
//        HAL_UART_DMAStop(&g_usart2_handler);      /* 关闭串口DMA */ 
				/* *************************************************************** */
				uint32_t dmarequest = 0x00U;
				dmarequest = HAL_IS_BIT_SET(g_usart2_handler.Instance->CR3, USART_CR3_DMAT);
				if ((g_usart2_handler.gState == HAL_UART_STATE_BUSY_TX) && dmarequest)
				{
					CLEAR_BIT(g_usart2_handler.Instance->CR3, USART_CR3_DMAT);

					/* Abort the UART DMA Tx stream */
					if (g_usart2_handler.hdmatx != NULL)
					{
						HAL_DMA_Abort(g_usart2_handler.hdmatx);
					}
					/* Disable TXEIE and TCIE interrupts */
					CLEAR_BIT(g_usart2_handler.Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));
					/* At end of Tx process, restore huart->gState to Ready */
					g_usart2_handler.gState = HAL_UART_STATE_READY;
				}
				/* *************************************************************** */			
    }
}

//void DMA1_Stream3_IRQHandler(void)
//{
//    if (__HAL_DMA_GET_FLAG(&g_dma_handle_usart3, DMA_FLAG_TCIF3_7))
//    {
//        __HAL_DMA_CLEAR_FLAG(&g_dma_handle_usart3, DMA_FLAG_TCIF3_7);
//        __HAL_UART_CLEAR_FLAG(&g_usart3_handler, UART_FLAG_TC);
//        HAL_UART_DMAStop(&g_usart3_handler);      /* 关闭串口DMA */  
//    }
//}

//void DMA1_Stream7_IRQHandler(void)
//{
//    if (__HAL_DMA_GET_FLAG(&g_dma_handle_uart5, DMA_FLAG_TCIF3_7))
//    {
//        __HAL_DMA_CLEAR_FLAG(&g_dma_handle_uart5, DMA_FLAG_TCIF3_7);
//        __HAL_UART_CLEAR_FLAG(&g_uart5_handler, UART_FLAG_TC);
//        HAL_UART_DMAStop(&g_uart5_handler);      /* 关闭串口DMA */  
//    }
//}

//void DMA2_Stream7_IRQHandler(void)
//{
//    if (__HAL_DMA_GET_FLAG(&g_dma_handle_usart6, DMA_FLAG_TCIF3_7))
//    {
//        __HAL_DMA_CLEAR_FLAG(&g_dma_handle_usart6, DMA_FLAG_TCIF3_7);
//        __HAL_UART_CLEAR_FLAG(&g_usart6_handler, UART_FLAG_TC);
//        HAL_UART_DMAStop(&g_usart6_handler);      /* 关闭串口DMA */  
//    }
//}
//void DMA2_Stream2_IRQHandler(void)
//{

//    if (__HAL_DMA_GET_FLAG(&g_dma_handle, DMA_FLAG_TCIF2_6))        /* DMA2_Stream7传输完成 */
//    {
//        __HAL_DMA_CLEAR_FLAG(&g_dma_handle, DMA_FLAG_TCIF2_6);      /* 清除DMA2_Stream7传输完成标志 */
//        __HAL_UART_CLEAR_FLAG(&g_usart1_handler, UART_FLAG_TC);
//        
//        HAL_UART_DMAStop(&g_usart1_handler);      /* 关闭串口DMA */  
//    }
//}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
