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

//E104-BT5032A的波特率
typedef enum
{
    BAUD_1200 = 0,
    BAUD_2400,
    BAUD_4800,
    BAUD_9600,     // = 3
    
    BAUD_14400,
    BAUD_19200,
    BAUD_28800,
    BAUD_38400,     // = 7
    
    BAUD_57600,
    BAUD_76800,
    BAUD_115200,     // = 10
    BAUD_230400,     // = 11
    
    BAUD_250000,
    BAUD_460800,
    BAUD_921600,     // = 14
    
    BAUD_INDEX_END,
}BLE_BAUD_INDEX;	 
	 
/**** Definition of macros ****/
#define BLE_TX_BUF_LEN     256
#define BLE_RX_BUF_LEN     256

//数据包帧头
#define UART_FRAME_HEAD_1  0xAA
#define UART_FRAME_HEAD_2  0x55

//升级包帧头
#define UART_UPGRADE_HEAD_1  0xBB
#define UART_UPGRADE_HEAD_2  0x44

//软件版本帧头
#define UART_SOFTWART_VER_HEAD_1  0xCC
#define UART_SOFTWART_VER_HEAD_2  0x33

//评估数据帧头
#define UART_EVALUATION_HEAD_1  0xDD
#define UART_EVALUATION_HEAD_2  0x22


//模式选择
typedef enum
{
    MODE_NORMAL = 0,    //助行模式
    MODE_HEMIPLEGIA,    //偏瘫模式
	MODE_RESISTANCE,    //帕金森模式
    MODE_PASSIVE,        //被动模式    
    MODE_EVALUATION,      //评估模式
    
    MODE_ZERO_FORCE,    //零力模式
    MODE_TESTING,        //测试模式
    

    MODE_PARKINSON, 

    MODE_TOTAL,
}MODE_INDEX;


//主板发送给APP 的数据
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

//APP 发送给主板的数据
typedef struct
{
    uint8_t ble_connected_flag;
    
    MODE_INDEX mode;  //模式选择(0、1、2、3)
    uint8_t assist_level;  //助力等级(1、2、3、4、5)
    uint8_t lift_level_left_hip;  //抬腿高度--左髋(1、2、3)
    uint8_t lift_level_right_hip;  //抬腿高度--右髋(1、2、3)
    uint8_t hemiplegiaLeg_or_liftLeg;  //不同模式，偏瘫(1-左、2-右)、帕金森(1-抬左、2-抬右)、被动(1-开始、2-停止)
    FLAG_STATUS parkinson_lift_leg_flag;  //帕金森模式下, 状态值从FLAG_FALSE 跳变为FLAG_TRUE, 说明按键被按下
    uint8_t voice_level;  //音量等级(0、1、2、3、4、5、6、7、8、9、10)
    uint8_t speed_level;  //速度等级(1、2、3)
    FLAG_STATUS param_cal;  //校准状态值, 从FLAG_FALSE 跳变为FLAG_TRUE, 说明按键被按下
    FLAG_STATUS check_alarm;  //是否启用摔倒报警, FLAG_TRUE 为启用
    FLAG_STATUS check_sitdown;  //是否启用坐下检测, FLAG_TRUE 为启用
    FLAG_STATUS passive_press_flag;  //被动模式下, 状态值从FLAG_FALSE 跳变为FLAG_TRUE, 说明按键被按下
} APP2MB_DATA;

//蓝牙通信数据的位操作的索引
typedef enum
{
    BLE_BIT_PARAM_CAL = 0,    //参数校准
    BLE_BIT_PARKINSON_LIFT,    //帕金森模式下, 是否按下按键
    BLE_BIT_CHECK_SITDOWN,    //是否启用坐下检测
    BLE_BIT_CHECK_ALARM,    //是否启用摔倒报警
    BLE_BIT_PASSIVE_PRESS,    //被动模式下, 是否按下按键
} BLE_BIT_INDEX;


/**** Declaration of variables ****/	 

extern MB2APP_DATA mb2app;
extern APP2MB_DATA app2mb;


/**** Declaration of functions ****/
void ble_task(void *argument);
uint8_t packing_data(uint8_t* packing_buf, uint8_t* buf, uint8_t len);
uint8_t unpacking_data(uint8_t* unpacking_buf, uint8_t* buf, uint8_t len);
void set_app2mb_from_flash(void);


#ifdef __cplusplus
}
#endif

#endif /*_BLE_IF_H_*/

