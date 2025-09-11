/*
*********************************************************************************************************
*
*	模块名称 : AD7606 驱动模块(8通道16位ADC)
*	文件名称 : bsp_spi_ad7606.h
*
*	Copyright (C), 2015-2016, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __ad7606_H__
#define __ad7606_H__

#include "./SYSTEM/sys/sys.h"

/* ------------------------------------------------------------------------- */
/* CSN片选 */
#define RCC_CS_1 		do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)
#define PORT_CS_1		GPIOB
#define PIN_CS_1		GPIO_PIN_8

/* RESET */
#define RCC_RESET_1 	do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
#define PORT_RESET_1	GPIOC
#define PIN_RESET_1		GPIO_PIN_0

/* RANGE */
#define RCC_RANGE_1 	do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
#define PORT_RANGE_1 	GPIOC
#define PIN_RANGE_1 	GPIO_PIN_5

/* CONVST */
#define RCC_CONVST_1 	do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
#define PORT_CONVST_1	GPIOC
#define PIN_CONVST_1 	(GPIO_PIN_2|GPIO_PIN_3)

/* BUSY */
#define RCC_BUSY_1 		do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)
#define PORT_BUSY_1 	GPIOB
#define PIN_BUSY_1 		GPIO_PIN_0

/*Pin Os*/
#define RCC_OS_10 		do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
#define PORT_OS_10		GPIOC
#define PIN_OS_10 		GPIO_PIN_6

/*Pin Os*/
#define RCC_OS_11 		do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
#define PORT_OS_11		GPIOC
#define PIN_OS_11 		GPIO_PIN_7

/*Pin Os*/
#define RCC_OS_12 		do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
#define PORT_OS_12		GPIOC
#define PIN_OS_12 		GPIO_PIN_4

/* SCK */
#define RCC_SCK_1 		do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
#define PORT_SCK_1		GPIOC
#define PIN_SCK_1		GPIO_PIN_1

/* MOSI */
#define RCC_MOSI_1 		do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)
#define PORT_MOSI_1		GPIOD
#define PIN_MOSI_1		GPIO_PIN_0

/* MISO */
#define RCC_MISO_1 		do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)
#define PORT_MISO_1		GPIOB
#define PIN_MISO_1		GPIO_PIN_9

/* ------------------------------------------------------------------------- */

/* 片选 */
#define AD_CS_0_1()				HAL_GPIO_WritePin(PORT_CS_1, PIN_CS_1, GPIO_PIN_RESET)
#define AD_CS_1_1()				HAL_GPIO_WritePin(PORT_CS_1, PIN_CS_1, GPIO_PIN_SET)	

/* 设置量程 */
#define AD_RANGE_5V_1()			HAL_GPIO_WritePin(PORT_RANGE_1, PIN_RANGE_1, GPIO_PIN_RESET)	//	GPIO_ResetBits(PORT_RANGE_1, PIN_RANGE_1);printf("ADC1:5v\r\n");
#define AD_RANGE_10V_1()		HAL_GPIO_WritePin(PORT_RANGE_1, PIN_RANGE_1, GPIO_PIN_SET)		//	GPIO_SetBits(PORT_RANGE_1, PIN_RANGE_1);printf("ADC1:10v\r\n");	//置1

/* 复位引脚 */
#define AD_RESET_LOW_1()		HAL_GPIO_WritePin(PORT_RESET_1, PIN_RESET_1, GPIO_PIN_RESET)	//		GPIO_ResetBits(PORT_RESET_1, PIN_RESET_1)
#define AD_RESET_HIGH_1()		HAL_GPIO_WritePin(PORT_RESET_1, PIN_RESET_1, GPIO_PIN_SET)		//	GPIO_SetBits(PORT_RESET_1, PIN_RESET_1)

#define RESET_0_1()	            HAL_GPIO_WritePin(PORT_RESET_1, PIN_RESET_1, GPIO_PIN_RESET)    //   GPIO_ResetBits(PORT_RESET_1, PIN_RESET_1)
#define RESET_1_1()	            HAL_GPIO_WritePin(PORT_RESET_1, PIN_RESET_1, GPIO_PIN_SET)      //  GPIO_SetBits(PORT_RESET_1, PIN_RESET_1)

/* 起始信号 */
#define	AD_CONVST_LOW_1()		HAL_GPIO_WritePin(PORT_CONVST_1, PIN_CONVST_1, GPIO_PIN_RESET)		//	GPIO_ResetBits(PORT_CONVST_1, PIN_CONVST_1)
#define	AD_CONVST_HIGH_1()		HAL_GPIO_WritePin(PORT_CONVST_1, PIN_CONVST_1, GPIO_PIN_SET)	    //	GPIO_SetBits(PORT_CONVST_1, PIN_CONVST_1)

#define SCK_0_1()		        HAL_GPIO_WritePin(PORT_SCK_1, PIN_SCK_1, GPIO_PIN_RESET)           //    GPIO_ResetBits(PORT_SCK_1, PIN_SCK_1)
#define SCK_1_1()		        HAL_GPIO_WritePin(PORT_SCK_1, PIN_SCK_1, GPIO_PIN_SET)            //     GPIO_SetBits(PORT_SCK_1, PIN_SCK_1)

#define MOSI_0_1()	            HAL_GPIO_WritePin(PORT_MISO_1, PIN_MISO_1, GPIO_PIN_RESET)        //     GPIO_ResetBits(PORT_MISO_1, PIN_MISO_1)
#define MOSI_1_1()	            HAL_GPIO_WritePin(PORT_MISO_1, PIN_MISO_1, GPIO_PIN_SET)         //    GPIO_SetBits(PORT_MISO_1, PIN_MISO_1)

#define BUSY_IS_LOW_1()			(HAL_GPIO_ReadPin(PORT_BUSY_1, PIN_BUSY_1) == GPIO_PIN_RESET)	  //    (GPIO_ReadInputDataBit(PORT_BUSY_1, PIN_BUSY_1) == Bit_RESET)

#define MISO_IS_HIGH_1()	    (HAL_GPIO_ReadPin(PORT_MISO_1, PIN_MISO_1) == GPIO_PIN_SET)        //   (GPIO_ReadInputDataBit(PORT_MISO_1, PIN_MISO_1) == Bit_SET)

/* ------------------------------------------------------------------------- */

#define VoltageRange 5

#define CH_NUM			8				/* 采集通道数 */

/*
	【SPI时钟最快是2分频，不支持不分频】
	如果是SPI1，2分频时SCK时钟 = 42M，4分频时SCK时钟 = 21M
	如果是SPI3, 2分频时SCK时钟 = 21M
*/
#define SPI_SPEED_42M		SPI_BaudRatePrescaler_2
#define SPI_SPEED_21M		SPI_BaudRatePrescaler_4
#define SPI_SPEED_5_2M		SPI_BaudRatePrescaler_8
#define SPI_SPEED_2_6M		SPI_BaudRatePrescaler_16
#define SPI_SPEED_1_3M		SPI_BaudRatePrescaler_32
#define SPI_SPEED_0_6M		SPI_BaudRatePrescaler_64

/* ------------------------------------------------------------------------- */


void AD7606_Init(void);

uint8_t bsp_spiRead1(void);

#endif
