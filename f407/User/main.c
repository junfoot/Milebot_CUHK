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
    dma_init();  							/* ��ʼ��DMA */
    usmart_dev.init(84);                    /* ��ʼ��USMART */
    usart_init(1000000);                    /* ���ڳ�ʼ�� */
	usart2_init(BAUD);
	iwdg_init(IWDG_PRESCALER_64, 50);   // 64 * 50 / 32 = 100ms (10Hz)   
    
    printf("\r\n HELLO! \r\n\r\n");
	// ���⣬����rtos֮ǰִ���������Ȼsnprintf����float
	char dummy[20];
	snprintf(dummy, sizeof(dummy), "%g", 3.14159);

    freertos_demo();


//    led_init();                             /* ��ʼ��LED */
//    lcd_init();                             /* ��ʼ��LCD */
//    extix_init();                             /* ��ʼ������ */
//    can_ini(CAN_SJW_1TQ, CAN_BS2_6TQ, CAN_BS1_7TQ, 3, CAN_MODE_NORMAL);  /* CAN��ʼ��, ����ģʽ, ������1000Kbps */
//    usart3_init(BAUD);
//    uart5_init(BAUD);
//    usart6_init(BAUD);
//    key_init();
}

