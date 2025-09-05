/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : fram.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/7/26
* Description : private header file of fram.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/7/26  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _FRAM_H_
#define _FRAM_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include <stdint.h>
     
/**** Definition of constants ****/


/**** Definition of types ****/ 
typedef struct
{
    uint8_t section;
    uint8_t fram_id_start;
    uint8_t fram_id_end;
}FRAM_DATA_INIT_TYPE;

/**** Definition of macros ****/
#define FRAM_SPI_SLAVE_IDX    0
#define FRAM_MUTEX_TIMEOUT    5    /* wait 5ms */

/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
static uint16_t fram_get_address (uint8_t id, uint16_t item);
static void fram_factory_reset_init(uint16_t items);

#ifdef __cplusplus
}
#endif

#endif /*_FRAM_H_*/

/****************************** END OF FILE ***********************************/
