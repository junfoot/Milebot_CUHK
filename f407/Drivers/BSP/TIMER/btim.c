/**
 ****************************************************************************************************
 * @file        btim.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-15
 * @brief       ������ʱ�� ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211015
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/LED/led.h"
#include "./BSP/TIMER/btim.h"
#include "./BSP/LCD/lcd.h"
#include "DS_300c.h"
#include "./SYSTEM/delay/delay.h"
#include "gait.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

TIM_HandleTypeDef g_tim6_handler;         /* ��ʱ��������� */
TIM_HandleTypeDef g_tim7_handler; 

extern QueueHandle_t sem1;
extern QueueHandle_t sem3;

/**
 * @brief       ������ʱ��TIMX��ʱ�жϳ�ʼ������
 * @note
 *              ������ʱ����ʱ������APB1,��PPRE1 �� 2��Ƶ��ʱ��
 *              ������ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ42M, ���Զ�ʱ��ʱ�� = 84Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr : �Զ���װֵ��
 * @param       psc : ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void btim_tim6_int_init(uint16_t arr, uint16_t psc)
{
	BTIM_TIM6_INT_CLK_ENABLE();                     /* ʹ��TIMxʱ�� */
	
    g_tim6_handler.Instance = BTIM_TIM6_INT;                      /* ��ʱ��x */
    g_tim6_handler.Init.Prescaler = psc;                          /* ��Ƶ */
    g_tim6_handler.Init.CounterMode = TIM_COUNTERMODE_UP;         /* ��������ģʽ */
    g_tim6_handler.Init.Period = arr;                             /* �Զ�װ��ֵ */
    HAL_TIM_Base_Init(&g_tim6_handler);
	
	HAL_NVIC_SetPriority(BTIM_TIM6_INT_IRQn, 5, 0); /* ��ռ1�������ȼ�4 */
	HAL_NVIC_EnableIRQ(BTIM_TIM6_INT_IRQn);         /* ����ITMx�ж� */
    
    HAL_TIM_Base_Start_IT(&g_tim6_handler);                       /* ʹ�ܶ�ʱ��x�Ͷ�ʱ�������ж� */
}

/**
 * @brief       ������ʱ��TIMX�жϷ�����
 * @param       ��
 * @retval      ��
 */
void BTIM_TIM6_INT_IRQHandler(void)
{
//    HAL_TIM_IRQHandler(&g_timx_handler);  /* ��ʱ���ص����� */
    BaseType_t err;
    if(__HAL_TIM_GET_FLAG(&g_tim6_handler, TIM_FLAG_UPDATE) != RESET)
    {	
        if(sem1 != NULL)
        {
            err = xSemaphoreGiveFromISR(sem1, NULL);
        }
        
    }
    __HAL_TIM_CLEAR_IT(&g_tim6_handler, TIM_IT_UPDATE);
}



void btim_tim7_int_init(uint16_t arr, uint16_t psc)
{
	BTIM_TIM7_INT_CLK_ENABLE();                     /* ʹ��TIMxʱ�� */
	
    g_tim7_handler.Instance = BTIM_TIM7_INT;                      /* ��ʱ��x */
    g_tim7_handler.Init.Prescaler = psc;                          /* ��Ƶ */
    g_tim7_handler.Init.CounterMode = TIM_COUNTERMODE_UP;         /* ��������ģʽ */
    g_tim7_handler.Init.Period = arr;                             /* �Զ�װ��ֵ */
    HAL_TIM_Base_Init(&g_tim7_handler);
	
	
	HAL_NVIC_SetPriority(BTIM_TIM7_INT_IRQn, 5, 0); /* ��ռ1�������ȼ�3 */
	HAL_NVIC_EnableIRQ(BTIM_TIM7_INT_IRQn);         /* ����ITMx�ж� */
   
    HAL_TIM_Base_Start_IT(&g_tim7_handler);                       /* ʹ�ܶ�ʱ��x�Ͷ�ʱ�������ж� */
}

/**
 * @brief       ������ʱ��TIMX�жϷ�����
 * @param       ��
 * @retval      ��
 */
void BTIM_TIM7_INT_IRQHandler(void)
{
//    HAL_TIM_IRQHandler(&g_timx_handler);  /* ��ʱ���ص����� */
    BaseType_t err;
    if(__HAL_TIM_GET_FLAG(&g_tim7_handler, TIM_FLAG_UPDATE) != RESET)
    {
        if(sem3 != NULL)  // �ͷ��ź���sem3
        {
            err = xSemaphoreGiveFromISR(sem3, NULL);
        }
    }
    __HAL_TIM_CLEAR_IT(&g_tim7_handler, TIM_IT_UPDATE);
}

