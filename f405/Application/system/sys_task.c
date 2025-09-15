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

//�Ƿ�ʹ�ܿ��ٸ���Ҷ�任����
//#define FFT_TEST

#ifdef FFT_TEST
//------------------------FFT ���ٸ���Ҷ�任�Ĳ���------------------------
#include "arm_math.h"

#define FFT_LENGTH		256 		//FFT����(����ʹ��radix4 ��4, ���Ը�ֵ������4 ��N �η�)

float fft_inputBuf[FFT_LENGTH*2];	//FFT��������
float fft_outputBuf[FFT_LENGTH];	//FFT�������
float fft_amplitude[FFT_LENGTH];	//FFT��ֵ����

static arm_cfft_radix4_instance_f32 scfft;

//------------------------FFT ���ٸ���Ҷ�任�Ĳ���------------------------
#endif





//�Ƿ�ʹ�ܾ����������
//#define MATRIX_TEST

#ifdef MATRIX_TEST
//------------------------�����������------------------------
#include "arm_math.h"

//����ĳ��ú����ӿ�:

//arm_mat_add_f32();  //����ӷ�

//arm_mat_sub_f32();  //�������

//arm_mat_mult_f32();  //����˷�

//arm_mat_inverse_f32();  //�����

//arm_mat_trans_f32();  //����ת��

//arm_mat_scale_f32();  //�������



arm_matrix_instance_f32 pSrcA;
arm_matrix_instance_f32 pSrcB;
arm_matrix_instance_f32 pDst;

//����ӷ�
float32_t pDataA[9] = {1.1, 1.2, 1.3, 2.1, 2.2, 2.3, 3.1, 3.2, 3.3};//����3*3
float32_t pDataB[9] = {1.1, 1.2, 1.3, 2.1, 2.2, 2.3, 3.1, 3.2, 3.3};//����3*3
float32_t pData_add[9] = {0.0};//����3*3

//����˷�
float32_t pDataC[6] = {1, 2, 3, 4, 5, 6};//����3*2
float32_t pDataD[6] = {7, 8, 9, 10, 11, 12};//����2*3
float32_t pData_multiply[4] = {0.0};//����2*2


//------------------------�����������------------------------
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
    //------------------------FFT ���ٸ���Ҷ�任�Ĳ���------------------------
    
    for(i = 0; i < FFT_LENGTH; i++)//�����ź�����
    {
         fft_inputBuf[2*i] = 1 + 
                           1*arm_sin_f32(2*PI*10*i/FFT_LENGTH)+
                           2.2*arm_sin_f32(2*PI*22*i/FFT_LENGTH)+
                           3*arm_sin_f32(2*PI*38*i/FFT_LENGTH)+
                           5*arm_sin_f32(2*PI*50*i/FFT_LENGTH); //���������ź�ʵ��
                           
         fft_inputBuf[2*i+1]=0;//�鲿ȫ��Ϊ0
    }


    /*
        //��ʼ��scfft�ṹ��, �趨FFT��ز���
        ���ڳ�ʼ��FFT������ز���, ���У�
        fftLen: ����ָ��FFT����(16/64/256/1024/4096), ����Ϊ1024��
        ifftFlag: ����ָ���Ǹ���Ҷ�任(0)���Ƿ�����Ҷ�任(1), ����Ϊ0��
        bitReverseFlag: ���������Ƿ�λȡ��, ����Ϊ1��
        ���, ������Щ�����洢��һ��arm_cfft_radix4_instance_f32�ṹ��ָ��S����.
    */
    arm_cfft_radix4_init_f32(&scfft, FFT_LENGTH, 0, 1);
    

    /*
        //FFT����(��4)
        ����ִ�л�4����FFT�����, 
        pSrc����ɼ����������ź�����(ʵ��+�鲿��ʽ), 
        ͬʱFFT�任�������, Ҳ��˳������pSrc����, pSrc������ڵ���2��fftLen����, 
        ����, S�ṹ��ָ�����������arm_cfft_radix4_init_f32�������ú�, 
        Ȼ����ú���.     
    */
    arm_cfft_radix4_f32(&scfft, fft_inputBuf);

    
    /*
        //��������������ģ�÷�ֵ 
        ���ڼ��㸴��ģֵ, ���Զ�FFT�任��Ľ������,ִ��ȡģ������
        pSrcΪ������������(��СΪ2*numSamples)ָ��, ָ��FFT�任��Ľ����
        pDstΪ�������(��СΪnumSamples)ָ��, �洢ȡģ���ֵ��
        numSamples�����ܹ��ж��ٸ�������Ҫȡģ.
    */
    arm_cmplx_mag_f32(fft_inputBuf, fft_outputBuf, FFT_LENGTH);


    //ת��ΪAmplitude - frequency
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
    
    //------------------------FFT ���ٸ���Ҷ�任�Ĳ���------------------------
#endif





#ifdef MATRIX_TEST
    //------------------------�����������------------------------

    //�ӷ����㣬��ʼ������ָ��������С���
    pSrcA.numCols = 3;
    pSrcA.numRows = 3;
    pSrcA.pData = pDataA;

    pSrcB.numCols = 3;
    pSrcB.numRows = 3;
    pSrcB.pData = pDataB;

    pDst.numCols = 3;
    pDst.numRows = 3;
    pDst.pData = pData_add;

    //����ӷ�����
    arm_mat_add_f32(&pSrcA, &pSrcB, &pDst);



    //�˷����㣬��ʼ������ָ��������С���
    pSrcA.numCols = 3;
    pSrcA.numRows = 2;
    pSrcA.pData = pDataC;

    pSrcB.numCols = 2;
    pSrcB.numRows = 3;
    pSrcB.pData = pDataD;

    pDst.numCols = 2;
    pDst.numRows = 2;
    pDst.pData = pData_multiply;

    //����˷�����
    arm_mat_mult_f32(&pSrcA, &pSrcB, &pDst);
    

    
    //------------------------�����������------------------------
#endif




    
    while(1)
    {

        if(HAL_GetTick() - pre_IMU_time_cnt >= (1000/IMU_FREQUENCE))   //���10 ms ִ��һ��
        {
            pre_IMU_time_cnt = HAL_GetTick();

            for(i = 0; i < IMU_BUF_LEN; i++)
            {
                imu_read_buf[i] = 0x00;//buf ����
            }
            
            ret = I2c_read_multi_byte(LSM6DS3_ACC_GYRO_STATUS_REG, imu_read_buf, 16);
            if(ret != 0)
            {
                //��ȡIMU ԭʼ����, û�����̱궨
                IMU_get_data_unscale();

                //�������õ�full-scale ������, ���б궨
                IMU_data_set_scale();

                //IMU ���ݵĵ�ͨ�˲�
                IMU_data_filter();

                //����ϵ�任TF
                transform_frame();

                #if 0
                //��̬����, �õ�ʵʱ�ĽǶ�
                IMU_get_angle();
                #else
                //��̬����, �õ�ʵʱ�ĽǶ�
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

