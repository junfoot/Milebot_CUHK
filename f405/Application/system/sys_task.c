/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : sys_task.c
* Author      : wb
* Version     : v0.01
* Date        : 2019/10/28
* Description : IMU attitude calculatation task
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2019/10/28  CZ    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/
#include "cmsis_os2.h"                  // ARM::CMSIS:RTOS2:Keil RTX5

/* Public Includes -----------------------------------------------------------*/
#include "adc_app_if.h"
#include "tim_if.h"
#include "timer_if.h"

/* Private Includes ----------------------------------------------------------*/
#include "sys_task.h"
#include "sys_task_if.h"

/* Global variables ----------------------------------------------------------*/
#include "i2c_if.h"
#include "lsm6ds.h"


/* Private variables ---------------------------------------------------------*/

//是否使能快速傅里叶变换测试
//#define FFT_TEST

#ifdef FFT_TEST
//------------------------FFT 快速傅里叶变换的测试------------------------
#include "arm_math.h"

#define FFT_LENGTH		256 		//FFT长度(由于使用radix4 基4, 所以该值必须是4 的N 次方)

float fft_inputBuf[FFT_LENGTH*2];	//FFT输入数组
float fft_outputBuf[FFT_LENGTH];	//FFT输出数组
float fft_amplitude[FFT_LENGTH];	//FFT幅值数组

static arm_cfft_radix4_instance_f32 scfft;

//------------------------FFT 快速傅里叶变换的测试------------------------
#endif





//是否使能矩阵运算测试
//#define MATRIX_TEST

#ifdef MATRIX_TEST
//------------------------矩阵运算测试------------------------
#include "arm_math.h"

//矩阵的常用函数接口:

//arm_mat_add_f32();  //矩阵加法

//arm_mat_sub_f32();  //矩阵减法

//arm_mat_mult_f32();  //矩阵乘法

//arm_mat_inverse_f32();  //逆矩阵

//arm_mat_trans_f32();  //矩阵转置

//arm_mat_scale_f32();  //矩阵放缩



arm_matrix_instance_f32 pSrcA;
arm_matrix_instance_f32 pSrcB;
arm_matrix_instance_f32 pDst;

//矩阵加法
float32_t pDataA[9] = {1.1, 1.2, 1.3, 2.1, 2.2, 2.3, 3.1, 3.2, 3.3};//矩阵3*3
float32_t pDataB[9] = {1.1, 1.2, 1.3, 2.1, 2.2, 2.3, 3.1, 3.2, 3.3};//矩阵3*3
float32_t pData_add[9] = {0.0};//矩阵3*3

//矩阵乘法
float32_t pDataC[6] = {1, 2, 3, 4, 5, 6};//矩阵3*2
float32_t pDataD[6] = {7, 8, 9, 10, 11, 12};//矩阵2*3
float32_t pData_multiply[4] = {0.0};//矩阵2*2


//------------------------矩阵运算测试------------------------
#endif



/**
  * @brief  system related task.
  * @param  None
  * @retval None
  */
void sys_task(void *argument)
{
    uint8_t ret = 0;
    uint32_t pre_IMU_time_cnt = 0;
    uint16_t i = 0;

    I2c_Init();
    osDelay(1000);
    IMU_filter_init();
    
    ret = lsm6ds_config();
    if(ret == 0)
    {
        osDelay(1000);
        ret = lsm6ds_config();
    }
    osDelay(100);



#ifdef FFT_TEST
    //------------------------FFT 快速傅里叶变换的测试------------------------
    
    for(i = 0; i < FFT_LENGTH; i++)//生成信号序列
    {
         fft_inputBuf[2*i] = 1 + 
                           1*arm_sin_f32(2*PI*10*i/FFT_LENGTH)+
                           2.2*arm_sin_f32(2*PI*22*i/FFT_LENGTH)+
                           3*arm_sin_f32(2*PI*38*i/FFT_LENGTH)+
                           5*arm_sin_f32(2*PI*50*i/FFT_LENGTH); //生成输入信号实部
                           
         fft_inputBuf[2*i+1]=0;//虚部全部为0
    }


    /*
        //初始化scfft结构体, 设定FFT相关参数
        用于初始化FFT运算相关参数, 其中：
        fftLen: 用于指定FFT长度(16/64/256/1024/4096), 设置为1024；
        ifftFlag: 用于指定是傅里叶变换(0)还是反傅里叶变换(1), 设置为0；
        bitReverseFlag: 用于设置是否按位取反, 设置为1；
        最后, 所有这些参数存储在一个arm_cfft_radix4_instance_f32结构体指针S里面.
    */
    arm_cfft_radix4_init_f32(&scfft, FFT_LENGTH, 0, 1);
    

    /*
        //FFT计算(基4)
        就是执行基4浮点FFT运算的, 
        pSrc传入采集到的输入信号数据(实部+虚部形式), 
        同时FFT变换后的数据, 也按顺序存放在pSrc里面, pSrc必须大于等于2倍fftLen长度, 
        另外, S结构体指针参数是先由arm_cfft_radix4_init_f32函数设置好, 
        然后传入该函数.     
    */
    arm_cfft_radix4_f32(&scfft, fft_inputBuf);

    
    /*
        //把运算结果复数求模得幅值 
        用于计算复数模值, 可以对FFT变换后的结果数据,执行取模操作。
        pSrc为复数输入数组(大小为2*numSamples)指针, 指向FFT变换后的结果；
        pDst为输出数组(大小为numSamples)指针, 存储取模后的值；
        numSamples就是总共有多少个数据需要取模.
    */
    arm_cmplx_mag_f32(fft_inputBuf, fft_outputBuf, FFT_LENGTH);


    //转换为Amplitude - frequency
    for(i = 0; i < FFT_LENGTH; i++)
    {
        if(i == 0)
        {
            fft_amplitude[i] = fft_outputBuf[i]*1/FFT_LENGTH;
        }
        else
        {
            fft_amplitude[i] = fft_outputBuf[i]*2/FFT_LENGTH;
        }
    }
    
    //------------------------FFT 快速傅里叶变换的测试------------------------
#endif





#ifdef MATRIX_TEST
    //------------------------矩阵运算测试------------------------

    //加法运算，初始化矩阵，指定矩阵的行、列
    pSrcA.numCols = 3;
    pSrcA.numRows = 3;
    pSrcA.pData = pDataA;

    pSrcB.numCols = 3;
    pSrcB.numRows = 3;
    pSrcB.pData = pDataB;

    pDst.numCols = 3;
    pDst.numRows = 3;
    pDst.pData = pData_add;

    //矩阵加法运算
    arm_mat_add_f32(&pSrcA, &pSrcB, &pDst);



    //乘法运算，初始化矩阵，指定矩阵的行、列
    pSrcA.numCols = 3;
    pSrcA.numRows = 2;
    pSrcA.pData = pDataC;

    pSrcB.numCols = 2;
    pSrcB.numRows = 3;
    pSrcB.pData = pDataD;

    pDst.numCols = 2;
    pDst.numRows = 2;
    pDst.pData = pData_multiply;

    //矩阵乘法运算
    arm_mat_mult_f32(&pSrcA, &pSrcB, &pDst);
    

    
    //------------------------矩阵运算测试------------------------
#endif




    
    while(1)
    {

        if(HAL_GetTick() - pre_IMU_time_cnt >= (1000/IMU_FREQUENCE))   //间隔10 ms 执行一次
        {
            pre_IMU_time_cnt = HAL_GetTick();

            for(i = 0; i < IMU_BUF_LEN; i++)
            {
                imu_read_buf[i] = 0x00;//buf 清零
            }
            
            ret = I2c_read_multi_byte(LSM6DS3_ACC_GYRO_STATUS_REG, imu_read_buf, 16);
            if(ret != 0)
            {
                //获取IMU 原始数据, 没有量程标定
                IMU_get_data_unscale();

                //根据设置的full-scale 满量程, 进行标定
                IMU_data_set_scale();

                //IMU 数据的低通滤波
                IMU_data_filter();

                //坐标系变换TF
                transform_frame();

                #if 0
                //姿态解算, 得到实时的角度
                IMU_get_angle();
                #else
                //姿态解算, 得到实时的角度
                IMUupdate(imu_data.gyro_data_filter.x, imu_data.gyro_data_filter.y, 
                    imu_data.gyro_data_filter.z, imu_data.accel_data_filter.x, 
                    imu_data.accel_data_filter.y, imu_data.accel_data_filter.z);
                #endif

            }
        }

        osDelay(2);
    }
}


/**
  * @brief  sys_task related reset create.
  * @param  None
  * @retval None
  */
void sys_task_reset_create(void)
{
    adc_reset_create();
    tm_reset_create();
}

/**
  * @brief  system timer isr handler. #define SYS_TIME_FREQUENCY 2000U, sys_time timer frequency in Hz,500us interrupt 
  * @param  None
  * @retval None
  */
void sys_timer_isr_handler(void)
{
    tm_task_tick_polling();
}


/****************************** END OF FILE ***********************************/

