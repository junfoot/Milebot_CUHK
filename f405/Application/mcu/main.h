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

//Ӳ��V1.1 �汾��V1.0 �汾�Ա�, ����������ģ�顢����eeprom �洢
//ע������ĺ궨��, ��ʾӲ���汾��V1.1
//#define IS_HARDWARE_VER_1_0         // V1.0 �汾


/*
    ����İ汾��˵��
    1��"�๻"�ĵ�����汾��Ϊ��V1.3.XX
    2��"T-Motor"�ĵ�����汾��Ϊ��V1.5.XX
    2��"Motec"�ĵ�����汾��Ϊ��   V1.7.XX
*/
#define SOFTWART_VER    "V1.7.4"






//���õ�λ����ָ��

//ָ����bit ��1
#define BIT_SET(value, bit) (value |= (0x01 << bit))

//ָ����bit ��0
#define BIT_CLR(value, bit) (value &= ~(0x01 << bit))

//ָ����bit ȡ��
#define BIT_REV(value, bit) (value ^= (0x01 << bit))

//��ȡָ����bit
#define BIT_GET(value, bit) (((value) >> (bit)) & 0x01)


/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/


#ifdef __cplusplus
}
#endif

#endif /*_MAIN_H_*/

/****************************** END OF FILE ***********************************/
