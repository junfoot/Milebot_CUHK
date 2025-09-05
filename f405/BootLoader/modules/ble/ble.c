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
    ģ�����Ĭ�����ã�
    ������: E104-BT5032A: 115200, N, 8, 1, ��ģʽ
*/

/*
    ���ݴ���ͽ��˵��:
    
    ͨ��"���ڵ�������" �·����ݸ������˵��(ʮ������)��
    ÿ�����Ĺ��������²��֣�֡ͷ������֡���ȡ��û����ݡ�У��ֵ��
    ֡ͷ(2���ֽ�)��0xAA 0x55
    ����֡����(1���ֽ�)������֡ͷ������֡���ȡ��û����ݡ�У��ֵ
    �û�����(������200���ֽ�)�����͵���Ч����
    У��ֵ(1���ֽ�)������֡ͷ������֡���ȡ��û����ݡ������ǵ��ۼ������ĺ�

    ���磺�û�Ҫ����3���ֽڵ���Ч���ݣ���0x11��0x22��0x33
    ���������������ݰ��ǣ�AA 55 07 11 22 33 6C
    ���У�
    ֡ͷ(2���ֽ�)��0xAA 0x55
    ����֡����(1���ֽ�)��0x07
    �û���Ч����(������200���ֽ�)��0x11��0x22��0x33
    У��ֵ(1���ֽ�)��6C  (��Ϊ0xAA + 0x55 + 0x07 + 0x11 + 0x22 + 0x33 = 0x16C��ȡ��8λ������0x6C)
 */


/* Private variables ---------------------------------------------------------*/

static uint8_t ble_tx_buf[BLE_TX_BUF_LEN] = {0x00};
static uint8_t ble_tx_buf_len = 0;
static uint8_t ble_packing_buf[BLE_TX_BUF_LEN] = {0x00};
static uint8_t ble_packing_buf_len = 0;

static uint8_t ble_unpacking_buf[BLE_RX_BUF_LEN] = {0x00};
static uint8_t ble_unpacking_buf_len = 0;


//������
uint16_t total_package = 0;
uint16_t current_package = 0;

//ÿ���������ж��ٸ��ֽ�
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
    
    //�ȴ������ȶ�
    osDelay(100);
    
    //��ʼ������ģ��, �޸Ĳ�����
    ble_init();

    upgrade_timeout_cnt = HAL_GetTick();
    while(1) 
    {
        //���ղ���������������
        if((uart5_dma_rx_buf_len > 0) && ((HAL_GetTick() - uart5_rx_data_tick) > 5))
        {
            ble_unpacking_buf_len = unpacking_data(ble_unpacking_buf, uart5_dma_rx_buf_raw, uart5_dma_rx_buf_len);
            if(ble_unpacking_buf_len > 0)
            {
                //�Ƿ�����������֡ͷ
                if((ble_unpacking_buf[0] == UART_UPGRADE_HEAD_1) && 
                    (ble_unpacking_buf[1] == UART_UPGRADE_HEAD_2))
                {
                    total_package = ble_unpacking_buf[2] + ble_unpacking_buf[3] * 256;
                    current_package = ble_unpacking_buf[4] + ble_unpacking_buf[5] * 256;

                    //����
                    upgrade_timeout_cnt = HAL_GetTick();

                    if(ble_unpacking_buf_len == (PER_PKG_LEN + 6))
                    {
                        //����һ��ִ��˳������ٶ�
                        //�����ϣ�Ӧ������дflash���ٷ���Ӧ��
                        
                        ble_tx_buf_len = 0;
                        ble_tx_buf[ble_tx_buf_len++] = UART_UPGRADE_HEAD_1;
                        ble_tx_buf[ble_tx_buf_len++] = UART_UPGRADE_HEAD_2;
                        ble_tx_buf[ble_tx_buf_len++] = (current_package >> 0);
                        ble_tx_buf[ble_tx_buf_len++] = (current_package >> 8);

                        //��������
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

                        //��������
                        ble_packing_buf_len = packing_data(ble_packing_buf, ble_tx_buf, ble_tx_buf_len);
                        ble_send_data(ble_packing_buf, ble_packing_buf_len);
                    }

                    if(total_package == current_package)
                    {
                        data = PARAM_NORMAL;
                        write_internal_flash(Parameters_ADDRESS, (uint8_t*)(&data), 4);

                        //�������Ƿ���ȷд��
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
                        //�������Ƿ���ȷд��

                        //�ظ���η������һ������, ȷ��APP �ܸ����յ�
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

                            //��������
                            ble_packing_buf_len = packing_data(ble_packing_buf, ble_tx_buf, ble_tx_buf_len);
                            ble_send_data(ble_packing_buf, ble_packing_buf_len);

                            HAL_Delay(100);
                            
                        }

                        //ϵͳ����
                        HAL_NVIC_SystemReset();
                    }
                }
            }
            
            uart5_dma_rx_buf_len = 0;
        }        

        //��ʱ��û�н��յ�������, ϵͳ����
        if((HAL_GetTick() - upgrade_timeout_cnt) > 10000)
        {
            HAL_NVIC_SystemReset();
        }

        osDelay(2);
    }
}



//����ͨ�ű��Ľṹ���Է��͵����ݽ��д��
//param 1 : ����󱣴������buf
//param 2 : ���ǰ������buf
//param 3 : ���ǰ���ݵĳ���
//����ֵ: �������������ֽڳ���
uint8_t packing_data(uint8_t* packing_buf, uint8_t* buf, uint8_t len)
{
    uint8_t index = 0;
    uint8_t i = 0;
    uint8_t check_sum = 0;

    //֡ͷ
    packing_buf[index++] = UART_FRAME_HEAD_1;
    packing_buf[index++] = UART_FRAME_HEAD_2;

    //����֡����
    packing_buf[index++] = (4 + len);

    //�û�����
    for(i = 0; i < len; i++)
    {
        packing_buf[index++] = buf[i];
    }

    //У��ֵ
    for(i = 0; i < index; i++)
    {
        check_sum += packing_buf[i];
    }
    packing_buf[index++] = check_sum;

    return index;
    
}


//����USARTͨ�ű��Ľṹ���Խ��յ����ݽ��н��
//param 1 : ��������ݵĴ��buf
//param 2 : ���ǰ��ԭʼ����
//param 3 : ���ǰ��ԭʼ���ݵĳ���
//����ֵ: ����Ϊ0 ����������Ч�������ֽڳ���
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
    
    for(i = 0; i < (buf[index+2] - 1); i++)//buf[index+2]����һ����֡���ܳ��ȣ�����check_sumҪ��һ
    {
        check_sum += buf[index+i];
    }

    if(check_sum == buf[index+i])//checkSum��ȷ
    {
        //ȡ����Ч����
        for(i = 0; i < (buf[index+2] - 4); i++)
        {
            unpacking_buf[i] = buf[index + 3 + i];
        }
        return (buf[index+2] - 4);
    }

    return 0;
}


//����ģ���⼰��ʼ��
static void ble_init(void)
{
#if 0   /* Begin if 0 by lison on 2021-2-8 */
    char* ret_ptr = NULL;

    //�͵�ƽ�� ����ģʽ
    BLE_MODE_CONFIG();


    //��������ģ�������Ϊ: MileBot, Ĭ�ϳ�������: (������)HMSoft��(������)E104-BT5032A
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
            //�޸�����ģ�������Ϊ: MileBot
            ble_set_name();
            osDelay(200);
        }
    }
#endif  /* End if 0 by lison on 2021-2-8 */

    //�ߵ�ƽ�� ͸��ģʽ
    BLE_MODE_TRANSMIT();

    //���buf
    uart5_dma_rx_buf_len = 0;
    memset(uart5_dma_rx_buf_raw, 0x00, UART5_DMA_BUF_LEN);
    
}

#if 0
//����ģ���ȡ����
static void ble_read_name(void)
{
    ble_tx_buf_len = sizeof("AT+NAME?") - 1;
    memcpy(ble_tx_buf, "AT+NAME?", ble_tx_buf_len);
    
    ble_send_data(ble_tx_buf, ble_tx_buf_len);
}

//�޸�����ģ�������Ϊ: MileBot
//��ָ��ִ�к�, �������ϵ�, �����õĲ���������Ч
static void ble_set_name(void)
{
    ble_tx_buf_len = sizeof("AT+NAME=MileBot") - 1;
    memcpy(ble_tx_buf, "AT+NAME=MileBot", ble_tx_buf_len);

    ble_send_data(ble_tx_buf, ble_tx_buf_len);
}
#endif

//�����������ݽӿں���
static void ble_send_data(uint8_t *pbuf, uint16_t size)
{
    uart5_dma_send_data(pbuf, size);
}

/****************************** END OF FILE ***********************************/

