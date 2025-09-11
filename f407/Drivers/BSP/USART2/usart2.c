/**
 ****************************************************************************************************
 *
 * USART2 PA2-TX PA3-RX
 * 1号关节模组 右腿髋关节
 *
 ****************************************************************************************************
 */

#include "./BSP/USART2/usart2.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/DMA/dma.h"

#include "string.h"
#include "stdio.h"

extern uint8_t data[4][8];

UART_HandleTypeDef g_usart2_handler;     /* USART2控制句柄(串口) */

#ifdef USART2_EN_RX /* 如果使能了接收 */

uint8_t g_USART2_rx_buf[USART2_REC_LEN];  /* 接收缓冲, 最大 USART2_REC_LEN 个字节. */
uint8_t g_USART2_rx_cnt = 0;             /* 接收到的数据长度 */

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
 * @brief       USART2初始化函数
 *   @note      该函数主要是初始化串口
 * @param       baudrate: 波特率, 根据自己需要设置波特率值
 * @retval      无
 */
void usart2_init(uint32_t baudrate)
{
    /* IO 及 时钟配置 */
    USART2_RE_GPIO_CLK_ENABLE(); /* 使能 USART2_RE 脚时钟 */
    USART2_TX_GPIO_CLK_ENABLE(); /* 使能 串口TX脚 时钟 */
    USART2_RX_GPIO_CLK_ENABLE(); /* 使能 串口RX脚 时钟 */
    USART2_UX_CLK_ENABLE();      /* 使能 串口 时钟 */

    GPIO_InitTypeDef gpio_initure;
    gpio_initure.Pin = USART2_TX_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_AF_PP;
    gpio_initure.Pull = GPIO_PULLUP;
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_initure.Alternate = GPIO_AF7_USART2;               /* 复用为串口2 */
    HAL_GPIO_Init(USART2_TX_GPIO_PORT, &gpio_initure);       /* 串口TX 脚 模式设置 */

    gpio_initure.Pin = USART2_RX_GPIO_PIN;
    HAL_GPIO_Init(USART2_RX_GPIO_PORT, &gpio_initure);       /* 串口RX 脚 必须设置成输入模式 */

//    gpio_initure.Pin = USART2_RE_GPIO_PIN;
//    gpio_initure.Mode = GPIO_MODE_OUTPUT_PP;
//    gpio_initure.Pull = GPIO_PULLUP;
//    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(USART2_RE_GPIO_PORT, &gpio_initure);       /* USART2_RE 脚 模式设置 */

    /* USART 初始化设置 */
    g_usart2_handler.Instance = USART2_UX;                    /* 选择485对应的串口 */
    g_usart2_handler.Init.BaudRate = baudrate;               /* 波特率 */
    g_usart2_handler.Init.WordLength = UART_WORDLENGTH_8B;   /* 字长为8位数据格式 */
    g_usart2_handler.Init.StopBits = UART_STOPBITS_1;        /* 一个停止位 */
    g_usart2_handler.Init.Parity = UART_PARITY_NONE;         /* 无奇偶校验位 */
    g_usart2_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;   /* 无硬件流控 */
    g_usart2_handler.Init.Mode = UART_MODE_TX_RX;            /* 收发模式 */
    HAL_UART_Init(&g_usart2_handler);                        /* 使能对应的串口, 但会调用MSp */
    __HAL_UART_DISABLE_IT(&g_usart2_handler, UART_IT_TC);

#if USART2_EN_RX /* 如果使能了接收 */
                /* 使能接收中断 */
//    __HAL_UART_ENABLE_IT(&g_usart2_handler, UART_IT_RXNE);   /* 开启接收中断 */
		HAL_UART_Receive_DMA(&g_usart2_handler, g_USART2_rx_buf, USART2_REC_LEN);
		__HAL_UART_ENABLE_IT(&g_usart2_handler, UART_IT_IDLE);  // 使能串口空闲中断
    HAL_NVIC_EnableIRQ(USART2_UX_IRQn);                      /* 使能USART2中断 */
    HAL_NVIC_SetPriority(USART2_UX_IRQn, 5, 0);              /* 抢占优先级3，子优先级3 */
    
//    HAL_UART_Receive_IT( &g_usart2_handler, g_USART2_rx_buf, 2);
#endif

//    USART2_RE(0); /* 默认为接收模式 */
}

/**
 * @brief       USART2发送len个字节
 * @param       buf     : 发送区首地址
 * @param       len     : 发送的字节数(为了和本代码的接收匹配,这里建议不要超过 USART2_REC_LEN 个字节)
 * @retval      无
 */
void usart2_send_data(uint8_t *buf, uint8_t len)
{
//    HAL_UART_Transmit(&g_usart2_handler, buf, len, 1);
    HAL_UART_Transmit_DMA(&g_usart2_handler, buf, len);
    g_USART2_rx_cnt = 0;
}

