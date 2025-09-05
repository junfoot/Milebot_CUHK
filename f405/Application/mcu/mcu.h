/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : mcu.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : private header file of mcu.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MCU_H_
#define _MCU_H_

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
void _Error_Handler(char * file, int line);
void mcu_fault_reset(void);

#ifdef __cplusplus
}
#endif

#endif /*_MCU_H_*/

/****************************** END OF FILE ***********************************/
