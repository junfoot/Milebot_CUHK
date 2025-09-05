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

//��ת��ָ���ĵ�ַ����
void iap_jump(uint32_t jump_addr)
{
    //����һ������ָ�� 
    void (*jump_fun)(void);
    uint8_t i = 0;
    
    //ֹͣ���е�DMA
    //dma_stop_disable();
        
    //��ֹȫ���ж�
    __set_PRIMASK(1);
    
    //�رյδ�ʱ��, ��λ��Ĭ��ֵ
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;
    
    //��������ʱ�ӵ�Ĭ��״̬, ʹ��HSIʱ��
    HAL_RCC_DeInit();
    
    //�ر������ж�, ��������жϹ����־
    for(i = 0; i < 8; i++)
    {
        NVIC->ICER[i]=0xFFFFFFFF;
        NVIC->ICPR[i]=0xFFFFFFFF;
    }
    
    //ʹ��ȫ���ж�
    __set_PRIMASK(0);
    
    //��ת��ָ����ַ, �׵�ַ��MSP, ��ַ+4�Ǹ�λ�жϷ�������ַ
    jump_fun = (void (*)(void)) (*((uint32_t *) (jump_addr + 4)));
    
    //��������ջָ��
    __set_MSP(*(uint32_t *)jump_addr);
    
    //��RTOS(RTXϵͳ), ����������Ҫ, ����Ϊ��Ȩ��ģʽ, ʹ��MSPָ��
    __set_CONTROL(0);
    
    //��ת��ָ����ַ
    jump_fun(); 
    
//    /* ��ת�ɹ��Ļ�,����ִ�е�����*/
//    while (1)
//    {
//    }

}


