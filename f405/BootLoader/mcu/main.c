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

    //Bootloader ������ƫ����
    SCB->VTOR = BootLoader_ADDRESS;
    
    HAL_Init();
    mcu_system_clk_config();
    mcu_io_init_reset();
    
    //mcu_init();  //����������ִ��, �����ٵ���

    //��ȡ������������
    read_internal_flash(Parameters_ADDRESS, (uint8_t*)(&parameter), 4);
    if(parameter == PARAM_PRE_UPGRADE)
    {
        //����ǰ, �رյ����Դ
        LMOTOR_POWER_disable();
        RMOTOR_POWER_disable();
        
        //����APP ����, ÿ������128K, Ϊ�˽�ʡʱ��, ֻ����һ������
        //����̼�����128K, ���в����������
        erase_sector(Application_ADDRESS, 2);

        //���Application ���Ѿ�������
        uint32_t data = PARAM_ERASE_APP;
        write_internal_flash(Parameters_ADDRESS, (uint8_t*)(&data), 4);

        //�������Ƿ���ȷд��
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
        //�������Ƿ���ȷд��
      
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
        //��ת��Application��������
        iap_jump(Application_ADDRESS);
    }
    else
    {
        //��ת��Backup��������
        iap_jump(Backup_ADDRESS);
    }
}
/****************************** END OF FILE ***********************************/

