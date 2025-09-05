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

//数据包帧头
#define UART_FRAME_HEAD_1  0xAA
#define UART_FRAME_HEAD_2  0x55

//升级包帧头
#define UART_UPGRADE_HEAD_1  0xBB
#define UART_UPGRADE_HEAD_2  0x44

//软件版本帧头
#define UART_SOFTWART_VER_HEAD_1  0xCC
#define UART_SOFTWART_VER_HEAD_2  0x33


//模式选择
typedef enum
{
    MODE_NORMAL = 0,    //助行模式
    MODE_HEMIPLEGIA,    //偏瘫模式
    MODE_PARKINSON,    //帕金森模式
    MODE_PASSIVE,        //被动模式    
    MODE_ZERO_FORCE,    //零力模式
    MODE_TESTING,        //测试模式
    
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
    uint8_t hemiplegiaLeg_or_liftLeg;  //值说明: 0没有选择、1左侧、2右侧，偏瘫模式--(左偏瘫、右偏瘫)、帕金森模式--(抬左腿、抬右腿)
    FLAG_STATUS parkinson_lift_leg_flag;  //状态值从FLAG_FALSE 跳变为FLAG_TRUE, 说明按键被按下
    uint8_t voice_level;  //音量等级(0、1、2、3、4、5、6、7、8、9、10)
    uint8_t speed_level;  //速度等级(1、2、3)
    FLAG_STATUS param_cal;  //校准状态值, 从FLAG_FALSE 跳变为FLAG_TRUE, 说明按键被按下
    FLAG_STATUS check_alarm;  //是否启用摔倒报警, FLAG_TRUE 为启用
    FLAG_STATUS check_sitdown;  //是否启用坐下检测, FLAG_TRUE 为启用
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

