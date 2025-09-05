/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : can_if.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : extern header file of can.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CAN_IF_H_
#define _CAN_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include <stdint.h>
#include "stm32f4xx.h"
#include "mcu_if.h"
     
/**** Definition of constants ****/


/**** Definition of types ****/ 
typedef struct {
  uint16_t cob_id;	/**< message's ID */
  uint8_t rtr;		/**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
  uint8_t len;		/**< message's length (0 to 8) */
  uint8_t Data[8]; /**< message's datas */
} CanMessage;


/**** Definition of macros ****/


/**** Declaration of constants ****/


/**** Declaration of variables ****/
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

/**** Declaration of functions ****/
void CAN1_TX_IRQHandler(void);
void CAN1_RX0_IRQHandler(void);
void CAN1_RX1_IRQHandler(void);
void CAN2_TX_IRQHandler(void);
void CAN2_RX0_IRQHandler(void);
void CAN2_RX1_IRQHandler(void);
extern void hal_can1_irq_enable(void);
extern void hal_can1_irq_disable(void);
extern void hal_can2_irq_enable(void);
extern void hal_can2_irq_disable(void);
extern void CAN_DeInit(CAN_TypeDef *CANx);
extern void CAN_Init(CAN_TypeDef *CANx, uint16_t CANBitRate);
extern uint8_t CAN1_Send_Msg(uint16_t ID, uint8_t* msg, uint8_t len);
extern uint8_t CAN2_Send_Msg(uint16_t ID, uint8_t* msg, uint8_t len);
extern uint8_t HAL_CAN_TransmitFail_MailBox(CAN_TypeDef *CANx);
extern void HAL_CAN_CancelTransmit(CAN_TypeDef *CANx, uint8_t MailBox);
extern HAL_StatusTypeDef CAN_SendMsg(CAN_TypeDef *CANx, uint32_t ident, const uint8_t *buf, uint8_t dlc, uint32_t rtr);

#ifdef __cplusplus
}
#endif

#endif /*_CAN_IF_H_*/

/****************************** END OF FILE ***********************************/
