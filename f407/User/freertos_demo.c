/**
 ****************************************************************************************************
 * @file        freertos.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.4
 * @date        2022-01-04
 * @brief       FreeRTOS 移植实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
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



/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO 1                   /* 任务优先级 */
#define START_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            StartTask_Handler;  /* 任务句柄 */
void start_task(void *pvParameters);        /* 任务函数 */

/* TASK1 任务 配置 */
#define TASK1_PRIO      3                   /* 任务优先级 */
#define TASK1_STK_SIZE  256                 /* 任务堆栈大小 */
TaskHandle_t            Task1Task_Handler;  /* 任务句柄 */
void task1(void *pvParameters);             /* 任务函数 */

/* TASK2 任务 配置  */
#define TASK2_PRIO      5                   /* 任务优先级 */
#define TASK2_STK_SIZE  512                 /* 任务堆栈大小 */
TaskHandle_t            Task2Task_Handler;  /* 任务句柄 */
void task2(void *pvParameters);             /* 任务函数 */

/* TASK3 任务 配置  */
#define TASK3_PRIO      2                   /* 任务优先级 */
#define TASK3_STK_SIZE  256                 /* 任务堆栈大小 */
TaskHandle_t            Task3Task_Handler;  /* 任务句柄 */
void task3(void *pvParameters);             /* 任务函数 */

/* TASK4 任务 配置  */
#define TASK4_PRIO      6                   /* 任务优先级 */
#define TASK4_STK_SIZE  512               /* 任务堆栈大小 */
TaskHandle_t            Task4Task_Handler;  /* 任务句柄 */
void task4(void *pvParameters);             /* 任务函数 */

/* --------------------------变量-------------------------------------------- */
QueueHandle_t sem1;
QueueHandle_t sem2;
QueueHandle_t sem3;
QueueHandle_t sem4;      // 暂时就用了sem1、sem3


float exo_state[32];

/* PC */
uint8_t pc_o[20];
float pc_t_l = 0;
float pc_t_r = 0;

uint8_t tcmd[10];


/* --------------------------变量-------------------------------------------- */

/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{    
    /* 创建信号量 */
    sem1 = xSemaphoreCreateBinary();
    if(sem1 != NULL)
    {
        printf("二值信号量sem1创建成功！！！\r\n");
    }
    sem2 = xSemaphoreCreateBinary();
    if(sem2 != NULL)
    {
        printf("二值信号量sem2创建成功！！！\r\n");
    }
    sem3 = xSemaphoreCreateBinary();
    if(sem3 != NULL)
    {
        printf("二值信号量sem3创建成功！！！\r\n");
    }
    sem4 = xSemaphoreCreateBinary();
    if(sem4 != NULL)
    {
        printf("二值信号量sem4创建成功！！！\r\n");
    }

    xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
                (const char*    )"start_task",          /* 任务名称 */
                (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传入给任务函数的参数 */
                (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
                (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */
    vTaskStartScheduler();
}

/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* 进入临界区 */
    /* 创建任务1 */
    xTaskCreate((TaskFunction_t )task1,
                (const char*    )"task1",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (TaskHandle_t*  )&Task1Task_Handler);
    /* 创建任务2 */
    xTaskCreate((TaskFunction_t )task2,
                (const char*    )"task2",
                (uint16_t       )TASK2_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK2_PRIO,
                (TaskHandle_t*  )&Task2Task_Handler);
	/* 创建任务3 */
    xTaskCreate((TaskFunction_t )task3,
                (const char*    )"task3",
                (uint16_t       )TASK3_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK3_PRIO,
                (TaskHandle_t*  )&Task3Task_Handler);
	/* 创建任务4 */
    xTaskCreate((TaskFunction_t )task4,
                (const char*    )"task4",
                (uint16_t       )TASK4_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK4_PRIO,
                (TaskHandle_t*  )&Task4Task_Handler);
    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
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

/*--------------------------------------------------------------------主程序----------------------------------------------------------------------------------------------*/
/**
 * @brief       task2 主程序
 */
void task2(void *pvParameters)
{   
    while(1) 
    {
        xSemaphoreTake(sem1,portMAX_DELAY); /* 获取信号量并死等 */
        
        
        /* 数据处理与滤波 */
				char *p = (char*)pc_o;		
				if (*p == 't' && *(p+1) == ':') {p += 2;}
				pc_t_l = strtof(p, &p); // 提取第一个数字
				p++;
				pc_t_r = strtof(p, &p); // 提取第二个数字
				// test
//				printf("%f %f\r\n", pc_t_l, pc_t_r);
		
				
				// send command to low level
				tcmd[0] = 0xAB;
				memcpy(tcmd + 1, &pc_t_l, sizeof(float));
				memcpy(tcmd + 1 + sizeof(float), &pc_t_r, sizeof(float));
				tcmd[sizeof(tcmd)-1] = 0xCD;
				usart2_send_data(tcmd, sizeof(tcmd));
				
		
        // 喂狗
        iwdg_feed();
    }
}

/*--------------------------------------------------------------------主程序---------------------------------------------------------------------------------------------*/


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
 * @brief       task4 发送数据到PC
 */
void task4(void *pvParameters)
{
	char buffer[200];  // 确保足够大
	int offset = 0;
	int i = 0;

	while(1)
    {
        xSemaphoreTake(sem3,portMAX_DELAY); /* 获取信号量并死等 */
	 
				printf("%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\r\n", pc_t_l, pc_t_r
																														, exo_state[0], exo_state[1], exo_state[2], exo_state[3]
																														, exo_state[4], exo_state[5], exo_state[6], exo_state[7]);
			
//				printf("test\r\n");
    }
}

