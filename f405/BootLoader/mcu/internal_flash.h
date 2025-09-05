
#ifndef __INTERNAL_FLASH_H__
#define __INTERNAL_FLASH_H__

//STM32F405RGT6 �ڲ�Flash Ϊ1024 Kbytes

/* Public define ------------------------------------------------------------*/
//Flash �洢�Ĳ���ֵ��λ������, ��Ҫ��4 �ֽڶ���(����4 �ı���)
typedef enum
{
    //���У׼����: ��24 ���ֽ�
    FLASH_PARAM_MOTOR_BEGIN = 0,
    FLASH_PARAM_MOTOR_END = 23,

    //ϵͳ���еĲ���(APP �����õĲ���)
    FLASH_PARAM_MODE,  //ģʽѡ��
    FLASH_PARAM_ASSIST,  //�����ȼ�(1��2��3��4��5)
    FLASH_PARAM_LEFT_HIP,  //̧�ȸ߶�--����(0��1��2)
    FLASH_PARAM_RIGHT_HIP,  //̧�ȸ߶�--����(0��1��2)
    
    FLASH_PARAM_HEMIPLEGIA_LEG,  //ƫ̱ģʽ: 0û��ѡ��1��ࡢ2�Ҳ�
    FLASH_PARAM_VOICE,  //�����ȼ�(0��1��2��3��4��5)
    FLASH_PARAM_SPEED,  //�ٶȵȼ�(1��2��3��4��5)
    FLASH_PARAM_CHECK_ALARM,  //ˤ��������־(0��1)
    
    FLASH_PARAM_CHECK_SITDOWN,  //���¼���־(0��1)
    FLASH_PARAM_RESERVE_1,  //����, Ϊ���ܹ�4 �ֽڶ���
    FLASH_PARAM_RESERVE_2,  //����, Ϊ���ܹ�4 �ֽڶ���
    FLASH_PARAM_RESERVE_3,  //����, Ϊ���ܹ�4 �ֽڶ���
    
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


//�����ӿ�
void erase_sector(uint32_t sector_addr, uint32_t sector_num);
void write_internal_flash_without_erase(uint32_t addr, uint8_t* buf, uint16_t len);
void write_internal_flash(uint32_t addr, uint8_t* buf, uint8_t len);
void read_internal_flash(uint32_t addr, uint8_t* buf, uint8_t len);


#endif


