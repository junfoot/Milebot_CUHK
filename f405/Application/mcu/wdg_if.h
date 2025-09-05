/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : wdg_if.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : extern header file of wdg.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _WDG_IF_H_
#define _WDG_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/

     
/**** Definition of constants ****/


/**** Definition of types ****/ 


/**** Definition of macros ****/
#define WDG_FEED_TIME    30     /* 30ms */

/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
extern void WWDG_IRQHandler(void);
extern void wdg_init(void);
extern void wdg_feed(void);

#ifdef __cplusplus
}
#endif

#endif /*_WDG_IF_H_*/

/****************************** END OF FILE ***********************************/
