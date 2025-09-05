/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : iap.h
* Author      : lison
* Version     : v0.01
* Date        : 2021/2/1
* Description : private header file of iap.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2021/2/1  lison    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IAP_H__
#define __IAP_H__

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include "stm32f4xx_hal.h"
#include "internal_flash.h"

     
/**** Definition of constants ****/


/**** Definition of types ****/ 


/**** Definition of macros ****/

//把内部Flash根据不同的功能，划分为4个分区，如下
#define BootLoader_ADDRESS  ADDR_FLASH_SECTOR_0    //BootLoader分区
#define Parameters_ADDRESS  ADDR_FLASH_SECTOR_3    //Parameters分区
#define Application_ADDRESS   ADDR_FLASH_SECTOR_4    //Application分区
#define Backup_ADDRESS        ADDR_FLASH_SECTOR_8    //Backup分区


//Parameters分区参数的标志
#define PARAM_NORMAL           0xFFFFFFFF  //从boot 跳转到Application
#define PARAM_PRE_UPGRADE 0x11111111  //从Application 跳转到boot, 准备擦除Application
#define PARAM_ERASE_APP      0x22222222  //Application已被擦除, 如果升级异常, boot 要跳转到Backup


/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
void iap_jump(uint32_t jump_addr);


#ifdef __cplusplus
}
#endif

#endif /*_ADC_H_*/

/****************************** END OF FILE ***********************************/
