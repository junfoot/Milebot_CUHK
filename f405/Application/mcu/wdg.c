/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : wdg.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : wdg interface layer driver.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/

/* Public Includes -----------------------------------------------------------*/
#include "mcu_if.h"

/* Private Includes ----------------------------------------------------------*/
#include "wdg.h"
#include "wdg_if.h"

/* Global variables ----------------------------------------------------------*/
WWDG_HandleTypeDef hwwdg;

/* Private variables ---------------------------------------------------------*/


/* Global functions ----------------------------------------------------------*/
/* WWDG init function */
void wdg_init(void)
{
  hwwdg.Instance = WWDG;
  /* WWDG clock counter = (PCLK1 (42MHz)/4096)/8 = 1281 Hz (~780 us)  */
  hwwdg.Init.Prescaler = WWDG_PRESCALER_8;
  /* Set Window value to 120; WWDG counter should be refreshed only when the counter
    is below 80 (and greater than 64) otherwise a reset will be generated */
  hwwdg.Init.Window = 120;
  /* Enable WWDG and set counter value to 127, WWDG timeout = ~780 us * 64 = 49.92 ms 
    In this case the refresh window is: ~780 * (127-120) = 5.46ms < refresh window < ~780 * 64 = 49.9ms
  */
  hwwdg.Init.Counter = 127;
  hwwdg.Init.EWIMode = WWDG_EWI_ENABLE;
  if (HAL_WWDG_Init(&hwwdg) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  __HAL_DBGMCU_FREEZE_WWDG();    /* TODOR: release version remove. */
}

/**
  * @brief  wdg feed.
  * @param  None
  * @retval None
  */
void wdg_feed(void)
{
    hwwdg.Init.Counter = 127;
    HAL_WWDG_Refresh(&hwwdg);
}

/**
* @brief This function handles Window watchdog interrupt.
*/
void WWDG_IRQHandler(void)
{
    HAL_WWDG_IRQHandler(&hwwdg);
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  mcu xx func config.
  * @param  None
  * @retval None
  */


/****************************** END OF FILE ***********************************/
