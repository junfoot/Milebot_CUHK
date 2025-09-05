/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                 All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : uart.c
* Author      : wb
* Version     : v0.01
* Date        : 2019/6/21
* Description : UART interface layer driver.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2019/6/21  wanli    initial version
*=============================================================================*/
#include <stdio.h>
#include "mcu_if.h"

#include "sysTick.h"
#include "sysTick_if.h"



#define CPU_FREQUENCY_MHZ    168
static __IO uint32_t TimingDelay;
void delay_us(__IO uint32_t nTime)
{
    int old_val,new_val,val;

    if(nTime > 900)
    {
        for(old_val = 0; old_val < nTime/900; old_val++)
        {
            delay_us(900);
        }
        nTime = nTime%900;
    }

    old_val = SysTick->VAL;
    new_val = old_val - CPU_FREQUENCY_MHZ*nTime;
    if(new_val >= 0)
    {
        do
        {
            val = SysTick->VAL;
        }
        while((val < old_val)&&(val >= new_val));
    }
    else
    {
        new_val +=CPU_FREQUENCY_MHZ*1000;
        do
        {
            val = SysTick->VAL;
        }
        while((val <= old_val)||(val > new_val));

    }
}

/**
  * @brief  delay ms.
  * @param  None
  * @retval None
  */
void delay_ms(uint16_t ms)
{
    while(ms--)
    {
        delay_us(1000);
    }
}

