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
    Ӧ����һ���͵�ƽ�źš�
    ��Ӧ����һ���ߵ�ƽ�źţ�Ҳ������<Ӧ���> �����ʡ�

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


/* Private variables ---------------------------------------------------------*/
uint8_t is_AT24C16_flag = 0;//Ϊ1��ʾ��AT24C16, Ϊ0 ��ʾ��AT24C256


/* Private function prototypes -----------------------------------------------*/
static void AT24CXX_Start(void);
static void AT24CXX_Stop(void);
static uint8_t AT24CXX_Wait_Ack(void);
static void AT24CXX_Ack(void);
static void AT24CXX_NAck(void);
static void AT24CXX_Send_Byte(uint8_t txd);
static uint8_t AT24CXX_Read_Byte(AT24CXX_SEND_NACK_ACK ack);



/* Private functions ----------------------------------------------------------*/

//��ʼ��IIC ��GPIO �ӿ�
void AT24CXX_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOC_CLK_ENABLE();   //ʹ��GPIOX ʱ��

    //(SCL), (SDA)��ʼ������
    GPIO_Initure.Pin = AT24CXX_SCL_PIN | AT24CXX_SDA_PIN;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(AT24CXX_SCL_PORT, &GPIO_Initure);

    AT24CXX_SDA(GPIO_PIN_SET);
    AT24CXX_SCL(GPIO_PIN_SET);

}

//���EEPROM оƬ���ͺ���AT24C16 ����AT24C256
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

//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr)
{
    uint8_t temp = 0;

    AT24CXX_Start();

    AT24CXX_Send_Byte(AT24CXX_ADDRESS | AT24CXX_WRITE_CMD);//����д����
    AT24CXX_Wait_Ack();

    if(is_AT24C16_flag == 0)
    {
        AT24CXX_Send_Byte(ReadAddr >> 8);//���͸ߵ�ַ
        AT24CXX_Wait_Ack();
    }
    AT24CXX_Send_Byte(ReadAddr >> 0);//���͵͵�ַ
    AT24CXX_Wait_Ack();
    
    AT24CXX_Start();
    AT24CXX_Send_Byte(AT24CXX_ADDRESS | AT24CXX_READ_CMD);//�������ģʽ
    AT24CXX_Wait_Ack();
    
    temp = AT24CXX_Read_Byte(AT24CXX_SEND_NACK);
    
    AT24CXX_Stop();//����һ��ֹͣ����
    
    return temp;
}

//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(uint16_t WriteAddr, uint8_t DataToWrite)
{
    AT24CXX_Start();

    AT24CXX_Send_Byte(AT24CXX_ADDRESS | AT24CXX_WRITE_CMD);//����д����
    AT24CXX_Wait_Ack();

    if(is_AT24C16_flag == 0)
    {
        AT24CXX_Send_Byte(WriteAddr >> 8);//���͸ߵ�ַ
        AT24CXX_Wait_Ack();
    }
    AT24CXX_Send_Byte(WriteAddr >> 0);//���͵͵�ַ
    AT24CXX_Wait_Ack();
    
    AT24CXX_Send_Byte(DataToWrite);//�����ֽ�
    AT24CXX_Wait_Ack();
    
    AT24CXX_Stop();//����һ��ֹͣ����

    //оƬ��д������5 ����
    delay_ms(5);
}


//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
uint8_t AT24CXX_Read_Bytes(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead)
{
    uint16_t i = 0;

    //ʣ��ռ䲻��
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

    AT24CXX_Send_Byte(AT24CXX_ADDRESS | AT24CXX_WRITE_CMD);//����д����
    AT24CXX_Wait_Ack();

    if(is_AT24C16_flag == 0)
    {
        AT24CXX_Send_Byte(ReadAddr >> 8);//���͸ߵ�ַ
        AT24CXX_Wait_Ack();
    }
    AT24CXX_Send_Byte(ReadAddr >> 0);//���͵͵�ַ
    AT24CXX_Wait_Ack();

    AT24CXX_Start();
    AT24CXX_Send_Byte(AT24CXX_ADDRESS | AT24CXX_READ_CMD);//�������ģʽ
    AT24CXX_Wait_Ack();

    while(i < (NumToRead - 1))
    {
        *(pBuffer + i) = AT24CXX_Read_Byte(AT24CXX_SEND_ACK);//ACK
        i++;
    }

    //�������һ���ֽ�, NACK, ֮ǰ����ACK
    *(pBuffer + i) = AT24CXX_Read_Byte(AT24CXX_SEND_NACK);//NACK
    AT24CXX_Stop();//����һ��ֹͣ����

    return 1;

}

//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
uint8_t AT24CXX_Write_Bytes(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite)
{
    uint16_t i = 0;
    uint8_t per_page_size = 0;
    
    //ʣ��ռ䲻��
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

    AT24CXX_Send_Byte(AT24CXX_ADDRESS | AT24CXX_WRITE_CMD);//����д����
    AT24CXX_Wait_Ack();

    if(is_AT24C16_flag == 0)
    {
        AT24CXX_Send_Byte(WriteAddr >> 8);//���͸ߵ�ַ
        AT24CXX_Wait_Ack();
    }
    AT24CXX_Send_Byte(WriteAddr >> 0);//���͵͵�ַ
    AT24CXX_Wait_Ack();

    while(i < NumToWrite)
    {
        AT24CXX_Send_Byte(*(pBuffer + i));//�����ֽ�
        AT24CXX_Wait_Ack();

        i++;
        WriteAddr++;//д���ַ��1 
        if(WriteAddr % per_page_size == 0)//�µ�һҳ
        {
            AT24CXX_Stop();//����һ��ֹͣ����

            // AT24CXX оƬ��д������5 ����
            delay_ms(10);

            //���¿�ʼ
            AT24CXX_Start();

            AT24CXX_Send_Byte(AT24CXX_ADDRESS | AT24CXX_WRITE_CMD);//����д����
            AT24CXX_Wait_Ack();

            if(is_AT24C16_flag == 0)
            {
                AT24CXX_Send_Byte(WriteAddr >> 8);//���͸ߵ�ַ
                AT24CXX_Wait_Ack();
            }
            AT24CXX_Send_Byte(WriteAddr >> 0);//���͵͵�ַ
            AT24CXX_Wait_Ack();
        }
    }

    AT24CXX_Stop();//����һ��ֹͣ����

    //оƬ��д������5 ����
    delay_ms(10);

    return 1;

}





//------------------------ģ��I2C ��ʱ��------------------------

//����IIC��ʼ�ź�
static void AT24CXX_Start(void)
{
    AT24CXX_SDA_OUT();//sda�����
    AT24CXX_SDA(GPIO_PIN_SET);
    AT24CXX_SCL(GPIO_PIN_SET);
    delay_us(1);
    AT24CXX_SDA(GPIO_PIN_RESET);//START:when CLK is high,DATA change form high to low
    delay_us(1);
    AT24CXX_SCL(GPIO_PIN_RESET);//ǯסI2C���ߣ�׼�����ͻ�������� 
}

//����IICֹͣ�ź�
static void AT24CXX_Stop(void)
{
    AT24CXX_SDA_OUT();//sda�����
    AT24CXX_SCL(GPIO_PIN_RESET);
    AT24CXX_SDA(GPIO_PIN_RESET);//STOP:when CLK is high DATA change form low to high
    delay_us(1);
    AT24CXX_SCL(GPIO_PIN_SET); 
    AT24CXX_SDA(GPIO_PIN_SET);//����I2C���߽����ź�
    delay_us(1);
}

//�����ȴ�Ӧ���źŵ���, ����ֵ: 0 ����Ӧ��ʧ��, 1 ����Ӧ��ɹ�
static uint8_t AT24CXX_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    
    AT24CXX_SDA_IN();      //SDA����Ϊ����
    
    AT24CXX_SDA(GPIO_PIN_SET);//�ӻ����Ӧ��, �����͸�SDA �ĵ�ƽ
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

//��������ACKӦ��(��SCK �ߵ�ƽ����, ����SDA ��ƽ)
static void AT24CXX_Ack(void)
{
    AT24CXX_SCL(GPIO_PIN_RESET);
    AT24CXX_SDA_OUT();
    
    AT24CXX_SDA(GPIO_PIN_RESET);//�õ�
    delay_us(1);

    //SCK �ߵ�ƽ����
    AT24CXX_SCL(GPIO_PIN_SET);
    delay_us(1);
    AT24CXX_SCL(GPIO_PIN_RESET);
}

//����������ACKӦ��(��SCK �ߵ�ƽ����, ����SDA ��ƽ)
static void AT24CXX_NAck(void)
{
    AT24CXX_SCL(GPIO_PIN_RESET);
    AT24CXX_SDA_OUT();
    
    AT24CXX_SDA(GPIO_PIN_SET);//�ø�
    delay_us(1);

    //SCK �ߵ�ƽ����
    AT24CXX_SCL(GPIO_PIN_SET);
    delay_us(1);
    AT24CXX_SCL(GPIO_PIN_RESET);
}

//IIC����һ���ֽ�, ���شӻ�����Ӧ��, 1 ��Ӧ��, 0 ��Ӧ��
static void AT24CXX_Send_Byte(uint8_t txd)
{
    uint8_t t;
    AT24CXX_SDA_OUT();
    AT24CXX_SCL(GPIO_PIN_RESET);//����ʱ�ӿ�ʼ���ݴ���

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


//��1���ֽ�, ack = 1ʱ, ����ACK; ack = 0ʱ, ��Ӧ��, ����nACK
static uint8_t AT24CXX_Read_Byte(AT24CXX_SEND_NACK_ACK ack)
{
    uint8_t i = 0;
    uint8_t receive = 0;
    
    AT24CXX_SDA_IN();//SDA����Ϊ����

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
        AT24CXX_NAck();//����nACK
    }
    else
    {
        AT24CXX_Ack(); //����ACK
    }
        
    return receive;
}

//------------------------ģ��I2C ��ʱ��------------------------



