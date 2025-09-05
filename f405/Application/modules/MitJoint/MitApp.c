/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : MitApp.c
* Author      : Hugo
* Version     : v0.01
* Date        : 2020/08/06
* Description : mit app task
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date        Name        Changes and comments
* v0.01     2020/08/06  Hugo        initial version
*=============================================================================*/

#include "cmsis_os2.h"
#include "systick_if.h"
#include "abi_messages.h"
#include "MitApp.h"

#include "MitProtocol_if.h"
#include "DataScope_DP.h"

#include "string.h"
#include "lsm6ds.h"

#define SWITCH_ON 1
#define SWITCH_OFF 0

int value=0;
/* Private variables ---------------------------------------------------------*/
// Temporary storage CAN message


// information of actuators, send to other tasks
static ACTUATOR_INFO actuator_info[ALL_JOINTS] = {0};

// need to init the can number, node id and motor mode
static ACTUATOR_STATE actuator_state[ALL_JOINTS] = 
{
    {
        .can_num = CAN_1,
        .node_id = NODE_ID_1,
        .joint_id = LEFT_HIP,
        .motor_mode = MIT_MOTOR_MODE,
        .dir = DIR_FORWARD,
//        .dir = DIR_REVERSE,
    },
    
    {
        .can_num = CAN_2,
        .node_id = NODE_ID_2,
        .joint_id = RIGHT_HIP,
        .motor_mode = MIT_MOTOR_MODE,
         .dir = DIR_REVERSE,
//        .dir = DIR_FORWARD,
    }
};


/* Global functions ----------------------------------------------------------*/
uint8_t advanced_mode_switch = SWITCH_ON;
//uint8_t advanced_mode_switch = SWITCH_OFF;
/**
  * @brief  task to handle all mit application function.
  * @param  None
  * @retval None
  */



// --------------------------------------------- 2025 ZHZ add -------------------------------------------

/**
  * @brief  打包数据，用于串口发送
  * @brief  Byte_0,Byte_1为帧头: 0xAA 0x55  
  * @brief  Byte_2为数据长度(包括帧头和校验): 0x08
  * @brief  Byte_3~ Byte_6为用户数据；
  * @brief  Byte_7为校验值(取低8位)；
  * @example 如发送整数100,十六进制为0x64，发送格式为：0xAA 0x55 0x08 0x00 0x00 0x00 0x64 0x6B
  * @example 0x6B为校验值，0xAA+0x55+0x08+0x00+0x00+0x00+0x64 = 0x016b，取低8位，即0x6B
  */
unsigned int PackingData(uint8_t* packing_buf, const uint8_t* buf, uint8_t len)
{
    int index = 0;
    int i = 0;
    uint8_t check_sum = 0;
    
    //帧头
    packing_buf[index++] = 0xaa;
    packing_buf[index++] = 0x55;

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

uint8_t * check_pack(uint8_t* buf, uint8_t len, int8_t *data_lenght)//返回解包后的有效数据总字节长度
{
    uint8_t index = 0;
    uint8_t i = 0;
    uint8_t check_sum = 0;

    if(len < 4)
    {
        return 0;
    }
    while((buf[index] != 0xaa) || (buf[index+1] != 0x55))
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
        *data_lenght = buf[index+2] - 4;
        return buf+3;
        
    }
    else
    {
        *data_lenght = 0;
    }

    return 0;
}

// --------------------------------------------- 2025 ZHZ add -------------------------------------------
#define FLOAT_SIZE 4
#define NUM_FLOATS 8
#define RAW_DATA_LEN (NUM_FLOATS * FLOAT_SIZE)

float TorqueLeftHip,TorqueRightHip = 0;
int TorqueLeftHip_dir,TorqueRightHip_dir = 0; // +1:flexion assist; 0:no assist; -1:extension assist
uint8_t *data_buf = 0;
int8_t data_lenght = 0;
int tmpTorque1,tmpTorque2,resolution = 50000;
uint8_t * data_buf_torque;


void mit_task(void *argument)
{
    static abi_event actuator_cmd_ev;
    uint8_t i = 0;
    uint8_t left_lift_sate = 0;
    uint8_t right_lift_sate = 0;
	
	uint8_t index = 0;
	float float_data[NUM_FLOATS];
	uint8_t buff[RAW_DATA_LEN];
	unsigned int packed_len;
	uint8_t packed_data[RAW_DATA_LEN + 4];
	float Torque_limit,Torque_limit_init = 1;
	
	int count_num = 0;
	
    
    // Wait for the actuators to power up
    osDelay(2000);

    // Receive actuator commands from control task
    if(!advanced_mode_switch)
        AbiBindMsgACTUATOR_CMD(ACTUATOR_CMD_ID, &actuator_cmd_ev, actuator_cmd_cb);

    actuator_init(&actuator_state[LEFT_HIP]);
    actuator_init(&actuator_state[RIGHT_HIP]);

    //loop while(1)
    while(1)
    {
        
//        if(advanced_mode_switch)
//        {
//            if(data_buf == 1234)
//            {
//                
//                 actuator_state[LEFT_HIP].actuator_cmd.set_torque = actuator_state[LEFT_HIP].dir * 1;
//            }
//            else
//            {
//                 actuator_state[LEFT_HIP].actuator_cmd.set_torque = actuator_state[LEFT_HIP].dir * 0;
//            }
//            actuator_state[RIGHT_HIP].actuator_cmd.set_torque = actuator_state[RIGHT_HIP].dir * 0;
//        }
		
		data_buf_torque = check_pack(uart1_dma_rx_buf_raw,uart1_dma_rx_buf_len,&data_lenght);			
		if(data_buf_torque && data_lenght == 8) // 检查是否是预期的两个int
		{
			memcpy(&tmpTorque1, data_buf_torque, 4);
			memcpy(&tmpTorque2, data_buf_torque+4, 4);

			TorqueLeftHip = fmax(fmin((float)(tmpTorque1 - resolution)*Torque_limit/resolution,Torque_limit),-Torque_limit);
			if ((tmpTorque1 - resolution)>0)
			{
					TorqueLeftHip_dir = 1;
			}else if ((tmpTorque1 - resolution)<0)
			{
					TorqueLeftHip_dir = -1;
			}else
			{
					TorqueLeftHip_dir = 0;
			}
		
			TorqueRightHip = fmax(fmin((float)(tmpTorque2 - resolution)*Torque_limit/resolution,Torque_limit),-Torque_limit);
			if ((tmpTorque2 - resolution)>0)
			{
					TorqueRightHip_dir = 1;
			}else if ((tmpTorque2 - resolution)<0)
			{
					TorqueRightHip_dir = -1;
			}else
			{
					TorqueRightHip_dir = 0;
			}
		
		
			uart1_dma_rx_buf_len = 0;
		}
		
		// angle limit
		if (((actuator_info[LEFT_HIP].output_angle * 180.0 / 3.14) > 90)&&(actuator_info[LEFT_HIP].output_angle * TorqueLeftHip_dir > 0))
		{
				TorqueLeftHip = 0;
		}
		if ((-(actuator_info[LEFT_HIP].output_angle * 180.0 / 3.14) > 25)&&(actuator_info[LEFT_HIP].output_angle * TorqueLeftHip_dir > 0))
		{
				TorqueLeftHip = 0;
		}
		if (((actuator_info[RIGHT_HIP].output_angle * 180.0 / 3.14) > 90)&&(actuator_info[RIGHT_HIP].output_angle * TorqueRightHip_dir > 0))
		{
				TorqueRightHip = 0;
		}
		if ((-(actuator_info[RIGHT_HIP].output_angle * 180.0 / 3.14) > 25)&&(actuator_info[RIGHT_HIP].output_angle * TorqueRightHip_dir > 0))
		{
				TorqueRightHip = 0;
		}
		
		
		TorqueLeftHip = 0;
		TorqueRightHip = 0;
		
		actuator_state[LEFT_HIP].actuator_cmd.set_torque = actuator_state[LEFT_HIP].dir * TorqueLeftHip;
		actuator_state[RIGHT_HIP].actuator_cmd.set_torque = actuator_state[RIGHT_HIP].dir * TorqueRightHip;
		
//		actuator_state[RIGHT_HIP].actuator_cmd.set_position = 0;
        
        //Circular send command to and get state from actuators
        for(i = FIRST_JOINT; i < ALL_JOINTS; i++)
        {
            send_cmd_to_actuator(&actuator_state[i]);
            get_state_from_actuator(&actuator_info[i], &actuator_state[i]);

        }
        
        /*检测抬腿：1-检测到抬腿，0-未检测到抬腿*/
        left_lift_sate = walk_detect_left_lift_state(&actuator_info);
        right_lift_sate = walk_detect_right_lift_state(&actuator_info);
        
        AbiSendMsgACTUATOR_STATE(ACTUATOR_STATE_ID, actuator_info);
        
        if(advanced_mode_switch)
        {
            
            count_num++;
            if (count_num >= 10)
            {
                count_num = 0;

//                DataScope_Get_Channel_Data(actuator_info[LEFT_HIP].output_angle * 180.0 / 3.14, 1);
//                DataScope_Get_Channel_Data(actuator_info[LEFT_HIP].output_velocity * 180.0 / 3.14, 2);
//                DataScope_Get_Channel_Data(actuator_info[LEFT_HIP].output_torque, 3);
//                
//                DataScope_Get_Channel_Data(actuator_info[RIGHT_HIP].output_angle * 180.0 / 3.14, 4);
//                DataScope_Get_Channel_Data(actuator_info[RIGHT_HIP].output_velocity * 180.0 / 3.14, 5);
//                DataScope_Get_Channel_Data(actuator_info[RIGHT_HIP].output_torque, 6);
//                DataScope_Get_Channel_Data(left_lift_sate, 7);
//                DataScope_Get_Channel_Data(right_lift_sate, 8);
//                
//                int Send_Count = DataScope_Data_Generate(10);
//                uart2_dma_send_data(DataScope_OutPut_Buffer, Send_Count);	
				
									// 按顺序填充float数据
									index = 0;
									float_data[index++] = actuator_info[LEFT_HIP].output_angle * 180.0f / 3.14f;
									float_data[index++] = actuator_info[LEFT_HIP].output_velocity * 180.0f / 3.14f;
									float_data[index++] = actuator_info[LEFT_HIP].output_torque;
									float_data[index++] = actuator_info[RIGHT_HIP].output_angle * 180.0f / 3.14f;
									float_data[index++] = actuator_info[RIGHT_HIP].output_velocity * 180.0f / 3.14f;
									float_data[index++] = actuator_info[RIGHT_HIP].output_torque;
									float_data[index++] = imu_data.gyro_data_filter.x;
									float_data[index++] = imu_data.accel_data_filter.x;
									// 将float数组转换为字节数组
									memcpy(buff, float_data, RAW_DATA_LEN);
									// 打包数据
									packed_len = PackingData(packed_data, buff, RAW_DATA_LEN);
									// 通过USART1发送数据
									uart1_send_data(packed_data, packed_len);	
            }
        }
					
				
        osDelay(1);

    }
}

void actuator_cmd_cb(uint8_t __attribute__((unused)) sender_id, ACTUATOR_CMD *cmddata)
{
    for (int i = FIRST_JOINT; i < ALL_JOINTS; i++)
    {
        actuator_state[i].actuator_cmd.set_torque = actuator_state[i].dir * cmddata[i].set_torque;
        actuator_state[i].actuator_cmd.out_vel = actuator_state[i].dir * cmddata[i].out_vel;
    }
}

