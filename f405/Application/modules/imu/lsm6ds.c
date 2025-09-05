
/* Private Includes ----------------------------------------------------------*/
#include "main.h"
#include "lsm6ds.h"
#include <math.h>
#include <stdio.h>
#include "i2c_if.h"


//IMU 数据缓冲区
uint8_t imu_read_buf[IMU_BUF_LEN] = {0x00};
uint8_t imu_write_buf[IMU_BUF_LEN] = {0x00};
IMU_DATA imu_data = {0x00};


/* Private function prototypes -----------------------------------------------*/
static float quick_invSqrt(float x);


//配置lsm6ds 的内部寄存器
uint8_t lsm6ds_config(void)
{
    uint8_t i2c_return_flag = 0;
    uint8_t temp_data = 0;

    //读取器件WHO_AM_I
    i2c_return_flag = I2c_read_byte(LSM6DS3_ACC_GYRO_WHO_AM_I_REG, imu_read_buf);
    if(i2c_return_flag == DISABLE)
    {
        return DISABLE;
    }

    if((imu_read_buf[0] != LSM6DS3_ACC_GYRO_WHO_AM_I) && 
        (imu_read_buf[0] != 0x6A))
    {
        return DISABLE;
    }

    temp_data = LSM6DS3_ACC_GYRO_IF_INC_ENABLED | LSM6DS3_ACC_GYRO_BDU_BLOCK_UPDATE;
    i2c_return_flag = I2c_write_byte(LSM6DS3_ACC_GYRO_CTRL3_C, temp_data);
    if(i2c_return_flag == DISABLE)
    {
        return DISABLE;
    }

    temp_data = LSM6DS3_ACC_GYRO_FIFO_MODE_BYPASS;
    i2c_return_flag = I2c_write_byte(LSM6DS3_ACC_GYRO_FIFO_CTRL5, temp_data);
    if(i2c_return_flag == DISABLE)
    {
        return DISABLE;
    }

    temp_data = LSM6DS3_ACC_GYRO_FS_XL_8g | LSM6DS3_ACC_GYRO_ODR_XL_1660Hz;
    i2c_return_flag = I2c_write_byte(LSM6DS3_ACC_GYRO_CTRL1_XL, temp_data);
    if(i2c_return_flag == DISABLE)
    {
        return DISABLE;
    }

    temp_data = LSM6DS3_ACC_GYRO_XEN_XL_ENABLED | LSM6DS3_ACC_GYRO_YEN_XL_ENABLED
            | LSM6DS3_ACC_GYRO_ZEN_XL_ENABLED;
    i2c_return_flag = I2c_write_byte(LSM6DS3_ACC_GYRO_CTRL9_XL, temp_data);
    if(i2c_return_flag == DISABLE)
    {
        return DISABLE;
    }

    temp_data = LSM6DS3_ACC_GYRO_FS_G_500dps | LSM6DS3_ACC_GYRO_ODR_G_1660Hz;
    i2c_return_flag = I2c_write_byte(LSM6DS3_ACC_GYRO_CTRL2_G, temp_data);
    if(i2c_return_flag == DISABLE)
    {
        return DISABLE;
    }

    temp_data = LSM6DS3_ACC_GYRO_XEN_G_ENABLED | LSM6DS3_ACC_GYRO_YEN_G_ENABLED
  	      | LSM6DS3_ACC_GYRO_ZEN_G_ENABLED;
    i2c_return_flag = I2c_write_byte(LSM6DS3_ACC_GYRO_CTRL10_C, temp_data);
    if(i2c_return_flag == DISABLE)
    {
        return DISABLE;
    }


    return ENABLE;
    
}


//获取IMU 原始数据, 没有量程标定
void IMU_get_data_unscale(void)
{
    //角速度、加速度是否有效
    if((imu_read_buf[0] & 0x03) == 0x03) /* gyro & accel data is available */
    {
        imu_data.gyro_data_unscaled.x = Int16FromBuf(imu_read_buf, 4);
        imu_data.gyro_data_unscaled.y = Int16FromBuf(imu_read_buf, 6);
        imu_data.gyro_data_unscaled.z = Int16FromBuf(imu_read_buf, 8);
        
        imu_data.accel_data_unscaled.x = Int16FromBuf(imu_read_buf, 10);
        imu_data.accel_data_unscaled.y = Int16FromBuf(imu_read_buf, 12);
        imu_data.accel_data_unscaled.z = Int16FromBuf(imu_read_buf, 14);
    }
    else
    {
        return;
    }
}

//根据设置的full-scale 满量程, 进行标定
void IMU_data_set_scale(void)
{
    //加速度full-scale 是8g(8*g=8*9.8=78.4), 对应int16_t, ±32768, 78.4/32768 = 0.002392578125
    imu_data.accel_data_scaled.x = (imu_data.accel_data_unscaled.x * 0.002392578125);
    imu_data.accel_data_scaled.y = (imu_data.accel_data_unscaled.y * 0.002392578125);
    imu_data.accel_data_scaled.z = (imu_data.accel_data_unscaled.z * 0.002392578125);

    //角速度full-scale 是500 deg/s, 对应int16_t, ±32768, 500/32768 = 0.0152587890625
    imu_data.gyro_data_scaled.x = (imu_data.gyro_data_unscaled.x * 0.0152587890625);
    imu_data.gyro_data_scaled.y = (imu_data.gyro_data_unscaled.y * 0.0152587890625);
    imu_data.gyro_data_scaled.z = (imu_data.gyro_data_unscaled.z * 0.0152587890625);  
}

void IMU_init_1th_filter(filter_info *filter, float cut_off, float value)
{
    filter->out = value;
    filter->ratio  = 1.0/(1.0/(cut_off*2.0*PI*IMU_CONTROL_CYCLE) + 1.0);
    //filter->ratio  = (1.0 - 0.618);  //黄金分割比例值
}

float IMU_update_1th_filter(filter_info *filter, float value)
{
    filter->out = filter->ratio * value + filter->out * (1 - filter->ratio);
    return filter->out;
}


//初始化IMU 滤波
void IMU_filter_init(void)
{
    IMU_init_1th_filter(&imu_data.accel_filter[IMU_AXIS_X], IMU_CUT_OFF_FREQ, 0);
    IMU_init_1th_filter(&imu_data.accel_filter[IMU_AXIS_Y], IMU_CUT_OFF_FREQ, 0);
    IMU_init_1th_filter(&imu_data.accel_filter[IMU_AXIS_Z], IMU_CUT_OFF_FREQ, 0);
    
    IMU_init_1th_filter(&imu_data.gyro_filter[IMU_AXIS_X], IMU_CUT_OFF_FREQ, 0);
    IMU_init_1th_filter(&imu_data.gyro_filter[IMU_AXIS_Y], IMU_CUT_OFF_FREQ, 0);
    IMU_init_1th_filter(&imu_data.gyro_filter[IMU_AXIS_Z], IMU_CUT_OFF_FREQ, 0);    
}


//IMU 数据的低通滤波
void IMU_data_filter(void)
{
    //加速度
    imu_data.accel_data_filter.x = IMU_update_1th_filter(
        &imu_data.accel_filter[IMU_AXIS_X], imu_data.accel_data_scaled.x);

    imu_data.accel_data_filter.y = IMU_update_1th_filter(
        &imu_data.accel_filter[IMU_AXIS_Y], imu_data.accel_data_scaled.y);

    imu_data.accel_data_filter.z = IMU_update_1th_filter(
        &imu_data.accel_filter[IMU_AXIS_Z], imu_data.accel_data_scaled.z);

    //角速度
    imu_data.gyro_data_filter.x = IMU_update_1th_filter(
        &imu_data.gyro_filter[IMU_AXIS_X], imu_data.gyro_data_scaled.x);

    imu_data.gyro_data_filter.y = IMU_update_1th_filter(
        &imu_data.gyro_filter[IMU_AXIS_Y], imu_data.gyro_data_scaled.y);

    imu_data.gyro_data_filter.z = IMU_update_1th_filter(
        &imu_data.gyro_filter[IMU_AXIS_Z], imu_data.gyro_data_scaled.z);
    
}

//坐标系变换TF
void transform_frame(void)
{
    /*  
        因为IMU器件贴片及安装方向不一样，要做坐标系变换
        这里的采集板IMU的坐标转换是: x = x; y = -z; z = y;
    */
    
    float temp = 0.0;
    
    //加速度计的数据坐标转换:
    temp = imu_data.accel_data_filter.y;
    imu_data.accel_data_filter.x = imu_data.accel_data_filter.x;
    imu_data.accel_data_filter.y = -imu_data.accel_data_filter.z;
    imu_data.accel_data_filter.z = temp;
    
    //陀螺仪的数据坐标转换:
    temp = imu_data.gyro_data_filter.x;
    imu_data.gyro_data_filter.x = -imu_data.gyro_data_filter.x;
    imu_data.gyro_data_filter.y = -imu_data.gyro_data_filter.z;
    imu_data.gyro_data_filter.z = temp;

}

//姿态解算, 得到实时的角度
void IMU_get_angle(void)
{
    //计算-atan2f(ax, sqrt(ay * ay + az * az))
    imu_data.eulerf.theta = -atan2f(imu_data.accel_data_filter.x, 
        1.0/quick_invSqrt(imu_data.accel_data_filter.y * imu_data.accel_data_filter.y + 
        imu_data.accel_data_filter.z * imu_data.accel_data_filter.z)) * 57.3;

#if 1   /* Begin if 0 by lison on 2020-11-16 */
    //计算atan2f(ay, sqrt(ax * ax + az * az))
    imu_data.eulerf.phi = atan2f(imu_data.accel_data_filter.y, 
        1.0/quick_invSqrt(imu_data.accel_data_filter.x * imu_data.accel_data_filter.x + 
        imu_data.accel_data_filter.z * imu_data.accel_data_filter.z)) * 57.3;
#else
    //计算atan2f(ay, az)
    imu_data.eulerf.phi = atan2f(imu_data.accel_data_filter.y, imu_data.accel_data_filter.z) * 57.3;
#endif  /* End if 0 by lison on 2020-11-16 */


    //修正角度
    imu_data.eulerf.phi = imu_data.eulerf.phi + 90;  //俯仰角
    imu_data.eulerf.theta = imu_data.eulerf.theta + 0;  //横滚角
    
}

//-------------------------Start IMUupdate()-------------------------
#define Kp     100.0f    // 比例增益支配率收敛到加速度计/磁强计
#define Ki      0.002f    // 积分增益支配率的陀螺仪偏见的衔接
#define halfT (0.5f/IMU_FREQUENCE)   // 采样周期的一半

//更新四元数, 得到角度
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{

    static float q0 = 1, q1 = 0, q2 = 0, q3 = 0;  // quaternion elements representing the estimated orientation
    static float exInt = 0, eyInt = 0, ezInt = 0;  // scaled integral error

    float norm;
    float vx, vy, vz;
    float ex, ey, ez;  

    float temp_phi; //in radians
    float temp_theta; // in radians

    if((ax == 0) || (ay == 0) || (az == 0))
    {
        return;
    }

    // 测量正常化
    norm = quick_invSqrt(ax*ax + ay*ay + az*az);      
    ax *= norm;                   //单位化
    ay *= norm;
    az *= norm;      

    // 估计方向的重力
    vx = 2*(q1*q3 - q0*q2);
    vy = 2*(q0*q1 + q2*q3);
    vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

    // 错误的领域和方向传感器测量参考方向之间的交叉乘积的总和
    ex = (ay*vz - az*vy);
    ey = (az*vx - ax*vz);
    ez = (ax*vy - ay*vx);

    // 积分误差比例积分增益
    exInt = exInt + ex*Ki;
    eyInt = eyInt + ey*Ki;
    ezInt = ezInt + ez*Ki;

    // 调整后的陀螺仪测量
    gx = gx + Kp*ex + exInt;
    gy = gy + Kp*ey + eyInt;
    gz = gz + Kp*ez + ezInt;

    // 整合四元数率和正常化
    q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  

    // 正常化四元
    norm = quick_invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 *= norm;
    q1 *= norm;
    q2 *= norm;
    q3 *= norm;

    //计算得到角度 + 180
    temp_phi = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1) * 57.3; // roll
    temp_theta  = asin(-2 * q1 * q3 + 2 * q0* q2) * 57.3; // pitch 
    //imu_data.eulerf_Mayhony.psi  = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;  //此处没有价值，注掉

    if((temp_phi > 90) || (temp_phi < -90))
    {
        if(temp_theta > 0)
        {
            temp_theta =180 - temp_theta;
        }
        else if(temp_theta < 0)
        {
            temp_theta = -(180 + temp_theta);
        }
    }

    //不要出现负数, 故加180 度
    imu_data.eulerf_Mayhony.phi = temp_phi + 0;
    imu_data.eulerf_Mayhony.theta = temp_theta + 0;


}
//-------------------------End IMUupdate()-------------------------


//快速求平方根倒数
static float quick_invSqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long*)&y;
    
    i = 0x5f3759df - (i>>1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));

    return y;
}



