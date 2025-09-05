/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : can.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : can interface layer driver.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/

/* Public Includes -----------------------------------------------------------*/
#include "..\power\power_drv_if.h"
#include "..\modules\can\canopen\CANopen.h"
#include "mitapp.h"
#include "MitProtocol_if.h"


/* Private Includes ----------------------------------------------------------*/
#include "can.h"
#include "can_if.h"

/* Global variables ----------------------------------------------------------*/
CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;
uint8_t can1_flg;
uint8_t can2_flg;
/* Private variables ---------------------------------------------------------*/
static CanTxMsgTypeDef can_tx_msg;
static CanRxMsgTypeDef can_rx_msg;

/* Global functions ----------------------------------------------------------*/
/**
  * @brief  can_init.
  * @param  None
  * @retval None
  */
/* CAN1 init function */
void CAN_Init(CAN_TypeDef *CANx, uint16_t CANBitRate)
{
    CAN_HandleTypeDef* hcan;

    if(CANx == CAN1)
    {
        hcan = &hcan1;
    }
    else if(CANx == CAN2)
    {
          hcan = &hcan2;
    }
  
    hcan->Instance = CANx;
    hcan->pTxMsg = &can_tx_msg;
    hcan->pRxMsg = &can_rx_msg;
    hcan->Init.Mode = CAN_MODE_NORMAL;
    hcan->Init.TTCM = DISABLE;
    hcan->Init.ABOM = ENABLE;
    hcan->Init.AWUM = DISABLE;
    hcan->Init.NART = DISABLE;
    hcan->Init.RFLM = DISABLE;
    hcan->Init.TXFP = DISABLE;
    switch(CANBitRate) 
    {
        case 1000:
            hcan->Init.SJW = CAN_SJW_1TQ;
            hcan->Init.BS1 = CAN_BS1_11TQ;
            hcan->Init.BS2 = CAN_BS2_2TQ;    /* 42M/14/3 = 1000kbps,sample point=85.7% */
            hcan->Init.Prescaler = 3;
            break;
        case 875:
            hcan->Init.SJW = CAN_SJW_1TQ;
            hcan->Init.BS1 = CAN_BS1_6TQ;
            hcan->Init.BS2 = CAN_BS2_1TQ;    /* 42M/8/6 = 875k, sample point=87.5%*/
            hcan->Init.Prescaler = 6;
            break;
        case 500:
            hcan->Init.SJW = CAN_SJW_1TQ;
            hcan->Init.BS1 = CAN_BS1_11TQ;
            hcan->Init.BS2 = CAN_BS2_2TQ;    /* 42M/14/6 = 500k */
            hcan->Init.Prescaler = 6;
            break;
        case 250: 
            hcan->Init.SJW = CAN_SJW_1TQ;
            hcan->Init.BS1 = CAN_BS1_11TQ;
            hcan->Init.BS2 = CAN_BS2_2TQ;
            hcan->Init.Prescaler = 12;
            break;
        case 125: 
            hcan->Init.SJW = CAN_SJW_1TQ;
            hcan->Init.BS1 = CAN_BS1_11TQ;
            hcan->Init.BS2 = CAN_BS2_2TQ;
            hcan->Init.Prescaler = 24;
            break;
        case 100: 
            hcan->Init.SJW = CAN_SJW_1TQ;
            hcan->Init.BS1 = CAN_BS1_11TQ;
            hcan->Init.BS2 = CAN_BS2_2TQ;
            hcan->Init.Prescaler = 30;
            break;
        default:
            hcan->Init.SJW = CAN_SJW_1TQ;
            hcan->Init.BS1 = CAN_BS1_11TQ;
            hcan->Init.BS2 = CAN_BS2_2TQ;    /* 42M/14/6 = 500k */
            hcan->Init.Prescaler = 6;
            break;
    }
  
    if (HAL_CAN_Init(hcan) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    if(CANx == CAN1)
    {
        CAN_FilterConfTypeDef  sFilterConfig;
        /*##-2- Configure the CAN Filter ###########################################*/
        sFilterConfig.FilterNumber = 0;
        sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
        sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
        sFilterConfig.FilterIdHigh = 0x0000;
        sFilterConfig.FilterIdLow = 0x0000;
        sFilterConfig.FilterMaskIdHigh = 0x0000;
        sFilterConfig.FilterMaskIdLow = 0x0000;
        sFilterConfig.FilterFIFOAssignment = CAN_FIFO0;
        sFilterConfig.FilterActivation = ENABLE;
        sFilterConfig.BankNumber = 15;
        
        if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
        {
            /* Filter configuration Error */
            _Error_Handler(__FILE__, __LINE__);
        }
        hcan1.pTxMsg->RTR = CAN_RTR_DATA;
        hcan1.pTxMsg->IDE = CAN_ID_STD;
    }
    else if(CANx == CAN2)
    {
        CAN_FilterConfTypeDef  sFilterConfig;
        /*##-2- Configure the CAN Filter ###########################################*/
        sFilterConfig.FilterNumber = 15;
        sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
        sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
        sFilterConfig.FilterIdHigh = 0x0000;
        sFilterConfig.FilterIdLow = 0x0000;
        sFilterConfig.FilterMaskIdHigh = 0x0000;
        sFilterConfig.FilterMaskIdLow = 0x0000;
        sFilterConfig.FilterFIFOAssignment = CAN_FIFO0;
        sFilterConfig.FilterActivation = ENABLE;
        sFilterConfig.BankNumber = 15;

        if(HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK)
        {
            /* Filter configuration Error */
            _Error_Handler(__FILE__, __LINE__);
        }
        hcan2.pTxMsg->RTR = CAN_RTR_DATA;
        hcan2.pTxMsg->IDE = CAN_ID_STD;
    }
}

void CAN_DeInit(CAN_TypeDef *CANx)
{
    if(CANx == CAN1)
    {
        HAL_CAN_DeInit(&hcan1);
    }
    else if(CANx == CAN2)
    {
          HAL_CAN_DeInit(&hcan2);
    }
}

/**
  * @brief  can1 tx msg 
  * @param  .
  * @retval None
  */
HAL_StatusTypeDef CAN_SendMsg(CAN_TypeDef *CANx, uint32_t ident, const uint8_t *buf, uint8_t dlc, uint32_t rtr)
{
    uint8_t i;
    
    if(CANx == CAN1)
    {
        hcan1.pTxMsg->StdId = ident;
        hcan1.pTxMsg->DLC = dlc;
        hcan1.pTxMsg->RTR = rtr;
        for(i=0; i<8; i++)
        {
            hcan1.pTxMsg->Data[i] = buf[i];
        }

        return ( HAL_CAN_Transmit_IT(&hcan1) );
    }
    else if(CANx == CAN2)
    {
		hcan2.pTxMsg->StdId = ident;
		hcan2.pTxMsg->DLC = dlc;
		hcan2.pTxMsg->RTR = rtr;
        for(i=0; i<8; i++)
        {
            hcan2.pTxMsg->Data[i] = buf[i];
        }

        return ( HAL_CAN_Transmit_IT(&hcan2) );
    }    
    
    return HAL_OK;
}

uint8_t CAN1_Send_Msg(uint16_t ID, uint8_t* msg, uint8_t len)
{
    uint8_t i;

    hcan1.pTxMsg->StdId=ID;		
    hcan1.pTxMsg->IDE=CAN_ID_STD;	
    hcan1.pTxMsg->RTR=CAN_RTR_DATA;			
    hcan1.pTxMsg->DLC = len;

    for(i=0; i<len; i++)
    {
        hcan1.pTxMsg->Data[i] = msg[i];
    }

    if( HAL_CAN_Transmit_IT(&hcan1)  != HAL_OK)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t CAN2_Send_Msg(uint16_t ID, uint8_t* msg, uint8_t len)
{
    uint8_t i;
    
    hcan2.pTxMsg->StdId=ID;		
    hcan2.pTxMsg->IDE=CAN_ID_STD;	
    hcan2.pTxMsg->RTR=CAN_RTR_DATA;			
    hcan2.pTxMsg->DLC = len;

    for(i=0; i<len; i++)
    {
        hcan2.pTxMsg->Data[i] = msg[i];
    }

    if( HAL_CAN_Transmit_IT(&hcan2)  != HAL_OK)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
  * @brief  Transmission  complete callback in non blocking mode 
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
void HAL_CAN_TxCpltCallback(CAN_HandleTypeDef* hcan)
{
    //CO_CAN_TxCpltCallback(hcan->Instance);
}

/**
  * @brief  Transmission  complete callback in non blocking mode 
  * @param  CanHandle: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{ 
    if(hcan == &hcan1)
    {
        MIT_CAN_RxCpltCallback(hcan1.Instance, hcan1.pRxMsg);

        /* Receive */
        if(HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0) != HAL_OK)
        {
            __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_FOV0 | CAN_IT_FMP0);
        }
    }
    else if(hcan == &hcan2)
    {
        MIT_CAN_RxCpltCallback(hcan2.Instance, hcan2.pRxMsg);

        /* Receive */
        if(HAL_CAN_Receive_IT(&hcan2, CAN_FIFO0) != HAL_OK)
        {
            __HAL_CAN_ENABLE_IT(&hcan2, CAN_IT_FOV0 | CAN_IT_FMP0);
        }
    }
}

/**
  * @brief  Error CAN callback.
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
    //PW_LED_TOGGLE();
    hcan->err_cnt++;
    if(HAL_CAN_Receive_IT(hcan, CAN_FIFO0) != HAL_OK)
    {
        /* Reception Error */
        __HAL_CAN_ENABLE_IT(hcan, CAN_IT_FOV0 | CAN_IT_FMP0);
    }
}

/**
  * @brief  Get transmit fail mailbox.
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval transmit fail mailbox
  */
uint8_t HAL_CAN_TransmitFail_MailBox(CAN_TypeDef *CANx)
{
    if(CANx == CAN1)
    {
        if(__HAL_CAN_TRANSMIT_STATUS(&hcan1, CAN_TXMAILBOX_0) == FALSE)
        {
            return CAN_TXMAILBOX_0;
        }
        else if(__HAL_CAN_TRANSMIT_STATUS(&hcan1, CAN_TXMAILBOX_1) == FALSE)
        {
            return CAN_TXMAILBOX_1;
        }
        else if(__HAL_CAN_TRANSMIT_STATUS(&hcan1, CAN_TXMAILBOX_2) == FALSE)
        {
            return CAN_TXMAILBOX_2;
        }
        else 
        {
            return CAN_TXSTATUS_NOMAILBOX;
        }
    }
    else if(CANx == CAN2)
    {
        if(__HAL_CAN_TRANSMIT_STATUS(&hcan2, CAN_TXMAILBOX_0) == FALSE)
        {
            return CAN_TXMAILBOX_0;
        }
        else if(__HAL_CAN_TRANSMIT_STATUS(&hcan2, CAN_TXMAILBOX_1) == FALSE)
        {
            return CAN_TXMAILBOX_1;
        }
        else if(__HAL_CAN_TRANSMIT_STATUS(&hcan2, CAN_TXMAILBOX_2) == FALSE)
        {
            return CAN_TXMAILBOX_2;
        }
        else 
        {
            return CAN_TXSTATUS_NOMAILBOX;
        }
    }

    return CAN_TXSTATUS_NOMAILBOX;
}

/**
  * @brief  Cancel mailbox transmit.
  * @param  CANx,MailBox number
  * @retval None
  */
void HAL_CAN_CancelTransmit(CAN_TypeDef *CANx, uint8_t MailBox)
{
    if(CANx == CAN1)
    {
        __HAL_CAN_CANCEL_TRANSMIT(&hcan1, MailBox);
    }
    else if(CANx == CAN2)
    {
        __HAL_CAN_CANCEL_TRANSMIT(&hcan2, MailBox);
    }
}

/**
  * @brief  CAN1 IRQ NVIC enable.
  * @param  None
  * @retval None
  */
void hal_can1_irq_enable(void)
{
      HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
}

/**
  * @brief  CAN1 IRQ NVIC disable.
  * @param  None
  * @retval None
  */
void hal_can1_irq_disable(void)
{
    HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
}

/**
  * @brief  CAN2 IRQ NVIC enable.
  * @param  None
  * @retval None
  */
void hal_can2_irq_enable(void)
{
    HAL_NVIC_EnableIRQ(CAN2_TX_IRQn);
    HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
    HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);
}

/**
  * @brief  CAN2 IRQ NVIC disable.
  * @param  None
  * @retval None
  */
void hal_can2_irq_disable(void)
{
    HAL_NVIC_DisableIRQ(CAN2_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN2_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN2_RX1_IRQn);
}

/**
* @brief This function handles CAN1 TX interrupts.
*/
void CAN1_TX_IRQHandler(void)
{
  /* USER CODE BEGIN CAN1_TX_IRQn 0 */

  /* USER CODE END CAN1_TX_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan1);
  /* USER CODE BEGIN CAN1_TX_IRQn 1 */

  /* USER CODE END CAN1_TX_IRQn 1 */
}

/**
* @brief This function handles CAN1 RX0 interrupts.
*/
void CAN1_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN CAN1_RX0_IRQn 0 */
   can1_flg = 1;
  /* USER CODE END CAN1_RX0_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan1);
  /* USER CODE BEGIN CAN1_RX0_IRQn 1 */

  /* USER CODE END CAN1_RX0_IRQn 1 */
}

/**
* @brief This function handles CAN1 RX1 interrupt.
*/
void CAN1_RX1_IRQHandler(void)
{
  /* USER CODE BEGIN CAN1_RX1_IRQn 0 */

  /* USER CODE END CAN1_RX1_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan1);
  /* USER CODE BEGIN CAN1_RX1_IRQn 1 */

  /* USER CODE END CAN1_RX1_IRQn 1 */
}

/**
* @brief This function handles CAN2 TX interrupts.
*/
void CAN2_TX_IRQHandler(void)
{
  /* USER CODE BEGIN CAN2_TX_IRQn 0 */

  /* USER CODE END CAN2_TX_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan2);
  /* USER CODE BEGIN CAN2_TX_IRQn 1 */

  /* USER CODE END CAN2_TX_IRQn 1 */
}

/**
* @brief This function handles CAN2 RX0 interrupts.
*/
void CAN2_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN CAN2_RX0_IRQn 0 */
   can2_flg = 1;
  /* USER CODE END CAN2_RX0_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan2);
  /* USER CODE BEGIN CAN2_RX0_IRQn 1 */

  /* USER CODE END CAN2_RX0_IRQn 1 */
}

/**
* @brief This function handles CAN2 RX1 interrupt.
*/
void CAN2_RX1_IRQHandler(void)
{
  /* USER CODE BEGIN CAN2_RX1_IRQn 0 */

  /* USER CODE END CAN2_RX1_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan2);
  /* USER CODE BEGIN CAN2_RX1_IRQn 1 */

  /* USER CODE END CAN2_RX1_IRQn 1 */
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  mcu xx func config.
  * @param  None
  * @retval None
  */


/****************************** END OF FILE ***********************************/
