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
    Ӧ����һ���͵�ƽ�źš�
    ��Ӧ����һ���ߵ�ƽ�źţ�Ҳ������Ӧ��Ǹ����ʡ�

    IICЭ��: ÿ�δ��䶼Ӧ�õ�Ӧ���źź��ٽ�����һ���ֽڴ���

     ����ÿ����8λ����֮�󣬶�����ACK�źţ�������3������
     1. ���ӻ�������Ӧ�ӻ���ַʱ(��������æ�������¶��޷���Ӧ
    IIC���ߵĲ��������������ַû�ж�Ӧ�Ĵӻ�)��
    �ڵ�9��SCL������SDA��û�����ͣ���û��ACK�źš���ʱ��
    ��������һ��P�ź���ֹ����������·���һ��S�źſ�ʼ�µĴ��䡣

     2. ����ӻ��������ڴ�������в��ܽ��ո��������ʱ��������
    ����ACK�źš������������Ϳ�����ʶ����㣬�Ӷ�����һ��P�ź�
    ��ֹ����������·���һ��S�źſ�ʼ�µĴ��䡣

     3. �����������ڽ��յ����һ���ֽں�Ҳ���ᷢ��ACK�źš�
    ���ǣ��ӻ��������ͷ�SDA�ߣ���������������P�źŽ������䡣
*/

/* Private function prototypes -----------------------------------------------*/
static void IIC_Start(void);
static void IIC_Stop(void);
static uint8_t IIC_Wait_Ack(void);
static void IIC_Ack(void);
static void IIC_NAck(void);
static void IIC_Send_Byte(uint8_t txd);
static uint8_t IIC_Read_Byte(IIC_SEND_NACK_ACK ack);



//I2C ��ʼ������, PB10(SCL), PB11(SDA)
void I2c_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOB_CLK_ENABLE();   //ʹ��GPIOBʱ��

    //(SCL), (SDA)��ʼ������
    GPIO_Initure.Pin = I2C_SCL_PIN | I2C_SDA_PIN;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(I2C_SCL_PORT, &GPIO_Initure);

    IIC_SDA(GPIO_PIN_SET);
    IIC_SCL(GPIO_PIN_SET);
}


//��ȡһ���ֽ�
uint8_t I2c_read_byte(uint8_t Addr, uint8_t *Data)
{
    /* 1.��ʼ */
    IIC_Start();

    /* 2.�豸��ַ/д */
    IIC_Send_Byte(I2C_ADDRESS | I2C_WRITE_CMD);
    if(!IIC_Wait_Ack())//��ȡӦ��λ
    {
        return DISABLE;
    }
    delay_us(1);

    /* 3.���ݵ�ַ */
    IIC_Send_Byte(Addr);         //���ݵ�ַ(8λ)
    if(!IIC_Wait_Ack())//��ȡӦ��λ
    {
        return DISABLE;
    }
    delay_us(1);

//    IIC_Stop();//����һ��ֹͣ����
//    delay_us(10);


    /* 4.���¿�ʼ */
    IIC_Start();

    /* 5.�豸��ַ/�� */
    IIC_Send_Byte(I2C_ADDRESS | I2C_READ_CMD);
    if(!IIC_Wait_Ack())//��ȡӦ��λ
    {
        return DISABLE;
    }
    delay_us(1);


    /* 6.��һ�ֽ����� */
    *Data  = IIC_Read_Byte(IIC_SEND_NACK); //������Ӧ��(���һ���ֽ�, ������Ӧ��)
    delay_us(1);

    /* 7.ֹͣ */
    IIC_Stop();

    return ENABLE;
    
}


//��ȡ����ֽ�����
uint8_t I2c_read_multi_byte(uint8_t Addr, uint8_t *Data, uint8_t len)
{
    uint8_t i = 0;
    
    /* 1.��ʼ */
    IIC_Start();

    /* 2.�豸��ַ/д */
    IIC_Send_Byte(I2C_ADDRESS | I2C_WRITE_CMD);
    if(!IIC_Wait_Ack())//��ȡӦ��λ
    {
        return DISABLE;
    }
    delay_us(1);

    /* 3.���ݵ�ַ */
    IIC_Send_Byte(Addr);         //���ݵ�ַ(8λ)
    if(!IIC_Wait_Ack())//��ȡӦ��λ
    {
        return DISABLE;
    }
    delay_us(1);

//    IIC_Stop();//����һ��ֹͣ����
//    delay_us(10);


    /* 4.���¿�ʼ */
    IIC_Start();

    /* 5.�豸��ַ/�� */
    IIC_Send_Byte(I2C_ADDRESS | I2C_READ_CMD);
    if(!IIC_Wait_Ack())//��ȡӦ��λ
    {
        return DISABLE;
    }
    delay_us(1);

    /* 6.��һ�������ֽ����� */
    for(i = 0; i < len; i++)
    {
        if(i != (len - 1))
        {
            Data[i] = IIC_Read_Byte(IIC_SEND_ACK); //����Ӧ��
            delay_us(1);
        }
        else
        {
            Data[i] = IIC_Read_Byte(IIC_SEND_NACK); //������Ӧ��(���һ���ֽ�, ������Ӧ��)
            delay_us(1);
        }
    }

    /* 7.ֹͣ */
    IIC_Stop();

    return ENABLE;
    
}


//д��һ���ֽ�
uint8_t I2c_write_byte(uint8_t Addr, uint8_t Data)
{
    /* 1.��ʼ */
    IIC_Start();

    /* 2.�豸��ַ/д */
    IIC_Send_Byte(I2C_ADDRESS | I2C_WRITE_CMD);
    if(!IIC_Wait_Ack())//��ȡӦ��λ
    {
        return DISABLE;
    }
    delay_us(1);

    /* 3.���ݵ�ַ */
    IIC_Send_Byte(Addr);         //���ݵ�ַ(8λ)
    if(!IIC_Wait_Ack())//��ȡӦ��λ
    {
        return DISABLE;
    }
    delay_us(1);

    /* 4.дһ�ֽ����� */
    IIC_Send_Byte(Data);
    if(!IIC_Wait_Ack())//��ȡӦ��λ
    {
        return DISABLE;
    }
    delay_us(1);

    /* 5.ֹͣ */
    IIC_Stop();

    return ENABLE;
    
}


//д�����ֽ�����
uint8_t I2c_write_multi_byte(uint8_t Addr, uint8_t *Data, uint8_t len)
{
    uint8_t i = 0;

    /* 1.��ʼ */
    IIC_Start();

    /* 2.�豸��ַ/д */
    IIC_Send_Byte(I2C_ADDRESS | I2C_WRITE_CMD);
    if(!IIC_Wait_Ack())//��ȡӦ��λ
    {
        return DISABLE;
    }
    delay_us(1);

    /* 3.���ݵ�ַ */
    IIC_Send_Byte(Addr);         //���ݵ�ַ(8λ)
    if(!IIC_Wait_Ack())//��ȡӦ��λ
    {
        return DISABLE;
    }
    delay_us(1);

    /* 4.дһ�������ֽ�����  */
    for(i = 0; i < len; i++)
    {
        IIC_Send_Byte(Data[i]);
        if(!IIC_Wait_Ack())//��ȡӦ��λ
        {
            return DISABLE;
        }
        delay_us(1);
    }

    /* 5.ֹͣ */
    IIC_Stop();

    return ENABLE;
    
}


//����IIC��ʼ�ź�
static void IIC_Start(void)
{
    SDA_OUT();     //sda�����
    IIC_SDA(GPIO_PIN_SET);
    IIC_SCL(GPIO_PIN_SET);
    delay_us(1);
    IIC_SDA(GPIO_PIN_RESET);//START:when CLK is high,DATA change form high to low 
    delay_us(1);
    IIC_SCL(GPIO_PIN_RESET);//ǯסI2C���ߣ�׼�����ͻ�������� 
}

//����IICֹͣ�ź�
static void IIC_Stop(void)
{
    SDA_OUT();//sda�����
    IIC_SCL(GPIO_PIN_RESET);
    IIC_SDA(GPIO_PIN_RESET);//STOP:when CLK is high DATA change form low to high
    delay_us(1);
    IIC_SCL(GPIO_PIN_SET); 
    IIC_SDA(GPIO_PIN_SET);//����I2C���߽����ź�
    delay_us(1);
}

//�����ȴ�Ӧ���źŵ���, ����ֵ: 0 ����Ӧ��ʧ��, 1 ����Ӧ��ɹ�
static uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    
    SDA_IN();      //SDA����Ϊ����  
    
    IIC_SDA(GPIO_PIN_SET);//�ӻ����Ӧ��, �����͸�SDA �ĵ�ƽ
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

//��������ACKӦ��(��SCK �ߵ�ƽ����, ����SDA ��ƽ)
static void IIC_Ack(void)
{
    IIC_SCL(GPIO_PIN_RESET);
    SDA_OUT();
    
    IIC_SDA(GPIO_PIN_RESET);//�õ�
    delay_us(1);

    //SCK �ߵ�ƽ����
    IIC_SCL(GPIO_PIN_SET);
    delay_us(1);
    IIC_SCL(GPIO_PIN_RESET);
}

//����������ACKӦ��(��SCK �ߵ�ƽ����, ����SDA ��ƽ)
static void IIC_NAck(void)
{
    IIC_SCL(GPIO_PIN_RESET);
    SDA_OUT();
    
    IIC_SDA(GPIO_PIN_SET);//�ø�
    delay_us(1);

    //SCK �ߵ�ƽ����
    IIC_SCL(GPIO_PIN_SET);
    delay_us(1);
    IIC_SCL(GPIO_PIN_RESET);
}

//IIC����һ���ֽ�, ���شӻ�����Ӧ��, 1 ��Ӧ��, 0 ��Ӧ��			  
static void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
    SDA_OUT(); 	    
    IIC_SCL(GPIO_PIN_RESET);//����ʱ�ӿ�ʼ���ݴ���

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


//��1���ֽ�, ack = 1ʱ, ����ACK; ack = 0ʱ, ��Ӧ��, ����nACK   
static uint8_t IIC_Read_Byte(IIC_SEND_NACK_ACK ack)
{
    uint8_t i = 0;
    uint8_t receive = 0;
    
    SDA_IN();//SDA����Ϊ����

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
        IIC_NAck();//����nACK
    }
    else
    {
        IIC_Ack(); //����ACK   
    }
        
    return receive;
}


