/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : ble.c
* Author      : LisonLiu
* Version     : v0.01
* Date        : 2018/6/1
* Description : 
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2018/6/1  LisonLiu    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/

#include "cmsis_os2.h"                  // ARM::CMSIS:RTOS2:Keil RTX5

/* Public Includes -----------------------------------------------------------*/

#include <stdlib.h>
#include "string.h"
#include<stdio.h>

#include "main.h"
#include "uart_if.h"
#include "power_mana_if.h"
#include "internal_flash.h"
#include "iap.h"

/* Private Includes ----------------------------------------------------------*/

#include "ble.h"
#include "ble_if.h"  


/*
    模块出厂默认配置：
    新蓝牙: E104-BT5032A: 115200, N, 8, 1, 从模式
*/

/*
    数据打包和解包说明:
    
    通过"串口调试助手" 下发数据给主板的说明(十六进制)：
    每个报文共包括以下部分：帧头、数据帧长度、用户数据、校验值。
    帧头(2个字节)：0xAA 0x55
    数据帧长度(1个字节)：包括帧头、数据帧长度、用户数据、校验值
    用户数据(不超过200个字节)：发送的有效数据
    校验值(1个字节)：包括帧头、数据帧长度、用户数据。把它们的累加起来的和

    例如：用户要发送3个字节的有效数据，是0x11、0x22、0x33
    则发送整个完整数据包是：AA 55 07 11 22 33 6C
    其中：
    帧头(2个字节)：0xAA 0x55
    数据帧长度(1个字节)：0x07
    用户有效数据(不超过200个字节)：0x11、0x22、0x33
    校验值(1个字节)：6C  (因为0xAA + 0x55 + 0x07 + 0x11 + 0x22 + 0x33 = 0x16C，取低8位，即是0x6C)
 */


/* Private variables ---------------------------------------------------------*/

static uint8_t ble_tx_buf[BLE_TX_BUF_LEN] = {0x00};
static uint8_t ble_tx_buf_len = 0;
static uint8_t ble_packing_buf[BLE_TX_BUF_LEN] = {0x00};
static uint8_t ble_packing_buf_len = 0;

static uint8_t ble_unpacking_buf[BLE_RX_BUF_LEN] = {0x00};
static uint8_t ble_unpacking_buf_len = 0;


//升级包
uint16_t total_package = 0;
uint16_t current_package = 0;

//每包升级包有多少个字节
#define PER_PKG_LEN     200


/* Private function prototypes -----------------------------------------------*/
static void ble_init(void);
static void ble_send_data(uint8_t *pbuf, uint16_t size);
#if 0
static void ble_read_name(void);
static void ble_set_name(void);
#endif

/* Private functions ----------------------------------------------------------*/
void ble_task(void *argument)
{
    static uint32_t upgrade_timeout_cnt = 0;
    uint32_t data = PARAM_NORMAL;
    uint8_t cnt = 0;
    
    //等待供电稳定
    osDelay(100);
    
    //初始化蓝牙模块, 修改波特率
    ble_init();

    upgrade_timeout_cnt = HAL_GetTick();
    while(1) 
    {
        //接收并解析蓝牙的数据
        if((uart5_dma_rx_buf_len > 0) && ((HAL_GetTick() - uart5_rx_data_tick) > 5))
        {
            ble_unpacking_buf_len = unpacking_data(ble_unpacking_buf, uart5_dma_rx_buf_raw, uart5_dma_rx_buf_len);
            if(ble_unpacking_buf_len > 0)
            {
                //是否是升级包的帧头
                if((ble_unpacking_buf[0] == UART_UPGRADE_HEAD_1) && 
                    (ble_unpacking_buf[1] == UART_UPGRADE_HEAD_2))
                {
                    total_package = ble_unpacking_buf[2] + ble_unpacking_buf[3] * 256;
                    current_package = ble_unpacking_buf[4] + ble_unpacking_buf[5] * 256;

                    //更新
                    upgrade_timeout_cnt = HAL_GetTick();

                    if(ble_unpacking_buf_len == (PER_PKG_LEN + 6))
                    {
                        //调整一下执行顺序，提高速度
                        //理论上，应该是先写flash，再发送应答
                        
                        ble_tx_buf_len = 0;
                        ble_tx_buf[ble_tx_buf_len++] = UART_UPGRADE_HEAD_1;
                        ble_tx_buf[ble_tx_buf_len++] = UART_UPGRADE_HEAD_2;
                        ble_tx_buf[ble_tx_buf_len++] = (current_package >> 0);
                        ble_tx_buf[ble_tx_buf_len++] = (current_package >> 8);

                        //发送数据
                        ble_packing_buf_len = packing_data(ble_packing_buf, ble_tx_buf, ble_tx_buf_len);
                        ble_send_data(ble_packing_buf, ble_packing_buf_len);

                        write_internal_flash_without_erase(Application_ADDRESS + PER_PKG_LEN*(current_package - 1), 
                            &ble_unpacking_buf[6], PER_PKG_LEN);
                    }
                    else
                    {
                        ble_tx_buf_len = 0;
                        ble_tx_buf[ble_tx_buf_len++] = UART_UPGRADE_HEAD_1;
                        ble_tx_buf[ble_tx_buf_len++] = UART_UPGRADE_HEAD_2;
                        ble_tx_buf[ble_tx_buf_len++] = (current_package >> 0);
                        ble_tx_buf[ble_tx_buf_len++] = (current_package >> 8); 

                        //发送数据
                        ble_packing_buf_len = packing_data(ble_packing_buf, ble_tx_buf, ble_tx_buf_len);
                        ble_send_data(ble_packing_buf, ble_packing_buf_len);
                    }

                    if(total_package == current_package)
                    {
                        data = PARAM_NORMAL;
                        write_internal_flash(Parameters_ADDRESS, (uint8_t*)(&data), 4);

                        //检查参数是否正确写入
                        cnt = 0;
                        HAL_Delay(5);
                        read_internal_flash(Parameters_ADDRESS, (uint8_t*)(&data), 4);
                        while((data != PARAM_NORMAL) && (cnt <= 3))
                        {
                            cnt++;
                            HAL_Delay(5);
                            data = PARAM_NORMAL;
                            write_internal_flash(Parameters_ADDRESS, (uint8_t*)(&data), 4);
                        
                            HAL_Delay(5);
                            read_internal_flash(Parameters_ADDRESS, (uint8_t*)(&data), 4);
                        }
                        //检查参数是否正确写入

                        //重复多次发送最后一包数据, 确保APP 能给接收到
                        cnt = 0;
                        while(cnt < 3)
                        {
                            cnt++;
                            HAL_Delay(100);

                            ble_tx_buf_len = 0;
                            ble_tx_buf[ble_tx_buf_len++] = UART_UPGRADE_HEAD_1;
                            ble_tx_buf[ble_tx_buf_len++] = UART_UPGRADE_HEAD_2;
                            ble_tx_buf[ble_tx_buf_len++] = (current_package >> 0);
                            ble_tx_buf[ble_tx_buf_len++] = (current_package >> 8);

                            //发送数据
                            ble_packing_buf_len = packing_data(ble_packing_buf, ble_tx_buf, ble_tx_buf_len);
                            ble_send_data(ble_packing_buf, ble_packing_buf_len);

                            HAL_Delay(100);
                            
                        }

                        //系统重启
                        HAL_NVIC_SystemReset();
                    }
                }
            }
            
            uart5_dma_rx_buf_len = 0;
        }        

        //长时间没有接收到升级包, 系统重启
        if((HAL_GetTick() - upgrade_timeout_cnt) > 10000)
        {
            HAL_NVIC_SystemReset();
        }

        osDelay(2);
    }
}



//根据通信报文结构，对发送的数据进行打包
//param 1 : 打包后保存的数据buf
//param 2 : 打包前的数据buf
//param 3 : 打包前数据的长度
//返回值: 打包后的数据总字节长度
uint8_t packing_data(uint8_t* packing_buf, uint8_t* buf, uint8_t len)
{
    uint8_t index = 0;
    uint8_t i = 0;
    uint8_t check_sum = 0;

    //帧头
    packing_buf[index++] = UART_FRAME_HEAD_1;
    packing_buf[index++] = UART_FRAME_HEAD_2;

    //数据帧长度
    packing_buf[index++] = (4 + len);

    //用户数据
    for(i = 0; i < len; i++)
    {
        packing_buf[index++] = buf[i];
    }

    //校验值
    for(i = 0; i < index; i++)
    {
        check_sum += packing_buf[i];
    }
    packing_buf[index++] = check_sum;

    return index;
    
}


//根据USART通信报文结构，对接收的数据进行解包
//param 1 : 解包后数据的存放buf
//param 2 : 解包前的原始数据
//param 3 : 解包前的原始数据的长度
//返回值: 出错为0 、解包后的有效数据总字节长度
uint8_t unpacking_data(uint8_t* unpacking_buf, uint8_t* buf, uint8_t len)
{
    uint8_t index = 0;
    uint8_t i = 0;
    uint8_t check_sum = 0;

    if(len < 4)
    {
        return 0;
    }

    while((buf[index] != UART_FRAME_HEAD_1) || (buf[index+1] != UART_FRAME_HEAD_2))
    {
        index++;
        if(index >= (len - 4))
        {
            return 0;
        }
    }
    
    for(i = 0; i < (buf[index+2] - 1); i++)//buf[index+2]是这一数据帧的总长度，计算check_sum要减一
    {
        check_sum += buf[index+i];
    }

    if(check_sum == buf[index+i])//checkSum正确
    {
        //取出有效数据
        for(i = 0; i < (buf[index+2] - 4); i++)
        {
            unpacking_buf[i] = buf[index + 3 + i];
        }
        return (buf[index+2] - 4);
    }

    return 0;
}


//蓝牙模块检测及初始化
static void ble_init(void)
{
#if 0   /* Begin if 0 by lison on 2021-2-8 */
    char* ret_ptr = NULL;

    //低电平： 配置模式
    BLE_MODE_CONFIG();


    //设置蓝牙模块的名字为: MileBot, 默认出厂名字: (旧蓝牙)HMSoft、(新蓝牙)E104-BT5032A
    uart5_dma_rx_buf_len = 0;
    memset(uart5_dma_rx_buf_raw, 0x00, UART5_DMA_BUF_LEN);
    ble_read_name();
    osDelay(500);
    ret_ptr = strstr((char*)uart5_dma_rx_buf_raw, "MileBot");
    if(ret_ptr == NULL)
    {
        uart5_dma_rx_buf_len = 0;
        memset(uart5_dma_rx_buf_raw, 0x00, UART5_DMA_BUF_LEN);
        ble_read_name();
        osDelay(500);
        ret_ptr = strstr((char*)uart5_dma_rx_buf_raw, "MileBot");
        if(ret_ptr == NULL)
        {
            //修改蓝牙模块的名字为: MileBot
            ble_set_name();
            osDelay(200);
        }
    }
#endif  /* End if 0 by lison on 2021-2-8 */

    //高电平： 透传模式
    BLE_MODE_TRANSMIT();

    //清空buf
    uart5_dma_rx_buf_len = 0;
    memset(uart5_dma_rx_buf_raw, 0x00, UART5_DMA_BUF_LEN);
    
}

#if 0
//蓝牙模块读取名字
static void ble_read_name(void)
{
    ble_tx_buf_len = sizeof("AT+NAME?") - 1;
    memcpy(ble_tx_buf, "AT+NAME?", ble_tx_buf_len);
    
    ble_send_data(ble_tx_buf, ble_tx_buf_len);
}

//修改蓝牙模块的名字为: MileBot
//该指令执行后, 须重新上电, 新设置的参数才能生效
static void ble_set_name(void)
{
    ble_tx_buf_len = sizeof("AT+NAME=MileBot") - 1;
    memcpy(ble_tx_buf, "AT+NAME=MileBot", ble_tx_buf_len);

    ble_send_data(ble_tx_buf, ble_tx_buf_len);
}
#endif

//蓝牙发送数据接口函数
static void ble_send_data(uint8_t *pbuf, uint16_t size)
{
    uart5_dma_send_data(pbuf, size);
}

/****************************** END OF FILE ***********************************/

