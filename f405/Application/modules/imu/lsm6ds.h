
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LSM6DS_H__
#define __LSM6DS_H__

/* Includes ------------------------------------------------------------------*/
#include "mcu_if.h"
#include <stdint.h>
#include "algorithm.h"


//--------------------------------函数接口--------------------------------
uint8_t lsm6ds_config(void);
void IMU_get_data_unscale(void);
void IMU_data_filter(void);
void IMU_data_set_scale(void);
void transform_frame(void);
void IMU_get_angle(void);
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);
void IMU_init_1th_filter(filter_info *filter, float cut_off, float value);
float IMU_update_1th_filter(filter_info *filter, float value);
void IMU_filter_init(void);


/* Exported macro ------------------------------------------------------------*/

//是否使用Mayhony 滤波(或只使用加速度值, 计算atan得到)
#define IMU_CALC_MODE_Mayhony  //使用Mayhony 滤波

typedef enum
{
    IMU_AXIS_X = 0,
    IMU_AXIS_Y,
    IMU_AXIS_Z,

    IMU_AXIS_TOTAL,    
} IMU_AXIS;


//IMU 的数据
struct Int16Vect3 
{
    int16_t x;
    int16_t y;
    int16_t z;
};

struct FloatVect3 
{
    float x;
    float y;
    float z;
};

struct FloatEulers 
{
    float phi; //in radians，俯仰角
    float theta; // in radians，横滚角
    float psi; // in radians
};

typedef struct
{
    struct Int16Vect3 accel_data_unscaled;//原始数据, 没有量程标定
    struct FloatVect3 accel_data_scaled;//尺度标定后的实际数据
    filter_info accel_filter[IMU_AXIS_TOTAL];
    struct FloatVect3 accel_data_filter;//滤波后的数据
    
    struct Int16Vect3 gyro_data_unscaled;
    struct FloatVect3 gyro_data_scaled;
    filter_info gyro_filter[IMU_AXIS_TOTAL];
    struct FloatVect3 gyro_data_filter;
    
    struct FloatEulers eulerf;//只使用加速度值, 计算atan得到的角度
    struct FloatEulers eulerf_Mayhony;//Mayhony滤波得到的角度
}IMU_DATA;


#define Int16FromBuf(_buf,_idx) ((int16_t)((_buf[_idx+1]<<8) | _buf[_idx]))

#define IMU_BUF_LEN     20
extern uint8_t imu_read_buf[];
extern uint8_t imu_write_buf[];
extern IMU_DATA imu_data;




//IMU 姿态解算的频率
#define IMU_FREQUENCE   100
#define IMU_CONTROL_CYCLE (1.0/IMU_FREQUENCE)

//一般IMU的数据滤波采用低通滤波即可，截止频率50以下
#define IMU_CUT_OFF_FREQ    10

//I2C Address (I2C write, bit0 = 0; IIC read, bit0 = 1)
#define LSM6D_ACC_GYRO_ADDR   0xD4  // SAD[0] = 0

/************** Who am I  *******************/
#define LSM6DS3_ACC_GYRO_WHO_AM_I         0x69


/************** Device Register  *******************/
#define LSM6DS3_ACC_GYRO_FUNC_CFG_ACCESS    0X01
#define LSM6DS3_ACC_GYRO_SENSOR_SYNC_TIME   0X04
#define LSM6DS3_ACC_GYRO_FIFO_CTRL1   0X06
#define LSM6DS3_ACC_GYRO_FIFO_CTRL2   0X07
#define LSM6DS3_ACC_GYRO_FIFO_CTRL3   0X08
#define LSM6DS3_ACC_GYRO_FIFO_CTRL4   0X09
#define LSM6DS3_ACC_GYRO_FIFO_CTRL5   0X0A
#define LSM6DS3_ACC_GYRO_ORIENT_CFG_G   0X0B
#define LSM6DS3_ACC_GYRO_INT1_CTRL    0X0D
#define LSM6DS3_ACC_GYRO_INT2_CTRL    0X0E
#define LSM6DS3_ACC_GYRO_WHO_AM_I_REG   0X0F
#define LSM6DS3_ACC_GYRO_CTRL1_XL   0X10
#define LSM6DS3_ACC_GYRO_CTRL2_G    0X11
#define LSM6DS3_ACC_GYRO_CTRL3_C    0X12
#define LSM6DS3_ACC_GYRO_CTRL4_C    0X13
#define LSM6DS3_ACC_GYRO_CTRL5_C    0X14
#define LSM6DS3_ACC_GYRO_CTRL6_G    0X15
#define LSM6DS3_ACC_GYRO_CTRL7_G    0X16
#define LSM6DS3_ACC_GYRO_CTRL8_XL   0X17
#define LSM6DS3_ACC_GYRO_CTRL9_XL   0X18
#define LSM6DS3_ACC_GYRO_CTRL10_C   0X19
#define LSM6DS3_ACC_GYRO_MASTER_CONFIG    0X1A
#define LSM6DS3_ACC_GYRO_WAKE_UP_SRC    0X1B
#define LSM6DS3_ACC_GYRO_TAP_SRC    0X1C
#define LSM6DS3_ACC_GYRO_D6D_SRC    0X1D
#define LSM6DS3_ACC_GYRO_STATUS_REG   0X1E
#define LSM6DS3_ACC_GYRO_OUT_TEMP_L   0X20
#define LSM6DS3_ACC_GYRO_OUT_TEMP_H   0X21
#define LSM6DS3_ACC_GYRO_OUTX_L_G   0X22
#define LSM6DS3_ACC_GYRO_OUTX_H_G   0X23
#define LSM6DS3_ACC_GYRO_OUTY_L_G   0X24
#define LSM6DS3_ACC_GYRO_OUTY_H_G   0X25
#define LSM6DS3_ACC_GYRO_OUTZ_L_G   0X26
#define LSM6DS3_ACC_GYRO_OUTZ_H_G   0X27
#define LSM6DS3_ACC_GYRO_OUTX_L_XL    0X28
#define LSM6DS3_ACC_GYRO_OUTX_H_XL    0X29
#define LSM6DS3_ACC_GYRO_OUTY_L_XL    0X2A
#define LSM6DS3_ACC_GYRO_OUTY_H_XL    0X2B
#define LSM6DS3_ACC_GYRO_OUTZ_L_XL    0X2C
#define LSM6DS3_ACC_GYRO_OUTZ_H_XL    0X2D
#define LSM6DS3_ACC_GYRO_SENSORHUB1_REG   0X2E
#define LSM6DS3_ACC_GYRO_SENSORHUB2_REG   0X2F
#define LSM6DS3_ACC_GYRO_SENSORHUB3_REG   0X30
#define LSM6DS3_ACC_GYRO_SENSORHUB4_REG   0X31
#define LSM6DS3_ACC_GYRO_SENSORHUB5_REG   0X32
#define LSM6DS3_ACC_GYRO_SENSORHUB6_REG   0X33
#define LSM6DS3_ACC_GYRO_SENSORHUB7_REG   0X34
#define LSM6DS3_ACC_GYRO_SENSORHUB8_REG   0X35
#define LSM6DS3_ACC_GYRO_SENSORHUB9_REG   0X36
#define LSM6DS3_ACC_GYRO_SENSORHUB10_REG    0X37
#define LSM6DS3_ACC_GYRO_SENSORHUB11_REG    0X38
#define LSM6DS3_ACC_GYRO_SENSORHUB12_REG    0X39
#define LSM6DS3_ACC_GYRO_FIFO_STATUS1   0X3A
#define LSM6DS3_ACC_GYRO_FIFO_STATUS2   0X3B
#define LSM6DS3_ACC_GYRO_FIFO_STATUS3   0X3C
#define LSM6DS3_ACC_GYRO_FIFO_STATUS4   0X3D
#define LSM6DS3_ACC_GYRO_FIFO_DATA_OUT_L    0X3E
#define LSM6DS3_ACC_GYRO_FIFO_DATA_OUT_H    0X3F
#define LSM6DS3_ACC_GYRO_TIMESTAMP0_REG   0X40
#define LSM6DS3_ACC_GYRO_TIMESTAMP1_REG   0X41
#define LSM6DS3_ACC_GYRO_TIMESTAMP2_REG   0X42

#define LSM6DS3_ACC_GYRO_TIMESTAMP_L    0X49
#define LSM6DS3_ACC_GYRO_TIMESTAMP_H    0X4A

#define LSM6DS3_ACC_GYRO_STEP_COUNTER_L   0X4B
#define LSM6DS3_ACC_GYRO_STEP_COUNTER_H   0X4C

#define LSM6DS3_ACC_GYRO_SENSORHUB13_REG    0X4D
#define LSM6DS3_ACC_GYRO_SENSORHUB14_REG    0X4E
#define LSM6DS3_ACC_GYRO_SENSORHUB15_REG    0X4F
#define LSM6DS3_ACC_GYRO_SENSORHUB16_REG    0X50
#define LSM6DS3_ACC_GYRO_SENSORHUB17_REG    0X51
#define LSM6DS3_ACC_GYRO_SENSORHUB18_REG    0X52

#define LSM6DS3_ACC_GYRO_FUNC_SRC   0X53
#define LSM6DS3_ACC_GYRO_TAP_CFG1   0X58
#define LSM6DS3_ACC_GYRO_TAP_THS_6D   0X59
#define LSM6DS3_ACC_GYRO_INT_DUR2   0X5A
#define LSM6DS3_ACC_GYRO_WAKE_UP_THS    0X5B
#define LSM6DS3_ACC_GYRO_WAKE_UP_DUR    0X5C
#define LSM6DS3_ACC_GYRO_FREE_FALL    0X5D
#define LSM6DS3_ACC_GYRO_MD1_CFG    0X5E
#define LSM6DS3_ACC_GYRO_MD2_CFG    0X5F



typedef enum
{
    LSM6DS3_ACC_GYRO_IF_INC_DISABLED     = 0x00,
    LSM6DS3_ACC_GYRO_IF_INC_ENABLED      = 0x04,
} LSM6DS3_ACC_GYRO_IF_INC_t;

typedef enum
{
    LSM6DS3_ACC_GYRO_BDU_CONTINUOS     = 0x00,
    LSM6DS3_ACC_GYRO_BDU_BLOCK_UPDATE      = 0x40,
} LSM6DS3_ACC_GYRO_BDU_t;

typedef enum
{
    LSM6DS3_ACC_GYRO_FIFO_MODE_BYPASS      = 0x00,
    LSM6DS3_ACC_GYRO_FIFO_MODE_FIFO      = 0x01,
    LSM6DS3_ACC_GYRO_FIFO_MODE_STREAM      = 0x02,
    LSM6DS3_ACC_GYRO_FIFO_MODE_STF     = 0x03,
    LSM6DS3_ACC_GYRO_FIFO_MODE_BTS     = 0x04,
    LSM6DS3_ACC_GYRO_FIFO_MODE_DYN_STREAM      = 0x05,
    LSM6DS3_ACC_GYRO_FIFO_MODE_DYN_STREAM_2      = 0x06,
    LSM6DS3_ACC_GYRO_FIFO_MODE_BTF     = 0x07,
} LSM6DS3_ACC_GYRO_FIFO_MODE_t;

typedef enum
{
    LSM6DS3_ACC_GYRO_FS_XL_2g      = 0x00,
    LSM6DS3_ACC_GYRO_FS_XL_16g     = 0x04,
    LSM6DS3_ACC_GYRO_FS_XL_4g      = 0x08,
    LSM6DS3_ACC_GYRO_FS_XL_8g      = 0x0C,
} LSM6DS3_ACC_GYRO_FS_XL_t;

typedef enum
{
    LSM6DS3_ACC_GYRO_ODR_XL_POWER_DOWN     = 0x00,
    LSM6DS3_ACC_GYRO_ODR_XL_13Hz     = 0x10,
    LSM6DS3_ACC_GYRO_ODR_XL_26Hz     = 0x20,
    LSM6DS3_ACC_GYRO_ODR_XL_52Hz     = 0x30,
    LSM6DS3_ACC_GYRO_ODR_XL_104Hz      = 0x40,
    LSM6DS3_ACC_GYRO_ODR_XL_208Hz      = 0x50,
    LSM6DS3_ACC_GYRO_ODR_XL_416Hz      = 0x60,
    LSM6DS3_ACC_GYRO_ODR_XL_833Hz      = 0x70,
    LSM6DS3_ACC_GYRO_ODR_XL_1660Hz     = 0x80,
    LSM6DS3_ACC_GYRO_ODR_XL_3330Hz     = 0x90,
    LSM6DS3_ACC_GYRO_ODR_XL_6660Hz     = 0xA0
} LSM6DS3_ACC_GYRO_ODR_XL_t;

typedef enum
{
    LSM6DS3_ACC_GYRO_XEN_XL_DISABLED     = 0x00,
    LSM6DS3_ACC_GYRO_XEN_XL_ENABLED      = 0x08,
} LSM6DS3_ACC_GYRO_XEN_XL_t;

typedef enum
{
    LSM6DS3_ACC_GYRO_YEN_XL_DISABLED     = 0x00,
    LSM6DS3_ACC_GYRO_YEN_XL_ENABLED      = 0x10,
} LSM6DS3_ACC_GYRO_YEN_XL_t;

typedef enum
{
    LSM6DS3_ACC_GYRO_ZEN_XL_DISABLED     = 0x00,
    LSM6DS3_ACC_GYRO_ZEN_XL_ENABLED      = 0x20,
} LSM6DS3_ACC_GYRO_ZEN_XL_t;

typedef enum
{
    LSM6DS3_ACC_GYRO_FS_G_245dps     = 0x00,
    LSM6DS3_ACC_GYRO_FS_G_500dps     = 0x04,
    LSM6DS3_ACC_GYRO_FS_G_1000dps      = 0x08,
    LSM6DS3_ACC_GYRO_FS_G_2000dps      = 0x0C,
} LSM6DS3_ACC_GYRO_FS_G_t;

typedef enum
{
    LSM6DS3_ACC_GYRO_ODR_G_POWER_DOWN      = 0x00,
    LSM6DS3_ACC_GYRO_ODR_G_13Hz      = 0x10,
    LSM6DS3_ACC_GYRO_ODR_G_26Hz      = 0x20,
    LSM6DS3_ACC_GYRO_ODR_G_52Hz      = 0x30,
    LSM6DS3_ACC_GYRO_ODR_G_104Hz     = 0x40,
    LSM6DS3_ACC_GYRO_ODR_G_208Hz     = 0x50,
    LSM6DS3_ACC_GYRO_ODR_G_416Hz     = 0x60,
    LSM6DS3_ACC_GYRO_ODR_G_833Hz     = 0x70,
    LSM6DS3_ACC_GYRO_ODR_G_1660Hz      = 0x80,
} LSM6DS3_ACC_GYRO_ODR_G_t;

typedef enum
{
    LSM6DS3_ACC_GYRO_XEN_G_DISABLED      = 0x00,
    LSM6DS3_ACC_GYRO_XEN_G_ENABLED     = 0x08,
} LSM6DS3_ACC_GYRO_XEN_G_t;

typedef enum
{
    LSM6DS3_ACC_GYRO_YEN_G_DISABLED      = 0x00,
    LSM6DS3_ACC_GYRO_YEN_G_ENABLED     = 0x10,
} LSM6DS3_ACC_GYRO_YEN_G_t;

typedef enum
{
    LSM6DS3_ACC_GYRO_ZEN_G_DISABLED      = 0x00,
    LSM6DS3_ACC_GYRO_ZEN_G_ENABLED     = 0x20,
} LSM6DS3_ACC_GYRO_ZEN_G_t;




#endif /* __LSM6DS_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

