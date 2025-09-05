/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : i2c.h
* Author      : Lison
* Version     : v0.01
* Date        : 2020/6/1
* Description : private header file of i2c.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2020/6/1  Lison    initial version
*============================================================*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H__
#define __I2C_H__

/* Includes ------------------------------------------------------------------*/
#include "mcu_if.h"
#include <stdint.h>
#include "lsm6ds.h"


/**** Definition of macros ****/

//地址的最低的bit0 是IIC 的读写标志位(IIC 发数据, bit0 = 0; IIC 读数据, bit0 = 1)
#define I2C_ADDRESS        (LSM6D_ACC_GYRO_ADDR)
#define I2C_READ_CMD      (0x01)
#define I2C_WRITE_CMD    (0x00)

// I2C 管脚定义, PB10(SCL), PB11(SDA)
#define I2C_SCL_PORT (GPIOB)
#define I2C_SCL_PIN   (GPIO_PIN_10)
#define I2C_SCL_Pin_NUM       10

#define I2C_SDA_PORT (GPIOB)
#define I2C_SDA_PIN   (GPIO_PIN_11)
#define I2C_SDA_Pin_NUM       11

//(SDA) IO方向设置
/*00-input, 01-output*/
#define SDA_IN()    do \
{ \
    I2C_SDA_PORT->MODER &= (~((0x00000001 << (2*I2C_SDA_Pin_NUM)) | (0x00000001 << (2*I2C_SDA_Pin_NUM+1))));     \
    I2C_SDA_PORT->MODER |= 0x00000000;     \
} while(0)

#define SDA_OUT()    do \
{ \
    I2C_SDA_PORT->MODER &= (~((0x00000001 << (2*I2C_SDA_Pin_NUM)) | (0x00000001 << (2*I2C_SDA_Pin_NUM+1))));     \
    I2C_SDA_PORT->MODER |= (0x00000001 << (2*I2C_SDA_Pin_NUM));     \
} while(0)


//SCL, SDA 的IO操作
#define IIC_SCL(PinState)    HAL_GPIO_WritePin(I2C_SCL_PORT, I2C_SCL_PIN, PinState)
#define IIC_SDA(PinState)   HAL_GPIO_WritePin(I2C_SDA_PORT, I2C_SDA_PIN, PinState)
#define READ_SDA()             HAL_GPIO_ReadPin(I2C_SDA_PORT, I2C_SDA_PIN)

//IIC 主机的应答或非应答
typedef enum
{
    IIC_SEND_NACK = 0,
    IIC_SEND_ACK = 1
} IIC_SEND_NACK_ACK;




/**** Declaration of functions ****/





#endif /* __I2C_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

