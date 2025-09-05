/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : iap.c
* Author      : lison
* Version     : v0.01
* Date        : 2021/2/1
* Description : iap interface layer driver.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2021/2/1  lison    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/

/* Public Includes -----------------------------------------------------------*/


/* Private Includes ----------------------------------------------------------*/
#include "iap.h"


/* Private defines -----------------------------------------------------------*/


/* Global variables ----------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Global functions ----------------------------------------------------------*/

//跳转到指定的地址运行
void iap_jump(uint32_t jump_addr)
{
    //声明一个函数指针 
    void (*jump_fun)(void);
    uint8_t i = 0;
    
    //停止所有的DMA
    //dma_stop_disable();
        
    //禁止全局中断
    __set_PRIMASK(1);
    
    //关闭滴答定时器, 复位到默认值
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;
    
    //设置所有时钟到默认状态, 使用HSI时钟
    HAL_RCC_DeInit();
    
    //关闭所有中断, 清除所有中断挂起标志
    for(i = 0; i < 8; i++)
    {
        NVIC->ICER[i]=0xFFFFFFFF;
        NVIC->ICPR[i]=0xFFFFFFFF;
    }
    
    //使能全局中断
    __set_PRIMASK(0);
    
    //跳转到指定地址, 首地址是MSP, 地址+4是复位中断服务程序地址
    jump_fun = (void (*)(void)) (*((uint32_t *) (jump_addr + 4)));
    
    //设置主堆栈指针
    __set_MSP(*(uint32_t *)jump_addr);
    
    //在RTOS(RTX系统), 这条语句很重要, 设置为特权级模式, 使用MSP指针
    __set_CONTROL(0);
    
    //跳转到指定地址
    jump_fun(); 
    
//    /* 跳转成功的话,不会执行到这里*/
//    while (1)
//    {
//    }

}


