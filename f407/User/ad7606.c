
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
  * @brief  ��ʼ��AD7606 GPIO��SPI����
  * @param  None
  * @retval None
  */
void AD7606_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
//    EXTI_InitTypeDef EXTI_InitStruct = {0};
    
    /* GPIOʱ��ʹ�� */
//    __HAL_RCC_GPIOA_CLK_ENABLE();    // ����ʵ�ʶ˿��޸�
    __HAL_RCC_GPIOB_CLK_ENABLE();    // ����ʵ�ʶ˿��޸�
    __HAL_RCC_GPIOC_CLK_ENABLE();    // ����ʵ�ʶ˿��޸�
    __HAL_RCC_GPIOD_CLK_ENABLE();    // ����ʵ�ʶ˿��޸�
    
    /* ����������ţ�RESET, CONVST, RANGE, CS, OS0, OS1, OS2 */
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
    
    /* ����OS���ų�ʼ״̬ */
    HAL_GPIO_WritePin(PORT_OS_10, PIN_OS_10, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PORT_OS_11, PIN_OS_11, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PORT_OS_12, PIN_OS_12, GPIO_PIN_SET);
    
    /* ����BUSY�������� */
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Pin = PIN_BUSY_1;
    HAL_GPIO_Init(PORT_BUSY_1, &GPIO_InitStruct);
    
    /* ����SPI�������� */
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
    
    /* �����ⲿ�жϣ�BUSY���ţ�*/
    /* ע�⣺��Ҫ����ʵ�������޸��ж��ߺ��������� */
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Pin = PIN_BUSY_1;
    HAL_GPIO_Init(PORT_BUSY_1, &GPIO_InitStruct);
    
    /* ����NVIC */
    HAL_NVIC_SetPriority(EXTI0_IRQn, 6, 0);  // ����ʵ���ж����޸�
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	
	
	// set input range
	AD_RANGE_5V_1();
	
	
	// AD7606_Reset
    AD_CS_1_1();   /* CS  SCLK���� */
    /* AD7606�Ǹߵ�ƽ��λ��Ҫ����С����50ns */
    AD_RESET_LOW_1();
    AD_RESET_HIGH_1();
    AD_RESET_HIGH_1();
    AD_RESET_HIGH_1();
    AD_RESET_HIGH_1();
    AD_RESET_LOW_1();
	
	
	/* CONVST������Ϊ�ߵ�ƽ */
	AD_CONVST_HIGH_1();         
	
	delay_ms(10);
	
}


/**
  * @brief  EXTI��0�жϴ�����
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);         // EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ
	
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		// ֪ͨ����
		vTaskNotifyGiveFromISR(Task3Task_Handler, &xHigherPriorityTaskWoken);
		// ����������ȼ����ߣ������л�
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        

}


/*
*********************************************************************************************************
*   �� �� ��: ad7606_ReadBytes
*   ����˵��: ��ȡAD7606�Ĳ������
*   ��    �Σ�
*   �� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t bsp_spiRead1(void)
{
/* ���SPI */
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


