/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : adc_app.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : adc application layer function.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/

/* Public Includes -----------------------------------------------------------*/
#include "..\mcu\adc_if.h"


/* Private Includes ----------------------------------------------------------*/
#include "adc_app.h"
#include "adc_app_if.h"

/* Global variables ----------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Global functions ----------------------------------------------------------*/
/**
  * @brief  init adc.
  * @param  None
  * @retval None
  */
void adc_reset_create(void)
{  
    adc_init();
}


