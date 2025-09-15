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
#include "./BSP/TIMER/btim.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "./USMART/usmart.h"
#include "./USMART/usmart_port.h"

#include "ad7606.h"

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO 1                   /* �������ȼ� */
#define START_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            StartTask_Handler;  /* ������ */
void start_task(void *pvParameters);        /* ������ */

/* TASK1 ���� ���� */
#define TASK1_PRIO      3                   /* �������ȼ� */
#define TASK1_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            Task1Task_Handler;  /* ������ */
void task1(void *pvParameters);             /* ������ */

/* TASK2 ���� ����  */
#define TASK2_PRIO      6                   /* �������ȼ� */
#define TASK2_STK_SIZE  1024                 /* �����ջ��С */
TaskHandle_t            Task2Task_Handler;  /* ������ */
void task2(void *pvParameters);             /* ������ */

/* TASK3 ���� ����  */
#define TASK3_PRIO      5                   /* �������ȼ� */
#define TASK3_STK_SIZE  512                 /* �����ջ��С */
TaskHandle_t            Task3Task_Handler;  /* ������ */
void task3(void *pvParameters);             /* ������ */

/* TASK4 ���� ����  */
#define TASK4_PRIO      4                   /* �������ȼ� */
#define TASK4_STK_SIZE  1024               /* �����ջ��С */
TaskHandle_t            Task4Task_Handler;  /* ������ */
void task4(void *pvParameters);             /* ������ */

/* --------------------------����-------------------------------------------- */
uint32_t notifyValueTask1;  
uint32_t notifyValueTask2; 
uint32_t notifyValueTask3; 
uint32_t notifyValueTask4; 

float exo_state[32];

/* PC */
uint8_t pc_o[20];
float pc_t_l = 0;
float pc_t_r = 0;

float pc_1 = 0, pc_2 = 0, pc_3 = 0;

uint8_t tcmd[10];
uint8_t tcmd_pid[14];

static int16_t adc_data_raw[8];
float adc_data_raw_f[8];

int test_cnt = 0;



/* --------------------------����-------------------------------------------- */

/**
 * @brief       FreeRTOS������ں���
 * @param       ��
 * @retval      ��
 */
void freertos_demo(void)
{    
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
								
		
		AD7606_Init();
    btim_tim6_int_init(50 - 1, 8400 - 1); /* 84 000 000 / 84 00 = 10 000(10Khz�ļ���Ƶ��)��10000 / 50 = 200Hz */
    btim_tim7_int_init(100 - 1, 8400 - 1); /* 84 000 000 / 84 00 / 100 = 100Hz */
		btim_tim3_int_init(10 - 1, 8400 - 1);  /* 10000 / 10 = 1000Hz */ 	
								
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
		char buf[20];
	
    while(1) 
    {
				notifyValueTask2 = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

     
				/* ���ݴ������˲� */
				char *p = (char*)pc_o;		
				if ( *(p+1) == ':') {
                    p += 2;
                }
                if ( *(p-2) == 'q')
                {
                    pc_1 = strtof(p, &p);
                    p++;
                    pc_2 = strtof(p, &p);
                    p++;
                    pc_3 = strtof(p, &p);
                }
                else
                {
                    pc_t_l = strtof(p, &p); // ��ȡ��һ������
                    p++;
                    pc_t_r = strtof(p, &p); // ��ȡ�ڶ�������
                }
				
				// MAIN CONTROL
				
		
				// send command to low level
				if (pc_o[0] == 0x71){              // 'q'
                    
                    tcmd_pid[0] = 0x12;
                    tcmd_pid[sizeof(tcmd_pid)-1] = 0x34;
                    memcpy(tcmd_pid + 1, &pc_1, sizeof(float));
                    memcpy(tcmd_pid + 1 + sizeof(float), &pc_2, sizeof(float));
                    memcpy(tcmd_pid + 1 + 2 * sizeof(float), &pc_3, sizeof(float));
                    usart2_send_data(tcmd_pid, sizeof(tcmd_pid));
                    
                }
                else{
                    
                    if (pc_o[0] == 0x74){   // 't' 
                        tcmd[0] = 0xAB;
                        tcmd[sizeof(tcmd)-1] = 0xCD;
                    }
                    else if (pc_o[0] == 0x70){   // 'p'
                        tcmd[0] = 0xDC;
                        tcmd[sizeof(tcmd)-1] = 0xBA;
                    }
                    memcpy(tcmd + 1, &pc_t_l, sizeof(float));
                    memcpy(tcmd + 1 + sizeof(float), &pc_t_r, sizeof(float));
                    
                    usart2_send_data(tcmd, sizeof(tcmd));
                    
                }
                

				
		
        // ι��
				iwdg_feed();
    }
}

/*--------------------------------------------------------------------������---------------------------------------------------------------------------------------------*/



float32_t emg_output_samples[NUM_CHANNELS];

/**
 * @brief       task3    ADC related
 */
void task3(void *pvParameters)
{
	uint8_t i;
	uint8_t data, high, low;

	int len;
	
	init_all_channels();
	float32_t input_samples[NUM_CHANNELS];

	
    while(1)
    {		
			notifyValueTask3 = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

			// adc data acquiring
			AD_CS_0_1(); /* SPIƬѡ = 0 */

			for (i = 0; i < CH_NUM; i++)
			{			
				high = bsp_spiRead1();
				low  = bsp_spiRead1();
				adc_data_raw[i] = ((uint16_t)high << 8) | low;
				adc_data_raw_f[i] = (float)adc_data_raw[i] * VoltageRange / 32768;
			}
			
			AD_CS_1_1();   /* SPIƬѡ = 1 */
		
			
			// emg data processing
			for (int ch = 0; ch < NUM_CHANNELS; ch++) {
				input_samples[ch] = adc_data_raw_f[ch];
				emg_output_samples[ch] = process_channel_sample(ch, input_samples[ch]);
			}
			
			

    }
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/**
 * @brief       task4 �������ݵ�PC
 */
#define BUF_SIZE 512

void task4(void *pvParameters)
{
	char buf[BUF_SIZE];
	int len = 0;
	uint8_t i = 0;

	while(1)
    {
				notifyValueTask4 = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
			
//							// ����֪ͨ����
//					printf("2��%d\r\n", notifyValueTask2);
//					printf("3��%d\r\n", notifyValueTask3);
//					printf("4��%d\r\n", notifyValueTask4);
	 
//				printf("%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\r\n", pc_t_l, pc_t_r
//																				, exo_state[0], exo_state[1], exo_state[2], exo_state[3]
//																				, exo_state[4], exo_state[5], exo_state[6], exo_state[7]);
				
				len = snprintf(buf, sizeof(buf), "a:");
				len += snprintf(buf + len, sizeof(buf) - len, "%d,%d,%d,", notifyValueTask2,notifyValueTask3,notifyValueTask4);
				len += snprintf(buf + len, sizeof(buf) - len, "%7.3f,%7.3f,", pc_t_l, pc_t_r);
				for (i = 0; i < NUM_CHANNELS; i++) {
					len += snprintf(buf + len, sizeof(buf) - len, "%7.3f,", emg_output_samples[i]);
				}
				for (i = 0; i < CH_NUM; i++)
				{
					len += snprintf(buf + len, sizeof(buf) - len, "%7.3f,", adc_data_raw_f[i]);
				}
				for (i = 0; i < 14; i++)
				{
					len += snprintf(buf + len, sizeof(buf) - len, "%7.3f,", exo_state[i]);
				}
				len += snprintf(buf + len, sizeof(buf) - len, "\r\n");
				
				if (g_usart1_handler.gState == HAL_UART_STATE_READY)
				{
						HAL_UART_Transmit_DMA(&g_usart1_handler, (uint8_t*)buf, len);
				}		
    }
}

