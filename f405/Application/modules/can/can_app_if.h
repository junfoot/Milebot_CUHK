/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*				  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : can_app_if.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : extern header file of can_app.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CAN_APP_IF_H_
#define _CAN_APP_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/

     
/**** Definition of constants ****/


/**** Definition of types ****/ 


/**** Definition of macros ****/


/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
extern void can_task(void *argument);
extern void can_app_pdo_handle(void);

extern void can_app_pdo_handle(void);
extern void can_app_rpdo_process(uint8_t param, uint16_t *msg);
extern void pdo_control_send(uint16_t data);


extern uint8_t can_write(uint32_t std_id,uint8_t *data);
#ifdef __cplusplus
}
#endif

#endif /*_CAN_APP_IF_H_*/

/****************************** END OF FILE ***********************************/
