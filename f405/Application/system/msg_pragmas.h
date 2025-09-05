/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : msg_pragmas.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : output pragmas information.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MSG_PRAGMAS_H_
#define _MSG_PRAGMAS_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/

     
/**** Definition of constants ****/


/**** Definition of types ****/ 


/**** Definition of macros ****/
/* some helper macros */
#define DO_PRAGMA(x) _Pragma (#x)
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)


/* some convenience macros to print debug/config messages at compile time */
#define MESSAGE(x) DO_PRAGMA(message (x))
#define TODO(x) DO_PRAGMA(message ("TODO - " x))
#define INFO(x) DO_PRAGMA(message ("Info: " x))
#define INFO_VALUE(x,v) DO_PRAGMA(message ("Info: " x VALUE(v)))
#define INFO_VAR(var) DO_PRAGMA(message ("INFO: " #var " = " VALUE(var)))

/* only if PRINT_CONFIG is true */
#if PRINT_CONFIG
#define PRINT_CONFIG_MSG(x) DO_PRAGMA(message ("Config: " x))
#define PRINT_CONFIG_MSG_VALUE(x,v) DO_PRAGMA(message ("Config: " x VALUE(v)))
#define PRINT_CONFIG_VAR(var) DO_PRAGMA(message ("Config: " #var " = " VALUE(var)))
#else
#define PRINT_CONFIG_MSG(x)
#define PRINT_CONFIG_MSG_VALUE(x,v)
#define PRINT_CONFIG_VAR(var)
#endif
/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/


#ifdef __cplusplus
}
#endif

#endif /*_MSG_PRAGMAS_H_*/

/****************************** END OF FILE ***********************************/
