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

#define ABI_C

/* Public Includes -----------------------------------------------------------*/
#include "mcu_if.h"
#include "..\os\task_if.h"
#include "..\system\abi\abi.h"
#include "..\power\power_drv_if.h"

#include "internal_flash.h"
#include "iap.h"


/* Private Includes ----------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
static osThreadId_t tid_init_power;
uint8_t testbuf[10] = {0x00};
/* Global functions ----------------------------------------------------------*/
int main(void)
{
    //中断向量表
#if defined APP_PRJ
    //Application 分区的偏移量
    SCB->VTOR = Application_ADDRESS;
#elif defined BACKUP_PRJ
    //Backup 分区的偏移量
    SCB->VTOR = Backup_ADDRESS;
#endif

    HAL_Init();
    mcu_system_clk_config();
    mcu_io_init_reset();

    //mcu_init();  //不能在这里执行, 后面再调用
    pw_check_mcu_reset_source();  

    osKernelInitialize();
    tid_init_power = osThreadNew(task_init_power, NULL, NULL);
    osKernelStart(); 
    osThreadTerminate(tid_init_power);
    
    while(1)
    {
        mcu_sw_reset();
    }
}
/****************************** END OF FILE ***********************************/
