/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : ble_if.h
* Author      : LisonLiu
* Version     : v0.01
* Date        : 2018/6/1
* Description : 
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2018/6/1  LisonLiu    initial version
*=============================================================================*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _BLE_IF_H_
#define _BLE_IF_H_ 

#ifdef __cplusplus
 extern "C" {
#endif
     
/**** Includes ****/
#include "stdint.h"  
#include "main.h"
     
/**** Definition of constants ****/ 
	 
/**** Definition of types ****/ 

union int16_type
{
    int16_t data_int16;
    uint8_t data_buf[2];
};

typedef enum
{
    BAUD_1200 = 0,
    BAUD_2400,
    BAUD_4800,
    BAUD_9600,     // = 3
    BAUD_19200,
    BAUD_38400,
    BAUD_57600,
    BAUD_115200,     // = 7
    BAUD_230400,
    
    BAUD_INDEX_END,
}BLE_BAUD_INDEX;	 
	 
/**** Definition of macros ****/
#define BLE_TX_BUF_LEN     512
#define BLE_RX_BUF_LEN     512

//���ݰ�֡ͷ
#define UART_FRAME_HEAD_1  0xAA
#define UART_FRAME_HEAD_2  0x55

//������֡ͷ
#define UART_UPGRADE_HEAD_1  0xBB
#define UART_UPGRADE_HEAD_2  0x44

//����汾֡ͷ
#define UART_SOFTWART_VER_HEAD_1  0xCC
#define UART_SOFTWART_VER_HEAD_2  0x33


//ģʽѡ��
typedef enum
{
    MODE_NORMAL = 0,    //����ģʽ
    MODE_HEMIPLEGIA,    //ƫ̱ģʽ
    MODE_PARKINSON,    //����ɭģʽ
    MODE_PASSIVE,        //����ģʽ    
    MODE_ZERO_FORCE,    //����ģʽ
    MODE_TESTING,        //����ģʽ
    
    MODE_TOTAL,
}MODE_INDEX;


//���巢�͸�APP ������
typedef struct
{
    float left_angle;
    float left_speed;
    float left_torque;
    float right_angle;
    float right_speed;
    float right_torque;
    uint8_t battery_level;
} MB2APP_DATA;

//APP ���͸����������
typedef struct
{
    uint8_t ble_connected_flag;
    
    MODE_INDEX mode;  //ģʽѡ��(0��1��2��3)
    uint8_t assist_level;  //�����ȼ�(1��2��3��4��5)
    uint8_t lift_level_left_hip;  //̧�ȸ߶�--����(1��2��3)
    uint8_t lift_level_right_hip;  //̧�ȸ߶�--����(1��2��3)
    uint8_t hemiplegiaLeg_or_liftLeg;  //ֵ˵��: 0û��ѡ��1��ࡢ2�Ҳ࣬ƫ̱ģʽ--(��ƫ̱����ƫ̱)������ɭģʽ--(̧���ȡ�̧����)
    FLAG_STATUS parkinson_lift_leg_flag;  //״ֵ̬��FLAG_FALSE ����ΪFLAG_TRUE, ˵������������
    uint8_t voice_level;  //�����ȼ�(0��1��2��3��4��5��6��7��8��9��10)
    uint8_t speed_level;  //�ٶȵȼ�(1��2��3)
    FLAG_STATUS param_cal;  //У׼״ֵ̬, ��FLAG_FALSE ����ΪFLAG_TRUE, ˵������������
    FLAG_STATUS check_alarm;  //�Ƿ�����ˤ������, FLAG_TRUE Ϊ����
    FLAG_STATUS check_sitdown;  //�Ƿ��������¼��, FLAG_TRUE Ϊ����
} APP2MB_DATA;



/**** Declaration of variables ****/	 


/**** Declaration of functions ****/
void ble_task(void *argument);
uint8_t packing_data(uint8_t* packing_buf, uint8_t* buf, uint8_t len);
uint8_t unpacking_data(uint8_t* unpacking_buf, uint8_t* buf, uint8_t len);
void set_app2mb_from_flash(void);


#ifdef __cplusplus
}
#endif

#endif /*_BLE_IF_H_*/

