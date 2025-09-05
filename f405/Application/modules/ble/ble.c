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
    ģ�����Ĭ�����ã�

    ������: E104-BT5032A: 115200, N, 8, 1, ��ģʽ
    ������MTU(����䵥Ԫ, ÿ������) ��� 247bytes
        
    //������: HMSoft: 9600, N, 8, 1, ��ģʽ, iBeacon �ر�, ������, ͸��+Զ��ģʽ
*/

/*
    ���ݴ���ͽ��˵��:
    
    ͨ��"���ڵ�������" �·����ݸ������˵��(ʮ������)��
    ÿ�����Ĺ��������²��֣�֡ͷ������֡���ȡ��û����ݡ�У��ֵ��
    ֡ͷ(2���ֽ�)��0xAA 0x55
    ����֡����(1���ֽ�)������֡ͷ������֡���ȡ��û����ݡ�У��ֵ
    �û�����(������200���ֽ�)�����͵���Ч����
    У��ֵ(1���ֽ�)������֡ͷ������֡���ȡ��û����ݡ������ǵ��ۼ������ĺ�

    ���磺�û�Ҫ����3���ֽڵ���Ч���ݣ���0x11��0x22��0x33
    ���������������ݰ��ǣ�AA 55 07 11 22 33 6C
    ���У�
    ֡ͷ(2���ֽ�)��0xAA 0x55
    ����֡����(1���ֽ�)��0x07
    �û���Ч����(������200���ֽ�)��0x11��0x22��0x33
    У��ֵ(1���ֽ�)��6C  (��Ϊ0xAA + 0x55 + 0x07 + 0x11 + 0x22 + 0x33 = 0x16C��ȡ��8λ������0x6C)
 */


/**** Definition of types ****/ 

//�����������ݵ�FSM
typedef enum
{
    E_SEND_FSM_PROGRESS_BAR = 0,		//������������
    E_SEND_FSM_FORM_DATA_BEGIN = 1,	//���ͱ�����������--��ʼ
    E_SEND_FSM_FORM_DATA_END = 8,		//���ͱ�����������--��β
    E_SEND_FSM_DIFF_DATA = 33,			//���Ͳ����������
    E_SEND_FSM_CURVE_DATA = 55,		//������������
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

//���巢�͸�APP ������
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

//APP ���͸����������
APP2MB_DATA app2mb = 
{
    .parkinson_lift_leg_flag = FLAG_FALSE, 
    .param_cal = FLAG_FALSE, 
    .check_alarm = FLAG_FALSE, 
    .check_sitdown = FLAG_FALSE, 
    .passive_press_flag = FLAG_FALSE, 
};

//����, �����ж����ò����Ƿ��޸�
APP2MB_DATA app2mb_backup;

//(��ֵ����׼����ֵ����Сֵ)
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

//����ģʽ��, ���ݴ�����ɵ�flag
static FLAG_STATUS evaluation_finish_flag = FLAG_FALSE;

//����ģʽ�·������ݵ�fsm
static E_SEND_FSM_INDEX evaluation_fsm = E_SEND_FSM_PROGRESS_BAR;

//��������
static uint8_t e_leg_id = LEFT_HIP;  //����Ϊ0, ����Ϊ1
static uint8_t e_total_pkg = 1;  //���ݰ�����, ���ݰ�= ����/ 50
static uint8_t e_cur_pkg = 1;  //���ݰ�������1 ��ʼ


//��������ÿ�����ݵĳ���
#define EVALUATION_PKG_LEN      10

//BLE ���ݵĸ�ʽ��־
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
    uint32_t ble_reset_tick = 0;  //������ʱ��û�н��յ����ݣ�Ҫ����
    uint8_t ble_send_cnt = 0;
    uint8_t save_data_cnt = 0;

    //static uint16_t total_package = 0;
    static uint16_t current_package = 0;

    static uint8_t ble_send_interval = 5;

    //�ȴ������ȶ�
    osDelay(1000);
    
    //��ʼ������ģ��, �޸Ĳ�����
    ble_init();
    
    while(1) 
    {
        //���ղ���������������
        if((uart5_dma_rx_buf_len > 0) && ((HAL_GetTick() - uart5_rx_data_tick) > 20))
        {
            if((format_flag == FORMAT_uncertain) || (format_flag == FORMAT_binary))
            {
                ble_unpacking_buf_len = unpacking_data(ble_unpacking_buf, uart5_dma_rx_buf_raw, uart5_dma_rx_buf_len);
                if(ble_unpacking_buf_len > 0)
                {
                    format_flag = FORMAT_binary;//�����Ƹ�ʽ
                    app2mb.ble_connected_flag = 1;

                    //�Ƿ�����������֡ͷ
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

            //���ݸ�ʽ��JSON ��ʽ
            if((format_flag == FORMAT_uncertain) || (format_flag == DATA_FORMAT_json))
            {
                if(0 != analyze_JSON_data(uart5_dma_rx_buf_raw, uart5_dma_rx_buf_len))
                {
                    format_flag = DATA_FORMAT_json;//JSON ��ʽ
                    app2mb.ble_connected_flag = 1;

                    analyze_json_set();

                    //SN����������仯, �����±��浽eeprom ��
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
            //BLE ����ģ������
            if(((get_ble_data_flag == FLAG_TRUE) && ((HAL_GetTick() - ble_reset_tick) > 5000)) || 
                ((get_ble_data_flag == FLAG_FALSE) && ((HAL_GetTick() - ble_reset_tick) > 25000)))
            {
                app2mb.ble_connected_flag = 0;

                get_ble_data_flag = FLAG_FALSE;

                ble_reset_tick = HAL_GetTick();
            }
        }

        //�ٴ���������
        if(gait_data_all.complete_per < 1.0)
        {
            evaluation_finish_flag = FLAG_FALSE;
        }

        //�������������������
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

                ble_send_interval = 10;  //����̫��, ble ���ܻ���ֹ���
            }
            else
            {
                ble_fill_data_send();

                if(format_flag == DATA_FORMAT_json)
                {
                    ble_send_interval = 20;  //����̫��, ble ���ܻ���ֹ���
                }
                else
                {
                    ble_send_interval = 10;
                }
            }
        }

        //���APP ���õĲ��������˱仯, �򱣴��²���
        save_data_cnt++;
        if(save_data_cnt >= 200)
        {
            save_data_cnt = 0;

            check_save_data();
        }
        
        osDelay(5);
    }
}

//�ѽ��յ��������json ���ݸ�ֵ 
static void analyze_json_set(void)
{
    static uint8_t temp_lift_side = 0;
    
    app2mb.mode = (MODE_INDEX)(s2c_data.mode - 1);
    app2mb.assist_level = s2c_data.assist;
    
    app2mb.lift_level_left_hip = s2c_data.lift_height;
    app2mb.lift_level_right_hip = app2mb.lift_level_left_hip; //��������һ����

    app2mb.speed_level = s2c_data.speed;

    switch(app2mb.mode)
    {
        case MODE_HEMIPLEGIA:    //ƫ̱ģʽ
            app2mb.hemiplegiaLeg_or_liftLeg = s2c_data.hmp_side;
            break;

        case MODE_PARKINSON:    //����ɭģʽ
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

        case MODE_PASSIVE:        //����ģʽ
            app2mb.hemiplegiaLeg_or_liftLeg = s2c_data.walk_ctrl;
            break;

        default:
            break;
    }
}

//���JSON ���ݲ�����
static void pack_json_send(void)
{
    uint16_t index = 0;
    char* ptr = (char*)ble_tx_buf_json;
    uint8_t len = 0;
#if 0   /* Begin if 0 by lison on 2021-10-15 */
    float float_buf[6] = {0};
#endif  /* End if 0 by lison on 2021-10-15 */
    static uint8_t send_SN_cnt = 0;

    //��ͷ, �������'{'
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

    //��β, �Ҵ�����'}'
    ptr[index++] = '}';

//    //�ַ���������, ��Ϊstrlen()
//    ptr[index++] = '\0';

    ble_send_data((uint8_t*)ptr, index);
    
}

//����������
static void dispose_upgrade_package(void)
{
    //����ǰ, �رյ����Դ
    LMOTOR_POWER_disable();
    RMOTOR_POWER_disable();

    //������������
    uint32_t data = PARAM_PRE_UPGRADE;
    write_internal_flash(Parameters_ADDRESS, (uint8_t*)(&data), 4);

    //�������Ƿ���ȷд��
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
    //�������Ƿ���ȷд��

    //�ظ��յ�׼��������׼, Ȼ����ת��boot
    ble_tx_buf_len = 0;
    ble_tx_buf[ble_tx_buf_len++] = UART_UPGRADE_HEAD_1;
    ble_tx_buf[ble_tx_buf_len++] = UART_UPGRADE_HEAD_2;
    ble_tx_buf[ble_tx_buf_len++] = (0 >> 0);
    ble_tx_buf[ble_tx_buf_len++] = (0 >> 8); 
    
    //��������
    ble_packing_buf_len = packing_data(ble_packing_buf, ble_tx_buf, ble_tx_buf_len);
    ble_send_data(ble_packing_buf, ble_packing_buf_len);
    HAL_Delay(5);

    //��ת��ָ���ĵ�ַ����
    iap_jump(BootLoader_ADDRESS);
}

//����������
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
            //��������
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
            e_leg_id = LEFT_HIP;  //����Ϊ0, ����Ϊ1
            e_cur_pkg = 1;  //���ݰ�������1 ��ʼ
            e_total_pkg = (gait_data_all.angle_data_length[e_leg_id] + (EVALUATION_PKG_LEN - 1))/EVALUATION_PKG_LEN;
        
            evaluation_fsm = E_SEND_FSM_CURVE_DATA;
        }
        else if(evaluation_fsm == E_SEND_FSM_CURVE_DATA)
        {
            if(e_total_pkg == e_cur_pkg)
            {
                if(e_leg_id == LEFT_HIP)
                {
                    e_leg_id = RIGHT_HIP;  //����Ϊ0, ����Ϊ1
                    e_cur_pkg = 1;  //���ݰ�������1 ��ʼ
                    e_total_pkg = (gait_data_all.angle_data_length[e_leg_id] + (EVALUATION_PKG_LEN - 1))/EVALUATION_PKG_LEN;
                }
                else if(e_leg_id == RIGHT_HIP)
                {
                    //���ݴ������
                    evaluation_fsm = E_SEND_FSM_PROGRESS_BAR;  //���¿�ʼ
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
    
    //��ֹ����Խ��
    if(e_cur_pkg < 1)
    {
        e_cur_pkg = 1;
    }

}

//������ͨ���ݰ�
static void dispose_normal_package(void)
{
    //����ͨ��Э�飬��ȡ��ص�����
    app2mb.mode = (MODE_INDEX)ble_unpacking_buf[0];
    app2mb.assist_level = ble_unpacking_buf[1];
    app2mb.lift_level_left_hip = ble_unpacking_buf[2];
    app2mb.lift_level_right_hip = app2mb.lift_level_left_hip; //��������һ����
    
    //Mess��λ����
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


//���APP ���õĲ��������˱仯, �򱣴��²���
static void check_save_data(void)
{        
    //ģʽѡ��
    if(app2mb_backup.mode != app2mb.mode)
    {
        app2mb_backup.mode = app2mb.mode;
        param_buf[FLASH_PARAM_MODE] = app2mb.mode;

        AT24CXX_WriteOneByte(FLASH_PARAM_MODE, param_buf[FLASH_PARAM_MODE]);

    }

    //�����ȼ�(1��2��3��4��5)
    if(app2mb_backup.assist_level != app2mb.assist_level)
    {
        app2mb_backup.assist_level = app2mb.assist_level;
        param_buf[FLASH_PARAM_ASSIST] = app2mb.assist_level;
        
        AT24CXX_WriteOneByte(FLASH_PARAM_ASSIST, param_buf[FLASH_PARAM_ASSIST]);

    }

    //̧�ȸ߶�--����(1��2��3)
    if(app2mb_backup.lift_level_left_hip != app2mb.lift_level_left_hip)
    {
        app2mb_backup.lift_level_left_hip = app2mb.lift_level_left_hip;
        param_buf[FLASH_PARAM_LEFT_HIP] = app2mb.lift_level_left_hip;

        AT24CXX_WriteOneByte(FLASH_PARAM_LEFT_HIP, param_buf[FLASH_PARAM_LEFT_HIP]);

    }

    //̧�ȸ߶�--����(1��2��3)
    if(app2mb_backup.lift_level_right_hip != app2mb.lift_level_right_hip)
    {
        app2mb_backup.lift_level_right_hip = app2mb.lift_level_right_hip;
        param_buf[FLASH_PARAM_RIGHT_HIP] = app2mb.lift_level_right_hip;

        AT24CXX_WriteOneByte(FLASH_PARAM_RIGHT_HIP, param_buf[FLASH_PARAM_RIGHT_HIP]);

    }

    //ƫ̱ģʽ: 1��ࡢ2�Ҳ�
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

    //�����ȼ�(0��1��2��3��4��5��6��7��8��9��10)
    if(app2mb_backup.voice_level != app2mb.voice_level)
    {
        app2mb_backup.voice_level = app2mb.voice_level;
        param_buf[FLASH_PARAM_VOICE] = app2mb.voice_level;

        AT24CXX_WriteOneByte(FLASH_PARAM_VOICE, param_buf[FLASH_PARAM_VOICE]);

    }
    
    //�ٶȵȼ�(1��2��3��4��5)
    if(app2mb_backup.speed_level != app2mb.speed_level)
    {
        app2mb_backup.speed_level = app2mb.speed_level;
        param_buf[FLASH_PARAM_SPEED] = app2mb.speed_level;

        AT24CXX_WriteOneByte(FLASH_PARAM_SPEED, param_buf[FLASH_PARAM_SPEED]);

    }

    //ˤ��������־(0��1)
    if(app2mb_backup.check_alarm != app2mb.check_alarm)
    {
        app2mb_backup.check_alarm = app2mb.check_alarm;
        param_buf[FLASH_PARAM_CHECK_ALARM] = app2mb.check_alarm;

        AT24CXX_WriteOneByte(FLASH_PARAM_CHECK_ALARM, param_buf[FLASH_PARAM_CHECK_ALARM]);

    }

    //���¼���־(0��1)
    if(app2mb_backup.check_sitdown != app2mb.check_sitdown)
    {
        app2mb_backup.check_sitdown = app2mb.check_sitdown;
        param_buf[FLASH_PARAM_CHECK_SITDOWN] = app2mb.check_sitdown;

        AT24CXX_WriteOneByte(FLASH_PARAM_CHECK_SITDOWN, param_buf[FLASH_PARAM_CHECK_SITDOWN]);

    }
}

//��flash �б���Ĳ�����ֵ��app2mb
void set_app2mb_from_flash(void)
{
    uint8_t temp = 0;
    
    //ģʽѡ��
    temp = param_buf[FLASH_PARAM_MODE];
    if(temp < MODE_TOTAL)
    {
        app2mb.mode = (MODE_INDEX)temp;
    }
    else
    {
        app2mb.mode = MODE_NORMAL;
    }

    //�����ȼ�(1��2��3��4��5)
    temp = param_buf[FLASH_PARAM_ASSIST];
    if((temp >= 1) && (temp <= 5))
    {
        app2mb.assist_level = temp;
    }
    else
    {
        app2mb.assist_level = 3;
    }

    //̧�ȸ߶�--����(1��2��3)
    temp = param_buf[FLASH_PARAM_LEFT_HIP];
    if(temp <= 3)
    {
        app2mb.lift_level_left_hip = temp;
    }
    else
    {
        app2mb.lift_level_left_hip = 1;
    }

    //̧�ȸ߶�--����(1��2��3)
    temp = param_buf[FLASH_PARAM_RIGHT_HIP];
    if(temp <= 3)
    {
        app2mb.lift_level_right_hip = temp;
    }
    else
    {
        app2mb.lift_level_right_hip = 1;
    }

    //ƫ̱ģʽ: 1��ࡢ2�Ҳ�
    if(app2mb.mode == MODE_HEMIPLEGIA)  //ֻ����ƫ̱ģʽ, ����Ҫ��ȡflash ��ֵ
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

    //�����ȼ�(0��1��2��3��4��5��6��7��8��9��10)
    temp = param_buf[FLASH_PARAM_VOICE];
    if(temp <= 10)
    {
        app2mb.voice_level = temp;
    }
    else
    {
        app2mb.voice_level = 3;
    }

    //�ٶȵȼ�(1��2��3��4��5)
    temp = param_buf[FLASH_PARAM_SPEED];
    if((temp >= 1) && (temp <= 5))
    {
        app2mb.speed_level = temp;
    }
    else
    {
        app2mb.speed_level = 1;
    }

    //ˤ��������־(0��1)
    temp = param_buf[FLASH_PARAM_CHECK_ALARM];
    if(temp <= 1)
    {
        app2mb.check_alarm = (FLAG_STATUS)temp;
    }
    else
    {
        app2mb.check_alarm = (FLAG_STATUS)0;
    }

    //���¼���־(0��1)
    temp = param_buf[FLASH_PARAM_CHECK_SITDOWN];
    if(temp <= 1)
    {
        app2mb.check_sitdown = (FLAG_STATUS)temp;
    }
    else
    {
        app2mb.check_sitdown = (FLAG_STATUS)0;
    }

    //���ݽṹ��ĸ�ֵ
    app2mb_backup = app2mb;
    
}

//���BLE ��buf, ������
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

    //��������, ���Ϸ�������汾��
    if(ble_connected_flag_last == 0 && app2mb.ble_connected_flag == 1)
    {
        send_ver_cnt = 250;
    }
    ble_connected_flag_last = app2mb.ble_connected_flag;

    send_ver_cnt++;
    if(send_ver_cnt > 50)  //��������汾�ź�SN ��
    {
        send_ver_cnt = 0;

        ble_tx_buf[ble_tx_buf_len++] = UART_SOFTWART_VER_HEAD_1;
        ble_tx_buf[ble_tx_buf_len++] = UART_SOFTWART_VER_HEAD_2;

        temp_len = sizeof(SOFTWART_VER) - 1;
        memcpy(&ble_tx_buf[ble_tx_buf_len], SOFTWART_VER, temp_len);
        ble_tx_buf_len += temp_len;

        //SN ��
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
        //���ŵĽǶȣ��Ŵ�100��
        temp.data_int16 = (int16_t)(mb2app.left_angle*100);
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];
        
        //���ŵ��ٶȣ��Ŵ�100��
        temp.data_int16 = (int16_t)(mb2app.left_speed*100);
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

        //���ŵ����أ��Ŵ�100��
        temp.data_int16 = (int16_t)(mb2app.left_torque*100);
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

        //���ŵĽǶȣ��Ŵ�100��
        temp.data_int16 = (int16_t)(mb2app.right_angle*100);
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];
        
        //���ŵ��ٶȣ��Ŵ�100��
        temp.data_int16 = (int16_t)(mb2app.right_speed*100);
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

        //���ŵ����أ��Ŵ�100��
        temp.data_int16 = (int16_t)(mb2app.right_torque*100);
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

        //ʣ������ȼ�
        ble_tx_buf[ble_tx_buf_len++] = mb2app.battery_level;

        //���߲���
        temp.data_int16 = (int16_t)(walk.steps);
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

        //����ʱ��
        temp.data_int16 = (int16_t)(HAL_GetTick()/1000);
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[0];
        ble_tx_buf[ble_tx_buf_len++] = temp.data_buf[1];

        //Mess��λ����
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

    //��������
    ble_packing_buf_len = packing_data(ble_packing_buf, ble_tx_buf, ble_tx_buf_len);
    ble_send_data(ble_packing_buf, ble_packing_buf_len);
    
}


//���BLE ��buf, ������evaluation ��������
static void ble_evaluation_data_send(void)
{
    union int16_type temp;
    float** ptr_buf = NULL;

    uint16_t i = 0;

    ble_tx_buf_len = 0;
    ble_tx_buf[ble_tx_buf_len++] = UART_EVALUATION_HEAD_1;
    ble_tx_buf[ble_tx_buf_len++] = UART_EVALUATION_HEAD_2;
    
    if(gait_data_all.complete_per < 1.0)  //������������
    {
        evaluation_fsm = E_SEND_FSM_PROGRESS_BAR;

        //fsm
        ble_tx_buf[ble_tx_buf_len++] = evaluation_fsm;

        //��������
        ble_tx_buf[ble_tx_buf_len++] = (uint8_t)(gait_data_all.complete_per*100);
    }
    else
    {
        if((evaluation_fsm >= E_SEND_FSM_FORM_DATA_BEGIN) && 
            (evaluation_fsm <= E_SEND_FSM_FORM_DATA_END))  //���ͱ�����������
        {
            ptr_buf = (float**)(ptr_evaluation[evaluation_fsm - 1]);

            //fsm
            ble_tx_buf[ble_tx_buf_len++] = evaluation_fsm;

            //����(��ֵ����׼����ֵ����Сֵ)���Ŵ�100��, ����2 λС��
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

            //����(��ֵ����׼����ֵ����Сֵ)���Ŵ�100��, ����2 λС��
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
        else if(evaluation_fsm == E_SEND_FSM_DIFF_DATA)  //���Ͳ����������
        {
            //fsm
            ble_tx_buf[ble_tx_buf_len++] = evaluation_fsm;

            //����(������̧�ȽǶȡ�������̧���ٶȡ��ڶ���ʱ��)
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
        else if(evaluation_fsm == E_SEND_FSM_CURVE_DATA)  //������������
        {
            //fsm
            ble_tx_buf[ble_tx_buf_len++] = evaluation_fsm;

            //���Ȼ�����(1 ���ֽ�)
            ble_tx_buf[ble_tx_buf_len++] = e_leg_id;

            //���ݰ�����, ���ݰ�= ����/ 50, (1 ���ֽ�)
            ble_tx_buf[ble_tx_buf_len++] = e_total_pkg;

            //��ǰ��������ݰ�����(1 ���ֽ�)
            ble_tx_buf[ble_tx_buf_len++] = e_cur_pkg;

            //����(ÿ�δ���50 ��float)
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

    //��������
    ble_packing_buf_len = packing_data(ble_packing_buf, ble_tx_buf, ble_tx_buf_len);
    ble_send_data(ble_packing_buf, ble_packing_buf_len);
    
}


//����ͨ�ű��Ľṹ���Է��͵����ݽ��д��
//param 1 : ����󱣴������buf
//param 2 : ���ǰ������buf
//param 3 : ���ǰ���ݵĳ���
//����ֵ: �������������ֽڳ���
uint8_t packing_data(uint8_t* packing_buf, uint8_t* buf, uint8_t len)
{
    uint8_t index = 0;
    uint8_t i = 0;
    uint8_t check_sum = 0;

    //֡ͷ
    packing_buf[index++] = UART_FRAME_HEAD_1;
    packing_buf[index++] = UART_FRAME_HEAD_2;

    //����֡����
    packing_buf[index++] = (4 + len);

    //�û�����
    for(i = 0; i < len; i++)
    {
        packing_buf[index++] = buf[i];
    }

    //У��ֵ
    for(i = 0; i < index; i++)
    {
        check_sum += packing_buf[i];
    }
    packing_buf[index++] = check_sum;

    return index;
    
}


//����USARTͨ�ű��Ľṹ���Խ��յ����ݽ��н��
//param 1 : ��������ݵĴ��buf
//param 2 : ���ǰ��ԭʼ����
//param 3 : ���ǰ��ԭʼ���ݵĳ���
//����ֵ: ����Ϊ0 ����������Ч�������ֽڳ���
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
    
    for(i = 0; i < (buf[index+2] - 1); i++)//buf[index+2]����һ����֡���ܳ��ȣ�����check_sumҪ��һ
    {
        check_sum += buf[index+i];
    }

    if(check_sum == buf[index+i])//checkSum��ȷ
    {
        //ȡ����Ч����
        for(i = 0; i < (buf[index+2] - 4); i++)
        {
            unpacking_buf[i] = buf[index + 3 + i];
        }
        return (buf[index+2] - 4);
    }

    return 0;
}


//����ģ���⼰��ʼ��
static void ble_init(void)
{
    char* ret_ptr = NULL;

    //�͵�ƽ�� ����ģʽ
    BLE_MODE_CONFIG();
    osDelay(200);

    //���ò�����ΪBAUD_921600
    ble_set_baud(BAUD_921600);
    osDelay(200);

    //��������STM32 �Ĵ��ڲ�����
    uart5_init(921600);
    osDelay(200);

    //��������ģ�������Ϊ: MileBot, Ĭ�ϳ�������: E104-BT5032A
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
            //�޸�����ģ�������Ϊ: MileBot
            ble_set_name();
            osDelay(200);
        }
    }


    //�ߵ�ƽ�� ͸��ģʽ
    BLE_MODE_TRANSMIT();

    //���buf
    uart5_dma_rx_buf_len = 0;
    memset(uart5_dma_rx_buf_raw, 0x00, UART5_DMA_BUF_LEN);
    
}

//����ģ��Ĳ�����AT+BAUD=10
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

//����ģ���ȡ����
static void ble_read_name(void)
{
    ble_tx_buf_len = sizeof("AT+NAME?") - 1;
    memcpy(ble_tx_buf, "AT+NAME?", ble_tx_buf_len);
    
    ble_send_data(ble_tx_buf, ble_tx_buf_len);
}

//�޸�����ģ�������Ϊ: MileBot
//��ָ��ִ�к�, �������ϵ�, �����õĲ���������Ч
static void ble_set_name(void)
{
    ble_tx_buf_len = sizeof("AT+NAME=MileBot") - 1;
    memcpy(ble_tx_buf, "AT+NAME=MileBot", ble_tx_buf_len);

    ble_send_data(ble_tx_buf, ble_tx_buf_len);
}

//�����������ݽӿں���
static void ble_send_data(uint8_t *pbuf, uint16_t size)
{
    uart5_dma_send_data(pbuf, size);
}

/****************************** END OF FILE ***********************************/

