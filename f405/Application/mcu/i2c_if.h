/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : i2c_if.h
* Author      : Lison
* Version     : v0.01
* Date        : 2020/6/1
* Description : extern header file of i2c.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2020/6/1  Lison    initial version
*=============================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef _I2C_IF_H_
#define _I2C_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/

/**** Definition of constants ****/


/**** Definition of types ****/ 


/**** Definition of macros ****/


/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
void I2c_Init(void);
uint8_t I2c_read_byte(uint8_t Addr, uint8_t *Data);
uint8_t I2c_read_multi_byte(uint8_t Addr, uint8_t *Data, uint8_t len);
uint8_t I2c_write_byte(uint8_t Addr, uint8_t Data);
uint8_t I2c_write_multi_byte(uint8_t Addr, uint8_t *Data, uint8_t len);


#ifdef __cplusplus
}
#endif

#endif /*_I2C_IF_H_*/

/****************************** END OF FILE ***********************************/


