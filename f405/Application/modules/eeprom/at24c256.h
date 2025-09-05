/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : at24c256.h
* Author      : LisonLiu
* Version     : v0.01
* Date        : 2020/11/1
* Description : 
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2020/11/1  LisonLiu    initial version
*=============================================================================*/

#ifndef __AT24C256_H__
#define __AT24C256_H__


/**** Includes ****/
#include "stdint.h"
#include "main.h"


/**** Definition of constants ****/


/**** Definition of types ****/


/**** Definition of macros ****/

//��ͬ�ͺŵ�EEPROM �����洢����byte, 
//Ŀǰ��·��ʹ�õ���AT24C256, ��32K byte �洢�ֽ�
#define AT24C16_MAX_BYTE         2047
#define AT24C256_MAX_BYTE       32767

//ÿҳ���ٸ��ֽ�
#define AT24C16_PAGE_SIZE        16
#define AT24C256_PAGE_SIZE      64


//��ַ����͵�bit0 ��IIC �Ķ�д��־λ(IIC ������, bit0 = 0; IIC ������, bit0 = 1)
#define AT24CXX_ADDRESS        (0xA0)
#define AT24CXX_READ_CMD      (0x01)
#define AT24CXX_WRITE_CMD    (0x00)

// I2C �ܽŶ���, PC10(SCL), PC11(SDA)
#define AT24CXX_SCL_PORT (GPIOC)
#define AT24CXX_SCL_PIN   (GPIO_PIN_10)
#define AT24CXX_SCL_Pin_NUM       10

#define AT24CXX_SDA_PORT (GPIOC)
#define AT24CXX_SDA_PIN   (GPIO_PIN_11)
#define AT24CXX_SDA_Pin_NUM       11

//(SDA) IO��������
/*00-input, 01-output*/
#define AT24CXX_SDA_IN()    do \
{ \
    AT24CXX_SDA_PORT->MODER &= (~((0x00000001 << (2*AT24CXX_SDA_Pin_NUM)) | (0x00000001 << (2*AT24CXX_SDA_Pin_NUM+1))));     \
    AT24CXX_SDA_PORT->MODER |= 0x00000000;     \
} while(0)

#define AT24CXX_SDA_OUT()    do \
{ \
    AT24CXX_SDA_PORT->MODER &= (~((0x00000001 << (2*AT24CXX_SDA_Pin_NUM)) | (0x00000001 << (2*AT24CXX_SDA_Pin_NUM+1))));     \
    AT24CXX_SDA_PORT->MODER |= (0x00000001 << (2*AT24CXX_SDA_Pin_NUM));     \
} while(0)


//SCL, SDA ��IO����
#define AT24CXX_SCL(PinState)    HAL_GPIO_WritePin(AT24CXX_SCL_PORT, AT24CXX_SCL_PIN, PinState)
#define AT24CXX_SDA(PinState)   HAL_GPIO_WritePin(AT24CXX_SDA_PORT, AT24CXX_SDA_PIN, PinState)
#define AT24CXX_READ_SDA()     HAL_GPIO_ReadPin(AT24CXX_SDA_PORT, AT24CXX_SDA_PIN)

//IIC ������Ӧ����Ӧ��
typedef enum
{
    AT24CXX_SEND_NACK = 0,
    AT24CXX_SEND_ACK = 1
} AT24CXX_SEND_NACK_ACK;



/**** Declaration of variables ****/



/**** Declaration of functions ****/
void AT24CXX_Init(void);
void AT24CXX_detect(void);
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr);
void AT24CXX_WriteOneByte(uint16_t WriteAddr, uint8_t DataToWrite);
uint8_t AT24CXX_Read_Bytes(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead);
uint8_t AT24CXX_Write_Bytes(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite);



#endif



