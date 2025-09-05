/**
 ****************************************************************************************************
 * @file        exti.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       �ⲿ�ж� ��������
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
 * V1.0 20211014
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/EXTI/exti.h"

#include "DS_300c.h"
#include "gait.h"

extern uint8_t mode;

/**
 * @brief       KEY0 �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
void KEY0_INT_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(KEY0_INT_GPIO_PIN);         /* �����жϴ����ú��� ���KEY0�����ж��� ���жϱ�־λ */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY0_INT_GPIO_PIN);         /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}

/**
 * @brief       KEY1 �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
void KEY1_INT_IRQHandler(void)
{ 
    HAL_GPIO_EXTI_IRQHandler(KEY1_INT_GPIO_PIN);         /* �����жϴ����ú��� ���KEY1�����ж��� ���жϱ�־λ���ж��°벿��HAL_GPIO_EXTI_Callbackִ�� */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY1_INT_GPIO_PIN);         /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}

/**
 * @brief       KEY2 �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
void KEY2_INT_IRQHandler(void)
{ 
    HAL_GPIO_EXTI_IRQHandler(KEY2_INT_GPIO_PIN);        /* �����жϴ����ú��� ���KEY2�����ж��� ���жϱ�־λ���ж��°벿��HAL_GPIO_EXTI_Callbackִ�� */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY2_INT_GPIO_PIN);        /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}

/**
 * @brief       WK_UP �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
//void WKUP_INT_IRQHandler(void)
//{ 
//    HAL_GPIO_EXTI_IRQHandler(WKUP_INT_GPIO_PIN);        /* �����жϴ����ú��� ���KEY_UP�����ж��� ���жϱ�־λ���ж��°벿��HAL_GPIO_EXTI_Callbackִ�� */
//    __HAL_GPIO_EXTI_CLEAR_IT(WKUP_INT_GPIO_PIN);        /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
//}

/**
 * @brief       �жϷ����������Ҫ��������
 *              ��HAL�������е��ⲿ�жϷ�����������ô˺���
 * @param       GPIO_Pin:�ж����ź�
 * @retval      ��
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    vTaskDelay(10);      /* ���� */
	uint8_t address[4] = {1, 2, 3, 4};
	int ptr = 10;
	uint8_t i;
    switch(GPIO_Pin)
    {
        case KEY0_INT_GPIO_PIN:
            if (KEY0 == 0)
            {
				for (i=0;i<4;i++)
				{
					motor_enable(&ptr, address[i]);
					vTaskDelay(1);
					motor_control_mode(&ptr, address[i], 2);
					vTaskDelay(1);
					motor_set_Tcurve_max_a(&ptr, address[i], 20);
					vTaskDelay(1);
					motor_set_max_v(&ptr, 50, address[i]);
					vTaskDelay(1);
					motor_reset_enc(&ptr, address[i]);
					vTaskDelay(1);
					motor_continue_curve_control(&ptr, address[i], 1);
					vTaskDelay(1);
				}
				
                mode = 1;
            }
            break;

        case KEY1_INT_GPIO_PIN:
            if (KEY1 == 0)
            {
                mode = 2;
            }
            break;

        case KEY2_INT_GPIO_PIN:
            if (KEY2 == 0)
            {
                LED1_TOGGLE();  /* LED1 ״̬ȡ�� */
                LED0_TOGGLE();  /* LED0 ״̬ȡ�� */ 
            }
            break;

//        case WKUP_INT_GPIO_PIN:
//            if (WK_UP == 1)
//            {
//				for (i=0;i<4;i++)
//				{
////					motor_stop(&ptr, address[i]);
////					vTaskDelay(1);
//					motor_disable(&ptr, address[i]);
////					vTaskDelay(1);
//				}
//				
//                mode = 0;
//            }
            break;

        default : break;
    }
}

/**
 * @brief       �ⲿ�жϳ�ʼ������
 * @param       ��
 * @retval      ��
 */
void extix_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    key_init();
    gpio_init_struct.Pin = KEY0_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* �½��ش��� */
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY0_INT_GPIO_PORT, &gpio_init_struct);    /* KEY0����Ϊ�½��ش����ж� */

    gpio_init_struct.Pin = KEY1_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* �½��ش��� */
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY1_INT_GPIO_PORT, &gpio_init_struct);    /* KEY1����Ϊ�½��ش����ж� */
    
    gpio_init_struct.Pin = KEY2_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* �½��ش��� */
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY2_INT_GPIO_PORT, &gpio_init_struct);    /* KEY2����Ϊ�½��ش����ж� */
    
//    gpio_init_struct.Pin = WKUP_INT_GPIO_PIN;
//    gpio_init_struct.Mode = GPIO_MODE_IT_RISING;             /* �����ش��� */
//    gpio_init_struct.Pull = GPIO_PULLDOWN;
//    HAL_GPIO_Init(WKUP_GPIO_PORT, &gpio_init_struct);        /* WKUP����Ϊ�����ش����ж� */

    HAL_NVIC_SetPriority(KEY0_INT_IRQn, 8, 0);               /* ��ռ0�������ȼ�2 */
    HAL_NVIC_EnableIRQ(KEY0_INT_IRQn);                       /* ʹ���ж���4 */

    HAL_NVIC_SetPriority(KEY1_INT_IRQn, 8, 0);               /* ��ռ1�������ȼ�2 */
    HAL_NVIC_EnableIRQ(KEY1_INT_IRQn);                       /* ʹ���ж���3 */
    
    HAL_NVIC_SetPriority(KEY2_INT_IRQn, 8, 0);               /* ��ռ2�������ȼ�2 */
    HAL_NVIC_EnableIRQ(KEY2_INT_IRQn);                       /* ʹ���ж���2 */

//    HAL_NVIC_SetPriority(WKUP_INT_IRQn, 8, 0);               /* ��ռ3�������ȼ�2 */
//    HAL_NVIC_EnableIRQ(WKUP_INT_IRQn);                       /* ʹ���ж���0 */

}












