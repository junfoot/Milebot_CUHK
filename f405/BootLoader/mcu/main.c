/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department : R&D SW
********************************************************************************
* File Name   : main.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : Main program body
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/
#include "cmsis_os2.h" 

/* Public Includes -----------------------------------------------------------*/
#include "mcu_if.h"
#include "task_if.h"
#include "internal_flash.h"
#include "iap.h"

/* Private Includes ----------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Global functions ----------------------------------------------------------*/
int main(void)
{
    uint32_t parameter = 0;

    //Bootloader 分区的偏移量
    SCB->VTOR = BootLoader_ADDRESS;
    
    HAL_Init();
    mcu_system_clk_config();
    mcu_io_init_reset();
    
    //mcu_init();  //不能在这里执行, 后面再调用

    //读取参数区的数据
    read_internal_flash(Parameters_ADDRESS, (uint8_t*)(&parameter), 4);
    if(parameter == PARAM_PRE_UPGRADE)
    {
        //升级前, 关闭电机电源
        LMOTOR_POWER_disable();
        RMOTOR_POWER_disable();
        
        //擦除APP 扇区, 每个扇区128K, 为了节省时间, 只擦除一个分区
        //如果固件大于128K, 就有擦除多个分区
        erase_sector(Application_ADDRESS, 2);

        //标记Application 区已经被擦除
        uint32_t data = PARAM_ERASE_APP;
        write_internal_flash(Parameters_ADDRESS, (uint8_t*)(&data), 4);

        //检查参数是否正确写入
        uint8_t cnt = 0;
        HAL_Delay(5);
        read_internal_flash(Parameters_ADDRESS, (uint8_t*)(&data), 4);
        while((data != PARAM_ERASE_APP) && (cnt <= 3))
        {
            cnt++;
            HAL_Delay(5);
            data = PARAM_ERASE_APP;
            write_internal_flash(Parameters_ADDRESS, (uint8_t*)(&data), 4);
        
            HAL_Delay(5);
            read_internal_flash(Parameters_ADDRESS, (uint8_t*)(&data), 4);
        }
        //检查参数是否正确写入
      
        osKernelInitialize();
        task_init_power();
        osKernelStart(); 

        while(1)
        {
            mcu_sw_reset();
        }
    }
    else if(parameter == PARAM_NORMAL)
    {
        //跳转到Application分区运行
        iap_jump(Application_ADDRESS);
    }
    else
    {
        //跳转到Backup分区运行
        iap_jump(Backup_ADDRESS);
    }
}
/****************************** END OF FILE ***********************************/

