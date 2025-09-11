/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-25
 * @brief       CANͨ�� ʵ��
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
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./USMART/usmart.h"
#include "./BSP/KEY/key.h"
#include "./BSP/TIMER/btim.h"
#include "freertos_demo.h"
#include "./BSP/USART2/usart2.h"
#include "./BSP/USART3/usart3.h"
#include "./BSP/UART5/uart5.h"
#include "./BSP/USART6/usart6.h"
#include "./BSP/DMA/dma.h"
#include "./BSP/WDG/wdg.h"

#include "ad7606.h"

#define BAUD 460800

int main(void)
{
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    dma_init();  /* ��ʼ��DMA */
    usart_init(1000000);                     /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(84);                    /* ��ʼ��USMART */
//    led_init();                             /* ��ʼ��LED */
//    lcd_init();                             /* ��ʼ��LCD */
//    extix_init();                             /* ��ʼ������ */
//    key_init();
    btim_tim6_int_init(50 - 1, 8400 - 1); /* 84 000 000 / 84 00 = 10 000(10Khz�ļ���Ƶ��)��10000 / 50 = 200Hz */
    btim_tim7_int_init(100 - 1, 8400 - 1); /* 84 000 000 / 84 00 / 100 = 100Hz */
	btim_tim3_int_init(10 - 1, 8400 - 1);  /* 10000 / 10 = 1000Hz */ 
	
    iwdg_init(IWDG_PRESCALER_64, 5000);   // 64 * 50 / 32 = 100ms (10Hz)     
    
//    can_ini(CAN_SJW_1TQ, CAN_BS2_6TQ, CAN_BS1_7TQ, 3, CAN_MODE_NORMAL);  /* CAN��ʼ��, ����ģʽ, ������1000Kbps */
    
    usart2_init(BAUD);
//    usart3_init(BAUD);
//    uart5_init(BAUD);
//    usart6_init(BAUD);
	
		AD7606_Init();
    
    printf("\r\n HELLO! \r\n\r\n");

    freertos_demo();
}

