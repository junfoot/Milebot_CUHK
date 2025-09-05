




/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*				  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : algorithm.h
* Author      : zx
* Version     : v0.01
* Date        : 2017/6/30
* Description : private header file of algorithm.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/30 zx          initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CAN_MOTION_CONTROL_H_
#define _CAN_MOTION_CONTROL_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include "stdint.h"
#include "..\walk\walk_if.h"
/**** Definition of constants ****/

/**** Definition of types ****/ 

     
/**** Definition of macros ****/


/**** Declaration of constants ****/


/**** Declaration of variables ****/

static void get_pdo_param(CANOPEN_COM_INFO *command,PDO_NUM pdo_num);
#ifdef __cplusplus
}
#endif

#endif /*_MCU_H_*/