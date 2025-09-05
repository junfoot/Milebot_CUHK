/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*				  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : CO_driver.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : can application driver for canopen interface.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include "CO_driver.h"
#include "CO_Emergency.h"
#include <string.h>
#include "..\..\..\mcu\can_if.h"
#include "..\..\..\system\timer_if.h"
#include <math.h>

/* Private macro -------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variable ----------------------------------------------------------*/
/* Private function ----------------------------------------------------------*/
static HAL_StatusTypeDef CO_CANsendToModule(CO_CANmodule_t *CANmodule, CO_CANtx_t *buffer);

void InitCanLeds(void)
{
    //vLED_InitRCC();
    //vLED_InitPort();
}
#if 0
void CanLedsSet(eCoLeds led)
{

	if (led & eCoLed_Green)
        vLED_OnPB14Led();
    else
        vLED_OffPB14Led();

    if (led & eCoLed_Red)
        vLED_OnPB15Led();
    else
        vLED_OffPB15Led();

}
#endif
/*******************************************************************************
   Macro and Constants - CAN module registers
 *******************************************************************************/


/******************************************************************************/
void CO_CANsetConfigurationMode(CAN_TypeDef* CANx){
}


/******************************************************************************/
void CO_CANsetNormalMode(CO_CANmodule_t *CANmodule){
    CANmodule->CANnormal = true;
}


/******************************************************************************/
CO_ReturnError_t CO_CANmodule_init(
        CO_CANmodule_t         *CANmodule,
        CAN_TypeDef            *CANx,
        CO_CANrx_t              rxArray[],
        uint16_t                rxSize,
        CO_CANtx_t              txArray[],
        uint16_t                txSize,
        uint16_t                CANbitRate)
{
    int i;

    /* verify arguments */
    if(CANmodule==NULL || rxArray==NULL || txArray==NULL) 
	{
        return CO_ERROR_ILLEGAL_ARGUMENT;
    }

    CANmodule->CANx = CANx;
    CANmodule->rxArray = rxArray;
    CANmodule->rxSize = rxSize;
    CANmodule->txArray = txArray;
    CANmodule->txSize = txSize;
    CANmodule->CANnormal = false;
    CANmodule->useCANrxFilters = false;
    CANmodule->bufferInhibitFlag = 0;
    CANmodule->firstCANtxMessage = 1;
    CANmodule->CANtxCount = 0;
    CANmodule->errOld = 0;
    CANmodule->em = 0;
	
    for (i = 0; i < rxSize; i++) 
	{
        CANmodule->rxArray[i].ident = 0;
        CANmodule->rxArray[i].pFunct = 0;
    }
    
    for (i = 0; i < txSize; i++) 
	{
        CANmodule->txArray[i].bufferFull = 0;
    }

    /* Init CAN controler */
	CAN_Init(CANmodule->CANx, CANbitRate);

    return CO_ERROR_NO;
}

/******************************************************************************/
void CO_CANmodule_disable(CO_CANmodule_t *CANmodule)
{
    CAN_DeInit(CANmodule->CANx);
}

/******************************************************************************/
CO_ReturnError_t CO_CANrxBufferInit(
        CO_CANmodule_t         *CANmodule,
        uint16_t                index,
        uint16_t                ident,
        uint16_t                mask,
        int8_t                  rtr,
        void                   *object,
        void                  (*pFunct)(void *object, const CO_CANrxMsg_t *message))
{
    CO_CANrx_t *rxBuffer;
    uint16_t RXF, RXM;

    //safety
    if (!CANmodule || !object || !pFunct || index >= CANmodule->rxSize) {
        return CO_ERROR_ILLEGAL_ARGUMENT;
    }

    /* buffer, which will be configured */
    rxBuffer = CANmodule->rxArray + index;

    /* Configure object variables */
    rxBuffer->object = object;
    rxBuffer->pFunct = pFunct;

    /* CAN identifier and CAN mask, bit aligned with CAN module registers */
    RXF = (ident & 0x07FF) << 2;
    if (rtr) RXF |= 0x02;
    RXM = (mask & 0x07FF) << 2;
    RXM |= 0x02;

    /* configure filter and mask */
    if (RXF != rxBuffer->ident || RXM != rxBuffer->mask) {
        rxBuffer->ident = RXF;
        rxBuffer->mask = RXM;
    }

    return CO_ERROR_NO;
}

/******************************************************************************/
CO_CANtx_t *CO_CANtxBufferInit(
        CO_CANmodule_t         *CANmodule,
        uint16_t                index,
        uint16_t                ident,
        int8_t                  rtr,
        uint8_t                 noOfBytes,
        int8_t                  syncFlag)
{
    uint32_t TXF;
    CO_CANtx_t *buffer;

    /* safety */
    if (!CANmodule || CANmodule->txSize <= index) return 0;

    /* get specific buffer */
    buffer = &CANmodule->txArray[index];

    /* write to buffer */
    buffer->ident = TXF;
	buffer->rtr = (uint32_t)rtr;
    buffer->DLC = noOfBytes;
    buffer->bufferFull = 0;
    buffer->syncFlag = syncFlag ? 1 : 0;

    return buffer;
}

/******************************************************************************/
CO_ReturnError_t CO_CANsend(CO_CANmodule_t *CANmodule, CO_CANtx_t *buffer)
{
    CO_ReturnError_t err = CO_ERROR_NO;
    HAL_StatusTypeDef txBuff;

    /* Verify overflow */
    if (buffer->bufferFull) 
	{
        if(!CANmodule->firstCANtxMessage) /* don't set error, if bootup message is still on buffers */
            CO_errorReport((CO_EM_t*)CANmodule->em, CO_EM_CAN_TX_OVERFLOW, CO_EMC_CAN_OVERRUN, 0);
        err = CO_ERROR_TX_OVERFLOW;
    }

    CO_LOCK_CAN_SEND();
    
    /* First try to transmit the message immediately if mailbox is free.
     * Only one TX mailbox is used of the three available in the hardware */
    CANmodule->bufferInhibitFlag = buffer->syncFlag;
    txBuff = CO_CANsendToModule(CANmodule, buffer);

    /* No free mailbox -> use interrupt for transmission */
    if (txBuff != HAL_OK) 
	{
        buffer->bufferFull = 1;
        CANmodule->CANtxCount++;
    }
    CO_UNLOCK_CAN_SEND();

    return err;
}

/******************************************************************************/
void CO_CANclearPendingSyncPDOs(CO_CANmodule_t *CANmodule)
{
	uint32_t tpdoDeleted = 0U;
    uint8_t mailbox = CAN_TXSTATUS_NOMAILBOX;

    CO_LOCK_CAN_SEND();
    /* Abort message from CAN module, if there is synchronous TPDO. */
    mailbox = HAL_CAN_TransmitFail_MailBox(CANmodule->CANx);
    if((mailbox != CAN_TXSTATUS_NOMAILBOX) && (CANmodule->bufferInhibitFlag)) 
	{
        HAL_CAN_CancelTransmit(CANmodule->CANx, mailbox);
        CANmodule->bufferInhibitFlag = false;
        tpdoDeleted = 1U;
    }
    
    /* delete also pending synchronous TPDOs in TX buffers */
    if(CANmodule->CANtxCount != 0U)
	{
        uint16_t i;
        CO_CANtx_t *buffer = &CANmodule->txArray[0];
        for(i = CANmodule->txSize; i > 0U; i--)
		{
            if(buffer->bufferFull)
			{
                if(buffer->syncFlag)
				{
                    buffer->bufferFull = false;
                    CANmodule->CANtxCount--;
                    tpdoDeleted = 2U;
                }
            }
            buffer++;
        }
    }
    CO_UNLOCK_CAN_SEND();


    if(tpdoDeleted != 0U)
	{
        CO_errorReport((CO_EM_t*)CANmodule->em, CO_EM_TPDO_OUTSIDE_WINDOW, CO_EMC_COMMUNICATION, tpdoDeleted);
    }
}

/******************************************************************************/
void CO_CANverifyErrors(CO_CANmodule_t *CANmodule)
{
    uint32_t err;
    CO_EM_t* em = (CO_EM_t*)CANmodule->em;

    err = CANmodule->CANx->ESR;

    if(CANmodule->errOld != err) {
        CANmodule->errOld = err;

        /* CAN RX bus overflow */
        if(CANmodule->CANx->RF0R & 0x10) {
            CO_errorReport(em, CO_EM_CAN_RXB_OVERFLOW, CO_EMC_CAN_OVERRUN, err);
            CANmodule->CANx->RF0R &=~0x10;//clear bits
        }

        /* CAN TX bus off */
        if(err & 0x04) {
            CO_errorReport(em, CO_EM_CAN_TX_BUS_OFF, CO_EMC_BUS_OFF_RECOVERED, err);
        }
        else {
            CO_errorReset(em, CO_EM_CAN_TX_BUS_OFF, err);
        }
        
        /* CAN TX or RX bus passive */
        if(err & 0x02) {
            if(!CANmodule->firstCANtxMessage) CO_errorReport(em, CO_EM_CAN_TX_BUS_PASSIVE, CO_EMC_CAN_PASSIVE, err);
        }
        else {
        // int16_t wasCleared;
        /* wasCleared = */CO_errorReset(em, CO_EM_CAN_TX_BUS_PASSIVE, err);
        /* if(wasCleared == 1) */CO_errorReset(em, CO_EM_CAN_TX_OVERFLOW, err);
        }


        /* CAN TX or RX bus warning */
        if(err & 0x01) {
            CO_errorReport(em, CO_EM_CAN_BUS_WARNING, CO_EMC_NO_ERROR, err);
        }
        else {
            CO_errorReset(em, CO_EM_CAN_BUS_WARNING, err);
        }
    }
}

/******************************************************************************/
/* Interrupt from receiver */
void CO_CANinterrupt_Rx(CanRxMsgTypeDef* pRxMsg, CO_CANmodule_t *CANmodule)
{
//    CanRxMsg      CAN1_RxMsg;
//    HAL_CAN_Receive(CANmodule->CANx, CAN_FilterFIFO0, &CAN1_RxMsg)
    {   
        uint16_t index;
        uint8_t msgMatched = 0;
        CO_CANrx_t *msgBuff = CANmodule->rxArray;
        

        for (index = 0; index < CANmodule->rxSize; index++) 
        {
            uint16_t msg = (pRxMsg->StdId << 2) | (pRxMsg->RTR ? 2 : 0);
            if (((msg ^ msgBuff->ident) & msgBuff->mask) == 0) 
            {
                msgMatched = 1;
                break;
            }
            msgBuff++;
        }
        
        /* Call specific function, which will process the message */
        if (msgMatched && msgBuff->pFunct) 
        {
            msgBuff->pFunct(msgBuff->object, (CO_CANrxMsg_t*)pRxMsg);
        }
    }
}

/******************************************************************************/
/* Interrupt from trasmitter */
void CO_CANinterrupt_Tx(CO_CANmodule_t *CANmodule)
{
    /* First CAN message (bootup) was sent successfully */
    CANmodule->firstCANtxMessage = 0;
    
    /* clear flag from previous message */
    CANmodule->bufferInhibitFlag = 0;
    
    /* Are there any new messages waiting to be send */
    if (CANmodule->CANtxCount > 0) 
	{
        uint16_t i;             /* index of transmitting message */

        /* first buffer */
        CO_CANtx_t *buffer = CANmodule->txArray;
        /* search through whole array of pointers to transmit message buffers. */
        for(i = CANmodule->txSize; i > 0; i--) 
		{
            /* if message buffer is full, send it. */
            if(buffer->bufferFull) 
			{
                buffer->bufferFull = 0;
                CANmodule->CANtxCount--;
                
                /* Copy message to CAN buffer */
                CANmodule->bufferInhibitFlag = buffer->syncFlag;
                CO_CANsendToModule(CANmodule, buffer);
                break;                      /* exit for loop */
            }
            buffer++;
        }/* end of for loop */

        /* Clear counter if no more messages */
        if(i == 0) CANmodule->CANtxCount = 0;
    }
}

/******************************************************************************/
static HAL_StatusTypeDef CO_CANsendToModule(CO_CANmodule_t *CANmodule, CO_CANtx_t *buffer)
{
	return CAN_SendMsg(CANmodule->CANx, buffer->ident, buffer->data, buffer->DLC, buffer->rtr);
}

/******************************************************************************/

