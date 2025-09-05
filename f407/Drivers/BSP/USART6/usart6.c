/**
 ****************************************************************************************************
 *
 * USART6 PC6-TX PC7-RX
 * 4�Źؽ�ģ�� ����ϥ�ؽ�
 *
 ****************************************************************************************************
 */

#include "./BSP/USART6/usart6.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"

extern uint8_t data[4][8];;

UART_HandleTypeDef g_usart6_handler;

#ifdef USART6_EN_RX /* ���ʹ���˽��� */
uint8_t g_USART6_rx_buf[USART6_REC_LEN];  /* ���ջ���, ��� USART2_REC_LEN ���ֽ�. */
uint8_t g_USART6_rx_cnt = 0;             /* ���յ������ݳ��� */
void USART6_UX_IRQHandler(void)
{
//    HAL_UART_IRQHandler(&g_usart6_handler);
    
    uint8_t res;
//    short tor_o;

    if ((__HAL_UART_GET_FLAG(&g_usart6_handler, UART_FLAG_RXNE) != RESET)) /* ���յ����� */
    {
        HAL_UART_Receive(&g_usart6_handler, &res, 1, 1000);

        if (g_USART6_rx_cnt < USART6_REC_LEN)         /* ������δ�� */
        {
            g_USART6_rx_buf[g_USART6_rx_cnt] = res;   /* ��¼���յ���ֵ */
            g_USART6_rx_cnt++;                       /* ������������1 */
        }
        
        /* ������� */
        if (g_USART6_rx_cnt == 8)
        {
//            tor_o[3] = (g_USART6_rx_buf[0]<<8)+g_USART6_rx_buf[1];
//            tor_origin[3] = -(float)tor_o[3]*0.1f;
            g_USART6_rx_cnt = 0;    /* ���� */
        }
    }
}
#endif

/**
 * @brief       USART2��ʼ������
 *   @note      �ú�����Ҫ�ǳ�ʼ������
 * @param       baudrate: ������, �����Լ���Ҫ���ò�����ֵ
 * @retval      ��
 */
void usart6_init(uint32_t baudrate)
{
    /* IO �� ʱ������ */
    USART6_TX_GPIO_CLK_ENABLE(); /* ʹ�� ����TX�� ʱ�� */
    USART6_RX_GPIO_CLK_ENABLE(); /* ʹ�� ����RX�� ʱ�� */
    USART6_UX_CLK_ENABLE();      /* ʹ�� ���� ʱ�� */

    GPIO_InitTypeDef gpio_initure;
    gpio_initure.Pin = USART6_TX_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_AF_PP;
    gpio_initure.Pull = GPIO_PULLUP;
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_initure.Alternate = GPIO_AF8_USART6;               /* ����Ϊ����4 */
    HAL_GPIO_Init(USART6_TX_GPIO_PORT, &gpio_initure);       /* ����TX �� ģʽ���� */

    gpio_initure.Pin = USART6_RX_GPIO_PIN;
    HAL_GPIO_Init(USART6_RX_GPIO_PORT, &gpio_initure);       /* ����RX �� �������ó�����ģʽ */

    /* USART ��ʼ������ */
    g_usart6_handler.Instance = USART6_UX;                    /* ѡ��485��Ӧ�Ĵ��� */
    g_usart6_handler.Init.BaudRate = baudrate;               /* ������ */
    g_usart6_handler.Init.WordLength = UART_WORDLENGTH_8B;   /* �ֳ�Ϊ8λ���ݸ�ʽ */
    g_usart6_handler.Init.StopBits = UART_STOPBITS_1;        /* һ��ֹͣλ */
    g_usart6_handler.Init.Parity = UART_PARITY_NONE;         /* ����żУ��λ */
    g_usart6_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;   /* ��Ӳ������ */
    g_usart6_handler.Init.Mode = UART_MODE_TX_RX;            /* �շ�ģʽ */
    HAL_UART_Init(&g_usart6_handler);                        /* ʹ�ܶ�Ӧ�Ĵ���, �������MSp */
    __HAL_UART_DISABLE_IT(&g_usart6_handler, UART_IT_TC);

#if USART6_EN_RX /* ���ʹ���˽��� */
                /* ʹ�ܽ����ж� */
    __HAL_UART_ENABLE_IT(&g_usart6_handler, UART_IT_RXNE);   /* ���������ж� */
    HAL_NVIC_EnableIRQ(USART6_UX_IRQn);                      /* ʹ��USART1�ж� */
    HAL_NVIC_SetPriority(USART6_UX_IRQn, 7, 0);              /* ��ռ���ȼ�3�������ȼ�3 */
    
//    HAL_UART_Receive_IT( &g_usart6_handler, g_USART6_rx_buf, 2);
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
void usart6_send_data(uint8_t *buf, uint8_t len)
{
//    HAL_UART_Transmit(&g_usart6_handler, buf, len, 1);    /* ����2�������� */
    HAL_UART_Transmit_DMA(&g_usart6_handler, buf, len);
    g_USART6_rx_cnt = 0;
}

/*------------------------------------------------------------------------------------------*/
