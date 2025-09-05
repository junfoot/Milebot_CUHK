/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : ble.c
* Author      : LisonLiu
* Version     : v0.01
* Date        : 2018/6/1
* Description : 
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2018/6/1  LisonLiu    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/

#include "cmsis_os2.h"                  // ARM::CMSIS:RTOS2:Keil RTX5

/* Public Includes -----------------------------------------------------------*/

#include <stdlib.h>
#include "string.h"
#include<stdio.h>

#include "main.h"
#include "uart_if.h"
#include "..\power\power_mana_if.h"
#include "internal_flash.h"
#include "at24c256.h"
#include "walk.h"
#include "iap.h"
#include "json_app.h"

/* Private Includes ----------------------------------------------------------*/

#include "ble.h"
#include "ble_if.h"  


/*
    模块出厂默认配置：

    新蓝牙: E104-BT5032A: 115200, N, 8, 1, 从模式
    新蓝牙MTU(最大传输单元, 每包数据) 最大 247bytes
        
    //旧蓝牙: HMSoft: 9600, N, 8, 1, 从模式, iBeacon 关闭, 不休眠, 透传+远控模式
*/

/*
    数据打包和解包说明:
    
    通过"串口调试助手" 下发数据给主板的说明(十六进制)：
    每个报文共包括以下部分：帧头、数据帧长度、用户数据、校验值。
    帧头(2个字节)：0xAA 0x55
    数据帧长度(1个字节)：包括帧头、数据帧长度、用户数据、校验值
    用户数据(不超过200个字节)：发送的有效数据
    校验值(1个字节)：包括帧头、数据帧长度、用户数据。把它们的累加起来的和

    例如：用户要发送3个字节的有效数据，是0x11、0x22、0x33
    则发送整个完整数据包是：AA 55 07 11 22 33 6C
    其中：
    帧头(2个字节)：0xAA 0x55
    数据帧长度(1个字节)：0x07
    用户有效数据(不超过200个字节)：0x11、0x22、0x33
    校验值(1个字节)：6C  (因为0xAA + 0x55 + 0x07 + 0x11 + 0x22 + 0x33 = 0x16C，取低8位，即是0x6C)
 */


/**** Definition of types ****/ 

//发送评估数据的FSM
typedef enum
{
    E_SEND_FSM_PROGRESS_BAR = 0,		//发送评估进度
    E_SEND_FSM_FORM_DATA_BEGIN = 1,	//发送表格里面的数据--开始
    E_SEND_FSM_FORM_DATA_END = 8,		//发送表格里面的数据--结尾
    E_SEND_FSM_DIFF_DATA = 33,			//发送差异分析数据
    E_SEND_FSM_CURVE_DATA = 55,		//发送曲线数据
} E_SEND_FSM_INDEX;


/* Private variables ---------------------------------------------------------*/

extern walk_struct walk;
extern gait_data gait_data_all;

static uint8_t ble_tx_buf[BLE_TX_BUF_LEN] = {0x00};
static uint8_t ble_tx_buf_len = 0;
static uint8_t ble_packing_buf[BLE_TX_BUF_LEN] = {0x00};
static uint8_t ble_packing_buf_len = 0;

static uint8_t ble_unpacking_buf[BLE_RX_BUF_LEN] = {0x00};
static uint8_t ble_unpacking_buf_len = 0;

static FLAG_STATUS get_ble_data_flag = FLAG_FALSE;

//JSON
static uint8_t ble_tx_buf_json[BLE_TX_BUF_LEN] = {0x00};

//主板发送给APP 的数据
MB2APP_DATA mb2app = 
{
    .left_angle = 0.0, 
    .left_speed = 0.0, 
    .left_torque = 0.0, 
    .right_angle = 0.0, 
    .right_speed = 0.0, 
    .right_torque = 0.0, 
    .battery_level = 0, 
};

//APP 发送给主板的数据
APP2MB_DATA app2mb = 
{
    .parkinson_lift_leg_flag = FLAG_FALSE, 
    .param_cal = FLAG_FALSE, 
    .check_alarm = FLAG_FALSE, 
    .check_sitdown = FLAG_FALSE, 
    .passive_press_flag = FLAG_FALSE, 
};

//备份, 用于判断配置参数是否被修改
APP2MB_DATA app2mb_backup;

//(均值、标准差、最大值、最小值)
static const float* ptr_step_freq[] = {gait_data_all.step_freq_avr, gait_data_all.step_freq_var, gait_data_all.step_freq_max, gait_data_all.step_freq_min};
static const float* ptr_support_cnt[] = {gait_data_all.support_cnt_avr, gait_data_all.support_cnt_var, gait_data_all.support_cnt_max, gait_data_all.support_cnt_min};
static const float* ptr_swing_cnt[] = {gait_data_all.swing_cnt_avr, gait_data_all.swing_cnt_var, gait_data_all.swing_cnt_max, gait_data_all.swing_cnt_min};
static const float* ptr_support_swing_per[] = {gait_data_all.support_swing_per_avr, gait_data_all.support_swing_per_var, gait_data_all.support_swing_per_max, gait_data_all.support_swing_per_min};
static const float* ptr_support_per[] = {gait_data_all.support_per_avr, gait_data_all.support_per_var, gait_data_all.support_per_max, gait_data_all.support_per_min};
static const float* ptr_swing_per[] = {gait_data_all.swing_per_avr, gait_data_all.swing_per_var, gait_data_all.swing_per_max, gait_data_all.swing_per_min};
static const float* ptr_flexion_angle_max[] = {gait_data_all.flexion_angle_max_avr, gait_data_all.flexion_angle_max_var, gait_data_all.flexion_angle_max_max, gait_data_all.flexion_angle_max_min};
static const float* ptr_flexion_vel_max[] = {gait_data_all.flexion_vel_max_avr, gait_data_all.flexion_vel_max_var, gait_data_all.flexion_vel_max_max, gait_data_all.flexion_vel_max_min};

static const float** ptr_evaluation[] = {ptr_step_freq, ptr_support_cnt, ptr_swing_cnt, ptr_support_swing_per,
                                                            ptr_support_per, ptr_swing_per, ptr_flexion_angle_max, ptr_flexion_vel_max};

//评估模式下, 数据传输完成的flag
static FLAG_STATUS evaluation_finish_flag = FLAG_FALSE;

//评估模式下发送数据的fsm
static E_SEND_FSM_INDEX evaluation_fsm = E_SEND_FSM_PROGRESS_BAR;

//曲线数据
static uint8_t e_leg_id = LEFT_HIP;  //左腿为0, 右腿为1
static uint8_t e_total_pkg = 1;  //数据包总量, 数据包= 数据/ 50
static uint8_t e_cur_pkg = 1;  //数据包索引从1 开始


//评估曲线每包数据的长度
#define EVALUATION_PKG_LEN      10

//BLE 数据的格式标志
FORMAT_LIST format_flag = FORMAT_uncertain;

/* Private function prototypes -----------------------------------------------*/
static void ble_init(void);
static void ble_send_data(uint8_t *pbuf, uint16_t size);
static void ble_set_baud(uint8_t baud);
static void ble_read_name(void);
static void ble_set_name(void);
static void ble_fill_data_send(void);
static void ble_evaluation_data_send(void);
static void check_save_data(void);
static void dispose_upgrade_package(void);
static void dispose_evaluation_package(void);
static void dispose_normal_package(void);

static void pack_json_send(void);
static void analyze_json_set(void);

/* Private functions ----------------------------------------------------------*/
void ble_task(void *argument)
{
    uint32_t ble_reset_tick = 0;  //蓝牙长时间没有接收到数据，要重启
    uint8_t ble_send_cnt = 0;
    uint8_t save_data_cnt = 0;

    //static uint16_t total_package = 0;
    static uint16_t current_package = 0;

    static uint8_t ble_send_interval = 5;

    //等待供电稳定
    osDelay(1000);
    
    //初始化蓝牙模块, 修改波特率
    ble_init();
    
    while(1) 
    {
        //接收并解析蓝牙的数据
        if((uart5_dma_rx_buf_len > 0) && ((HAL_GetTick() - uart5_rx_data_tick) > 20))
        {
            if((format_flag == FORMAT_uncertain) || (format_flag == FORMAT_binary))
            {
                ble_unpacking_buf_len = unpacking_data(ble_unpacking_buf, uart5_dma_rx_buf_raw, uart5_dma_rx_buf_len);
                if(ble_unpacking_buf_len > 0)
                {
                    format_flag = FORMAT_binary;//二进制格式
                    app2mb.ble_connected_flag = 1;

                    //是否是升级包的帧头
                    if((ble_unpacking_buf[0] == UART_UPGRADE_HEAD_1) && 
                        (ble_unpacking_buf[1] == UART_UPGRADE_HEAD_2))
                    {
                        if(ble_unpacking_buf_len == 6)
                        {
                            //total_package = ble_unpacking_buf[2] + ble_unpacking_buf[3] * 256;
                            current_package = ble_unpacking_buf[4] + ble_unpacking_buf[5] * 256;

                            if(current_package == 0)
                            {
                                dispose_upgrade_package();
                            }
                        }
                    }
                    else if((ble_unpacking_buf[0] == UART_EVALUATION_HEAD_1) && 
                        (ble_unpacking_buf[1] == UART_EVALUATION_HEAD_2))
                    {
                        dispose_evaluation_package();
                    }
                    else
                    {
                        dispose_normal_package();
                    }
                    
                    get_ble_data_flag = FLAG_TRUE;
                    ble_reset_tick = HAL_GetTick();
                }
            }

            //数据格式是JSON 格式
            if((format_flag == FORMAT_uncertain) || (format_flag == DATA_FORMAT_json))
            {
                if(0 != analyze_JSON_data(uart5_dma_rx_buf_raw, uart5_dma_rx_buf_len))
                {
                    format_flag = DATA_FORMAT_json;//JSON 格式
                    app2mb.ble_connected_flag = 1;

                    analyze_json_set();

                    //SN号如果发生变化, 则重新保存到eeprom 中
                    if((s2c_data.device_SN_buf[0] == 'H') && 
                        (s2c_data.device_SN_buf[1] == 'X') && 
                        (s2c_data.device_SN_buf[2] == 'O'))
                    {
                        if(strcmp((char*)device_SN_buf, (char*)s2c_data.device_SN_buf) != 0)
                        {
                            memcpy(device_SN_buf, s2c_data.device_SN_buf, DEVICE_SN_LEN);
                            AT24CXX_Write_Bytes(FLASH_PARAM_SN_BEGIN, device_SN_buf, DEVICE_SN_LEN);
                        }
                    }
                    
                    get_ble_data_flag = FLAG_TRUE;
                    ble_reset_tick = HAL_GetTick();
                }
            }

            uart5_dma_rx_buf_len = 0;
        }
        else
        {
            //BLE 蓝牙模块重启
            if(((get_ble_data_flag == FLAG_TRUE) && ((HAL_GetTick() - ble_reset_tick) > 5000)) || 
                ((get_ble_data_flag == FLAG_FALSE) && ((HAL_GetTick() - ble_reset_tick) > 25000)))
            {
                app2mb.ble_connected_flag = 0;

                get_ble_data_flag = FLAG_FALSE;

                ble_reset_tick = HAL_GetTick();
            }
        }

        //再次重新评估
        if(gait_data_all.complete_per < 1.0)
        {
            evaluation_finish_flag = FLAG_FALSE;
        }

        //打包并发送蓝牙的数据
        ble_send_cnt++;
        if(ble_send_cnt >= ble_send_interval)
        {
            ble_send_cnt = 0;

            if((app2mb_backup.mode == MODE_EVALUATION) && 
                (evaluation_finish_flag == FLAG_FALSE))
            {
                static uint8_t time_division_cnt = 0;
                
                time_division_cnt++;
                if(time_division_cnt < 5)
                {
                    ble_evaluation_data_send();
                }
                else
                {
                    time_division_cnt = 0;
                    ble_fill_data_send();
                }

                ble_send_interval = 10;  //发送太快, ble 可能会出现故障
            }
            else
            {
                ble_fill_data_send();

                if(format_flag == DATA_FORMAT_json)
                {
                    ble_send_interval = 20;  //发送太快, ble 可能会出现故障
                }
                else
                {
                    ble_send_interval = 10;
                }
            }
        }

        //如果APP 设置的参数发生了变化, 则保存新参数
        save_data_cnt++;
        if(save_data_cnt >= 200)
        {
            save_data_cnt = 0;

            check_save_data();
        }
        
        osDelay(5);
    }
}

//把接收到后解析的json 数据赋值 
static void analyze_json_set(void)
{
    static uint8_t temp_lift_side = 0;
    
    app2mb.mode = (MODE_INDEX)(s2c_data.mode - 1);
    app2mb.assist_level = s2c_data.assist;
    
    app2mb.lift_level_left_hip = s2c_data.lift_height;
    app2mb.lift_level_right_hip = app2mb.lift_level_left_hip; //左右腿是一样的

    app2mb.speed_level = s2c_data.speed;

    switch(app2mb.mode)
    {
        case MODE_HEMIPLEGIA:    //偏瘫模式
            app2mb.hemiplegiaLeg_or_liftLeg = s2c_data.hmp_side;
            break;

        case MODE_PARKINSON:    //帕金森模式
            app2mb.hemiplegiaLeg_or_liftLeg = s2c_data.lift_side;

            if((temp_lift_side == 0) && (app2mb.hemiplegiaLeg_or_liftLeg != 0))
            {
                app2mb.parkinson_lift_leg_flag = FLAG_TRUE;
            }
            else
            {
                app2mb.parkinson_lift_leg_flag = FLAG_FALSE;
            }

            temp_lift_side = app2mb.hemiplegiaLeg_or_liftLeg;
            break;

        case MODE_PASSIVE:        //被动模式
            app2mb.hemiplegiaLeg_or_liftLeg = s2c_data.walk_ctrl;
            break;

        default:
            break;
    }
}

//打包JSON 数据并发送
static void pack_json_send(void)
{
    uint16_t index = 0;
    char* ptr = (char*)ble_tx_buf_json;
    uint8_t len = 0;
#if 0   /* Begin if 0 by lison on 2021-10-15 */
    float float_buf[6] = {0};
#endif  /* End if 0 by lison on 2021-10-15 */
    static uint8_t send_SN_cnt = 0;

    //开头, 左大括号'{'
    ptr[index++] = '{';

#if 0   /* Begin if 0 by lison on 2021-10-15 */
    //"joint_info": [0.1, 0.2, 0.3, 0.4, 0.5, 0.6]
    float_buf[0] = mb2app.left_angle;
    float_buf[1] = mb2app.left_speed;
    float_buf[2] = mb2app.left_torque;
    float_buf[3] = mb2app.right_angle;
    float_buf[4] = mb2app.right_speed;
    float_buf[5] = mb2app.right_torque;
    len = json_add_float_array(&ptr[index], "joint_info", float_buf, 6);
    index += len;
    ptr[index++] = ',';

    //bat_level
    len = json_add_int(&ptr[index], "bat_level", (uint8_t)get_bat_level());
    index += len;
    ptr[index++] = ',';

    //walk_step
    len = json_add_int(&ptr[index], "walk_step", walk.steps);
    index += len;
    ptr[index++] = ',';
#endif  /* End if 0 by lison on 2021-10-15 */

    //run_time
    len = json_add_int(&ptr[index], "run_time", (HAL_GetTick()/1000));
    index += len;

    //SN
    send_SN_cnt++;
    if(send_SN_cnt > 10)
    {
        send_SN_cnt = 0;
        
        ptr[index++] = ',';
        len = json_add_string(&ptr[index], "SN", (char*)device_SN_buf);
        index += len;
        //ptr[index++] = ',';
    }

    //结尾, 右大括号'}'
    ptr[index++] = '}';

//    //字符串结束符, 因为strlen()
//    ptr[index++] = '\0';

    ble_send_data((uint8_t*)ptr, index);
    
}

//处理升级包
static void dispose_upgrade_package(void)
{
    //升级前, 关闭电机电源
    LMOTOR_POWER_disable();
    RMOTOR_POWER_disable();

    //更新升级参数
    uint32_t data = PARAM_PRE_UPGRADE;
    write_internal_flash(Parameters_ADDRESS, (uint8_t*)(&data), 4);

    //检查参数是否正确写入
    uint8_t cnt = 0;
    HAL_Delay(5);
    read_internal_flash(Parameters_ADDRESS, (uint8_t*)(&data), 4);
    while((data != PARAM_PRE_UPGRADE) && (cnt <= 3))
    {
        cnt++;
        HAL_Delay(5);
        data = PARAM_PRE_UPGRADE;
        write_internal_flash(Parameters_ADDRESS, (uint8_t*)(&data), 4);

        HAL_Delay(5);
        read_internal_flash(Parameters_ADDRESS, (uint8_t*)(&data), 4);
    }
    //检查参数是否正确写入

    //回复收到准备升级标准, 然后跳转到boot
    ble_tx_buf_len = 0;
    ble_tx_buf[ble_tx_buf_len++] = UART_UPGRADE_HEAD_1;
    ble_tx_buf[ble_tx_buf_len++] = UART_UPGRADE_HEAD_2;
    ble_tx_buf[ble_tx_buf_len++] = (0 >> 0);
    ble_tx_buf[ble_tx_buf_len++] = (0 >> 8); 
    
    //发送数据
    ble_packing_buf_len = packing_data(ble_packing_buf, ble_tx_buf, ble_tx_buf_len);
    ble_send_data(ble_packing_buf, ble_packing_buf_len);
    HAL_Delay(5);

    //跳转到指定的地址运行
    iap_jump(BootLoader_ADDRESS);
}

//处理评估包
static void dispose_evaluation_package(void)
{
    if(ble_unpacking_buf_len == 5)
    {
        evaluation_fsm = (E_SEND_FSM_INDEX)ble_unpacking_buf[2];
        if(e_leg_id ==  ble_unpacking_buf[3])
        {
            e_cur_pkg = ble_unpacking_buf[4];
        }
        else
        {
            e_cur_pkg = 0;
        }
    
        if(evaluation_fsm == E_SEND_FSM_PROGRESS_BAR)
        {
            //评估进度
            if(gait_data_all.complete_per >= 1.0)
            {
                evaluation_fsm = E_SEND_FSM_FORM_DATA_BEGIN;
            }
        }
        else if((evaluation_fsm >= E_SEND_FSM_FORM_DATA_BEGIN) && 
            (evaluation_fsm < E_SEND_FSM_FORM_DATA_END))
        {
            evaluation_fsm++;
        }
        else if(evaluation_fsm == E_SEND_FSM_FORM_DATA_END)
        {
            evaluation_fsm = E_SEND_FSM_DIFF_DATA;
        }
        else if(evaluation_fsm == E_SEND_FSM_DIFF_DATA)
        {
            e_leg_id = LEFT_HIP;  //左腿为0, 右腿为1
            e_cur_pkg = 1;  //数据包索引从1 开始
            e_total_pkg = (gait_data_all.angle_data_length[e_leg_id] + (EVALUATION_PKG_LEN - 1))/EVALUATION_PKG_LEN;
        
            evaluation_fsm = E_SEND_FSM_CURVE_DATA;
        }
        else if(evaluation_fsm == E_SEND_FSM_CURVE_DATA)
        {
            if(e_total_pkg == e_cur_pkg)
            {
                if(e_leg_id == LEFT_HIP)
                {
                    e_leg_id = RIGHT_HIP;  //左腿为0, 右腿为1
                    e_cur_pkg = 1;  //数据包索引从1 开始
                    e_total_pkg = (gait_data_all.angle_data_length[e_leg_id] + (EVALUATION_PKG_LEN - 1))/EVALUATION_PKG_LEN;
                }
                else if(e_leg_id == RIGHT_HIP)
                {
                    //数据传输完成
                    evaluation_fsm = E_SEND_FSM_PROGRESS_BAR;  //重新开始
                    evaluation_finish_flag = FLAG_TRUE;
                }
            }
            else
            {
                e_cur_pkg++;
                if(e_cur_pkg > e_total_pkg)
                {
                    e_cur_pkg = e_total_pkg;
                }
            }
        }
    }
    
    //防止出错越界
    if(e_cur_pkg < 1)
    {
        e_cur_pkg = 1;
    }

}

//处理普通数据包
static void dispose_normal_package(void)
{
    //根据通信协议，提取相关的数据
    app2mb.mode = (MODE_INDEX)ble_unpacking_buf[0];
    app2mb.assist_level = ble_unpacking_buf[1];
    app2mb.lift_level_left_hip = ble_unpacking_buf[2];
    app2mb.lift_level_right_hip = app2mb.lift_level_left_hip; //左右腿是一样的
    
    //Mess，位操作
    if(BIT_GET(ble_unpacking_buf[3], BLE_BIT_PARAM_CAL))
    {
        app2mb.param_cal = FLAG_TRUE;
    }
    else
    {
        app2mb.param_cal = FLAG_FALSE;
    }
    
    if(BIT_GET(ble_unpacking_buf[3], BLE_BIT_PARKINSON_LIFT))
    {
        app2mb.parkinson_lift_leg_flag = FLAG_TRUE;
    }
    else
    {
        app2mb.parkinson_lift_leg_flag = FLAG_FALSE;
    }
    
    if(BIT_GET(ble_unpacking_buf[3], BLE_BIT_CHECK_SITDOWN))
    {
        app2mb.check_sitdown = FLAG_TRUE;
    }
    else
    {
        app2mb.check_sitdown = FLAG_FALSE;
    }
    
    if(BIT_GET(ble_unpacking_buf[3], BLE_BIT_CHECK_ALARM))
    {
        app2mb.check_alarm = FLAG_TRUE;
    }
    else
    {
        app2mb.check_alarm = FLAG_FALSE;
    }
    
    if(BIT_GET(ble_unpacking_buf[3], BLE_BIT_PASSIVE_PRESS))
    {
        app2mb.passive_press_flag = FLAG_TRUE;
    }
    else
    {
        app2mb.passive_press_flag = FLAG_FALSE;
    }
    
    app2mb.hemiplegiaLeg_or_liftLeg = ble_unpacking_buf[4];
    app2mb.voice_level = ble_unpacking_buf[5];
    app2mb.speed_level = ble_unpacking_buf[6];

}


//如果APP 设置的参数发生了变化, 则保存新参数
static void check_save_data(void)
{        
    //模式选择
    if(app2mb_backup.mode != app2mb.mode)
    {
        app2mb_backup.mode = app2mb.mode;
        param_buf[FLASH_PARAM_MODE] = app2mb.mode;

        AT24CXX_WriteOneByte(FLASH_PARAM_MODE, param_buf[FLASH_PARAM_MODE]);

    }

    //助力等级(1、2、3、4、5)
    if(app2mb_backup.assist_level != app2mb.assist_level)
    {
        app2mb_backup.assist_level = app2mb.assist_level;
        param_buf[FLASH_PARAM_ASSIST] = app2mb.assist_level;
        
        AT24CXX_WriteOneByte(FLASH_PARAM_ASSIST, param_buf[FLASH_PARAM_ASSIST]);

    }

    //抬腿高度--左髋(1、2、3)
    if(app2mb_backup.lift_level_left_hip != app2mb.lift_level_left_hip)
    {
        app2mb_backup.lift_level_left_hip = app2mb.lift_level_left_hip;
        param_buf[FLASH_PARAM_LEFT_HIP] = app2mb.lift_level_left_hip;

        AT24CXX_WriteOneByte(FLASH_PARAM_LEFT_HIP, param_buf[FLASH_PARAM_LEFT_HIP]);

    }

    //抬腿高度--右髋(1、2、3)
    if(app2mb_backup.lift_level_right_hip != app2mb.lift_level_right_hip)
    {
        app2mb_backup.lift_level_right_hip = app2mb.lift_level_right_hip;
        param_buf[FLASH_PARAM_RIGHT_HIP] = app2mb.lift_level_right_hip;

        AT24CXX_WriteOneByte(FLASH_PARAM_RIGHT_HIP, param_buf[FLASH_PARAM_RIGHT_HIP]);

    }

    //偏瘫模式: 1左侧、2右侧
    if(app2mb.mode == MODE_HEMIPLEGIA)
    {
        if((app2mb.hemiplegiaLeg_or_liftLeg == 1) || ((app2mb.hemiplegiaLeg_or_liftLeg == 2)))
        {
            if(app2mb_backup.hemiplegiaLeg_or_liftLeg != app2mb.hemiplegiaLeg_or_liftLeg)
            {
                app2mb_backup.hemiplegiaLeg_or_liftLeg = app2mb.hemiplegiaLeg_or_liftLeg;
                param_buf[FLASH_PARAM_HEMIPLEGIA_LEG] = app2mb.hemiplegiaLeg_or_liftLeg;

                AT24CXX_WriteOneByte(FLASH_PARAM_HEMIPLEGIA_LEG, param_buf[FLASH_PARAM_HEMIPLEGIA_LEG]);

            }
        }
    }

    //音量等级(0、1、2、3、4、5、6、7、8、9、10)
    if(app2mb_backup.voice_level != app2mb.voice_level)
    {
        app2mb_backup.voice_level = app2mb.voice_level;
        param_buf[FLASH_PARAM_VOICE] = app2mb.voice_level;

        AT24CXX_WriteOneByte(FLASH_PARAM_VOICE, param_buf[FLASH_PARAM_VOICE]);

    }
    
    //速度等级(1、2、3、4、5)
    if(app2mb_backup.speed_level != app2mb.speed_level)
    {
        app2mb_backup.speed_level = app2mb.speed_level;
        param_buf[FLASH_PARAM_SPEED] = app2mb.speed_level;

        AT24CXX_WriteOneByte(FLASH_PARAM_SPEED, param_buf[FLASH_PARAM_SPEED]);

    }

    //摔倒报警标志(0、1)
    if(app2mb_backup.check_alarm != app2mb.check_alarm)
    {
        app2mb_backup.check_alarm = app2mb.check_alarm;
        param_buf[FLASH_PARAM_CHECK_ALARM] = app2mb.check_alarm;

        AT24CXX_WriteOneByte(FLASH_PARAM_CHECK_ALARM, param_buf[FLASH_PARAM_CHECK_ALARM]);

    }

    //坐下检测标志(0、1)
    if(app2mb_backup.check_sitdown != app2mb.check_sitdown)
    {
        app2mb_backup.check_sitdown = app2mb.check_sitdown;
        param_buf[FLASH_PARAM_CHECK_SITDOWN] = app2mb.check_sitdown;

        AT24CXX_WriteOneByte(FLASH_PARAM_CHECK_SITDOWN, param_buf[FLASH_PARAM_CHECK_SITDOWN]);

    }
}

//把flash 中保存的参数赋值给app2mb
void set_app2mb_from_flash(void)
{
    uint8_t temp = 0;
    
    //模式选择
    temp = param_buf[FLASH_PARAM_MODE];
    if(temp < MODE_TOTAL)
    {
        app2mb.mode = (MODE_INDEX)temp;
    }
    else
    {
        app2mb.mode = MODE_NORMAL;
    }

    //助力等级(1、2、3、4、5)
    temp = param_buf[FLASH_PARAM_ASSIST];
    if((temp >= 1) && (temp <= 5))
    {
        app2mb.assist_level = temp;
    }
    else
    {
        app2mb.assist_level = 3;
    }

    //抬腿高度--左髋(1、2、3)
    temp = param_buf[FLASH_PARAM_LEFT_HIP];
    if(temp <= 3)
    {
        app2mb.lift_level_left_hip = temp;
    }
    else
    {
        app2mb.lift_level_left_hip = 1;
    }

    //抬腿高度--右髋(1、2、3)
    temp = param_buf[FLASH_PARAM_RIGHT_HIP];
    if(temp <= 3)
    {
        app2mb.lift_level_right_hip = temp;
    }
    else
    {
        app2mb.lift_level_right_hip = 1;
    }

    //偏瘫模式: 1左侧、2右侧
    if(app2mb.mode == MODE_HEMIPLEGIA)  //只有在偏瘫模式, 才需要获取flash 的值
    {
        temp = param_buf[FLASH_PARAM_HEMIPLEGIA_LEG];
        if((temp >= 1) && (temp <= 2))
        {
            app2mb.hemiplegiaLeg_or_liftLeg = temp;
        }
        else
        {
            app2mb.hemiplegiaLeg_or_liftLeg = 1;
        }
    }
    else
    {
        app2mb.hemiplegiaLeg_or_liftLeg = 0;
    }

    //音量等级(0、1、2、3、4、5、6、7、8、9、10)
    temp = param_buf[FLASH_PARAM_VOICE];
    if(temp <= 10)
    {
        app2mb.voice_level = temp;
    }
    else
    {
        app2mb.voice_level = 3;
    }

    //速度等级(1、2、3、4、5)
    temp = param_buf[FLASH_PARAM_SPEED];
    if((temp >= 1) && (temp <= 5))
    {
        app2mb.speed_level = temp;
    }
    else
    {
        app2mb.speed_level = 1;
    }

    //摔倒报警标志(0、1)
    temp = param_buf[FLASH_PARAM_CHECK_ALARM];
    if(temp <= 1)
    {
        app2mb.check_alarm = (FLAG_STATUS)temp;
    }
    else
    {
        app2mb.check_alarm = (FLAG_STATUS)0;
    }

    //坐下检测标志(0、1)
    temp = param_buf[FLASH_PARAM_CHECK_SITDOWN];
    if(temp <= 1)
    {
        app2mb.check_sitdown = (FLAG_STATUS)temp;
    }
    else
    {
        app2mb.check_sitdown = (FLAG_STATUS)0;
    }

    //备份结构体的赋值
    app2mb_backup = app2mb;
    
}

//填充BLE 的buf, 并发送
static void ble_fill_data_send(void)
{
    union int16_type temp;
    static uint8_t mess_flag = 0;
    static uint8_t send_ver_cnt = 0;
    uint8_t temp_len = 0;
    static uint8_t ble_connected_flag_last = 0;

    if(format_flag == FORMAT_uncertain)
    {
        return;
    }
    else if(format_flag == DATA_FORMAT_json)
    {
        pack_json_send();
        return;
    }
    
    ble_tx_buf_len = 0;
    mb2app.battery_level = (uint8_t)get_bat_level();

    //蓝牙连接, 马上发送软件版本号
    if(ble_connected_flag_last == 0 && app2mb.ble_connected_flag == 1)
    {
        send_ver_cnt = 250;
    }
    ble_connected_flag_last = app2mb.ble_connected_flag;

    send_ver_cnt++;
    if(send_ver_cnt > 50)  //发送软件版本号和SN 号
    {
        send_ver_cnt = 0;

        ble_tx_buf[ble_tx_buf_len++] = UART_SOFTWART_VER_HEAD_1;
        ble_tx_buf[ble_tx_buf_len++] = UART_SOFTWART_VER_HEAD_2;

        temp_len = sizeof(SOFTWART_VER) - 1;
        memcpy(&ble_tx_buf[ble_tx_buf_len], SOFTWART_VER, temp_len);
        ble_tx_buf_len += temp_len;

        //SN 号
        if((device_SN_buf[0] == 'H') && 
            (device_SN_buf[1] == 'X') && 
            (device_SN_buf[2] == 'O'))
        {
            ble_tx_buf[ble_tx_buf_len++] = ',';
            memcpy(&ble_tx_buf[ble_tx_buf_len], device_SN_buf, DEVICE_SN_LEN);
            ble_tx_buf_len += DEVICE_SN_LEN;
        }
    }
    else
    {
        //左髋的角度，放大100倍
        temp.data_int16 = (int16_t)(mb2app.left_angle*100);
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];
        
        //左髋的速度，放大100倍
        temp.data_int16 = (int16_t)(mb2app.left_speed*100);
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

        //左髋的力矩，放大100倍
        temp.data_int16 = (int16_t)(mb2app.left_torque*100);
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

        //右髋的角度，放大100倍
        temp.data_int16 = (int16_t)(mb2app.right_angle*100);
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];
        
        //右髋的速度，放大100倍
        temp.data_int16 = (int16_t)(mb2app.right_speed*100);
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

        //右髋的力矩，放大100倍
        temp.data_int16 = (int16_t)(mb2app.right_torque*100);
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

        //剩余电量等级
        ble_tx_buf[ble_tx_buf_len++] = mb2app.battery_level;

        //行走步数
        temp.data_int16 = (int16_t)(walk.steps);
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

        //行走时间
        temp.data_int16 = (int16_t)(HAL_GetTick()/1000);
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

        //Mess，位操作
        if(walk.slip_flag == 0)
        {
            BIT_CLR(mess_flag, 0);
        }
        else
        {
            BIT_SET(mess_flag, 0);
        }

        ble_tx_buf[ble_tx_buf_len++] = mess_flag;
    }

    //发送数据
    ble_packing_buf_len = packing_data(ble_packing_buf, ble_tx_buf, ble_tx_buf_len);
    ble_send_data(ble_packing_buf, ble_packing_buf_len);
    
}


//填充BLE 的buf, 并发送evaluation 评估数据
static void ble_evaluation_data_send(void)
{
    union int16_type temp;
    float** ptr_buf = NULL;

    uint16_t i = 0;

    ble_tx_buf_len = 0;
    ble_tx_buf[ble_tx_buf_len++] = UART_EVALUATION_HEAD_1;
    ble_tx_buf[ble_tx_buf_len++] = UART_EVALUATION_HEAD_2;
    
    if(gait_data_all.complete_per < 1.0)  //发送评估进度
    {
        evaluation_fsm = E_SEND_FSM_PROGRESS_BAR;

        //fsm
        ble_tx_buf[ble_tx_buf_len++] = evaluation_fsm;

        //评估进度
        ble_tx_buf[ble_tx_buf_len++] = (uint8_t)(gait_data_all.complete_per*100);
    }
    else
    {
        if((evaluation_fsm >= E_SEND_FSM_FORM_DATA_BEGIN) && 
            (evaluation_fsm <= E_SEND_FSM_FORM_DATA_END))  //发送表格里面的数据
        {
            ptr_buf = (float**)(ptr_evaluation[evaluation_fsm - 1]);

            //fsm
            ble_tx_buf[ble_tx_buf_len++] = evaluation_fsm;

            //左腿(均值、标准差、最大值、最小值)、放大100倍, 保留2 位小数
            temp.data_int16 = (int16_t)(ptr_buf[0][LEFT_HIP]*100);
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

            temp.data_int16 = (int16_t)(ptr_buf[1][LEFT_HIP]*100);
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

            temp.data_int16 = (int16_t)(ptr_buf[2][LEFT_HIP]*100);
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

            temp.data_int16 = (int16_t)(ptr_buf[3][LEFT_HIP]*100);
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

            //右腿(均值、标准差、最大值、最小值)、放大100倍, 保留2 位小数
            temp.data_int16 = (int16_t)(ptr_buf[0][RIGHT_HIP]*100);
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

            temp.data_int16 = (int16_t)(ptr_buf[1][RIGHT_HIP]*100);
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

            temp.data_int16 = (int16_t)(ptr_buf[2][RIGHT_HIP]*100);
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

            temp.data_int16 = (int16_t)(ptr_buf[3][RIGHT_HIP]*100);
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];
        }
        else if(evaluation_fsm == E_SEND_FSM_DIFF_DATA)  //发送差异分析数据
        {
            //fsm
            ble_tx_buf[ble_tx_buf_len++] = evaluation_fsm;

            //差异(左右髋抬腿角度、左右髋抬腿速度、摆动期时长)
            temp.data_int16 = (int16_t)(gait_data_all.diff_angle*100);
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

            temp.data_int16 = (int16_t)(gait_data_all.diff_vel*100);
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

            temp.data_int16 = (int16_t)(gait_data_all.diff_support_time*100);
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
            ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];
        }
        else if(evaluation_fsm == E_SEND_FSM_CURVE_DATA)  //发送曲线数据
        {
            //fsm
            ble_tx_buf[ble_tx_buf_len++] = evaluation_fsm;

            //左腿或右腿(1 个字节)
            ble_tx_buf[ble_tx_buf_len++] = e_leg_id;

            //数据包总量, 数据包= 数据/ 50, (1 个字节)
            ble_tx_buf[ble_tx_buf_len++] = e_total_pkg;

            //当前传输的数据包索引(1 个字节)
            ble_tx_buf[ble_tx_buf_len++] = e_cur_pkg;

            //数据(每次传输50 个float)
            if(e_cur_pkg < e_total_pkg)
            {
                for(i = 0; i < EVALUATION_PKG_LEN; i++)
                {
                    temp.data_int16 = (int16_t)(gait_data_all.angle_data[e_leg_id][(e_cur_pkg-1)*EVALUATION_PKG_LEN+i]*100);
                    ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
                    ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];
                }
            }
            else
            {
                for(i = ((e_total_pkg-1)*EVALUATION_PKG_LEN); i < gait_data_all.angle_data_length[e_leg_id]; i++)
                {
                    temp.data_int16 = (int16_t)(gait_data_all.angle_data[e_leg_id][i]*100);
                    ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
                    ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];
                }
            }                        
        }
    }

    //发送数据
    ble_packing_buf_len = packing_data(ble_packing_buf, ble_tx_buf, ble_tx_buf_len);
    ble_send_data(ble_packing_buf, ble_packing_buf_len);
    
}


//根据通信报文结构，对发送的数据进行打包
//param 1 : 打包后保存的数据buf
//param 2 : 打包前的数据buf
//param 3 : 打包前数据的长度
//返回值: 打包后的数据总字节长度
uint8_t packing_data(uint8_t* packing_buf, uint8_t* buf, uint8_t len)
{
    uint8_t index = 0;
    uint8_t i = 0;
    uint8_t check_sum = 0;

    //帧头
    packing_buf[index++] = UART_FRAME_HEAD_1;
    packing_buf[index++] = UART_FRAME_HEAD_2;

    //数据帧长度
    packing_buf[index++] = (4 + len);

    //用户数据
    for(i = 0; i < len; i++)
    {
        packing_buf[index++] = buf[i];
    }

    //校验值
    for(i = 0; i < index; i++)
    {
        check_sum += packing_buf[i];
    }
    packing_buf[index++] = check_sum;

    return index;
    
}


//根据USART通信报文结构，对接收的数据进行解包
//param 1 : 解包后数据的存放buf
//param 2 : 解包前的原始数据
//param 3 : 解包前的原始数据的长度
//返回值: 出错为0 、解包后的有效数据总字节长度
uint8_t unpacking_data(uint8_t* unpacking_buf, uint8_t* buf, uint8_t len)
{
    uint8_t index = 0;
    uint8_t i = 0;
    uint8_t check_sum = 0;

    if(len < 4)
    {
        return 0;
    }

    while((buf[index] != UART_FRAME_HEAD_1) || (buf[index+1] != UART_FRAME_HEAD_2))
    {
        index++;
        if(index >= (len - 4))
        {
            return 0;
        }
    }
    
    for(i = 0; i < (buf[index+2] - 1); i++)//buf[index+2]是这一数据帧的总长度，计算check_sum要减一
    {
        check_sum += buf[index+i];
    }

    if(check_sum == buf[index+i])//checkSum正确
    {
        //取出有效数据
        for(i = 0; i < (buf[index+2] - 4); i++)
        {
            unpacking_buf[i] = buf[index + 3 + i];
        }
        return (buf[index+2] - 4);
    }

    return 0;
}


//蓝牙模块检测及初始化
static void ble_init(void)
{
    char* ret_ptr = NULL;

    //低电平： 配置模式
    BLE_MODE_CONFIG();
    osDelay(200);

    //设置波特率为BAUD_921600
    ble_set_baud(BAUD_921600);
    osDelay(200);

    //重新设置STM32 的串口波特率
    uart5_init(921600);
    osDelay(200);

    //设置蓝牙模块的名字为: MileBot, 默认出厂名字: E104-BT5032A
    uart5_dma_rx_buf_len = 0;
    memset(uart5_dma_rx_buf_raw, 0x00, UART5_DMA_BUF_LEN);
    ble_read_name();
    osDelay(500);
    ret_ptr = strstr((char*)uart5_dma_rx_buf_raw, "MileBot");
    if(ret_ptr == NULL)
    {
        uart5_dma_rx_buf_len = 0;
        memset(uart5_dma_rx_buf_raw, 0x00, UART5_DMA_BUF_LEN);
        ble_read_name();
        osDelay(500);
        ret_ptr = strstr((char*)uart5_dma_rx_buf_raw, "MileBot");
        if(ret_ptr == NULL)
        {
            //修改蓝牙模块的名字为: MileBot
            ble_set_name();
            osDelay(200);
        }
    }


    //高电平： 透传模式
    BLE_MODE_TRANSMIT();

    //清空buf
    uart5_dma_rx_buf_len = 0;
    memset(uart5_dma_rx_buf_raw, 0x00, UART5_DMA_BUF_LEN);
    
}

//蓝牙模块的波特率AT+BAUD=10
static void ble_set_baud(uint8_t baud)
{
    uint8_t buf[12] = "AT+BAUD=";
    if(baud >= 10)
    {
        buf[8] = baud/10 + '0';
        buf[9] = baud%10 + '0';
        ble_tx_buf_len = 10;
    }
    else
    {
        buf[8] = baud + '0';
        ble_tx_buf_len = 9;
    }
    
    memcpy(ble_tx_buf, buf, ble_tx_buf_len);
    ble_send_data(ble_tx_buf, ble_tx_buf_len);
}

//蓝牙模块读取名字
static void ble_read_name(void)
{
    ble_tx_buf_len = sizeof("AT+NAME?") - 1;
    memcpy(ble_tx_buf, "AT+NAME?", ble_tx_buf_len);
    
    ble_send_data(ble_tx_buf, ble_tx_buf_len);
}

//修改蓝牙模块的名字为: MileBot
//该指令执行后, 须重新上电, 新设置的参数才能生效
static void ble_set_name(void)
{
    ble_tx_buf_len = sizeof("AT+NAME=MileBot") - 1;
    memcpy(ble_tx_buf, "AT+NAME=MileBot", ble_tx_buf_len);

    ble_send_data(ble_tx_buf, ble_tx_buf_len);
}

//蓝牙发送数据接口函数
static void ble_send_data(uint8_t *pbuf, uint16_t size)
{
    uart5_dma_send_data(pbuf, size);
}

/****************************** END OF FILE ***********************************/

