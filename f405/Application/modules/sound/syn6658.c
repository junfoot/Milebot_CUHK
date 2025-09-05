/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : can.c
* Author      : LisonLiu
* Version     : v0.01
* Date        : 2020/6/1
* Description : 
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2020/6/1  LisonLiu    initial version
*=============================================================================*/
/* System Includes -----------------------------------------------------------*/

/* Public Includes -----------------------------------------------------------*/
#include "cmsis_os2.h"                  // ARM::CMSIS:RTOS2:Keil RTX5
#include "stm32f4xx_hal.h"

#include "main.h"
#include "..\system\std.h"

#include "syn6658.h"
#include "uart.h"
#include "uart_if.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "ble_if.h"  


/* Private variables ---------------------------------------------------------*/

//�ж�оƬ�Ƿ��ڲ�������
static uint8_t syn6658_status = SYN6658_STATUS_IDLE; // Ĭ��״̬

//������������list ,����ҪС��128 ���ֽ�
//const char* TTS_list[] =  
//{
//	"��ӭʹ������������",
//	"��ȷ��ϵ�ð��",
//	"�뿪ʼ����",
//	"��ʼ����",
//	"��ص����ͣ��뼰ʱ���",
//	"����������",
//	"�����ѶϿ�",
//	"��̧����",
//	"��̧����",
//	"����[=xing2]ģʽ",
//	"��ƫ̱ģʽ",
//	"��ƫ̱ģʽ",
//	"����ɭģʽ",
//	"����ģʽ",
//	"����ģʽ",
//	"����ģʽ",
//	"sound401[p100]sound401[p100]",
//	"sound401[p200]",
//	"sound303[p200]sound303",
//	"Ϊ�˸��õ�ʹ�����飬���������豸���",
//	"����ģʽ",
//	"�뱣��վ��",
//	"��ʼ�������뿪ʼ����",
//	"��������",
//	"һ������",
//	"��������",
//	"��������",
//	"�ĵ�����",
//	"�嵲����",
//};

#if 1//��ͨ���汾 �� ע�⣬��ͨ���汾��Ӣ��汾IC�汾��һ��
const char* TTS_list[] =  
{
	"��ӭʹ������������",//"��ӭʹ������[=xing2]������",
	"��ȷ��ϵ�ð��",
	"�뿪ʼ����",
	"��ʼ����",
	"��ص����ͣ��뼰ʱ���",
	"����������",
	"�����ѶϿ�",
	"��̧����",
	"��̧����",
	"˫������ģʽ",
	"������ģʽ",
	"������ģʽ",
	"����ģʽ",
	"����ģʽ",
	"����ģʽ",
	"����ģʽ",
	"sound401[p100]sound401[p100]",
	"sound401[p200]",
	"sound303[p200]sound303",
	"Ϊ�˸��õ�ʹ�����飬���������豸���",
	"����ģʽ",
	"�뱣��վ��",
	"��ʼ�������뿪ʼ����",
	"��������",
	"һ������",
	"��������",
	"��������",
	"�ĵ�����",
	"�嵲����",
	"һ������",
	"��������",
	"��������",
	"�ĵ�����",
	"�嵲����",
};
#else
const char* TTS_list[] =  

{
	"welcome to maibu", 
	"please confirm to tie your bandage", 
	"start to walk", 
	"start to assist", 
	"battery is low, please charge in time", 
	"device is connected with cellphone", 
	"device is offline", 
	"please lift left leg", 
	"please lift right leg", 
	"assist mode", 
	"left hemiplegia mode", 
	"right hemiplegia mode", 
	"parkinson mode", 
	"zero force mode", 
	"passive mode", 
	"test mode", 
	"sound401[p100]sound401[p100]", 
	"sound401[p200]", 
	"sound303[p200]sound303", 
	"ful better use, it is recommended to upgrade the device software", 
	"evaluation mode", 
	"please keep standing", 
	"start evaluation, please walk", 
	"evaluation is end", 
	"level one", 
	"level two", 
	"level three", 
	"level ful", 
	"level five", 
};
#endif 

//�������Ŷ���
SYN6658_QUEUE syn6658_queue;


/* Private function prototypes -----------------------------------------------*/
static void syn6658_init(void);
static void syn6658_combine_play(SOUND_LIST list);
static void syn6658_send_data(const char* data, uint8_t len);


/* Private user code ---------------------------------------------------------*/

/**
  * @brief  task to handle all sound function .
  * @param  None
  * @retval None
  */
void sound_task(void *argument)
{
    SOUND_FSM syn6658_FSM = SOUND_FSM_IDLE;
    SOUND_LIST play_item = SOUND_MILEBOT_COMPANY;
    uint8_t ret = 0;
    uint32_t sound_timeout_tick = 0;
    uint8_t voice_level_backup = 0;

    //��ʼ��
    osDelay(1000);
    syn6658_queue_init(&syn6658_queue);
    syn6658_init();
    voice_level_backup = app2mb.voice_level;
    osDelay(500);

    //loop
    while(1)
    {
        switch(syn6658_FSM)
        {
            case SOUND_FSM_IDLE:
                ret = syn6658_queue_out(&syn6658_queue, (uint8_t*)(&play_item));
                if(ret == 1)
                {
                    syn6658_FSM = SOUND_FSM_PRE_PLAYING_CHECK;
                    sound_timeout_tick = HAL_GetTick();
                }
                else
                {
                    if(voice_level_backup != app2mb.voice_level)
                    {
                        voice_level_backup = app2mb.voice_level;
                        osDelay(500);
                        syn6658_init();
                        osDelay(500);
                    }
                }
                break;

            case SOUND_FSM_PRE_PLAYING_CHECK:
                RunOnceEvery(20,                  //���XXX ms
                {
                    //��ʱ
                    if((HAL_GetTick() - sound_timeout_tick) > 5000)
                    {
                        syn6658_status = SYN6658_STATUS_IDLE;
                    }
                                        
                    if(syn6658_status == SYN6658_STATUS_IDLE)
                    {
                        syn6658_FSM = SOUND_FSM_PLAYING;
                    }
                });
                break;

            case SOUND_FSM_PLAYING:
                    if(play_item < SOUND_LIST_NUM)
                    {
                        syn6658_combine_play(play_item);
                        syn6658_FSM = SOUND_FSM_PLAYING_CHECK;
                        sound_timeout_tick = HAL_GetTick();
                    }
                    else
                    {
                        syn6658_FSM = SOUND_FSM_IDLE;
                    }
                break;

            case SOUND_FSM_PLAYING_CHECK:
                RunOnceEvery(20,                  //���XXX ms
                {
                    //��ʱ
                    if((HAL_GetTick() - sound_timeout_tick) > 5000)
                    {
                        syn6658_status = SYN6658_STATUS_IDLE;
                    }
                    
                    if(syn6658_status == SYN6658_STATUS_IDLE)
                    {
                        syn6658_FSM = SOUND_FSM_IDLE;
                    }
                });
                break;

            default:
                syn6658_FSM = SOUND_FSM_IDLE;
                break;

        }

        osDelay(10);

    }
}


//��Ӧ�ó������, ��ָ�����ŵ�����, ��ӵ����Ŷ�����
void user_play_sound(SOUND_LIST list)
{
    if(list >= SOUND_LIST_NUM)
    {
        return;
    }

    //��ӵ����Ŷ�����
    syn6658_queue_in(&syn6658_queue, (uint8_t)list);
}

//��ʼ�����к���, ����������յ�ǰ�Ĳ����б�
void syn6658_queue_init(SYN6658_QUEUE* qu) //�ڶ�������жϵ��������Ҫʵ��ԭ�Ӳ��������ܱ�֤���ݲ�������ȷ�ԡ�
{
    memset(qu->qBuf, 0x00, SYN6658_QUEUE_SIZE);
    qu->head = 0;
    qu->tail = 0;

    // TODO: ����
    user_play_sound(SOUND_MILEBOT_COMPANY);
    user_play_sound(SOUND_TIE_BANDAGE);
    
#if defined BACKUP_PRJ
    user_play_sound(SOUND_UPGRADE_SOFTWARE);
#endif
    
    user_play_sound(SOUND_START_WALK);
}

//syn6658оƬ��������, 0-10, 0Ϊ����
void syn6658_set_volume(uint8_t level)
{
    char data[5] = {0};
    char len = 0;

    data[0] = '[';
    data[1] = 'v';
    
    // ���ܳ������������� �ȼ�
    if(level > SYN6658_MAX_VOLUME_LEVEL)
    {
        level = SYN6658_MAX_VOLUME_LEVEL;
    }

    if(level < 10)
    {
        data[2] = '0' + level;
        data[3] = ']';

        len = 4;
    }
    else
    {
        data[2] = '0' + 1;
        data[3] = '0' + level%10;
        data[4] = ']';

        len = 5;
    }

    syn6658_send_data(data, len);
    
}

//���ڽ������ݵĴ���ص�����
/*
оƬ���յ���������֡��������λ������1 ���ֽڵ�״̬�ش���
��λ���ɸ�������ش����ж�оƬĿǰ�Ĺ���״̬
*/
void syn6658_uart_receive_callback(uint8_t value)
{
    if(value == SYN6658_IDLE)
    {
        syn6658_status = SYN6658_STATUS_IDLE;
    }
    else
    {
        syn6658_status = SYN6658_STATUS_BUSY;
    }
}


//syn6658оƬ��ʼ��
static void syn6658_init(void)
{
    //[d], �ָ�Ĭ��ֵ
    //[m3], ѡ������, 3 - ���� (Ů��)
    //[s5], ����, 0-10, ����ֵԽС, ����Խ��
    //[t5], ���, 0-10, ���ֵԽС, ��ƵֵԽ��
    //[v5], ��������, 0-10, 0Ϊ����, 10 Ϊ����������
#if 0   /* Begin if 0 by lison on 2020-10-20 */
    char data[] = "[d][m3][s5][t5][v2]";

    char len = 0;

    len = strlen(data);

    syn6658_send_data(data, len);
    
#else

    char data[] = "[d][m3][s5][t5][v2]";
    char data_max_voice[] = "[d][m3][s5][t5][v10]";
    
    char len = 0;

    if(app2mb.voice_level >= 10)
    {
        len = strlen(data_max_voice);
        syn6658_send_data(data_max_voice, len);
    }
    else
    {
        data[17] = app2mb.voice_level + '0';
        
        len = strlen(data);
        syn6658_send_data(data, len);
    }

#endif  /* End if 0 by lison on 2020-10-20 */

}

//�ϳ�����������
static void syn6658_combine_play(SOUND_LIST list)
{
    char buf[128] = {0};
    uint8_t buf_len = 0;

    if(list >= SOUND_LIST_NUM)
    {
        return;
    }

    //��ʼ��������ʱ, ����оƬæ
    syn6658_status = SYN6658_STATUS_BUSY;

    buf_len = strlen(TTS_list[list]);//�������ַ���������
    memcpy(buf, TTS_list[list], buf_len);

    //���źϳɵ�����
    syn6658_send_data(buf, buf_len);
    
}

//syn6658оƬ��������(��֪��Ϊʲô��ָ���0x45)
void syn6658_other_CMD(SYN6658_CMD cmd)
{
    char send_buf[5] = {0};
    uint8_t i = 0;
    
    if(cmd == VOICE_PLAY)
    {
        return;
    }

    //֡ͷ
    send_buf[i++] = SYN6658_FRAME_HEAD;

    //����������(2�ֽ�, ���ֽ���ǰ), Ŀǰ����֪���������ĳ���
    send_buf[i++] = 0;
    send_buf[i++] = 0;

    //������(������, �������, �����ı�)

    //������
    send_buf[i++] = (char)cmd;

    //�������(�����ǿ�, û���κζ���)

    //��Ҫ���͵��ı���Ϣ(�����ǿ�, û���κζ���)

    //����������(2�ֽ�, ���ֽ���ǰ), ������֡ͷ(1 �ֽ�) ������������(2�ֽ�)
    send_buf[DATA_LEN_HIGH_BYTE_INDEX] = ((i - 3) >> 8);
    send_buf[DATA_LEN_LOW_BYTE_INDEX]  = ((i - 3) >> 0); 

    //���ڷ�������
    uart6_dma_send_data((uint8_t*)send_buf, i);

}


//syn6658оƬ������������
static void syn6658_send_data(const char* data, uint8_t len)
{
    //��ʽ: ֡ͷ, ����������(2�ֽ�, ���ֽ���ǰ), ������(������, �������, �����ı�)
    static char send_buf[128] = {0};
    uint8_t i = 0;

    if(len == 0)
    {
        return;
    }

    //֡ͷ
    send_buf[i++] = SYN6658_FRAME_HEAD;

    //����������(2�ֽ�, ���ֽ���ǰ), Ŀǰ����֪���������ĳ���
    send_buf[i++] = 0;
    send_buf[i++] = 0;

    //������(������, �������, �����ı�)

    //������
    send_buf[i++] = VOICE_PLAY;

    //�������
    send_buf[i++] = SYN6658_CMD_PARAM;

    //��Ҫ���͵��ı���Ϣ
    memcpy(&send_buf[i], data, len);
    i += len;

    //����������(2�ֽ�, ���ֽ���ǰ), ������֡ͷ(1 �ֽ�) ������������(2�ֽ�)
    send_buf[DATA_LEN_HIGH_BYTE_INDEX] = ((i - 3) >> 8);
    send_buf[DATA_LEN_LOW_BYTE_INDEX]  = ((i - 3) >> 0); 

    //���ڷ�������
    uart6_dma_send_data((uint8_t*)send_buf, i);
    
}

//������к���
uint8_t syn6658_queue_in(SYN6658_QUEUE* qu, uint8_t data) //�ڶ�������жϵ��������Ҫʵ��ԭ�Ӳ��������ܱ�֤���ݲ�������ȷ�ԡ�
{
    if(((qu->tail + 1) % SYN6658_QUEUE_SIZE) == qu->head)  //��������, ���ݶ���
    {
        return 0; 								  	
    }

    qu->qBuf[qu->tail] = data; //���ݷ������β��
    qu->tail = (qu->tail + 1) % SYN6658_QUEUE_SIZE;  //���º��tailָʾ����д�������

    return 1;
}

//ȡ�����к���
uint8_t syn6658_queue_out(SYN6658_QUEUE* qu ,uint8_t* data) //�ڶ�������жϵ��������Ҫʵ��ԭ�Ӳ��������ܱ�֤���ݲ�������ȷ�ԡ�
{	  
    if(((qu->tail) % SYN6658_QUEUE_SIZE) == qu->head) //���пգ�û�����ݿɹ�����
    {
        return 0;
    }

    *data = qu->qBuf[qu->head]; //�Ӷ���ͷ����ȡ����
    qu->head = (qu->head + 1) % SYN6658_QUEUE_SIZE; //���º��headָʾ��һ���ɶ�������

    return 1;
}


