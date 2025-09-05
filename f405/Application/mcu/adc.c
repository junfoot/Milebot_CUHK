/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : adc.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : adc interface layer driver.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/

/* Public Includes -----------------------------------------------------------*/
#include "..\system\timer_if.h"

/* Private Includes ----------------------------------------------------------*/
#include "adc.h"
#include "adc_if.h"
#include "mcu_if.h"

/* Private defines -----------------------------------------------------------*/
#define BAT_BUF_LEN     150


/* Global variables ----------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;


__IO uint16_t uhADC3ConvertedValue[1];
/* Private variables ---------------------------------------------------------*/


/* Global functions ----------------------------------------------------------*/

/* ADC init function */
void adc_init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_112CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if(HAL_ADC_Start_DMA(&hadc1, (uint32_t*)uhADC3ConvertedValue, 1) != HAL_OK)
  {
    /* Start Conversation Error */
    _Error_Handler(__FILE__, __LINE__); 
  }

  //HAL_ADC_Start(&hadc1);

}


uint16_t get_ADC_value(uint8_t buf_index)
{
    return uhADC3ConvertedValue[buf_index];
}


//返回单个电芯的电压值, 毫伏
uint16_t get_bat_value(void)
{
    //电池为6个电芯, ADC 采样分压1/21
    //vol = (3300/4096)*(21/6)*ADC = 2.81982421875*ADC
    
    uint32_t volt_value = 0;
    uint8_t i = 0;
    static uint16_t volt_adc_table[BAT_BUF_LEN] = {0};
    static uint8_t index = 0;
    static uint8_t total_num = 0;

    volt_adc_table[index] = uhADC3ConvertedValue[0];
    index++;
    if (index >= BAT_BUF_LEN)
    {
        index = 0;
    }

    total_num++;
    if (total_num >= BAT_BUF_LEN)
    {
        total_num = BAT_BUF_LEN;
    }

    for (i = 0; i < total_num; i++)
    {
        volt_value += volt_adc_table[i];
    }
    volt_value = (uint32_t)((volt_value/total_num)*2.81982421875F);

    return (uint16_t)(volt_value);
    
}



/**
  * @brief This function handles ADC1, ADC2 and ADC3 global interrupts.
  */
void ADC_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&hadc1);
}


/**
  * @brief This function handles DMA2 stream0 global interrupt.
  */
void DMA2_Stream0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_adc1);
}


