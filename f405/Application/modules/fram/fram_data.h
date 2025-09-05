/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : fram_data.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/7/26
* Description : fram_data.h.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/7/26  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _FRAM_DATA_H_
#define _FRAM_DATA_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include <stdint.h>
     
/**** Definition of constants ****/
/**** Definition of constants ****/
#define SW_VERSION_LONGTH     16

/**** Definition of types ****/ 


/**** Definition of macros ****/


/**** Declaration of constants ****/


/**** Declaration of variables ****/
extern const uint8_t cl_software_version_array[];
extern const uint8_t* const cl_data_array[];
extern const uint8_t cl_mb_serial_number_array[];
extern const uint8_t cl_mb_hw_version_array[];

/**** Declaration of functions ****/

#ifdef __cplusplus
}
#endif

#endif /* _FRAM_DATA_H_ */

/****************************** END OF FILE ***********************************/
