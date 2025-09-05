/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : i2c.c
* Author      : Lison
* Version     : v0.01
* Date        : 2020/6/1
* Description : i2c peripherals config.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2020/6/1  Lison    initial version
*===============================================================*/
/* System Includes -----------------------------------------------------------*/

/* Private Includes ----------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "i2c_if.h"
#include "sysTick_if.h"

/*
    应答：是一个低电平信号。
    非应答：是一个高电平信号，也许，叫做应答非更合适。

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

/* Private function prototypes -----------------------------------------------*/
static void IIC_Start(void);
static void IIC_Stop(void);
static uint8_t IIC_Wait_Ack(void);
static void IIC_Ack(void);
static void IIC_NAck(void);
static void IIC_Send_Byte(uint8_t txd);
static uint8_t IIC_Read_Byte(IIC_SEND_NACK_ACK ack);



//I2C 初始化函数, PB10(SCL), PB11(SDA)
void I2c_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOB_CLK_ENABLE();   //使能GPIOB时钟

    //(SCL), (SDA)初始化设置
    GPIO_Initure.Pin = I2C_SCL_PIN | I2C_SDA_PIN;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(I2C_SCL_PORT, &GPIO_Initure);

    IIC_SDA(GPIO_PIN_SET);
    IIC_SCL(GPIO_PIN_SET);
}


//读取一个字节
uint8_t I2c_read_byte(uint8_t Addr, uint8_t *Data)
{
    /* 1.开始 */
    IIC_Start();

    /* 2.设备地址/写 */
    IIC_Send_Byte(I2C_ADDRESS | I2C_WRITE_CMD);
    if(!IIC_Wait_Ack())//读取应答位
    {
        return DISABLE;
    }
    delay_us(1);

    /* 3.数据地址 */
    IIC_Send_Byte(Addr);         //数据地址(8位)
    if(!IIC_Wait_Ack())//读取应答位
    {
        return DISABLE;
    }
    delay_us(1);

//    IIC_Stop();//产生一个停止条件
//    delay_us(10);


    /* 4.重新开始 */
    IIC_Start();

    /* 5.设备地址/读 */
    IIC_Send_Byte(I2C_ADDRESS | I2C_READ_CMD);
    if(!IIC_Wait_Ack())//读取应答位
    {
        return DISABLE;
    }
    delay_us(1);


    /* 6.读一字节数据 */
    *Data  = IIC_Read_Byte(IIC_SEND_NACK); //主机不应答(最后一个字节, 产生非应答)
    delay_us(1);

    /* 7.停止 */
    IIC_Stop();

    return ENABLE;
    
}


//读取多个字节数据
uint8_t I2c_read_multi_byte(uint8_t Addr, uint8_t *Data, uint8_t len)
{
    uint8_t i = 0;
    
    /* 1.开始 */
    IIC_Start();

    /* 2.设备地址/写 */
    IIC_Send_Byte(I2C_ADDRESS | I2C_WRITE_CMD);
    if(!IIC_Wait_Ack())//读取应答位
    {
        return DISABLE;
    }
    delay_us(1);

    /* 3.数据地址 */
    IIC_Send_Byte(Addr);         //数据地址(8位)
    if(!IIC_Wait_Ack())//读取应答位
    {
        return DISABLE;
    }
    delay_us(1);

//    IIC_Stop();//产生一个停止条件
//    delay_us(10);


    /* 4.重新开始 */
    IIC_Start();

    /* 5.设备地址/读 */
    IIC_Send_Byte(I2C_ADDRESS | I2C_READ_CMD);
    if(!IIC_Wait_Ack())//读取应答位
    {
        return DISABLE;
    }
    delay_us(1);

    /* 6.读一个或多个字节数据 */
    for(i = 0; i < len; i++)
    {
        if(i != (len - 1))
        {
            Data[i] = IIC_Read_Byte(IIC_SEND_ACK); //主机应答
            delay_us(1);
        }
        else
        {
            Data[i] = IIC_Read_Byte(IIC_SEND_NACK); //主机不应答(最后一个字节, 产生非应答)
            delay_us(1);
        }
    }

    /* 7.停止 */
    IIC_Stop();

    return ENABLE;
    
}


//写入一个字节
uint8_t I2c_write_byte(uint8_t Addr, uint8_t Data)
{
    /* 1.开始 */
    IIC_Start();

    /* 2.设备地址/写 */
    IIC_Send_Byte(I2C_ADDRESS | I2C_WRITE_CMD);
    if(!IIC_Wait_Ack())//读取应答位
    {
        return DISABLE;
    }
    delay_us(1);

    /* 3.数据地址 */
    IIC_Send_Byte(Addr);         //数据地址(8位)
    if(!IIC_Wait_Ack())//读取应答位
    {
        return DISABLE;
    }
    delay_us(1);

    /* 4.写一字节数据 */
    IIC_Send_Byte(Data);
    if(!IIC_Wait_Ack())//读取应答位
    {
        return DISABLE;
    }
    delay_us(1);

    /* 5.停止 */
    IIC_Stop();

    return ENABLE;
    
}


//写入多个字节数据
uint8_t I2c_write_multi_byte(uint8_t Addr, uint8_t *Data, uint8_t len)
{
    uint8_t i = 0;

    /* 1.开始 */
    IIC_Start();

    /* 2.设备地址/写 */
    IIC_Send_Byte(I2C_ADDRESS | I2C_WRITE_CMD);
    if(!IIC_Wait_Ack())//读取应答位
    {
        return DISABLE;
    }
    delay_us(1);

    /* 3.数据地址 */
    IIC_Send_Byte(Addr);         //数据地址(8位)
    if(!IIC_Wait_Ack())//读取应答位
    {
        return DISABLE;
    }
    delay_us(1);

    /* 4.写一个或多个字节数据  */
    for(i = 0; i < len; i++)
    {
        IIC_Send_Byte(Data[i]);
        if(!IIC_Wait_Ack())//读取应答位
        {
            return DISABLE;
        }
        delay_us(1);
    }

    /* 5.停止 */
    IIC_Stop();

    return ENABLE;
    
}


//产生IIC起始信号
static void IIC_Start(void)
{
    SDA_OUT();     //sda线输出
    IIC_SDA(GPIO_PIN_SET);
    IIC_SCL(GPIO_PIN_SET);
    delay_us(1);
    IIC_SDA(GPIO_PIN_RESET);//START:when CLK is high,DATA change form high to low 
    delay_us(1);
    IIC_SCL(GPIO_PIN_RESET);//钳住I2C总线，准备发送或接收数据 
}

//产生IIC停止信号
static void IIC_Stop(void)
{
    SDA_OUT();//sda线输出
    IIC_SCL(GPIO_PIN_RESET);
    IIC_SDA(GPIO_PIN_RESET);//STOP:when CLK is high DATA change form low to high
    delay_us(1);
    IIC_SCL(GPIO_PIN_SET); 
    IIC_SDA(GPIO_PIN_SET);//发送I2C总线结束信号
    delay_us(1);
}

//主机等待应答信号到来, 返回值: 0 接收应答失败, 1 接收应答成功
static uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    
    SDA_IN();      //SDA设置为输入  
    
    IIC_SDA(GPIO_PIN_SET);//从机如果应答, 会拉低该SDA 的电平
    delay_us(1);
    
    IIC_SCL(GPIO_PIN_SET);
    delay_us(1);

    while(READ_SDA())
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
            IIC_Stop();
            return 0;
        }
    }
    
    IIC_SCL(GPIO_PIN_RESET);
    return 1;  
}

//主机产生ACK应答(在SCK 高电平区间, 拉低SDA 电平)
static void IIC_Ack(void)
{
    IIC_SCL(GPIO_PIN_RESET);
    SDA_OUT();
    
    IIC_SDA(GPIO_PIN_RESET);//置低
    delay_us(1);

    //SCK 高电平区间
    IIC_SCL(GPIO_PIN_SET);
    delay_us(1);
    IIC_SCL(GPIO_PIN_RESET);
}

//主机不产生ACK应答(在SCK 高电平区间, 拉高SDA 电平)
static void IIC_NAck(void)
{
    IIC_SCL(GPIO_PIN_RESET);
    SDA_OUT();
    
    IIC_SDA(GPIO_PIN_SET);//置高
    delay_us(1);

    //SCK 高电平区间
    IIC_SCL(GPIO_PIN_SET);
    delay_us(1);
    IIC_SCL(GPIO_PIN_RESET);
}

//IIC发送一个字节, 返回从机有无应答, 1 有应答, 0 无应答			  
static void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
    SDA_OUT(); 	    
    IIC_SCL(GPIO_PIN_RESET);//拉低时钟开始数据传输

    for(t = 0; t < 8; t++)
    {

        if(txd & 0x80)
        {
            IIC_SDA(GPIO_PIN_SET);
        }
        else
        {
            IIC_SDA(GPIO_PIN_RESET);
        }
        
        txd <<= 1; 	  
        delay_us(1);
        IIC_SCL(GPIO_PIN_SET);
        delay_us(1); 
        IIC_SCL(GPIO_PIN_RESET);	
        delay_us(1);
    }	 
}


//读1个字节, ack = 1时, 发送ACK; ack = 0时, 不应答, 发送nACK   
static uint8_t IIC_Read_Byte(IIC_SEND_NACK_ACK ack)
{
    uint8_t i = 0;
    uint8_t receive = 0;
    
    SDA_IN();//SDA设置为输入

    for(i = 0; i < 8; i++)
    {
        IIC_SCL(GPIO_PIN_RESET); 
        delay_us(1);
        IIC_SCL(GPIO_PIN_SET);
        receive <<= 1;
        
        if(READ_SDA())
        {
            receive++;   
        }
        delay_us(1); 
    }	
    
    if(ack == IIC_SEND_NACK)
    {
        IIC_NAck();//发送nACK
    }
    else
    {
        IIC_Ack(); //发送ACK   
    }
        
    return receive;
}


