/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : bat.c
* Author      : wb
* Version     : v0.01
* Date        : 2018/3/7
* Description : battery manager.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2018/3/7  wb    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/


/* Public Includes -----------------------------------------------------------*/
#include "..\..\mcu\mcu_if.h"
#include "..\..\system\timer_if.h"

/* Private Includes ----------------------------------------------------------*/
#include "bat.h"
#include "bat_if.h"


/* Global variables ----------------------------------------------------------*/
BAT_INFO bat_info;

/* Private variables ---------------------------------------------------------*/


/* Global functions ----------------------------------------------------------*/
/* PEC X8+X2+X1+1 */
static const uint8_t crc_table[] = {
    0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15, 0x38, 0x3f, 0x36, 0x31,
    0x24, 0x23, 0x2a, 0x2d, 0x70, 0x77, 0x7e, 0x79, 0x6c, 0x6b, 0x62, 0x65,
    0x48, 0x4f, 0x46, 0x41, 0x54, 0x53, 0x5a, 0x5d, 0xe0, 0xe7, 0xee, 0xe9,
    0xfc, 0xfb, 0xf2, 0xf5, 0xd8, 0xdf, 0xd6, 0xd1, 0xc4, 0xc3, 0xca, 0xcd,
    0x90, 0x97, 0x9e, 0x99, 0x8c, 0x8b, 0x82, 0x85, 0xa8, 0xaf, 0xa6, 0xa1,
    0xb4, 0xb3, 0xba, 0xbd, 0xc7, 0xc0, 0xc9, 0xce, 0xdb, 0xdc, 0xd5, 0xd2,
    0xff, 0xf8, 0xf1, 0xf6, 0xe3, 0xe4, 0xed, 0xea, 0xb7, 0xb0, 0xb9, 0xbe,
    0xab, 0xac, 0xa5, 0xa2, 0x8f, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9d, 0x9a,
    0x27, 0x20, 0x29, 0x2e, 0x3b, 0x3c, 0x35, 0x32, 0x1f, 0x18, 0x11, 0x16,
    0x03, 0x04, 0x0d, 0x0a, 0x57, 0x50, 0x59, 0x5e, 0x4b, 0x4c, 0x45, 0x42,
    0x6f, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7d, 0x7a, 0x89, 0x8e, 0x87, 0x80,
    0x95, 0x92, 0x9b, 0x9c, 0xb1, 0xb6, 0xbf, 0xb8, 0xad, 0xaa, 0xa3, 0xa4,
    0xf9, 0xfe, 0xf7, 0xf0, 0xe5, 0xe2, 0xeb, 0xec, 0xc1, 0xc6, 0xcf, 0xc8,
    0xdd, 0xda, 0xd3, 0xd4, 0x69, 0x6e, 0x67, 0x60, 0x75, 0x72, 0x7b, 0x7c,
    0x51, 0x56, 0x5f, 0x58, 0x4d, 0x4a, 0x43, 0x44, 0x19, 0x1e, 0x17, 0x10,
    0x05, 0x02, 0x0b, 0x0c, 0x21, 0x26, 0x2f, 0x28, 0x3d, 0x3a, 0x33, 0x34,
    0x4e, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5c, 0x5b, 0x76, 0x71, 0x78, 0x7f,
    0x6a, 0x6d, 0x64, 0x63, 0x3e, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2c, 0x2b,
    0x06, 0x01, 0x08, 0x0f, 0x1a, 0x1d, 0x14, 0x13, 0xae, 0xa9, 0xa0, 0xa7,
    0xb2, 0xb5, 0xbc, 0xbb, 0x96, 0x91, 0x98, 0x9f, 0x8a, 0x8d, 0x84, 0x83,
    0xde, 0xd9, 0xd0, 0xd7, 0xc2, 0xc5, 0xcc, 0xcb, 0xe6, 0xe1, 0xe8, 0xef,
    0xfa, 0xfd, 0xf4, 0xf3
};

uint8_t crc8(uint8_t *p, uint8_t len)
{
        uint16_t i;
        uint16_t crc = 0x0;

        while (len--) {
                i = (crc ^ *p++) & 0xFF;
                crc = (crc_table[i] ^ (crc << 8)) & 0xFF;
        }

        return crc & 0xFF;
}

/**
  * @brief  bat init
  * @param  None
  * @retval None
  */
void bat_init(void)
{  
    bat_i2c_create();
    bat_info.status = BAT_CMD_INIT;
    bat_info.status++;
}

void bat_polling(void)
{
    
    switch(bat_info.status)
    {
//        case BAT_CMD_TEMP:
//            bat_read_temp(&bat_info);
//            bat_info.status++;
//            break;
        case BAT_CMD_VOLT:
            bat_read_volt(&bat_info);
            bat_info.status++;
            break;
//        case BAT_CMD_CURRENT:
//            bat_read_current(&bat_info);;
//            bat_info.status++;
//            break;
//        case BAT_CMD_AVR_CURR:
//            bat_read_avr_current(&bat_info);
//            bat_info.status++;
//            break;
//        case BAT_CMD_REMAINING_CAPACITY:
//            bat_read_remaining_capacity(&bat_info);
//            bat_info.status++;
//            break;
//        case BAT_CMD_FULL_CHARGE_CAPACITY:
//            bat_read_full_charge_capacity(&bat_info);
//            bat_info.status++;
//            break;
//        case BAT_CMD_STATUS:
//            bat_read_status(&bat_info);
//            bat_info.status++;
//            break;
//        case BAT_CMD_RELATIVE_PERCENT:
//            bat_read_relative_percent(&bat_info);
//            bat_info.status++;
//            break;
//        case BAT_CMD_ABS_PERCENT:
//            bat_read_abs_percent(&bat_info);
//            bat_info.status++;
//            break;
        case BAT_CMD_DONE:
            bat_info.status = BAT_CMD_INIT;
            bat_info.status++;
            break;
        default:
            break;
            
    }

}

/**
  * @brief  read data with pec
  * @param  None
  * @retval 1-pec normal
  */
unsigned char bat_i2c_read_reg(uint8_t addr, uint8_t reg, uint8_t length, uint8_t* buf)
{
    uint8_t param[20];
    uint8_t data;
    uint8_t len = length;
    
    bat_i2c_start();
    bat_i2c_send_byte((addr<<1) | 0x00);/* addr write */
    param[0] = (addr<<1) | 0x00;
    bat_i2c_send_byte(reg);
    param[1] = reg;

    bat_i2c_start();
    bat_i2c_send_byte((addr<<1) | 0x01);/* addr read */
    param[2] = (addr<<1) | 0x01;
    while(length)
    {
        if(length == 1)
        {
            data = bat_i2c_read_byte(NOACK);
            *buf = data;
            param[len+2] = data;
        }
        else
        {
            data = bat_i2c_read_byte(ACK);
            *buf++ = data;
            param[3+len-length] = data;
        }
        length--;
    }
    bat_i2c_stop();

#ifdef USE_OLD_BAT
    if(param[len+2] == crc8(param, len+2))
    {
        return 1;
    }
    else
    {
        return 0;
    }
#else
    param[0] = param[1];
    return 1;
#endif
}

/**
  * @brief  write data with pec
  * @param  None
  * @retval 1
  */
uint8_t bat_i2c_write_reg(uint8_t addr, uint8_t reg, uint8_t length, uint8_t* buf)
{    
    uint8_t param[20];
    uint8_t data;
    uint8_t i = 0;

    bat_i2c_start();
    bat_i2c_send_byte((addr<<1) | 0x00);
    param[0] = (addr<<1) | 0x00;
    bat_i2c_send_byte(reg);
    param[1] = reg;
    
    while(length--)
    {
        data = *buf++;
        bat_i2c_send_byte(data);
        param[2+i] = data;
        i++;
    }
    bat_i2c_send_byte(crc8(param, i+2));
    bat_i2c_stop();    

    return 1;
}

void bat_read_volt(BAT_INFO *bat)
{
    uint8_t buf[5] = {0};
    uint16_t temp_volt = 0;
    static uint8_t wait_ack_cnt = 0;

#ifdef USE_OLD_BAT
    if(bat_i2c_read_reg(BAT_SMBUS_ADDR, SBS_CMD_VOLT, 3, buf))    
    {
        bat->volt = buf[0]|(buf[1]<<8);
    }
    else
    {
        bat->error++;

        temp_volt = buf[0]|(buf[1]<<8);
        if(temp_volt == 0xFFFF)//IIC没有连接
        {
            wait_ack_cnt++;
            if(wait_ack_cnt > 100)
            {
                wait_ack_cnt = 250;//防止累加溢出
                bat->volt = 0;
            }
        }
        else
        {
            wait_ack_cnt = 0;
        }
    }
#else
    if(bat_i2c_read_reg(BAT_SMBUS_ADDR, 0x08, 3, buf))
    {
        temp_volt = buf[0]|(buf[1]<<8);
        if((temp_volt > 18000) && (temp_volt < 25200))//由于没有校验码, 只能这样判断数据是否正确
        {
            bat->volt = temp_volt;
        }
        else
        {
            bat->error++;

            if(temp_volt == 0xFFFF)//IIC没有连接
            {
                wait_ack_cnt++;
                if(wait_ack_cnt > 100)
                {
                    wait_ack_cnt = 250;//防止累加溢出
                    bat->volt = 0;
                }
            }
            else
            {
                wait_ack_cnt = 0;
            }
        }
    }
    else
    {
 		//TO DO 
	
    }
#endif
}

void bat_read_temp(BAT_INFO *bat)
{
    uint8_t buf[5];
    if(bat_i2c_read_reg(BAT_SMBUS_ADDR, SBS_CMD_TEMP, 3, buf))
    {
        bat->temp = buf[0]|(buf[1]<<8) - 2731;
    }
    else
    {
        bat->error++;
    }
}

void bat_read_current(BAT_INFO *bat)
{    
    uint8_t buf[5];
    if(bat_i2c_read_reg(BAT_SMBUS_ADDR, SBS_CMD_CURRENT, 3, buf))
    {
        bat->curr = buf[0]|(buf[1]<<8);
    }
    else
    {
        bat->error++;
    }
}

void bat_read_avr_current(BAT_INFO *bat)
{
    uint8_t buf[5];
    if(bat_i2c_read_reg(BAT_SMBUS_ADDR, SBS_CMD_AVR_CURR, 3, buf))
    {
        bat->avrcurr = buf[0]|(buf[1]<<8);
    }
    else
    {
        bat->error++;
    }
}

void bat_read_remaining_capacity(BAT_INFO *bat)
{
    uint8_t buf[5];
    if(bat_i2c_read_reg(BAT_SMBUS_ADDR, SBS_CMD_REMAINING_CAPACITY, 3, buf))
    {
        bat->remaining = buf[0]|(buf[1]<<8);
    }
    else
    {
        bat->error++;
    }
}

void bat_read_full_charge_capacity(BAT_INFO *bat)
{
    uint8_t buf[5];
    if(bat_i2c_read_reg(BAT_SMBUS_ADDR, SBS_CMD_FULL_CHARGE_CAPACITY, 3, buf))
    {
        bat->fullcharge= buf[0]|(buf[1]<<8);
    }
    else
    {
        bat->error++;
    }
}

void bat_read_status(BAT_INFO *bat)
{
    uint8_t buf[5];
    if(bat_i2c_read_reg(BAT_SMBUS_ADDR, SBS_CMD_BAT_STATUS, 3, buf))
    {
        bat->bat_status = buf[0]|(buf[1]<<8);
    }
    else
    {
        bat->error++;
    }
}

void bat_read_relative_percent(BAT_INFO *bat)
{
    uint8_t buf[5];
    if(bat_i2c_read_reg(BAT_SMBUS_ADDR, SBS_CMD_RELATIVE_PERCENT, 3, buf))
    {
        bat->relative_percent = buf[0]|(buf[1]<<8);
    }
    else
    {
        bat->error++;
    }
}

void bat_read_abs_percent(BAT_INFO *bat)
{
    uint8_t buf[5];
    if(bat_i2c_read_reg(BAT_SMBUS_ADDR, SBS_CMD_ABS_PERCENT, 3, buf))
    {
        bat->abs_percent = buf[0]|(buf[1]<<8);
    }
    else
    {
        bat->error++;
    }
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  configure i2c pin to general io
  * @param  None
  */
void bat_pin_set_io(void)
{
//    GPIO_InitTypeDef GPIO_InitStruct;
//    /**I2C2 GPIO Configuration    
//    PC1     ------> SMBUS_SDA
//    PC0     ------> SMBUS_SCL 
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/**
  * @brief  delay forsmubs clk 98kHz.
  * @param  None
  * @retval None
  */
void i2c_delay(void)
{
    uint32_t i = 50;
    while(i--)
    {
        __asm__("nop");
    }
}

/**
  * @brief  delay 500ns.
  * @param  None
  * @retval None
  */
void i2c_delay_us(uint16_t us)
{
    uint32_t i = 30*us;
    while(i--)
    {
        __asm__("nop");
    }
}

/**
  * @brief  sends an i2c start condition on the bus,The clock must wait at least 4 μs after
            the data line goes low before it goes low.
  * @param  None
  * @retval None
  */
void bat_i2c_start(void)
{
    BAT_SDA_SET_INPUT();
    BAT_SCL_SET_INPUT();

    i2c_delay();

    BAT_SDA_CLR_VAL();
    BAT_SDA_SET_OUTPUT();

    i2c_delay_us(6);

    BAT_SCL_CLR_VAL();
    BAT_SCL_SET_OUTPUT();
    i2c_delay();
}

/**
  * @brief  sends an i2c stop condition on the bus
  * @param  None
  * @retval None
  */
void bat_i2c_stop(void)
{
    BAT_SCL_CLR_VAL();
    BAT_SCL_SET_OUTPUT();
    i2c_delay();

    BAT_SDA_CLR_VAL();
    BAT_SDA_SET_OUTPUT();
    i2c_delay();

    BAT_SCL_SET_VAL();
    BAT_SCL_SET_INPUT();
    i2c_delay();

    BAT_SDA_SET_VAL();
    BAT_SDA_SET_INPUT();
    i2c_delay();
}

void bat_i2c_start_of_all(void)
{
    bat_i2c_start();
    bat_i2c_start();
    bat_i2c_start();
    bat_i2c_start();
    bat_i2c_start();
    bat_i2c_start();
    bat_i2c_start();
    bat_i2c_start();
    bat_i2c_start();
    bat_i2c_stop();
}

void bat_i2c_create(void)
{
    bat_pin_set_io();
    bat_i2c_start_of_all();
}

void bat_i2c_wait_clk(void)
{
    unsigned long i = 0;

    while ((!BAT_SCL_GET_VAL()) && (++i < 0xffff) );
}

/**
  * @brief  receives one byte of data from an I2C slave device.
  * @param  ack 0-generates nack,1-generates ack.
  * @retval None
  */
unsigned char bat_i2c_read_byte(unsigned char ack)
{
    unsigned char readmask = 0;
    unsigned char count=8;
    BAT_SDA_SET_INPUT();
    while (count--)
    {
        BAT_SCL_SET_OUTPUT();/* low */
        i2c_delay();
        i2c_delay();
        readmask <<= 1;
        BAT_SCL_SET_INPUT();/* high */
        bat_i2c_wait_clk();
        i2c_delay();
        i2c_delay();
        if (BAT_SDA_GET_VAL())
        {
            readmask+=1;
        }
    };

    if(BAT_SDA_GET_VAL())
    {
        BAT_SDA_SET_VAL();  
    }
    else
    {
        BAT_SDA_CLR_VAL();
    }
    BAT_SDA_SET_OUTPUT();

    BAT_SCL_SET_OUTPUT();/* low */
    if (ack)
    {
        BAT_SDA_CLR_VAL();
    }
    else
    {
        BAT_SDA_SET_VAL();
    }
    i2c_delay();
    i2c_delay();
    BAT_SCL_SET_INPUT();/* high */
    bat_i2c_wait_clk();
    i2c_delay();
    i2c_delay();
    BAT_SCL_SET_OUTPUT();/* low */
    i2c_delay();
    i2c_delay();
    return readmask;
}

unsigned char bat_i2c_send_byte(unsigned char dat)
{
    unsigned char sendmask = 0x80;
    while (sendmask)
    {
        if (dat & sendmask)
        {
            BAT_SDA_SET_INPUT();
        }
        else
        {
            BAT_SDA_SET_OUTPUT();
        }
        i2c_delay();
        i2c_delay();
        BAT_SCL_SET_INPUT();/* high */
        bat_i2c_wait_clk();
        i2c_delay();
        i2c_delay();
        sendmask >>= 1;
        BAT_SCL_SET_OUTPUT();/* low */
        i2c_delay();
        i2c_delay();
    };

    BAT_SDA_SET_INPUT();/* high */
    i2c_delay();
    i2c_delay();
    BAT_SCL_SET_INPUT();/* high */
    bat_i2c_wait_clk();
    i2c_delay();
    i2c_delay();
    sendmask = BAT_SDA_GET_VAL();
    BAT_SCL_SET_OUTPUT();/* low */
    i2c_delay();
    i2c_delay();
    return sendmask;
    //return ( bat_i2c_wait_ack() );
}

uint8_t cal_crc(uint8_t *ptr, unsigned int len)
{
    unsigned char i;
    unsigned char crc=0;
    while(len--!=0)         
    {
        for(i=0x80; i!=0; i/=2)  
        {
            if((crc&0x80)!=0)  
            {
                crc*=2; 
                crc^=0x07;
            } 
            else 
            {
                crc*=2;
            }
                        
            if((*ptr&i)!=0) 
            {
                crc^=0x07; 
            }
        }
        ptr++;
    }
       return(crc);
}
uint16_t return_bat_volt(void)
{
    return bat_info.volt;
}
uint16_t return_bat_remaining_capacity(void)
{
    return bat_info.remaining;
}
uint16_t return_bat_fullcharge_capacity(void)
{
    return bat_info.fullcharge;
}
uint8_t return_bat_abs_percent(void)
{
    return bat_info.abs_percent;
}

/****************************** END OF FILE ***********************************/

