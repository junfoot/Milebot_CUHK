/**
 ****************************************************************************************************
 *
 * USART6 PC6-TX PC7-RX
 * 4号关节模组 左腿膝关节
 *
 ****************************************************************************************************
 */

#include "./BSP/USART6/usart6.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"

extern uint8_t data[4][8];;

UART_HandleTypeDef g_usart6_handler;

#ifdef USART6_EN_RX /* 如果使能了接收 */
uint8_t g_USART6_rx_buf[USART6_REC_LEN];  /* 接收缓冲, 最大 USART2_REC_LEN 个字节. */
uint8_t g_USART6_rx_cnt = 0;             /* 接收到的数据长度 */
void USART6_UX_IRQHandler(void)
{
//    HAL_UART_IRQHandler(&g_usart6_handler);
    
    uint8_t res;
//    short tor_o;

    if ((__HAL_UART_GET_FLAG(&g_usart6_handler, UART_FLAG_RXNE) != RESET)) /* 接收到数据 */
    {
        HAL_UART_Receive(&g_usart6_handler, &res, 1, 1000);

        if (g_USART6_rx_cnt < USART6_REC_LEN)         /* 缓冲区未满 */
        {
            g_USART6_rx_buf[g_USART6_rx_cnt] = res;   /* 记录接收到的值 */
            g_USART6_rx_cnt++;                       /* 接收数据增加1 */
        }
        
        /* 接收完成 */
        if (g_USART6_rx_cnt == 8)
        {
//            tor_o[3] = (g_USART6_rx_buf[0]<<8)+g_USART6_rx_buf[1];
//            tor_origin[3] = -(float)tor_o[3]*0.1f;
            g_USART6_rx_cnt = 0;    /* 清零 */
        }
    }
}
#endif

/**
 * @brief       USART2初始化函数
 *   @note      该函数主要是初始化串口
 * @param       baudrate: 波特率, 根据自己需要设置波特率值
 * @retval      无
 */
void usart6_init(uint32_t baudrate)
{
    /* IO 及 时钟配置 */
    USART6_TX_GPIO_CLK_ENABLE(); /* 使能 串口TX脚 时钟 */
    USART6_RX_GPIO_CLK_ENABLE(); /* 使能 串口RX脚 时钟 */
    USART6_UX_CLK_ENABLE();      /* 使能 串口 时钟 */

    GPIO_InitTypeDef gpio_initure;
    gpio_initure.Pin = USART6_TX_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_AF_PP;
    gpio_initure.Pull = GPIO_PULLUP;
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_initure.Alternate = GPIO_AF8_USART6;               /* 复用为串口4 */
    HAL_GPIO_Init(USART6_TX_GPIO_PORT, &gpio_initure);       /* 串口TX 脚 模式设置 */

    gpio_initure.Pin = USART6_RX_GPIO_PIN;
    HAL_GPIO_Init(USART6_RX_GPIO_PORT, &gpio_initure);       /* 串口RX 脚 必须设置成输入模式 */

    /* USART 初始化设置 */
    g_usart6_handler.Instance = USART6_UX;                    /* 选择485对应的串口 */
    g_usart6_handler.Init.BaudRate = baudrate;               /* 波特率 */
    g_usart6_handler.Init.WordLength = UART_WORDLENGTH_8B;   /* 字长为8位数据格式 */
    g_usart6_handler.Init.StopBits = UART_STOPBITS_1;        /* 一个停止位 */
    g_usart6_handler.Init.Parity = UART_PARITY_NONE;         /* 无奇偶校验位 */
    g_usart6_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;   /* 无硬件流控 */
    g_usart6_handler.Init.Mode = UART_MODE_TX_RX;            /* 收发模式 */
    HAL_UART_Init(&g_usart6_handler);                        /* 使能对应的串口, 但会调用MSp */
    __HAL_UART_DISABLE_IT(&g_usart6_handler, UART_IT_TC);

#if USART6_EN_RX /* 如果使能了接收 */
                /* 使能接收中断 */
    __HAL_UART_ENABLE_IT(&g_usart6_handler, UART_IT_RXNE);   /* 开启接收中断 */
    HAL_NVIC_EnableIRQ(USART6_UX_IRQn);                      /* 使能USART1中断 */
    HAL_NVIC_SetPriority(USART6_UX_IRQn, 7, 0);              /* 抢占优先级3，子优先级3 */
    
//    HAL_UART_Receive_IT( &g_usart6_handler, g_USART6_rx_buf, 2);
#endif
}
/*------------------------------------------------------------------------------------------------------------------------------------*/


/**
 * @brief       USART2发送len个字节
 * @param       buf     : 发送区首地址
 * @param       len     : 发送的字节数(为了和本代码的接收匹配,这里建议不要超过 USART2_REC_LEN 个字节)
 * @retval      无
 */
/*------------------------------------------------------------------------------------------*/
void usart6_send_data(uint8_t *buf, uint8_t len)
{
//    HAL_UART_Transmit(&g_usart6_handler, buf, len, 1);    /* 串口2发送数据 */
    HAL_UART_Transmit_DMA(&g_usart6_handler, buf, len);
    g_USART6_rx_cnt = 0;
}

/*------------------------------------------------------------------------------------------*/
