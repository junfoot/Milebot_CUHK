/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-25
 * @brief       CAN通信 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
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
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    delay_init(168);                        /* 延时初始化 */
    dma_init();  /* 初始化DMA */
    usart_init(1000000);                     /* 串口初始化为115200 */
    usmart_dev.init(84);                    /* 初始化USMART */
//    led_init();                             /* 初始化LED */
//    lcd_init();                             /* 初始化LCD */
//    extix_init();                             /* 初始化按键 */
//    key_init();
    btim_tim6_int_init(50 - 1, 8400 - 1); /* 84 000 000 / 84 00 = 10 000(10Khz的计数频率)，10000 / 50 = 200Hz */
    btim_tim7_int_init(100 - 1, 8400 - 1); /* 84 000 000 / 84 00 / 100 = 100Hz */
	btim_tim3_int_init(10 - 1, 8400 - 1);  /* 10000 / 10 = 1000Hz */ 
	
    iwdg_init(IWDG_PRESCALER_64, 5000);   // 64 * 50 / 32 = 100ms (10Hz)     
    
//    can_ini(CAN_SJW_1TQ, CAN_BS2_6TQ, CAN_BS1_7TQ, 3, CAN_MODE_NORMAL);  /* CAN初始化, 环回模式, 波特率1000Kbps */
    
    usart2_init(BAUD);
//    usart3_init(BAUD);
//    uart5_init(BAUD);
//    usart6_init(BAUD);
	
		AD7606_Init();
    
    printf("\r\n HELLO! \r\n\r\n");

    freertos_demo();
}

