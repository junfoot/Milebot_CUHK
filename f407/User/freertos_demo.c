/**
 ****************************************************************************************************
 * @file        freertos.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.4
 * @date        2022-01-04
 * @brief       FreeRTOS ��ֲʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 ****************************************************************************************************
 */

#include "freertos_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include <string.h>
#include <stdio.h>
#include "DS_300c.h"
#include "gait.h"
#include "./BSP/USART2/usart2.h"
#include "./BSP/USART3/usart3.h"
#include "./BSP/UART5/uart5.h"
#include "./BSP/USART6/usart6.h"
#include "arm_math.h"
#include "utils.h"
#include "./BSP/DMA/dma.h"
#include "./BSP/WDG/wdg.h"
#include "math.h"
#include "stdlib.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "./USMART/usmart.h"
#include "./USMART/usmart_port.h"



/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO 1                   /* �������ȼ� */
#define START_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            StartTask_Handler;  /* ������ */
void start_task(void *pvParameters);        /* ������ */

/* TASK1 ���� ���� */
#define TASK1_PRIO      3                   /* �������ȼ� */
#define TASK1_STK_SIZE  256                 /* �����ջ��С */
TaskHandle_t            Task1Task_Handler;  /* ������ */
void task1(void *pvParameters);             /* ������ */

/* TASK2 ���� ����  */
#define TASK2_PRIO      5                   /* �������ȼ� */
#define TASK2_STK_SIZE  512                 /* �����ջ��С */
TaskHandle_t            Task2Task_Handler;  /* ������ */
void task2(void *pvParameters);             /* ������ */

/* TASK3 ���� ����  */
#define TASK3_PRIO      2                   /* �������ȼ� */
#define TASK3_STK_SIZE  256                 /* �����ջ��С */
TaskHandle_t            Task3Task_Handler;  /* ������ */
void task3(void *pvParameters);             /* ������ */

/* TASK4 ���� ����  */
#define TASK4_PRIO      6                   /* �������ȼ� */
#define TASK4_STK_SIZE  512               /* �����ջ��С */
TaskHandle_t            Task4Task_Handler;  /* ������ */
void task4(void *pvParameters);             /* ������ */

/* --------------------------����-------------------------------------------- */
QueueHandle_t sem1;
QueueHandle_t sem2;
QueueHandle_t sem3;
QueueHandle_t sem4;      // ��ʱ������sem1��sem3


float exo_state[32];

/* PC */
uint8_t pc_o[20];
float pc_t_l = 0;
float pc_t_r = 0;

uint8_t tcmd[10];


/* --------------------------����-------------------------------------------- */

/**
 * @brief       FreeRTOS������ں���
 * @param       ��
 * @retval      ��
 */
void freertos_demo(void)
{    
    /* �����ź��� */
    sem1 = xSemaphoreCreateBinary();
    if(sem1 != NULL)
    {
        printf("��ֵ�ź���sem1�����ɹ�������\r\n");
    }
    sem2 = xSemaphoreCreateBinary();
    if(sem2 != NULL)
    {
        printf("��ֵ�ź���sem2�����ɹ�������\r\n");
    }
    sem3 = xSemaphoreCreateBinary();
    if(sem3 != NULL)
    {
        printf("��ֵ�ź���sem3�����ɹ�������\r\n");
    }
    sem4 = xSemaphoreCreateBinary();
    if(sem4 != NULL)
    {
        printf("��ֵ�ź���sem4�����ɹ�������\r\n");
    }

    xTaskCreate((TaskFunction_t )start_task,            /* ������ */
                (const char*    )"start_task",          /* �������� */
                (uint16_t       )START_STK_SIZE,        /* �����ջ��С */
                (void*          )NULL,                  /* ������������Ĳ��� */
                (UBaseType_t    )START_TASK_PRIO,       /* �������ȼ� */
                (TaskHandle_t*  )&StartTask_Handler);   /* ������ */
    vTaskStartScheduler();
}

/**
 * @brief       start_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* �����ٽ��� */
    /* ��������1 */
    xTaskCreate((TaskFunction_t )task1,
                (const char*    )"task1",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (TaskHandle_t*  )&Task1Task_Handler);
    /* ��������2 */
    xTaskCreate((TaskFunction_t )task2,
                (const char*    )"task2",
                (uint16_t       )TASK2_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK2_PRIO,
                (TaskHandle_t*  )&Task2Task_Handler);
	/* ��������3 */
    xTaskCreate((TaskFunction_t )task3,
                (const char*    )"task3",
                (uint16_t       )TASK3_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK3_PRIO,
                (TaskHandle_t*  )&Task3Task_Handler);
	/* ��������4 */
    xTaskCreate((TaskFunction_t )task4,
                (const char*    )"task4",
                (uint16_t       )TASK4_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK4_PRIO,
                (TaskHandle_t*  )&Task4Task_Handler);
    vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
    taskEXIT_CRITICAL();            /* �˳��ٽ��� */
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------*/

/**
 * @brief       task1
 */
void task1(void *pvParameters)
{
    while(1) 
    {     
        vTaskDelay(1000);
    }
}

/*--------------------------------------------------------------------������----------------------------------------------------------------------------------------------*/
/**
 * @brief       task2 ������
 */
void task2(void *pvParameters)
{   
    while(1) 
    {
        xSemaphoreTake(sem1,portMAX_DELAY); /* ��ȡ�ź��������� */
        
        
        /* ���ݴ������˲� */
				char *p = (char*)pc_o;		
				if (*p == 't' && *(p+1) == ':') {p += 2;}
				pc_t_l = strtof(p, &p); // ��ȡ��һ������
				p++;
				pc_t_r = strtof(p, &p); // ��ȡ�ڶ�������
				// test
//				printf("%f %f\r\n", pc_t_l, pc_t_r);
		
				
				// send command to low level
				tcmd[0] = 0xAB;
				memcpy(tcmd + 1, &pc_t_l, sizeof(float));
				memcpy(tcmd + 1 + sizeof(float), &pc_t_r, sizeof(float));
				tcmd[sizeof(tcmd)-1] = 0xCD;
				usart2_send_data(tcmd, sizeof(tcmd));
				
		
        // ι��
        iwdg_feed();
    }
}

/*--------------------------------------------------------------------������---------------------------------------------------------------------------------------------*/


/**
 * @brief       task3 
 */
void task3(void *pvParameters)
{
    while(1)
    {
        vTaskDelay(1000);
    }
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/**
 * @brief       task4 �������ݵ�PC
 */
void task4(void *pvParameters)
{
	char buffer[200];  // ȷ���㹻��
	int offset = 0;
	int i = 0;

	while(1)
    {
        xSemaphoreTake(sem3,portMAX_DELAY); /* ��ȡ�ź��������� */
	 
				printf("%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\r\n", pc_t_l, pc_t_r
																														, exo_state[0], exo_state[1], exo_state[2], exo_state[3]
																														, exo_state[4], exo_state[5], exo_state[6], exo_state[7]);
			
//				printf("test\r\n");
    }
}

