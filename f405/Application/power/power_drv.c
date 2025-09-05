/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : power_drv.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : power control driver.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/

/* Public Includes -----------------------------------------------------------*/
#include "..\mcu\mcu_if.h"
#include "..\modules\fram\fram_if.h"

/* Private Includes ----------------------------------------------------------*/
#include "..\power\power_mana.h"
#include "..\power\power_mana_if.h"
#include "..\power\power_drv.h"
#include "..\power\power_drv_if.h"


/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t pw_is_first_connect;
static uint8_t pw_is_reset_by_watchdog;

/* Global functions ----------------------------------------------------------*/

/**
  * @brief  switch on all power for active mode.
  * @param  None
  * @retval None
  */
void pw_power_on(void)
{
    BT_WAKEUP_enable();
    BT_POW_enable();
    CAN1_receive_enable();
    CAN2_receive_enable();
    LMOTOR_POWER_enable();
    RMOTOR_POWER_enable();
    STATLED_enable();
}

/**
  * @brief  check which source trigger the mcu start.
  * @param  None
  * @retval None
  */
void pw_check_mcu_reset_source(void)
{
    uint16_t rst_source;
    rst_source = (uint16_t)mcu_get_reset_reason();
    pw_is_reset_by_watchdog = FALSE;
    switch(rst_source)
    {
        case MCU_RESET_BY_PWR:
        case MCU_RESET_BY_PIN:    /* reset pin issue a reset */
            //pw_init_all_misc_variables();            
            pw_is_first_connect = TRUE;      
            break; 
        case MCU_RESET_BY_ERR:  /* watchdog issue a reset */
            pw_is_first_connect = TRUE;
            pw_is_reset_by_watchdog = TRUE;
            break;
        case MCU_RESET_BY_SW:
        default:
            pw_is_first_connect = FALSE;
            break;     
    }
}


/**
  * @brief  handle power reset.
  * @param  None
  * @retval None
  */
void pw_reset_cause_handle(void)
{
    if(pw_is_reset_by_watchdog)
    {
#if 0   /* Begin if 0 by lison on 2020-6-16 */
        volatile uint32_t rbuf[3];
        //uint8_t wbuf[10];
        mcu_get_fault_stack_data((uint32_t*)rbuf);
        wbuf[0] = (uint8_t)(rbuf[0]&0xFF);
        wbuf[1] = (uint8_t)((rbuf[0]>>8)&0xFF);
        wbuf[2] = (uint8_t)((rbuf[0]>>16)&0xFF);
        wbuf[3] = (uint8_t)((rbuf[0]>>24)&0xFF);
        wbuf[4] = (uint8_t)(rbuf[1]&0xFF);
        wbuf[5] = (uint8_t)((rbuf[1]>>8)&0xFF);
        wbuf[6] = (uint8_t)((rbuf[1]>>16)&0xFF);
        wbuf[7] = (uint8_t)((rbuf[1]>>24)&0xFF);
        wbuf[8] = (uint8_t)((rbuf[2]>>24)&0xFF);
        fram_write_mcu_fault(wbuf);
#endif  /* End if 0 by lison on 2020-6-16 */
    }
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  mcu xx func config.
  * @param  None
  * @retval None
  */


/****************************** END OF FILE ***********************************/

