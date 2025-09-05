/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : power_mana.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : power management.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/
#include "cmsis_os2.h"                  // ARM::CMSIS:RTOS2:Keil RTX5


/* Public Includes -----------------------------------------------------------*/
#include "main.h"
#include "syn6658.h"

#include "..\system\std.h"
#include "..\os\task_if.h"
#include "..\walk\walk.h"
#include "..\system\sys_task_if.h"
#include "..\mcu\wdg_if.h"
#include "..\mcu\adc_if.h"
#include "..\system\abi\abi.h"
#include "..\system\timer_if.h"
#include "..\system\timer_class.h"
#include "..\system\timer_def.h"
#include "..\modules\bat\bat_if.h"
#include "internal_flash.h"
#include "ble_if.h"  
#include "at24c256.h"
#include <string.h>

/* Private Includes ----------------------------------------------------------*/
#include "..\power\power_mana.h"
#include "..\power\power_mana_if.h"
#include "..\power\power_drv.h"
#include "..\power\power_drv_if.h"

/* Global variables ----------------------------------------------------------*/
static BATTERY_LEVEL_STATUS bat_level = BATTERY_LEVEL_2;
static SYN6658_QUEUE key_queue = 
{
    .head = 0,
    .tail = 0,
};


/* Global functions ----------------------------------------------------------*/
static void bat_level_calc(BATTERY_LEVEL_STATUS* bat_value);
static FLAG_STATUS check_bat_charge(void);
static void key_scan(void);


/**
  * @brief  task to handle all power function .
  * @param  None
  * @retval None
  */
void power_task(void *argument)
{
    FLAG_STATUS bat_charge_flag = FLAG_FALSE;
    uint32_t bat_tick = 0;
    uint32_t led_heartbeat_tick = 0;
    uint16_t delay_cnt = 0;
    FLAG_STATUS alarm_enable_flag = FLAG_TRUE;
    uint32_t alarm_enable_tick = 0;
    
    pw_reset_create();

    //��ȡ�洢��EEPROM �еĲ���ֵ
    HAL_Delay(10);
    AT24CXX_detect();//���EEPROM оƬ���ͺ���AT24C16 ����AT24C256
    HAL_Delay(10);
    AT24CXX_Read_Bytes(FLASH_PARAM_MOTOR_BEGIN, param_buf, PARAM_BUF_LEN);
    HAL_Delay(10);

    //��flash �б����SN ��, ��ֵ��device_SN_buf
    memcpy(device_SN_buf, &param_buf[FLASH_PARAM_SN_BEGIN], DEVICE_SN_LEN);
    device_SN_buf[DEVICE_SN_LEN] = '\0';//����ַ���������

    //��flash �б���Ĳ�����ֵ��app2mb
    set_app2mb_from_flash();
    
    while(1)
    {
        //��ص������
        bat_charge_flag = check_bat_charge();
        
        if((HAL_GetTick() - bat_tick) >= 100)
        {
            bat_tick = HAL_GetTick();

            if(FLAG_FALSE == bat_charge_flag)
            {
                if((HAL_GetTick() - alarm_enable_tick) >= 60000)  //�����ص�����, ÿ���60 �벥��һ��
                {
                    alarm_enable_flag = FLAG_TRUE;
                }

                if(delay_cnt > 1000)
                {
                    bat_level_calc(&bat_level);
                    if(bat_level < BATTERY_LEVEL_2) //��ص����ͣ��뼰ʱ���
                    {
                        if(alarm_enable_flag == FLAG_TRUE)
                        {
                            alarm_enable_tick = HAL_GetTick();
                            alarm_enable_flag = FLAG_FALSE;
                            user_play_sound(SOUND_LOW_BATTERY);
                        }
                    }
                }
            }
            else
            {
                //����ʱ��, �رյ����Դ
                LMOTOR_POWER_disable();
                RMOTOR_POWER_disable();
            }
        }

        if((HAL_GetTick() - led_heartbeat_tick) >= 250)
        {
            led_heartbeat_tick = HAL_GetTick();
            LED_HEARTBEAT_Toggle();

            if(FLAG_FALSE == bat_charge_flag)
            {
                STATLED_enable();
            }
            else
            {
                STATLED_Toggle();
            }
        }

        delay_cnt++;
        if(delay_cnt > 5000)
        {
            delay_cnt = 5000;
        }

        //����״̬ɨ����
        key_scan();

        osDelay(2);
    }
}


//��ȡ������״̬
KEY_STATUS get_key_status(void)
{
    KEY_STATUS key_status = KEY_CLICK_NOT;
    
    syn6658_queue_out(&key_queue, (uint8_t*)(&key_status));
    return key_status;
}

//������ʱɨ�躯��, ��systick �ж�(1ms) �ڲ�����
static void key_scan(void)
{
    static uint32_t pre_time_cnt = 0;
    static uint8_t key_press_cnt = 0;           //���������µ�������������
    static uint8_t key_release_cnt = 0;     //�������ͷŵ�������������

    static KEY_CURRENT_STATUS key_status_flag = KEY_RELEASE;  //�����Ƿ񱻰��µ�flag(����������)
    static uint16_t key_press_hold_on_ms = 0;  //���������µ�ʱ�����ms

    static uint32_t long_click_ms = 1000;

    if(HAL_GetTick() - pre_time_cnt >= KEY_SCAN_TIME)   //ɨ����ʱ��
    {
        pre_time_cnt = HAL_GetTick();

        if(HAL_GPIO_ReadPin(FUNCTION_SWITCH_GPIO_Port, FUNCTION_SWITCH_Pin) == KEY_PIN_PRESS)
        {
            key_release_cnt = 0;
            
            key_press_cnt++;
            if(key_press_cnt >= KEY_SCAN_NUM)      //����������
            {
                //����������
                key_status_flag = KEY_PRESS;

                key_press_cnt = KEY_SCAN_NUM;      //��ֹ�ۼ����
            }
        }
        else
        {
            key_press_cnt = 0;
            
            key_release_cnt++;
            if(key_release_cnt >= KEY_SCAN_NUM)      //����������
            {
                //�������ͷ�
                key_status_flag = KEY_RELEASE;
        
                key_release_cnt = KEY_SCAN_NUM;      //��ֹ�ۼ����
            }
        }
    }

    //��鰴���ǵ���������(1ms ִ��һ������Ĵ���)
    if(key_status_flag == KEY_PRESS)//����������
    {
        if(key_press_hold_on_ms <= long_click_ms)  
        {
            key_press_hold_on_ms++; 
        }
        else
        {
            long_click_ms = 0xFFFFFFFF;  //��ֹ����������, ������γ����¼�
            syn6658_queue_in(&key_queue, (uint8_t)KEY_CLICK_LONG);
        }
    }
    else //����̧��
    {
        long_click_ms = 1000;
        
        if((key_press_hold_on_ms > 25) && (key_press_hold_on_ms < 250))//����ʱ�����50ms�����ɵ�����־
        {  
            syn6658_queue_in(&key_queue, (uint8_t)KEY_CLICK_SHORT);
        }
        else  //��������ʱ��С��50ms������ 
        {
        }

        key_press_hold_on_ms = 0; 
    }
}


//�����ص����ȼ�
 static void bat_level_calc(BATTERY_LEVEL_STATUS* bat_value)
{
    uint16_t bat_vol_mV = 0;

    bat_vol_mV = get_bat_value();
    if(bat_vol_mV > 4000)
    {
        *bat_value = BATTERY_LEVEL_5;
    }
    else if(bat_vol_mV > 3800)
    {
        *bat_value = BATTERY_LEVEL_4;
    }
    else if(bat_vol_mV > 3600)
    {
        *bat_value = BATTERY_LEVEL_3;
    }
    else if(bat_vol_mV > 3400)
    {
        *bat_value = BATTERY_LEVEL_2;
    }
    else if(bat_vol_mV > 3300)
    {
        *bat_value = BATTERY_LEVEL_1;
    }
    else
    {
        *bat_value = BATTERY_LEVEL_0;
    }    
}

//��ȡ�����ȼ�
BATTERY_LEVEL_STATUS get_bat_level(void)
{
    return bat_level;
}


/* Private functions ---------------------------------------------------------*/
void pw_all_modules_reset_create(void)
{
    sys_task_reset_create();
//    comm_spi_init();
}


/**
  * @brief  check_bat_charge
  * @param  None
  * @retval average volt
  */
static FLAG_STATUS check_bat_charge(void)
{
    static uint8_t charge_cnt = 0;
    FLAG_STATUS ret_val = FLAG_FALSE;
    
    if(HAL_GPIO_ReadPin(PWR_CHARGE_DET_GPIO_Port,PWR_CHARGE_DET_Pin) == 0)
    {
        charge_cnt++;
        if(charge_cnt > 50)//�ظ��ж϶��
        {
            charge_cnt = 200;
            ret_val = FLAG_TRUE;
        }
        else
        {
            ret_val = FLAG_FALSE;
        }
    }
    else
    {
        charge_cnt = 0;
        ret_val = FLAG_FALSE;
    }

    return ret_val;
}

/**
  * @brief  handle power reset.
  * @param  None
  * @retval None
  */
static void pw_reset_create(void)
{
    pw_reset_cause_handle();
    pw_power_on();
    osDelay(PW_POWER_SUPPLY_STABLE_TIME);
    mcu_init();
    pw_all_modules_reset_create();
    task_create_all();
}


/****************************** END OF FILE ***********************************/

