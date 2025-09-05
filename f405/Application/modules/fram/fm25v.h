/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : fm25v.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/7/26
* Description : private header file of fm25v.c.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/7/26  wb          initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _FM25V_H_
#define _FM25V_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include <stdint.h>
     
/**** Definition of constants ****/

/**** Definition of macros ****/
#define FM25V_WREN        0x06
#define FM25V_WRDI        0x04
#define FM25V_RDSR        0x05
#define FM25V_WRSR        0x01
#define FM25V_READ        0x03
#define FM25V_FSTRD        0x0B
#define FM25V_WRITE        0x02
#define FM25V_SLEEP        0xB9
#define FM25V_RDID        0x9F

#define FM25V_DEV_ID_7    0xC2
#define FM25V_DEV_ID_8    0x21
#define FM25V_DEV_ID_9    0x00

#define FM25V_CNT_PER_US    21    /* 21 counter is 1us */    
#define FM25V_TIMEOUT_US    50U    /* 50us */

/**** Definition of types ****/ 


/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
static void fm25_spi_error_cb(SPI_HandleTypeDef *object);

#ifdef __cplusplus
}
#endif

#endif /*_FM25V_H_*/

/****************************** END OF FILE ***********************************/
