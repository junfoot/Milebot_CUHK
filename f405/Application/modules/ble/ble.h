/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : ble.h
* Author      : LisonLiu
* Version     : v0.01
* Date        : 2018/6/1
* Description : 
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2018/6/1  LisonLiu    initial version
*=============================================================================*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _BLE_H_ 
#define _BLE_H_ 

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include "stdint.h"  

/**** Definition of constants ****/

/**** Definition of types ****/ 

//蓝牙数据是二进制格式，或者是JSON 格式
typedef enum
{
    FORMAT_uncertain = 0,	//刚刚启动, 不确定数据格式
    FORMAT_binary,			//二进制格式
    DATA_FORMAT_json,			//JSON 格式
} FORMAT_LIST;

/**** Definition of macros ****/

/**** Declaration of constants ****/

/**** Declaration of variables ****/	 

/**** Declaration of functions ****/

#ifdef __cplusplus
}
#endif

#endif /*_BLE_H_*/

