/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : bat.h
* Author      : wb
* Version     : v0.01
* Date        : 2018/3/7
* Description : private header file of smbus.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2018/3/7  wb    initial version
*=============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _BAT_H_
#define _BAT_H_

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include <stdint.h>
#include "..\..\mcu\mcu_if.h"
#include "bat_if.h"
     
/**** Definition of constants ****/


/**** Definition of types ****/ 


/**** Definition of macros ****/
#define I2C_NO_STOPC             0x10
#define I2C_NO_STARTC            0x20
#define I2C_WAIT_BETWEEN_BYTES  0x40
#define I2C_ACKNOWLEDGE         0x80
#define I2C_WAIT_CYCLES         10

#define ACK             1
#define NOACK           0

   
/* PD6    ------> I2C1_SCL
   PD7    ------> I2C1_SDA  */
#define BAT_SCL_Pin_NUM       6
#define BAT_SCL_Pin               GPIO_PIN_6
#define BAT_SCL_GPIO_Port   GPIOD

#define BAT_SDA_Pin_NUM       7
#define BAT_SDA_Pin              GPIO_PIN_7
#define BAT_SDA_GPIO_Port   GPIOD


/*00-input, 01-output*/
#define BAT_SDA_SET_INPUT()    \
{ \
    BAT_SDA_GPIO_Port->MODER &= (~((0x00000001 << (2*BAT_SDA_Pin_NUM)) | (0x00000001 << (2*BAT_SDA_Pin_NUM+1))));     \
    BAT_SDA_GPIO_Port->MODER |= 0x00000000;     \
}
#define BAT_SCL_SET_INPUT()    \
{ \
    BAT_SCL_GPIO_Port->MODER &= (~((0x00000001 << (2*BAT_SCL_Pin_NUM)) | (0x00000001 << (2*BAT_SCL_Pin_NUM+1))));     \
    BAT_SCL_GPIO_Port->MODER |= 0x00000000;     \
}

/*00-input, 01-output*/
#define BAT_SDA_SET_OUTPUT()    \
{ \
    BAT_SDA_GPIO_Port->MODER &= (~((0x00000001 << (2*BAT_SDA_Pin_NUM)) | (0x00000001 << (2*BAT_SDA_Pin_NUM+1))));     \
    BAT_SDA_GPIO_Port->MODER |= (0x00000001 << (2*BAT_SDA_Pin_NUM));     \
}
#define BAT_SCL_SET_OUTPUT()    \
{ \
    BAT_SCL_GPIO_Port->MODER &= (~((0x00000001 << (2*BAT_SCL_Pin_NUM)) | (0x00000001 << (2*BAT_SCL_Pin_NUM+1))));     \
    BAT_SCL_GPIO_Port->MODER |= (0x00000001 << (2*BAT_SCL_Pin_NUM));     \
}

#define BAT_SDA_SET_VAL()            HAL_GPIO_WritePin(BAT_SDA_GPIO_Port, BAT_SDA_Pin, GPIO_PIN_SET)
#define BAT_SDA_CLR_VAL()            HAL_GPIO_WritePin(BAT_SDA_GPIO_Port, BAT_SDA_Pin, GPIO_PIN_RESET)

#define BAT_SCL_SET_VAL()            HAL_GPIO_WritePin(BAT_SCL_GPIO_Port, BAT_SCL_Pin, GPIO_PIN_SET)
#define BAT_SCL_CLR_VAL()            HAL_GPIO_WritePin(BAT_SCL_GPIO_Port, BAT_SCL_Pin, GPIO_PIN_RESET)


#define BAT_SDA_GET_VAL()            HAL_GPIO_ReadPin(BAT_SDA_GPIO_Port, BAT_SDA_Pin)
#define BAT_SCL_GET_VAL()            HAL_GPIO_ReadPin(BAT_SCL_GPIO_Port, BAT_SCL_Pin)
/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
void i2c_delay(void);
void i2c_delay_us(uint16_t us);
void bat_pin_set_io(void);
void bat_i2c_start(void);
void bat_i2c_stop(void);
void bat_i2c_start_of_all(void);
void bat_i2c_create(void);
void bat_i2c_wait_clk(void);

unsigned char bat_i2c_read_byte(unsigned char ack);
unsigned char bat_i2c_send_byte(unsigned char dat);

void bat_read_volt(BAT_INFO *bat);
void bat_read_temp(BAT_INFO *bat);
void bat_read_current(BAT_INFO *bat);
void bat_read_avr_current(BAT_INFO *bat);
void bat_read_remaining_capacity(BAT_INFO *bat);
void bat_read_full_charge_capacity(BAT_INFO *bat);
void bat_read_status(BAT_INFO *bat);
void bat_read_relative_percent(BAT_INFO *bat);
void bat_read_abs_percent(BAT_INFO *bat);
void bat_read_remaining_capacity_alarm(BAT_INFO *bat);
void bat_write_remaining_capacity_alarm(BAT_INFO *bat);

uint8_t cal_crc(uint8_t *ptr, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif /*_BAT_H_*/

/****************************** END OF FILE ***********************************/
