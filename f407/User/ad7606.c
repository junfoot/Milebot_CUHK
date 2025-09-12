
#include "ad7606.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "./SYSTEM/delay/delay.h"
#include "stdio.h"

extern TaskHandle_t Task3Task_Handler;

//extern QueueHandle_t sem2;

/**
  * @brief  初始化AD7606 GPIO和SPI总线
  * @param  None
  * @retval None
  */
void AD7606_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
//    EXTI_InitTypeDef EXTI_InitStruct = {0};
    
    /* GPIO时钟使能 */
//    __HAL_RCC_GPIOA_CLK_ENABLE();    // 根据实际端口修改
    __HAL_RCC_GPIOB_CLK_ENABLE();    // 根据实际端口修改
    __HAL_RCC_GPIOC_CLK_ENABLE();    // 根据实际端口修改
    __HAL_RCC_GPIOD_CLK_ENABLE();    // 根据实际端口修改
    
    /* 配置输出引脚：RESET, CONVST, RANGE, CS, OS0, OS1, OS2 */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    GPIO_InitStruct.Pin = PIN_RESET_1;
    HAL_GPIO_Init(PORT_RESET_1, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = PIN_CONVST_1;
    HAL_GPIO_Init(PORT_CONVST_1, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = PIN_RANGE_1;
    HAL_GPIO_Init(PORT_RANGE_1, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = PIN_CS_1;
    HAL_GPIO_Init(PORT_CS_1, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = PIN_OS_10;
    HAL_GPIO_Init(PORT_OS_10, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = PIN_OS_11;
    HAL_GPIO_Init(PORT_OS_11, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = PIN_OS_12;
    HAL_GPIO_Init(PORT_OS_12, &GPIO_InitStruct);
    
    /* 设置OS引脚初始状态 */
    HAL_GPIO_WritePin(PORT_OS_10, PIN_OS_10, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PORT_OS_11, PIN_OS_11, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PORT_OS_12, PIN_OS_12, GPIO_PIN_SET);
    
    /* 配置BUSY输入引脚 */
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Pin = PIN_BUSY_1;
    HAL_GPIO_Init(PORT_BUSY_1, &GPIO_InitStruct);
    
    /* 配置SPI总线引脚 */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    
    GPIO_InitStruct.Pin = PIN_SCK_1;
    HAL_GPIO_Init(PORT_SCK_1, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = PIN_MOSI_1;
    HAL_GPIO_Init(PORT_MOSI_1, &GPIO_InitStruct);
    
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin = PIN_MISO_1;
    HAL_GPIO_Init(PORT_MISO_1, &GPIO_InitStruct);
    
    /* 配置外部中断（BUSY引脚）*/
    /* 注意：需要根据实际连接修改中断线和引脚配置 */
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Pin = PIN_BUSY_1;
    HAL_GPIO_Init(PORT_BUSY_1, &GPIO_InitStruct);
    
    /* 配置NVIC */
    HAL_NVIC_SetPriority(EXTI0_IRQn, 6, 0);  // 根据实际中断线修改
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	
	
	// set input range
	AD_RANGE_5V_1();
	
	
	// AD7606_Reset
    AD_CS_1_1();   /* CS  SCLK拉高 */
    /* AD7606是高电平复位，要求最小脉宽50ns */
    AD_RESET_LOW_1();
    AD_RESET_HIGH_1();
    AD_RESET_HIGH_1();
    AD_RESET_HIGH_1();
    AD_RESET_HIGH_1();
    AD_RESET_LOW_1();
	
	
	/* CONVST脚设置为高电平 */
	AD_CONVST_HIGH_1();         
	
	delay_ms(10);
	
}


/**
  * @brief  EXTI线0中断处理函数
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);         // EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位
	
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		// 通知任务
		vTaskNotifyGiveFromISR(Task3Task_Handler, &xHigherPriorityTaskWoken);
		// 如果任务优先级更高，立即切换
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        

}


/*
*********************************************************************************************************
*   函 数 名: ad7606_ReadBytes
*   功能说明: 读取AD7606的采样结果
*   形    参：
*   返 回 值: 无
*********************************************************************************************************
*/
uint8_t bsp_spiRead1(void)
{
/* 软件SPI */
    uint8_t i, j;
    uint8_t read = 0;

    for (i = 0; i < 8; i++)
    {
        SCK_0_1();
        for (j = 0; j < 1; j++);   //  bsp_spiDelay_1();
        read = read << 1;
        if (MISO_IS_HIGH_1())
        {
            read++;
        }
        SCK_1_1();
        for (j = 0; j < 1; j++);   //  bsp_spiDelay_1();
    }

    return read;
}


