/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : at24c256.c
* Author      : LisonLiu
* Version     : v0.01
* Date        : 2020/11/1
* Description : 
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2020/11/1  LisonLiu    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/

/* Public Includes -----------------------------------------------------------*/
#include "at24c256.h"
#include "sysTick_if.h"
#include "internal_flash.h"

/*
    应答：是一个低电平信号。
    非应答：是一个高电平信号，也许，叫做<应答非> 更合适。

    IIC协议: 每次传输都应得到应答信号后再进行下一个字节传送

     并非每传输8位数据之后，都会有ACK信号，有以下3中例外
     1. 当从机不能响应从机地址时(例如它正忙于其他事而无法响应
    IIC总线的操作，或者这个地址没有对应的从机)，
    在第9个SCL周期内SDA线没有拉低，即没有ACK信号。这时，
    主机发出一个P信号终止传输或者重新发出一个S信号开始新的传输。

     2. 如果从机接收器在传输过程中不能接收更多的数据时，它不会
    发出ACK信号。这样，主机就可以意识到这点，从而发出一个P信号
    终止传输或者重新发出一个S信号开始新的传输。

     3. 主机接收器在接收到最后一个字节后，也不会发出ACK信号。
    于是，从机发送器释放SDA线，以允许主机发出P信号结束传输。
*/


/* Private variables ---------------------------------------------------------*/
uint8_t is_AT24C16_flag = 0;//为1表示是AT24C16, 为0 表示是AT24C256


/* Private function prototypes -----------------------------------------------*/
static void AT24CXX_Start(void);
static void AT24CXX_Stop(void);
static uint8_t AT24CXX_Wait_Ack(void);
static void AT24CXX_Ack(void);
static void AT24CXX_NAck(void);
static void AT24CXX_Send_Byte(uint8_t txd);
static uint8_t AT24CXX_Read_Byte(AT24CXX_SEND_NACK_ACK ack);



/* Private functions ----------------------------------------------------------*/

//初始化IIC 的GPIO 接口
void AT24CXX_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOC_CLK_ENABLE();   //使能GPIOX 时钟

    //(SCL), (SDA)初始化设置
    GPIO_Initure.Pin = AT24CXX_SCL_PIN | AT24CXX_SDA_PIN;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(AT24CXX_SCL_PORT, &GPIO_Initure);

    AT24CXX_SDA(GPIO_PIN_SET);
    AT24CXX_SCL(GPIO_PIN_SET);

}

//检测EEPROM 芯片的型号是AT24C16 或者AT24C256
void AT24CXX_detect(void)
{
    uint8_t read_buf[4] = {0x00, 0x00, 0x00, 0x00};
    uint8_t write_buf[4] = {0x11, 0x22, 0x33, 0x44};

    HAL_Delay(10);
    AT24CXX_Write_Bytes(PARAM_BUF_LEN + 1, write_buf, 4);

    HAL_Delay(10);
    read_buf[0] = AT24CXX_ReadOneByte(PARAM_BUF_LEN + 1);
    read_buf[1] = AT24CXX_ReadOneByte(PARAM_BUF_LEN + 2);
    read_buf[2] = AT24CXX_ReadOneByte(PARAM_BUF_LEN + 3);
    read_buf[3] = AT24CXX_ReadOneByte(PARAM_BUF_LEN + 4);

    if((read_buf[0] == 0x11) && (read_buf[1] == 0x22) && 
        (read_buf[2] == 0x33) && (read_buf[3] == 0x44))
    {
        is_AT24C16_flag = 0;
    }
    else
    {
        is_AT24C16_flag = 1;
    }
}

//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr)
{
    uint8_t temp = 0;

    AT24CXX_Start();

    AT24CXX_Send_Byte(AT24CXX_ADDRESS | AT24CXX_WRITE_CMD);//发送写命令
    AT24CXX_Wait_Ack();

    if(is_AT24C16_flag == 0)
    {
        AT24CXX_Send_Byte(ReadAddr >> 8);//发送高地址
        AT24CXX_Wait_Ack();
    }
    AT24CXX_Send_Byte(ReadAddr >> 0);//发送低地址
    AT24CXX_Wait_Ack();
    
    AT24CXX_Start();
    AT24CXX_Send_Byte(AT24CXX_ADDRESS | AT24CXX_READ_CMD);//进入接收模式
    AT24CXX_Wait_Ack();
    
    temp = AT24CXX_Read_Byte(AT24CXX_SEND_NACK);
    
    AT24CXX_Stop();//产生一个停止条件
    
    return temp;
}

//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(uint16_t WriteAddr, uint8_t DataToWrite)
{
    AT24CXX_Start();

    AT24CXX_Send_Byte(AT24CXX_ADDRESS | AT24CXX_WRITE_CMD);//发送写命令
    AT24CXX_Wait_Ack();

    if(is_AT24C16_flag == 0)
    {
        AT24CXX_Send_Byte(WriteAddr >> 8);//发送高地址
        AT24CXX_Wait_Ack();
    }
    AT24CXX_Send_Byte(WriteAddr >> 0);//发送低地址
    AT24CXX_Wait_Ack();
    
    AT24CXX_Send_Byte(DataToWrite);//发送字节
    AT24CXX_Wait_Ack();
    
    AT24CXX_Stop();//产生一个停止条件

    //芯片的写周期是5 毫秒
    delay_ms(5);
}


//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
uint8_t AT24CXX_Read_Bytes(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead)
{
    uint16_t i = 0;

    //剩余空间不足
    if(is_AT24C16_flag == 0)
    {
        if((ReadAddr + NumToRead) > AT24C256_MAX_BYTE)
        {
            return 0;
        }
    }
    else
    {
        if((ReadAddr + NumToRead) > AT24C16_MAX_BYTE)
        {
            return 0;
        }
    }

    if(NumToRead < 1)
    {
        return 0;
    }

    AT24CXX_Start();

    AT24CXX_Send_Byte(AT24CXX_ADDRESS | AT24CXX_WRITE_CMD);//发送写命令
    AT24CXX_Wait_Ack();

    if(is_AT24C16_flag == 0)
    {
        AT24CXX_Send_Byte(ReadAddr >> 8);//发送高地址
        AT24CXX_Wait_Ack();
    }
    AT24CXX_Send_Byte(ReadAddr >> 0);//发送低地址
    AT24CXX_Wait_Ack();

    AT24CXX_Start();
    AT24CXX_Send_Byte(AT24CXX_ADDRESS | AT24CXX_READ_CMD);//进入接收模式
    AT24CXX_Wait_Ack();

    while(i < (NumToRead - 1))
    {
        *(pBuffer + i) = AT24CXX_Read_Byte(AT24CXX_SEND_ACK);//ACK
        i++;
    }

    //读的最后一个字节, NACK, 之前的是ACK
    *(pBuffer + i) = AT24CXX_Read_Byte(AT24CXX_SEND_NACK);//NACK
    AT24CXX_Stop();//产生一个停止条件

    return 1;

}

//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
uint8_t AT24CXX_Write_Bytes(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite)
{
    uint16_t i = 0;
    uint8_t per_page_size = 0;
    
    //剩余空间不足
    if(is_AT24C16_flag == 0)
    {
        if((WriteAddr + NumToWrite) > AT24C256_MAX_BYTE)
        {
            return 0;
        }
        per_page_size = AT24C256_PAGE_SIZE;
    }
    else
    {
        if((WriteAddr + NumToWrite) > AT24C16_MAX_BYTE)
        {
            return 0;
        }
        per_page_size = AT24C16_PAGE_SIZE;
    }

    if(NumToWrite < 1)
    {
        return 0;
    }

    AT24CXX_Start();

    AT24CXX_Send_Byte(AT24CXX_ADDRESS | AT24CXX_WRITE_CMD);//发送写命令
    AT24CXX_Wait_Ack();

    if(is_AT24C16_flag == 0)
    {
        AT24CXX_Send_Byte(WriteAddr >> 8);//发送高地址
        AT24CXX_Wait_Ack();
    }
    AT24CXX_Send_Byte(WriteAddr >> 0);//发送低地址
    AT24CXX_Wait_Ack();

    while(i < NumToWrite)
    {
        AT24CXX_Send_Byte(*(pBuffer + i));//发送字节
        AT24CXX_Wait_Ack();

        i++;
        WriteAddr++;//写入地址加1 
        if(WriteAddr % per_page_size == 0)//新的一页
        {
            AT24CXX_Stop();//产生一个停止条件

            // AT24CXX 芯片的写周期是5 毫秒
            delay_ms(10);

            //重新开始
            AT24CXX_Start();

            AT24CXX_Send_Byte(AT24CXX_ADDRESS | AT24CXX_WRITE_CMD);//发送写命令
            AT24CXX_Wait_Ack();

            if(is_AT24C16_flag == 0)
            {
                AT24CXX_Send_Byte(WriteAddr >> 8);//发送高地址
                AT24CXX_Wait_Ack();
            }
            AT24CXX_Send_Byte(WriteAddr >> 0);//发送低地址
            AT24CXX_Wait_Ack();
        }
    }

    AT24CXX_Stop();//产生一个停止条件

    //芯片的写周期是5 毫秒
    delay_ms(10);

    return 1;

}





//------------------------模拟I2C 的时序------------------------

//产生IIC起始信号
static void AT24CXX_Start(void)
{
    AT24CXX_SDA_OUT();//sda线输出
    AT24CXX_SDA(GPIO_PIN_SET);
    AT24CXX_SCL(GPIO_PIN_SET);
    delay_us(1);
    AT24CXX_SDA(GPIO_PIN_RESET);//START:when CLK is high,DATA change form high to low
    delay_us(1);
    AT24CXX_SCL(GPIO_PIN_RESET);//钳住I2C总线，准备发送或接收数据 
}

//产生IIC停止信号
static void AT24CXX_Stop(void)
{
    AT24CXX_SDA_OUT();//sda线输出
    AT24CXX_SCL(GPIO_PIN_RESET);
    AT24CXX_SDA(GPIO_PIN_RESET);//STOP:when CLK is high DATA change form low to high
    delay_us(1);
    AT24CXX_SCL(GPIO_PIN_SET); 
    AT24CXX_SDA(GPIO_PIN_SET);//发送I2C总线结束信号
    delay_us(1);
}

//主机等待应答信号到来, 返回值: 0 接收应答失败, 1 接收应答成功
static uint8_t AT24CXX_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    
    AT24CXX_SDA_IN();      //SDA设置为输入
    
    AT24CXX_SDA(GPIO_PIN_SET);//从机如果应答, 会拉低该SDA 的电平
    delay_us(1);
    
    AT24CXX_SCL(GPIO_PIN_SET);
    delay_us(1);

    while(AT24CXX_READ_SDA())
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
            AT24CXX_Stop();
            return 0;
        }
    }
    
    AT24CXX_SCL(GPIO_PIN_RESET);
    return 1;  
}

//主机产生ACK应答(在SCK 高电平区间, 拉低SDA 电平)
static void AT24CXX_Ack(void)
{
    AT24CXX_SCL(GPIO_PIN_RESET);
    AT24CXX_SDA_OUT();
    
    AT24CXX_SDA(GPIO_PIN_RESET);//置低
    delay_us(1);

    //SCK 高电平区间
    AT24CXX_SCL(GPIO_PIN_SET);
    delay_us(1);
    AT24CXX_SCL(GPIO_PIN_RESET);
}

//主机不产生ACK应答(在SCK 高电平区间, 拉高SDA 电平)
static void AT24CXX_NAck(void)
{
    AT24CXX_SCL(GPIO_PIN_RESET);
    AT24CXX_SDA_OUT();
    
    AT24CXX_SDA(GPIO_PIN_SET);//置高
    delay_us(1);

    //SCK 高电平区间
    AT24CXX_SCL(GPIO_PIN_SET);
    delay_us(1);
    AT24CXX_SCL(GPIO_PIN_RESET);
}

//IIC发送一个字节, 返回从机有无应答, 1 有应答, 0 无应答
static void AT24CXX_Send_Byte(uint8_t txd)
{
    uint8_t t;
    AT24CXX_SDA_OUT();
    AT24CXX_SCL(GPIO_PIN_RESET);//拉低时钟开始数据传输

    for(t = 0; t < 8; t++)
    {

        if(txd & 0x80)
        {
            AT24CXX_SDA(GPIO_PIN_SET);
        }
        else
        {
            AT24CXX_SDA(GPIO_PIN_RESET);
        }
        
        txd <<= 1;
        delay_us(1);
        AT24CXX_SCL(GPIO_PIN_SET);
        delay_us(1); 
        AT24CXX_SCL(GPIO_PIN_RESET);
        delay_us(1);
    }
}


//读1个字节, ack = 1时, 发送ACK; ack = 0时, 不应答, 发送nACK
static uint8_t AT24CXX_Read_Byte(AT24CXX_SEND_NACK_ACK ack)
{
    uint8_t i = 0;
    uint8_t receive = 0;
    
    AT24CXX_SDA_IN();//SDA设置为输入

    for(i = 0; i < 8; i++)
    {
        AT24CXX_SCL(GPIO_PIN_RESET);
        delay_us(1);
        AT24CXX_SCL(GPIO_PIN_SET);
        receive <<= 1;
        
        if(AT24CXX_READ_SDA())
        {
            receive++;
        }
        delay_us(1); 
    }	
    
    if(ack == AT24CXX_SEND_NACK)
    {
        AT24CXX_NAck();//发送nACK
    }
    else
    {
        AT24CXX_Ack(); //发送ACK
    }
        
    return receive;
}

//------------------------模拟I2C 的时序------------------------



