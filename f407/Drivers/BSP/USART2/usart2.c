/**
 ****************************************************************************************************
 *
 * USART2 PA2-TX PA3-RX
 * 1�Źؽ�ģ�� �����Źؽ�
 *
 ****************************************************************************************************
 */

#include "./BSP/USART2/usart2.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/DMA/dma.h"

#include "string.h"
#include "stdio.h"

extern uint8_t data[4][8];

UART_HandleTypeDef g_usart2_handler;     /* USART2���ƾ��(����) */

#ifdef USART2_EN_RX /* ���ʹ���˽��� */

uint8_t g_USART2_rx_buf[USART2_REC_LEN];  /* ���ջ���, ��� USART2_REC_LEN ���ֽ�. */
uint8_t g_USART2_rx_cnt = 0;             /* ���յ������ݳ��� */

typedef enum {
    RX_STATE_WAIT_HEAD1,
    RX_STATE_WAIT_HEAD2,
    RX_STATE_WAIT_LENGTH,
    RX_STATE_RECEIVING_DATA,
    RX_STATE_CHECK_SUM
} RxState;

extern float exo_state[];

void USART2_UX_IRQHandler(void)
{
    uint16_t rx_len = 0;
		uint16_t expected_len = 0;
    uint8_t checksum = 0;
	
    if(__HAL_UART_GET_FLAG(&g_usart2_handler, UART_FLAG_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&g_usart2_handler);
		
				HAL_UART_DMAStop(&g_usart2_handler);
//				HAL_DMA_Abort(&g_dma_handle_usart2_rx);
		
				// receive length
        rx_len = USART2_REC_LEN - __HAL_DMA_GET_COUNTER(&g_dma_handle_usart2_rx);
			
//				printf("66:%d\r\n", rx_len);
//				printf("buf: %02X %02X %02X %02X\r\n",
//					 g_USART2_rx_buf[0], g_USART2_rx_buf[1],
//					 g_USART2_rx_buf[2], g_USART2_rx_buf[3]);


        // check
        if (rx_len >= 4 && g_USART2_rx_buf[0] == 0xAA && g_USART2_rx_buf[1] == 0x55)
        {
            expected_len = g_USART2_rx_buf[2];
            if (rx_len == expected_len)
            {
                checksum = 0;
                for(uint16_t i=0; i<expected_len-1; i++)
                    checksum += g_USART2_rx_buf[i];

                if(checksum == g_USART2_rx_buf[expected_len-1])
                {
										memcpy(&exo_state, &g_USART2_rx_buf[3], expected_len - 4);
                }
            }
        }

        // reopen dma
        HAL_UART_Receive_DMA(&g_usart2_handler, g_USART2_rx_buf, USART2_REC_LEN);
    }

  		
//		static uint16_t rx_index = 0;
//    static uint8_t expected_len = 0;
//    static uint8_t checksum = 0;
//		static RxState state = RX_STATE_WAIT_HEAD1;
//    uint8_t data;  
//    if ((__HAL_UART_GET_FLAG(&g_usart2_handler, UART_FLAG_RXNE) != RESET)) /* ????? */
//    {
//        HAL_UART_Receive(&g_usart2_handler, &data, 1, 1000);

//				// ------------------ check data received -------------------------
//				switch(state)
//        {
//        case RX_STATE_WAIT_HEAD1:
//            if(data == 0xAA)
//            {
//                state = RX_STATE_WAIT_HEAD2;
//                checksum = data;
//            }
//            break;
//            
//        case RX_STATE_WAIT_HEAD2:
//            if(data == 0x55)
//            {
//                state = RX_STATE_WAIT_LENGTH;
//                checksum += data;
//            }
//            else
//            {
//                state = RX_STATE_WAIT_HEAD1;
//            }
//            break;
//            
//        case RX_STATE_WAIT_LENGTH:
////						printf("e:%d\r\n",expected_len);
//            expected_len = data;
//            checksum += data;
//            
//            if(expected_len <= USART2_REC_LEN)
//            {
//                state = RX_STATE_RECEIVING_DATA;
//                rx_index = 0;
//            }
//            else
//            {
//                state = RX_STATE_WAIT_HEAD1;
//            }
//            break;
//            
//        case RX_STATE_RECEIVING_DATA:
//            g_USART2_rx_buf[rx_index++] = data;
////            checksum += 1;
//						checksum += data;
//            
//            if(rx_index >= (expected_len - 4))  // ?????
//            {
//                state = RX_STATE_CHECK_SUM;
//            }
//            break;
//            
//        case RX_STATE_CHECK_SUM:
////						printf("%x %x\r\n", checksum, data);
//            if(checksum == data)
////						if(1)
//            {
//                // ????,????
//                if(expected_len == 36)  // 8?float + ??2 + ??1 + ??1
//                {
//                    memcpy(&exo_state, g_USART2_rx_buf, 32);
//                }
//            }
//            state = RX_STATE_WAIT_HEAD1;
//            break;
//        }
//		}
						
}

#endif

/**
 * @brief       USART2��ʼ������
 *   @note      �ú�����Ҫ�ǳ�ʼ������
 * @param       baudrate: ������, �����Լ���Ҫ���ò�����ֵ
 * @retval      ��
 */
void usart2_init(uint32_t baudrate)
{
    /* IO �� ʱ������ */
    USART2_RE_GPIO_CLK_ENABLE(); /* ʹ�� USART2_RE ��ʱ�� */
    USART2_TX_GPIO_CLK_ENABLE(); /* ʹ�� ����TX�� ʱ�� */
    USART2_RX_GPIO_CLK_ENABLE(); /* ʹ�� ����RX�� ʱ�� */
    USART2_UX_CLK_ENABLE();      /* ʹ�� ���� ʱ�� */

    GPIO_InitTypeDef gpio_initure;
    gpio_initure.Pin = USART2_TX_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_AF_PP;
    gpio_initure.Pull = GPIO_PULLUP;
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_initure.Alternate = GPIO_AF7_USART2;               /* ����Ϊ����2 */
    HAL_GPIO_Init(USART2_TX_GPIO_PORT, &gpio_initure);       /* ����TX �� ģʽ���� */

    gpio_initure.Pin = USART2_RX_GPIO_PIN;
    HAL_GPIO_Init(USART2_RX_GPIO_PORT, &gpio_initure);       /* ����RX �� �������ó�����ģʽ */

//    gpio_initure.Pin = USART2_RE_GPIO_PIN;
//    gpio_initure.Mode = GPIO_MODE_OUTPUT_PP;
//    gpio_initure.Pull = GPIO_PULLUP;
//    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(USART2_RE_GPIO_PORT, &gpio_initure);       /* USART2_RE �� ģʽ���� */

    /* USART ��ʼ������ */
    g_usart2_handler.Instance = USART2_UX;                    /* ѡ��485��Ӧ�Ĵ��� */
    g_usart2_handler.Init.BaudRate = baudrate;               /* ������ */
    g_usart2_handler.Init.WordLength = UART_WORDLENGTH_8B;   /* �ֳ�Ϊ8λ���ݸ�ʽ */
    g_usart2_handler.Init.StopBits = UART_STOPBITS_1;        /* һ��ֹͣλ */
    g_usart2_handler.Init.Parity = UART_PARITY_NONE;         /* ����żУ��λ */
    g_usart2_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;   /* ��Ӳ������ */
    g_usart2_handler.Init.Mode = UART_MODE_TX_RX;            /* �շ�ģʽ */
    HAL_UART_Init(&g_usart2_handler);                        /* ʹ�ܶ�Ӧ�Ĵ���, �������MSp */
    __HAL_UART_DISABLE_IT(&g_usart2_handler, UART_IT_TC);

#if USART2_EN_RX /* ���ʹ���˽��� */
                /* ʹ�ܽ����ж� */
//    __HAL_UART_ENABLE_IT(&g_usart2_handler, UART_IT_RXNE);   /* ���������ж� */
		HAL_UART_Receive_DMA(&g_usart2_handler, g_USART2_rx_buf, USART2_REC_LEN);
		__HAL_UART_ENABLE_IT(&g_usart2_handler, UART_IT_IDLE);  // ʹ�ܴ��ڿ����ж�
    HAL_NVIC_EnableIRQ(USART2_UX_IRQn);                      /* ʹ��USART2�ж� */
    HAL_NVIC_SetPriority(USART2_UX_IRQn, 5, 0);              /* ��ռ���ȼ�3�������ȼ�3 */
    
//    HAL_UART_Receive_IT( &g_usart2_handler, g_USART2_rx_buf, 2);
#endif

//    USART2_RE(0); /* Ĭ��Ϊ����ģʽ */
}

/**
 * @brief       USART2����len���ֽ�
 * @param       buf     : �������׵�ַ
 * @param       len     : ���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ���� USART2_REC_LEN ���ֽ�)
 * @retval      ��
 */
void usart2_send_data(uint8_t *buf, uint8_t len)
{
//    HAL_UART_Transmit(&g_usart2_handler, buf, len, 1);
    HAL_UART_Transmit_DMA(&g_usart2_handler, buf, len);
    g_USART2_rx_cnt = 0;
}

