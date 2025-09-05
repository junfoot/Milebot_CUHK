/**
 ****************************************************************************************************
 *
 * USART6 PC12-TX PD2-RX
 * 3�Źؽ�ģ�� ����ϥ�ؽ�
 *
 ****************************************************************************************************
 */

#include "./BSP/UART5/uart5.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"

extern uint8_t data[4][8];

UART_HandleTypeDef g_uart5_handler;

#ifdef UART5_EN_RX /* ���ʹ���˽��� */
uint8_t g_UART5_rx_buf[UART5_REC_LEN];  /* ���ջ���, ��� USART2_REC_LEN ���ֽ�. */
uint8_t g_UART5_rx_cnt = 0;             /* ���յ������ݳ��� */
void UART5_UX_IRQHandler(void)
{
    
//    HAL_UART_IRQHandler(&g_uart5_handler);
    
    uint8_t res;
//    short tor_o;

    if ((__HAL_UART_GET_FLAG(&g_uart5_handler, UART_FLAG_RXNE) != RESET)) /* ���յ����� */
    {
        HAL_UART_Receive(&g_uart5_handler, &res, 1, 1000);

        if (g_UART5_rx_cnt < UART5_REC_LEN)         /* ������δ�� */
        {
            g_UART5_rx_buf[g_UART5_rx_cnt] = res;   /* ��¼���յ���ֵ */
            g_UART5_rx_cnt++;                       /* ������������1 */
        }
        
        /* ������� */
        if (g_UART5_rx_cnt == 8)
        {
//            tor_o[2] = (g_UART5_rx_buf[0]<<8)+g_UART5_rx_buf[1];
//            tor_origin[2] = -(float)tor_o[2]*0.1f;
            g_UART5_rx_cnt = 0;    /* ���� */
        }
    }

}
#endif


void uart5_init(uint32_t baudrate)
{
    /* IO �� ʱ������ */
    UART5_TX_GPIO_CLK_ENABLE(); /* ʹ�� ����TX�� ʱ�� */
    UART5_RX_GPIO_CLK_ENABLE(); /* ʹ�� ����RX�� ʱ�� */
    UART5_UX_CLK_ENABLE();      /* ʹ�� ���� ʱ�� */

    GPIO_InitTypeDef gpio_initure;
    gpio_initure.Pin = UART5_TX_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_AF_PP;
    gpio_initure.Pull = GPIO_PULLUP;
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_initure.Alternate = GPIO_AF8_UART5;               /* ����Ϊ����4 */
    HAL_GPIO_Init(UART5_TX_GPIO_PORT, &gpio_initure);       /* ����TX �� ģʽ���� */

    gpio_initure.Pin = UART5_RX_GPIO_PIN;
    HAL_GPIO_Init(UART5_RX_GPIO_PORT, &gpio_initure);       /* ����RX �� �������ó�����ģʽ */

    /* USART ��ʼ������ */
    g_uart5_handler.Instance = UART5_UX;                    /* ѡ��485��Ӧ�Ĵ��� */
    g_uart5_handler.Init.BaudRate = baudrate;               /* ������ */
    g_uart5_handler.Init.WordLength = UART_WORDLENGTH_8B;   /* �ֳ�Ϊ8λ���ݸ�ʽ */
    g_uart5_handler.Init.StopBits = UART_STOPBITS_1;        /* һ��ֹͣλ */
    g_uart5_handler.Init.Parity = UART_PARITY_NONE;         /* ����żУ��λ */
    g_uart5_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;   /* ��Ӳ������ */
    g_uart5_handler.Init.Mode = UART_MODE_TX_RX;            /* �շ�ģʽ */
    HAL_UART_Init(&g_uart5_handler);                        /* ʹ�ܶ�Ӧ�Ĵ���, �������MSp */
    __HAL_UART_DISABLE_IT(&g_uart5_handler, UART_IT_TC);

#if UART5_EN_RX /* ���ʹ���˽��� */
                /* ʹ�ܽ����ж� */
    __HAL_UART_ENABLE_IT(&g_uart5_handler, UART_IT_RXNE);   /* ���������ж� */
    HAL_NVIC_EnableIRQ(UART5_UX_IRQn);                      /* ʹ��USART1�ж� */
    HAL_NVIC_SetPriority(UART5_UX_IRQn, 7, 0);              /* ��ռ���ȼ�3�������ȼ�3 */
    
//    HAL_UART_Receive_IT( &g_uart5_handler, g_UART5_rx_buf, 2);
#endif
}

/*------------------------------------------------------------------------------------------------------------------------------------*/


/**
 * @brief       USART2����len���ֽ�
 * @param       buf     : �������׵�ַ
 * @param       len     : ���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ���� USART2_REC_LEN ���ֽ�)
 * @retval      ��
 */
/*------------------------------------------------------------------------------------------*/
void uart5_send_data(uint8_t *buf, uint8_t len)
{
//    HAL_UART_Transmit(&g_uart5_handler, buf, len, 1);    /* ����2�������� */
    HAL_UART_Transmit_DMA(&g_uart5_handler, buf, len);    /* ����2�������� */
//    printf("uart5 send\r\n");
    g_UART5_rx_cnt = 0;
}

/*------------------------------------------------------------------------------------------*/
