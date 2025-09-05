/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*				  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : can_app.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : can application function.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/
#include "cmsis_os2.h"

/* Public Includes -----------------------------------------------------------*/
#include "..\config\walk_config.h"
#include "..\power\power_mana_if.h"
#include ".\canopen\CANopen.h"
#include "stm32f4xx_hal.h"
#include "..\mcu\tim_if.h"
#include "..\\system\abi\abi.h"
#include "..\math\pprz_algebra_int.h"
#include "..\..\mcu\adc_if.h"
//#include "..\sensor\encoder_if.h"
#include "..\..\system\timer_if.h"
#include "..\..\mcu\uart_if.h"
#include "..\..\mcu\can_if.h"
#include "string.h"
/* Private Includes ----------------------------------------------------------*/
#include "..\modules\can\can_app.h"
#include "..\modules\can\can_app_if.h"
#include "..\modules\can\can_motion_control_if.h"
#include "..\modules\can\canopen\data_model.h"
/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
//static abi_event can_euleri_ev;
//static abi_event can_enc_inc_ev;
//static abi_event can_enc_abs_ev;
//static abi_event can_enc_tawa_ev;
//static abi_event can_motion_send_ev;

 static PDO_MAPPING_STATE_MACHINE tpdo_fsm = DISABLE_PDO;
 static PDO_MAPPING_STATE_MACHINE rpdo_fsm = DISABLE_PDO;
 static CIA402_MASTER_FSM can_fsm = CIA_MASTER_Init;
static CANOPEN_CMD_INFO canopen_command= {0};
static CIA402_MB2DRIVER_ABI can_send ;
static SERVO_CMD servo_fsm = SERVO_INIT;
/* Global functions ----------------------------------------------------------*/
/**
  * @brief  task to handle all can application function.
  * @param  None
  * @retval None
  */
void can_task(void *argument)
{  
	CO_NMT_reset_cmd_t co_reset = CO_RESET_NOT;
	static uint32_t last_stamp = 0;

	can_app_program_start();
//  AbiBindMsgCanSend(ABI_BROADCAST, &can_motion_send_ev, can_motion_send_cb);

	while(co_reset != CO_RESET_APP)
	{
		/* CANopen communication reset - initialize CANopen objects *******************/

		can_app_comm_init();
		//HAL_GPIO_WritePin(CAN1_TX_STB_port, CAN1_TX_STB_pin, GPIO_PIN_RESET);/*can stb low*/
		/* start CAN */
		CO_CANsetNormalMode(COO.CANmodule[0]);

		co_reset = CO_RESET_NOT;
		cia_set_mode(&can_send,INTERPOLATE_POSITION);
		while(co_reset == CO_RESET_NOT)
		{
			/* loop for normal program execution ******************************************/
			uint32_t now_ts = get_sys_time_usec();
			int32_t dt;
			dt = now_ts - last_stamp;
			sys_time_usec_ctl_overflow(&dt, CAN_TASK_SLEEEP_TIME*1000+2000);	/* 12ms */
			last_stamp = now_ts;

			/*---------------------can_master_fsm-----------------------*/
			switch(can_fsm)//error 注册还没加
			{
				case CIA_MASTER_Init://初始化从机  ，使从机进入pre-op,初始化映射
					if(tpdo_fsm!= MAP_DONE)
					{
						CIA_PDO_MAPPING(canopen_command,&tpdo_fsm,10,TPDO1,3,0x60c1,0x60c1,0x6040);
					}  
					else if(rpdo_fsm != MAP_DONE)
					{
						CIA_PDO_MAPPING(canopen_command,&rpdo_fsm,10,RPDO1,2,0x6064,0x6041,0);
					}
					else
					{
						can_app_start_node1();//发送nmt指令使从机进入operation
						osDelay(10);
						can_fsm = CIA_MASTER_PreOperation;//TBD  应该要加上收到从机的05后  主机切换状态
					}
					break;
				case CIA_MASTER_PreOperation://检查主从机状态，没问题就operation
					can_fsm = CIA_MASTER_Operation;
					break;
				case CIA_MASTER_Operation://开始跑pdo  ,跑算法
					co_reset = can_app_program_async(dt/1000);
					cia_motion_run(&can_send,&servo_fsm);
					break;
				case MASTER_ERROR:
					break;
			}
			/*---------------------------------------------------*/      
#ifdef POWER_WATCHDOG_OPTION
			pw_set_can_task_watchdog_flag();
#endif/* POWER_WATCHDOG_OPTION */
			osDelay(CAN_TASK_SLEEEP_TIME);
		}

		osDelay(CAN_TASK_SLEEEP_TIME);
    }
}
#if CO_NO_NMT_MASTER == 1
    void can_app_start_node1(void)
    {
        CO_sendNMTcommand(&COO, CO_NMT_ENTER_OPERATIONAL, 0x06);
    }
    #if CO_NO_CAN_MODULES>=2
    void can_app_start_node2(void)
    {
        CO_sendNMT2command(&COO, CO_NMT_ENTER_OPERATIONAL, 0x0A);
    }
    #endif
#endif

int  sdo_client_send(CO_SDOclient_t *SDOclient,
        uint8_t         nodeID,
        uint16_t        idx,
        uint8_t         subidx,
        uint8_t        *dataRx,
        uint32_t        dataRxSize,
        uint32_t       *dataRxLen,
        uint32_t       *SDOabortCode,
        uint16_t        SDOtimeoutTime,
        uint8_t         blockTransferEnable)
{
    
    int err = 0;
//       /* Setup client. */
//    if(CO_SDOclient_setup(SDOclient, 0, 0, nodeID) != CO_SDOcli_ok_communicationEnd) {
//        err = 1;
//    }

    /* Initiate upload. */
    if(err == 0){
        if(CO_SDOclientUploadInitiate(SDOclient, idx, subidx, dataRx,
                dataRxSize, blockTransferEnable) != CO_SDOcli_ok_communicationEnd)
        {
            err = 1;
        }
    }
    
    /* Upload data. Loop in 10 ms intervals. */
    if(err == 0){
        CO_SDOclient_return_t ret;
            do {
            ret = CO_SDOclientUpload(SDOclient, 10, SDOtimeoutTime, dataRxLen, SDOabortCode);
           osDelay(1);
        }while(ret > 0);

        CO_SDOclientClose(SDOclient);
    }
        return err;
}
/**
  * @brief  tpdo and rpdo handle for canopen.
  * @param  None
  * @retval None
  */
void can_app_pdo_handle(void)
{
	if(COO.CANmodule[0]->CANnormal | COO.CANmodule[1]->CANnormal)
	{
	    bool_t syncWas;

	    /* Process Sync and read inputs */
	    syncWas = CO_process_SYNC_RPDO(&COO, CO_TMR_TASK_INTERVAL);
	    /* Further I/O or nonblocking application code may go here. */
		
	    /* Write outputs */
	    CO_process_TPDO(&COO, syncWas, CO_TMR_TASK_INTERVAL);
	    /* verify timer overflow */
	    if(0)
		{
	        CO_errorReport(COO.em, CO_EM_ISR_TIMER_OVERFLOW, CO_EMC_SOFTWARE_INTERNAL, 0U);
	    }
	}
}

/**
  * @brief  canopen RPDO realtime process.
  * @param  None
  * @retval None
  */
void can_app_rpdo_process(uint8_t param, uint16_t *msg)
{
//	CAN_SENSOR_ABI can_sensor_abi;
//	can_sensor_abi.msg = msg;
//	AbiSendMsgCanReceive(CAN_MOTION_RECEIVE_ID, param, &can_sensor_abi);
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Called after mcu reset.
  * @param  None
  * @retval None
  */
void can_app_program_start(void)
{
#if 0
    /* Verify, if OD structures have proper alignment of initial values */
    if (CO_OD_RAM.FirstWord != CO_OD_RAM.LastWord)
        {
            while(1){}
        }
    if (CO_OD_EEPROM.FirstWord != CO_OD_EEPROM.LastWord)
        {
            while(1){}
        }
    if (CO_OD_ROM.FirstWord != CO_OD_ROM.LastWord)
        {
            while(1){}
        }
#endif
    /* increase variable each startup. Variable is stored in eeprom. */
    OD_powerOnCounter++;

}

/**
  * @brief  Called after communication reset.
  * @param  None
  * @retval None
  */
void can_app_comm_init(void)
{
	CO_ReturnError_t err;
	/* initialize CANopen */
    err = CO_init(CAN1, 0x06, 1000);//CIA_NODE_C1_ID CAN_NODE_RATE

    if(err != CO_ERROR_NO)
	{
        CO_errorReport(COO.em, CO_EM_MEMORY_ALLOCATION_ERROR, CO_EMC_SOFTWARE_INTERNAL, err);
    }
}

/**
  * @brief  Called cyclically from main..
  * @param  timer_1ms_diff Time difference since last call
  * @retval None
  */
CO_NMT_reset_cmd_t can_app_program_async(uint16_t timer_1ms_diff)
{
	CO_NMT_reset_cmd_t co_reset = CO_RESET_NOT;
	/* CANopen process */
    co_reset = CO_process(&COO, timer_1ms_diff, NULL);

	return co_reset;
}










/****************************** END OF FILE ***********************************/
