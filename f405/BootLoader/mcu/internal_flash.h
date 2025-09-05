
#ifndef __INTERNAL_FLASH_H__
#define __INTERNAL_FLASH_H__

//STM32F405RGT6 内部Flash 为1024 Kbytes

/* Public define ------------------------------------------------------------*/
//Flash 存储的参数值的位置索引, 需要以4 字节对齐(即是4 的倍数)
typedef enum
{
    //电机校准参数: 共24 个字节
    FLASH_PARAM_MOTOR_BEGIN = 0,
    FLASH_PARAM_MOTOR_END = 23,

    //系统运行的参数(APP 端设置的参数)
    FLASH_PARAM_MODE,  //模式选择
    FLASH_PARAM_ASSIST,  //助力等级(1、2、3、4、5)
    FLASH_PARAM_LEFT_HIP,  //抬腿高度--左髋(0、1、2)
    FLASH_PARAM_RIGHT_HIP,  //抬腿高度--右髋(0、1、2)
    
    FLASH_PARAM_HEMIPLEGIA_LEG,  //偏瘫模式: 0没有选择、1左侧、2右侧
    FLASH_PARAM_VOICE,  //音量等级(0、1、2、3、4、5)
    FLASH_PARAM_SPEED,  //速度等级(1、2、3、4、5)
    FLASH_PARAM_CHECK_ALARM,  //摔倒报警标志(0、1)
    
    FLASH_PARAM_CHECK_SITDOWN,  //坐下检测标志(0、1)
    FLASH_PARAM_RESERVE_1,  //保留, 为了能够4 字节对齐
    FLASH_PARAM_RESERVE_2,  //保留, 为了能够4 字节对齐
    FLASH_PARAM_RESERVE_3,  //保留, 为了能够4 字节对齐
    
    PARAM_BUF_LEN,
}FLASH_PARAM_INDEX;	

extern uint8_t param_buf[PARAM_BUF_LEN];


/* Private define ------------------------------------------------------------*/

/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */


//函数接口
void erase_sector(uint32_t sector_addr, uint32_t sector_num);
void write_internal_flash_without_erase(uint32_t addr, uint8_t* buf, uint16_t len);
void write_internal_flash(uint32_t addr, uint8_t* buf, uint8_t len);
void read_internal_flash(uint32_t addr, uint8_t* buf, uint8_t len);


#endif


