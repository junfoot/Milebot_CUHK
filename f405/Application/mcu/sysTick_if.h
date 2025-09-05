/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*				  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : uart.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : private header file of uart.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _UART_H_
#define _UART_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/

//#include "main.h"
#include "stm32f4xx_hal.h"
/**** Definition of constants ****/
extern void delay_us(__IO uint32_t nTime);
extern void delay_ms(uint16_t ms);

/**** Definition of types ****/ 

/**** Definition of macros ****/



/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/


#ifdef __cplusplus
}
#endif

#endif /*_UART_H_*/

/****************************** END OF FILE ***********************************/

