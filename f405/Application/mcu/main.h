/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : main.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : private header file of main.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MAIN_H_
#define _MAIN_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
     
/**** Definition of constants ****/


/**** Definition of types ****/ 
typedef enum
{
    FLAG_FALSE = 0,
    FLAG_TRUE = 1,
}FLAG_STATUS;



/**** Definition of macros ****/

//硬件V1.1 版本和V1.0 版本对比, 更换了蓝牙模块、增加eeprom 存储
//注释下面的宏定义, 表示硬件版本是V1.1
//#define IS_HARDWARE_VER_1_0         // V1.0 版本


/*
    软件的版本号说明
    1、"多够"的电机，版本号为：V1.3.XX
    2、"T-Motor"的电机，版本号为：V1.5.XX
    2、"Motec"的电机，版本号为：   V1.7.XX
*/
#define SOFTWART_VER    "V1.7.4"






//常用的位操作指令

//指定的bit 置1
#define BIT_SET(value, bit) (value |= (0x01 << bit))

//指定的bit 置0
#define BIT_CLR(value, bit) (value &= ~(0x01 << bit))

//指定的bit 取反
#define BIT_REV(value, bit) (value ^= (0x01 << bit))

//获取指定的bit
#define BIT_GET(value, bit) (((value) >> (bit)) & 0x01)


/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/


#ifdef __cplusplus
}
#endif

#endif /*_MAIN_H_*/

/****************************** END OF FILE ***********************************/
