/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*				  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : can_motion_control.c
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : can application function.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     20190707  wyq    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/
#include "cmsis_os2.h"
#include "RTE_Components.h"             // Component selection
/* Public Includes -----------------------------------------------------------*/
#include "..\config\walk_config.h"
#include ".\canopen\CANopen.h"
#include ".\can_app_msg.h"
#include "stm32f4xx_hal.h"
#include ".\can_motion_control_if.h"
#include ".\can_motion_control.h"
#include "..\system\abi\abi.h"

uint8_t actual_mode;
//CAN_SEND_ABI can_send;
CIA402_DRIVER2MB_ABI can_receive = {0};
/* Private Includes ----------------------------------------------------------*/


/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/



/* Global functions ----------------------------------------------------------*/
/**
  * @brief  init pdo mapping by sdo-client-to-server.
  * @param  None
  * @retval None
  */

uint32_t can_app_csdo_tx_err,can_app_csdo_rx_err;
int CIA_PDO_MAPPING(CANOPEN_COM_INFO command,PDO_MAPPING_STATE_MACHINE *cmd ,int diff_ms,PDO_NUM pdo_num,uint8_t subindex,int word1,int word2,int word3)
{
    int error = 0;
    uint8_t can_app_rx_buf[20];
    uint8_t tx_buf[10];
	static CO_SDOclient_return_t sdo_down_ret = CO_SDOcli_ok_communicationEnd;
	static CO_SDOclient_return_t sdo_up_ret = CO_SDOcli_ok_communicationEnd;
	get_pdo_param(&command,pdo_num);
    
	if(sdo_down_ret > 0)
	{
		sdo_down_ret = CO_SDOclientDownload(COO.SDOclient, diff_ms, 10, &can_app_csdo_tx_err);
	}
    switch(*cmd)
    {
        case DISABLE_PDO:
            if(pdo_num == TPDO1)
            {
                tx_buf[0] = CIA_NODE_C1_ID;
                tx_buf[1] = 0x02;
                tx_buf[2] = 0x00;
                tx_buf[3] = 0x80;
            }
            else if(pdo_num == RPDO1)
            {
                tx_buf[0] = 0x80+CIA_NODE_C1_ID;
                tx_buf[1] = 0x01;
                tx_buf[2] = 0x00;
                tx_buf[3] = 0x80;
            }
            CO_SDOclientDownloadInitiate(COO.SDOclient, command.pdo_comm_param, 0x01, tx_buf, 4, 0);
           // sdo_down_ret = CO_SDOclientDownload(COO.SDOclient, diff_ms, 100, &can_app_csdo_tx_err);
            osDelay(1);
            if(COO.SDOclient->CANrxData[0]== 0x60)
            {
                *cmd = CLEAR_MAP;
            }     
            else 
            {
                error =1;
            return error;   
            }                
            break;
            
        case CLEAR_MAP:      
            tx_buf[0] = 00;
            CO_SDOclientDownloadInitiate(COO.SDOclient, command.pdo_map_param, 0x00, tx_buf, 1, 0);
        //sdo_down_ret = CO_SDOclientDownload(COO.SDOclient, diff_ms, 100, &can_app_csdo_tx_err);
            osDelay(1);
           if(COO.SDOclient->CANrxData[0]== 0x60)
            {
                *cmd  = SET_WORD_1;
            }
            else 
            {
                error =1;
            return error;   
            }  
            break;
        case SET_WORD_1://subindex 1
          
           if(word1 == 0x6064)
            tx_buf[0] = 0x20;
           else 
               tx_buf[0] = 0x10;
           
        if(word1 == 0x60c1)
            tx_buf[1] =0x01;
        else 
            tx_buf[1] = 0x00;
           
            tx_buf[2]=  (word1&0xff);
            tx_buf[3] =(word1&0xff00)>>8;
         
            CO_SDOclientDownloadInitiate(COO.SDOclient, command.pdo_map_param, 0x01, tx_buf, 4, 0);
           // sdo_down_ret = CO_SDOclientDownload(COO.SDOclient, diff_ms, 100, &can_app_csdo_tx_err);
            osDelay(1); 
            if(COO.SDOclient->CANrxData[0]== 0x60)
            {
                if(subindex >1)
               *cmd = SET_WORD_2;
                else 
                *cmd = MAPPING_TWO;
            }
             else
            {
                error =1;
            return error;   
            }  
            break;
        case SET_WORD_2://subindex2
           
         tx_buf[0] = 0x10;
        
         if(word1 == 0x60c1)
            tx_buf[1] =0x02;
        else 
            tx_buf[1] = 0x00;

              tx_buf[2]=(word2&0xff); 
              tx_buf[3] =(word2&0xff00)>>8;
           
           CO_SDOclientDownloadInitiate(COO.SDOclient, command.pdo_map_param, 0x02, tx_buf, 4, 0);
            osDelay(1);
            if(COO.SDOclient->CANrxData[0]== 0x60)
            {
                if(subindex >2)
                    *cmd = SET_WORD_3;
                else
                *cmd= MAPPING_TWO; 
            }
            else 
            {
                error =1;
            return error;   
            }  
            break;
        case SET_WORD_3:
            tx_buf[0] = 0x10;
            tx_buf[1] = 0x00;
            tx_buf[2] = (word3&0xff);
            tx_buf[3] = (word3&0xff00)>>8;
             
           CO_SDOclientDownloadInitiate(COO.SDOclient, command.pdo_map_param, 0x03, tx_buf, 4, 0);
            osDelay(1);
            if(COO.SDOclient->CANrxData[0]== 0x60)
            {
                *cmd= MAPPING_TWO; 
            }
            else 
            {
                error =1;
            return error;   
            }  
            break;
        case MAPPING_TWO://map_one
              tx_buf[0] = subindex;
            CO_SDOclientDownloadInitiate(COO.SDOclient, command.pdo_map_param, 0x00, tx_buf,1, 0);
        //sdo_down_ret = CO_SDOclientDownload(COO.SDOclient, diff_ms, 100, &can_app_csdo_tx_err);
            
            if(COO.SDOclient->CANrxData[0]== 0x60)
            {
                osDelay(1);
               *cmd = SET_TRANSMISSION_TYPE;   
            }
               else 
            {
                error =1;
            return error;   
            }  
              
            break;
            
        case SET_TRANSMISSION_TYPE:

                tx_buf[0] = 0xFE;
            
            CO_SDOclientDownloadInitiate(COO.SDOclient, command.pdo_comm_param, 0x02, tx_buf,1, 0);
            //sdo_down_ret = CO_SDOclientDownload(COO.SDOclient, diff_ms, 100, &can_app_csdo_tx_err);
            if(COO.SDOclient->CANrxData[0]== 0x60)
            {
                osDelay(1);
                *cmd= SET_ASYNC_TIME;
            }
               else 
            {
                error =1;
            return error;   
            }  
               
            break;
            
        case SET_ASYNC_TIME:
            tx_buf[0] = 0xE8;
            tx_buf[1] = 0x03;
          
            CO_SDOclientDownloadInitiate(COO.SDOclient, 0x1017 ,0x00, tx_buf, 2, 0);
          //  sdo_down_ret = CO_SDOclientDownload(COO.SDOclient, diff_ms, 100, &can_app_csdo_tx_err);
              if(COO.SDOclient->CANrxData[0]== 0x60)
            {
                osDelay(1);
                *cmd = ENABLE_PDO; 
            }
             else 
            {
                error =1;
            return error;   
            }  
            break;
            
        case ENABLE_PDO:
            if(pdo_num == TPDO1)
            {
            tx_buf[0] = CIA_NODE_C1_ID;
            tx_buf[1] = 0x02;
            tx_buf[2] = 0x00;
            tx_buf[3] = 0x00;
            }
            else if(pdo_num == RPDO1)
            {
            tx_buf[0] = 0x80+CIA_NODE_C1_ID;
            tx_buf[1] = 0x01;
            tx_buf[2] = 0x00;
            tx_buf[3] = 0x00;
            }
             
            CO_SDOclientDownloadInitiate(COO.SDOclient, command.pdo_comm_param, 0x01, tx_buf,4, 0);
            //sdo_down_ret = CO_SDOclientDownload(COO.SDOclient, diff_ms, 100, &can_app_csdo_tx_err);
            if(COO.SDOclient->CANrxData[0]== 0x60)
            { 
                osDelay(1);
                *cmd= MAP_DONE; 
            }
            else 
            {
                error =1;
            return error;   
            }  
          break;
            
        case MAP_DONE:
            
            break;
        default:
            break;
    }
}
/**
  * @brief  init pdo mapping by sdo-client-to-server.
  * @param  None
  * @retval None
  */
#if 0
void motion_run(CAN_SEND_ABI*output,uint8_t cmd)
{   
    #if 1
    switch(*cmd)
    {
        case ServoCommandOFF: // init value: If in Switch_On_Disabled state, change to Ready_To_Switch On
            
                output->control_word = 0x06; // operation: Shut down
                break;
               
        case ServoCommandON:

           // if( (can_receive.status_word & 0x0040) == 0x0040 )
        if((OD_status_word & 0x0040) == 0x0040)
                output->control_word = 0x06; // operation: Shut down

          // else if ( (can_receive.status_word&0x007f) == 0x0031 ) // FSA State: Ready to Switch On
         else if((OD_status_word & 0x007f) == 0x0021)
            {
                output->control_word= 0x07; // operation: Switch On
            }
            else if ( (OD_status_word&0x007b) == 0x0033 ) 
            {
             *cmd = ServoCommandEnable;
            }
            break;

        case ServoCommandEnable: // 上使能

            if ( (OD_status_word&0x007b) == 0x0033 ) // FSA State: Switch On
            {
            output->control_word = 0x0f; // Enable Operation
            }
            else if(OD_status_word  ==0x0237 )
            {
                *cmd = ServoCommandRunMode;
           //output->control_word = 0x0b; // 急停， Quick Stop
            }
        break;

            
        case ServoCommandRunMode:
        {
            if(OD_status_word  ==0x0237 )
            {
                set_mode(output,INTERPOLATE_POSITION);//发送模式指令 开始跑控制模式
            }

        }
        case ServoCommandDisable: // 下使能

        output->control_word = 0x07; // Disable operation
        break;

        case ServoCommandEmeStop: // 急停

            output->control_word = 0x0b; // 急停， Quick Stop
        break;

        case ServoCommandFaultReset: // 错误恢复, 错误恢复后需要重新使能

            if( OD_status_word&0x0008 )
            {
            output->control_word = output->control_word | 0x0080;
            }
            else
            {
            output->control_word = output->control_word & 0xff7f;
            }
        break;

        case ServoCommandHalt: // 暂停 Halt On; Warning!!! 在暂停驱动器之前确保控制器已经停止发送位置指令

        output->control_word = output->control_word | 0x0100;
        break;

        case ServoCommandHaltReset: // 取消暂停 Halt Off, Warning!!!

            output->control_word = output->control_word & 0xfeff;
            // if ( (input.status_word_& 0x007f) == 0x0037) // FSA State: Operation enabled (正常工作状态) = Switch On + Enable Operation + Quick Stop Off + No warning + Voltage On
            // {
            // output.target_cmd_ = data_->out.pos;
            // }
        break;

        default:
            output->control_word = 0x0b; // 急停， Quick Stop
        break;
        }
        #endif
   
    AbiSendMsgCanSend(ABI_CAN_MOTION_SEND_ID, output);
}

#endif

static int32_t cia_get_encoder_data()
{
    int inc[2];
    inc[1] = OD_custom_actual_pos;
    inc[0] = inc[1];
    return inc[1];

}

static int32_t generate_trace()
{
static int32_t time = 0;
   int32_t angle;
   time += 5;
//    angle = 8192*sin(0.001*time);
    angle = time;
   
return angle;
}

void cia_motion_run(CIA402_MB2DRIVER_ABI *output,SERVO_CMD *cmd)
{
   
    switch(*cmd)
    {
        case SERVO_INIT:
            
            if (OD_status_word != 0x250)
                *cmd = SERVO_READY;
            else
                *cmd = SERVO_BREAK;
            
            break;
        case SERVO_READY:
            output->control_word = 0x06;
            osDelay(1);
            if((OD_status_word & 0x007f) == 0x0021)
            {
            *cmd = SERVO_WAIT_FOR_ENABLE;
            }
          
        break;
        case SERVO_WAIT_FOR_ENABLE:
            output->control_word = 0x07;
            osDelay(1);
          if ((OD_status_word & 0x007b) == 0x0033 ) 
          {
            *cmd = SERVO_ENABLE;
          }
           
            break;
        case SERVO_ENABLE:
        if (OD_status_word == 0x0233 ) 
        {
            output->control_word = 0x0f;
        }
        else if(OD_status_word==0x0237 )  //TBD   还可能是1237
            {
              
               *cmd = SERVO_RUN;
            }    
            break;
    
        case SERVO_RUN :
            if(OD_status_word  ==0x0237)
        //if(OD_status_word &0x007f  ==0x0037 )   
            {
                 output->control_word = 0x1f;
            }
            else if  (OD_status_word  ==0x1237)
            {
                 output->position_demand_value = generate_trace();
            }
//            else if( OD_status_word&0x0008 == 0x0008 )
//            {
//            *cmd = SERVO_BREAK;
//            }
        break;
        case SERVO_BREAK:
            break;
        default:
            break;
    }
// AbiSendMsgMB2BRIVER(ABI_CIA402_MB2DRIVER_ID, output);

}
/**
  * @brief  init pdo mapping by sdo-client-to-server.
  * @param  None
  * @retval None
  *  注：这里的MAP要依据驱动器来配置，例如，对于control word ，驱动器为RPDO，comm=1400 map_param = 1600,cob-id = 200+node,
  *     但对于主板来说control_word是主板发送驱动器接收，为TPDO。 pdo_num为主板定的名称，节点号和cob-id根据驱动器的配置来写
  */
void get_pdo_param(CANOPEN_COM_INFO *command,PDO_NUM pdo_num)
{
if (pdo_num == TPDO1)//cob-id 0x206 
    {
        command->pdo_comm_param = 0x1400;
        command->pdo_map_param = 0x1600;
    }
    else if (pdo_num == RPDO1)//cob-id 0x186
    {
        command->pdo_comm_param = 0x1800;
        command->pdo_map_param = 0x1a00;
    }
    else if (pdo_num == TPDO2)//cob-id 0x306
    {
        command->pdo_comm_param = 0x1401;
        command->pdo_map_param = 0x1601;
    }
}

/**
  * @brief  system setting ,by sdo-client-to-server;
  * @param  max-acc   max-dacc  software-limit  ...
  * @retval None
  */
uint8_t cia_parameter_init(void)
{
        static uint8_t cnt = 0x01;
        uint8_t min_p_limit[4];
        uint8_t max_p_limit[4];
        uint8_t max_acc[4];
        uint8_t max_dacc[4];
        uint8_t max_speed[4];
      //  CO_setUint32(min_p_limit,MIN_P_LIMIT);
      //  CO_setUint32(max_p_limit,MAX_P_LIMIT);
        CO_setUint32(max_acc,MAX_ACC);
        CO_setUint32(max_dacc,MAX_DACC);
        CO_setUint32(max_speed,MAX_SPEED);
        
        switch (cnt)
        {
       case 0x01: 
           CO_SDOclientDownloadInitiate(COO.SDOclient, 0x60c5, 0x00,max_acc,4, 0);//  最大加速度
            osDelay(1);
            if(COO.SDOclient->CANrxData[0]== 0x60)
            {
               cnt++;
            }    
            break;
       case 0x02:
        CO_SDOclientDownloadInitiate(COO.SDOclient, 0x60c6, 0x00,max_dacc,4, 0);//  最大减速度
        osDelay(1);
        if(COO.SDOclient->CANrxData[0]== 0x60)
            {
               cnt++;
            }    
            break;
       case 0x03:
           
        CO_SDOclientDownloadInitiate(COO.SDOclient, 0x607f, 0x00,max_speed,4, 0);//  最大速度   
        osDelay(1);
        if(COO.SDOclient->CANrxData[0]== 0x60)
            {
               cnt++;
            }    
            break;
       case 0x04:
           //    CO_SDOclientDownloadInitiate(COO.SDOclient, 0x607d, 0x01,min_p_limit,4, 0); //软件限位，用户单位
            //    osDelay(1); 
            if(COO.SDOclient->CANrxData[0]== 0x60)
            {
               cnt++;
            }    
            break;
       case 0x05:
           //    CO_SDOclientDownloadInitiate(COO.SDOclient, 0x607d, 0x02,max_p_limit,4, 0); //软件限位，用户单位
//    osDelay(1);
       if(COO.SDOclient->CANrxData[0]== 0x60)
            {
               cnt = 100;
            }    
            break;
       default:
           break;
             
        }
        return cnt;
}

MODE_DISPLAY cia_set_mode(CIA402_MB2DRIVER_ABI*output,uint8_t mode)
{
 
    switch (mode)
    {
        case PROFILE_POSTION://
            output->mode = 0x01;
            break;
        case VELOCITY:
            break;
        case PROFILE_VELOCITY:
            break;
        case TORQUE:
            break;
        case NA:
            break;
        case HOMING_MODE:
            break;
        case INTERPOLATE_POSITION:
             CO_SDOclientDownloadInitiate(COO.SDOclient, 0x6060, 0x00,&mode,1, 0);//  设置插补模式
             osDelay(1);
             CO_SDOclientDownloadInitiate(COO.SDOclient, 0x60c2, 0x01,INTERPOLATE_TIME,1, 0);//  设置插补时间 10ms
             osDelay(1);
             CO_SDOclientDownloadInitiate(COO.SDOclient, 0x60c1, 0x01,INTERPOLATE_FIRST_POS,4, 0);// 设置插补的第一个目标位置  rpdo_write;
             osDelay(1);
            break;
           
        default:
            break;

    }
    return mode;
}


/****************************** END OF FILE ***********************************/
