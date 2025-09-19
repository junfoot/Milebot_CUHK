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

#include <arm_math.h>

#include "can.h"
#include "can_if.h"

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
//uint8_t advanced_mode_switch = SWITCH_ON;
uint8_t advanced_mode_switch = SWITCH_OFF;         // 会卡 不知道为什么
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


#define FLOAT_SIZE 4
#define NUM_FLOATS 14
#define RAW_DATA_LEN (NUM_FLOATS * FLOAT_SIZE)

#define SIGN(x)  ((x) >= 0 ? 1 : -1)

//#define P 0
//#define I 0
//#define D 0

float P = 8, I = 0, D = 650;   // 初始值，粗调

uint8_t *data_buf = 0;
int8_t data_lenght = 0;
uint8_t * data_buf_torque;

uint8_t can_flag = 0;

uint8_t dummy[8];

// --------------------------------------------- 2025 ZHZ add -------------------------------------------

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
	
    float TorqueLeftHip_PID = 0,TorqueRightHip_PID = 0;
	float TorqueLeftHip = 0,TorqueRightHip = 0;
	float TorqueLeftRaw = 0, TorqueRightRaw = 0;
	float PosLeftCmd = 0, PosRightCmd = 0;
	float PosLeftRaw = 0, PosRightRaw = 0;
    
    float TorqueLimit;
	
	// PID pos control
	float LeftError, LeftPrevError1, LeftPrevError2;
	float LeftProportional, LeftIntegral, LeftDerivative;
	float RightError, RightPrevError1, RightPrevError2;
	float RightProportional, RightIntegral, RightDerivative;
	
	uint8_t mode = 0;
	uint8_t switch_mode = 0;
			
            // Wait for the actuators to power up
            osDelay(2000);

            // Receive actuator commands from control task
//            if(!advanced_mode_switch)
//                AbiBindMsgACTUATOR_CMD(ACTUATOR_CMD_ID, &actuator_cmd_ev, actuator_cmd_cb);  // 订阅cmd

            actuator_init(&actuator_state[LEFT_HIP]);
            actuator_init(&actuator_state[RIGHT_HIP]);

            //loop while(1)
            while(1)
            {
                
                // get torque command
                if(uart1_dma_rx_buf_raw[0] == 0xAB && uart1_dma_rx_buf_raw[uart1_dma_rx_buf_len-1] == 0xCD)     // torque control
                {
                    uart1_dma_rx_buf_len = 0;
                    if (mode != 1){
                        switch_mode = 1;
                        mode = 1;
                        
                        TorqueLeftHip = 0;
                        TorqueRightHip = 0;
                    }
                    else{
                        switch_mode = 0;
                    }
                    
                    memcpy(&TorqueLeftRaw, uart1_dma_rx_buf_raw + 1, sizeof(float));
                    memcpy(&TorqueRightRaw, uart1_dma_rx_buf_raw + 1 + sizeof(float), sizeof(float));
                    
                }
                else if(uart1_dma_rx_buf_raw[0] == 0xDC && uart1_dma_rx_buf_raw[uart1_dma_rx_buf_len-1] == 0xBA)  // position control
                {
                    uart1_dma_rx_buf_len = 0;
                    if (mode != 2){
                        switch_mode = 1;
                        mode = 2;
                        
                        TorqueLeftHip = 0;
                        TorqueRightHip = 0;
                        PosLeftRaw = 0;
                        PosRightRaw = 0;
                        TorqueRightHip_PID = 0;
                        TorqueRightHip_PID = 0;
                    }
                    else{
                        switch_mode = 0;
                    }
                    
                    memcpy(&PosLeftRaw, uart1_dma_rx_buf_raw + 1, sizeof(float));
                    memcpy(&PosRightRaw, uart1_dma_rx_buf_raw + 1 + sizeof(float), sizeof(float));
                    
                }
                else if(uart1_dma_rx_buf_raw[0] == 0x12 && uart1_dma_rx_buf_raw[uart1_dma_rx_buf_len-1] == 0x34)     // get PID
                {
                    uart1_dma_rx_buf_len = 0;
                    
                    memcpy(&P, uart1_dma_rx_buf_raw + 1, sizeof(float));
                    memcpy(&I, uart1_dma_rx_buf_raw + 1 + sizeof(float), sizeof(float));
                    memcpy(&D, uart1_dma_rx_buf_raw + 1 + 2 * sizeof(float), sizeof(float));
                    
                }

                    
                    
                switch (mode)
                {
                    case 0:
                        
                        TorqueLeftHip = 0;
                        TorqueRightHip = 0;
                        
                        break;
                    
                    case 1:     // torque command

                        // torque limit
                        TorqueLimit = 1;
                        if (fabs(TorqueLeftRaw) < TorqueLimit){
                            TorqueLeftHip = TorqueLeftRaw;
                        }
                        else{
                            TorqueLeftHip = SIGN(TorqueLeftRaw) * TorqueLimit;
                        }
                        if (fabs(TorqueRightRaw) < TorqueLimit){
                            TorqueRightHip = TorqueRightRaw;
                        }
                        else{
                            TorqueRightHip = SIGN(TorqueRightRaw) * TorqueLimit;
                        }
                        
                        
                        
                        break;
                        
                    case 2:     // position command
                    
                        // degree to rad
                        PosLeftCmd = PosLeftRaw * PI / 180.0f;
                        PosRightCmd = PosRightRaw * PI / 180.0f;
                        
                        // pos limit
                        if (PosLeftCmd > 1.571){
                            PosLeftCmd = 1.571;
                        }
                        else if (PosLeftCmd < -0.436){
                            PosLeftCmd = -0.436;
                        }
                        if (PosRightCmd > 1.571){
                            PosRightCmd = 1.571;
                        }
                        else if (PosRightCmd < -0.436){
                            PosRightCmd = -0.436;
                        }
                        
                        /*****************************************************************************/
                        // gravity feedback
                        TorqueLeftHip = 0.05 * arm_sin_f32(actuator_info[LEFT_HIP].output_angle);
                        TorqueRightHip = 0.05 * arm_sin_f32(actuator_info[RIGHT_HIP].output_angle);
                        
                        // PID control ( Increment )
                        LeftError = PosLeftCmd - actuator_info[LEFT_HIP].output_angle;
                        LeftProportional = LeftError - LeftPrevError1;
                        LeftIntegral = LeftError;
                        LeftDerivative = LeftError - 2 * LeftPrevError1 + LeftPrevError2;
                        
                        TorqueLeftHip_PID += P * LeftProportional + I * LeftIntegral + D * LeftDerivative;
                        LeftPrevError2 = LeftPrevError1;
                        LeftPrevError1 = LeftError;
                        
                        RightError = PosRightCmd - actuator_info[RIGHT_HIP].output_angle;
                        RightProportional = RightError - RightPrevError1;
                        RightIntegral = RightError;
                        RightDerivative = RightError - 2 * RightPrevError1 + RightPrevError2;
                        
                        TorqueRightHip_PID += P * RightProportional + I * RightIntegral + D * RightDerivative;
                        RightPrevError2 = RightPrevError1;
                        RightPrevError1 = RightError;

                        // froward feedback
                        if (fabs(LeftError) > 0.05 && TorqueLeftHip_PID != 0){
                            TorqueLeftHip += TorqueLeftHip_PID + 0 * SIGN(TorqueLeftHip_PID);
                        }
                        if (fabs(RightError) > 0.05 && TorqueRightHip_PID != 0){
                            TorqueRightHip += TorqueRightHip_PID + 0 * SIGN(TorqueRightHip_PID);
                        }
                        
                        /*****************************************************************************/
                        
                        // torque limit
                        TorqueLimit = 3;
                        if (fabs(TorqueLeftHip) < TorqueLimit){
                            TorqueLeftHip = TorqueLeftHip;
                        }
                        else{
                            TorqueLeftHip = SIGN(TorqueLeftHip) * TorqueLimit;
                        }
                        if (fabs(TorqueRightHip) < TorqueLimit){
                            TorqueRightHip = TorqueRightHip;
                        }
                        else{
                            TorqueRightHip = SIGN(TorqueRightHip) * TorqueLimit;
                        }
                        
//                        TorqueLeftHip = 0; 
//                        TorqueRightHip = 0;    // q:8,0,650
                        
                        break;
                        
                    case 3:            // 
                        
                        break;
                        
                    default:
                        break;
                }
                    
                    
                
                // angle limit
                if ((actuator_info[LEFT_HIP].output_angle > 1.571)||(actuator_info[LEFT_HIP].output_angle < -0.436))    //  -25~90 degree
                {
                    TorqueLeftHip = 0;
                }
                if ((actuator_info[RIGHT_HIP].output_angle > 1.571)||(actuator_info[RIGHT_HIP].output_angle < -0.436))  //  -25~90 degree
                { 
                    TorqueRightHip = 0;
                }
                
//                
                actuator_state[LEFT_HIP].actuator_cmd.set_torque = TorqueLeftHip;
                actuator_state[RIGHT_HIP].actuator_cmd.set_torque = -TorqueRightHip;   // right is reverse

//                actuator_state[LEFT_HIP].actuator_cmd.set_torque = 0;
//                actuator_state[RIGHT_HIP].actuator_cmd.set_torque = 0;   // right is reverse
                
                
//                //Circular send command to and get state from actuators
//                for(i = FIRST_JOINT; i < ALL_JOINTS; i++)
//                {
//                    // check CAN Tx Mailbox
//                    CAN1->MCR |= CAN_MCR_TTCM;
//                    CAN2->MCR |= CAN_MCR_TTCM;
//                    if ((CAN1->TSR & CAN_TSR_TME0) == 0 && (CAN1->TSR & CAN_TSR_TME1) == 0 && (CAN1->TSR & CAN_TSR_TME2) == 0) 
//                    {
//                        CAN1->TSR |= CAN_TSR_ABRQ0 | CAN_TSR_ABRQ1 | CAN_TSR_ABRQ2;
//                    }
//                    if ((CAN2->TSR & CAN_TSR_TME0) == 0 && (CAN2->TSR & CAN_TSR_TME1) == 0 && (CAN2->TSR & CAN_TSR_TME2) == 0) 
//                    {
//                        CAN2->TSR |= CAN_TSR_ABRQ0 | CAN_TSR_ABRQ1 | CAN_TSR_ABRQ2;
//                    }
//                    CAN1->MCR &= ~CAN_MCR_TTCM;
//                    CAN2->MCR &= ~CAN_MCR_TTCM;
////                    CAN1->TSR |= CAN_TSR_ABRQ0 | CAN_TSR_ABRQ1 | CAN_TSR_ABRQ2;
////                    CAN2->TSR |= CAN_TSR_ABRQ0 | CAN_TSR_ABRQ1 | CAN_TSR_ABRQ2;
//                    
//                    send_cmd_to_actuator(&actuator_state[i]);
//                    
//                    // check CAN Tx Mailbox
//                    CAN1->MCR |= CAN_MCR_TTCM;
//                    CAN2->MCR |= CAN_MCR_TTCM;
//                    if ((CAN1->TSR & CAN_TSR_TME0) == 0 && (CAN1->TSR & CAN_TSR_TME1) == 0 && (CAN1->TSR & CAN_TSR_TME2) == 0) 
//                    {
//                        CAN1->TSR |= CAN_TSR_ABRQ0 | CAN_TSR_ABRQ1 | CAN_TSR_ABRQ2;
//                    }
//                    if ((CAN2->TSR & CAN_TSR_TME0) == 0 && (CAN2->TSR & CAN_TSR_TME1) == 0 && (CAN2->TSR & CAN_TSR_TME2) == 0) 
//                    {
//                        CAN2->TSR |= CAN_TSR_ABRQ0 | CAN_TSR_ABRQ1 | CAN_TSR_ABRQ2;
//                    }
//                    CAN1->MCR &= ~CAN_MCR_TTCM;
//                    CAN2->MCR &= ~CAN_MCR_TTCM;
////                    CAN1->TSR |= CAN_TSR_ABRQ0 | CAN_TSR_ABRQ1 | CAN_TSR_ABRQ2;
////                    CAN2->TSR |= CAN_TSR_ABRQ0 | CAN_TSR_ABRQ1 | CAN_TSR_ABRQ2;
//                    
//                    get_state_from_actuator(&actuator_info[i], &actuator_state[i]);
//                }

        
//        if(advanced_mode_switch)
        if(1)
        {
            
            count_num++;
            if (count_num >= 10)
            {
                count_num = 0;
                
                //Circular send command to and get state from actuators
                for(i = FIRST_JOINT; i < ALL_JOINTS; i++)
                {
                    
                    send_cmd_to_actuator(&actuator_state[i]);
                    get_state_from_actuator(&actuator_info[i], &actuator_state[i]);
                
                    /*检测抬腿：1-检测到抬腿，0-未检测到抬腿*/
                    left_lift_sate = walk_detect_left_lift_state(&actuator_info);
                    right_lift_sate = walk_detect_right_lift_state(&actuator_info);
                    
                    AbiSendMsgACTUATOR_STATE(ACTUATOR_STATE_ID, actuator_info);
                        
                }

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
                
                
    //					actuator_state[LEFT_HIP].dir
                
                // 按顺序填充float数据
                index = 0;
                float_data[index++] = actuator_info[LEFT_HIP].output_angle * 180.0f / PI;
                float_data[index++] = actuator_info[LEFT_HIP].output_velocity * 180.0f / PI;
                float_data[index++] = actuator_info[LEFT_HIP].output_torque;
                float_data[index++] = actuator_info[RIGHT_HIP].output_angle * 180.0f / PI;
                float_data[index++] = actuator_info[RIGHT_HIP].output_velocity * 180.0f / PI;
                float_data[index++] = actuator_info[RIGHT_HIP].output_torque;
                float_data[index++] = imu_data.eulerf_Mayhony.phi;
                float_data[index++] = imu_data.eulerf_Mayhony.theta;
                float_data[index++] = (float)left_lift_sate;
                float_data[index++] = (float)right_lift_sate;
                // changeable, for tuing
                float_data[index++] = (float)mode;
                float_data[index++] = TorqueLeftHip;
                float_data[index++] = TorqueRightHip;
                float_data[index++] = I;
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

