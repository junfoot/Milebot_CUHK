/**
 ****************************************************************************************************
 * @file        dma.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-20
 * @brief       DMA ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211020
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __DMA_H
#define	__DMA_H

#include "./SYSTEM/sys/sys.h"


extern DMA_HandleTypeDef g_dma_handle; 
extern DMA_HandleTypeDef g_dma_handle_usart2; 
extern DMA_HandleTypeDef g_dma_handle_usart3; 
extern DMA_HandleTypeDef g_dma_handle_uart5; 
extern DMA_HandleTypeDef g_dma_handle_usart6; 

void dma_init(void);  /* ����DMAx_CHx */

#endif






























