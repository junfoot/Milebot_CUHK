/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : adc_if.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : extern header file of adc.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _ADC_IF_H_
#define _ADC_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include <stdint.h>
#include "stm32f4xx_hal.h"
     
/**** Definition of constants ****/


/**** Definition of types ****/ 


/**** Definition of macros ****/


/**** Declaration of constants ****/


/**** Declaration of variables ****/
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;


/**** Declaration of functions ****/
void adc_init(void);
uint16_t get_ADC_value(uint8_t buf_index);
uint16_t get_bat_value(void);


#ifdef __cplusplus
}
#endif

#endif /*_ADC_IF_H_*/

/****************************** END OF FILE ***********************************/
