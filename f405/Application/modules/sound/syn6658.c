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

//判断芯片是否在播放语音
static uint8_t syn6658_status = SYN6658_STATUS_IDLE; // 默认状态

//语音播放内容list ,单个要小于128 个字节
//const char* TTS_list[] =  
//{
//	"欢迎使用迈步机器人",
//	"请确认系好绑带",
//	"请开始行走",
//	"开始助力",
//	"电池电量低，请及时充电",
//	"蓝牙已连接",
//	"蓝牙已断开",
//	"请抬左腿",
//	"请抬右腿",
//	"助行[=xing2]模式",
//	"左偏瘫模式",
//	"右偏瘫模式",
//	"帕金森模式",
//	"零力模式",
//	"被动模式",
//	"测试模式",
//	"sound401[p100]sound401[p100]",
//	"sound401[p200]",
//	"sound303[p200]sound303",
//	"为了更好的使用体验，建议升级设备软件",
//	"评估模式",
//	"请保持站立",
//	"开始评估，请开始行走",
//	"评估结束",
//	"一挡助力",
//	"二挡助力",
//	"三挡助力",
//	"四挡助力",
//	"五挡助力",
//};

#if 1//普通话版本 ， 注意，普通话版本和英语版本IC版本不一样
const char* TTS_list[] =  
{
	"欢迎使用迈步机器人",//"欢迎使用助行[=xing2]机器人",
	"请确认系好绑带",
	"请开始行走",
	"开始助力",
	"电池电量低，请及时充电",
	"蓝牙已连接",
	"蓝牙已断开",
	"请抬左腿",
	"请抬右腿",
	"双腿助力模式",
	"左助力模式",
	"右助力模式",
	"阻力模式",
	"零力模式",
	"工程模式",
	"测试模式",
	"sound401[p100]sound401[p100]",
	"sound401[p200]",
	"sound303[p200]sound303",
	"为了更好的使用体验，建议升级设备软件",
	"评估模式",
	"请保持站立",
	"开始评估，请开始行走",
	"评估结束",
	"一挡助力",
	"二挡助力",
	"三挡助力",
	"四挡助力",
	"五挡助力",
	"一挡阻力",
	"二挡阻力",
	"三挡阻力",
	"四挡阻力",
	"五挡阻力",
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

//语音播放队列
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

    //初始化
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
                RunOnceEvery(20,                  //间隔XXX ms
                {
                    //超时
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
                RunOnceEvery(20,                  //间隔XXX ms
                {
                    //超时
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


//给应用程序调用, 把指定播放的语音, 添加到播放队列中
void user_play_sound(SOUND_LIST list)
{
    if(list >= SOUND_LIST_NUM)
    {
        return;
    }

    //添加到播放队列中
    syn6658_queue_in(&syn6658_queue, (uint8_t)list);
}

//初始化队列函数, 可以用于清空当前的播放列表
void syn6658_queue_init(SYN6658_QUEUE* qu) //在多任务和中断的情况下需要实现原子操作，才能保证数据操作的正确性。
{
    memset(qu->qBuf, 0x00, SYN6658_QUEUE_SIZE);
    qu->head = 0;
    qu->tail = 0;

    // TODO: 测试
    user_play_sound(SOUND_MILEBOT_COMPANY);
    user_play_sound(SOUND_TIE_BANDAGE);
    
#if defined BACKUP_PRJ
    user_play_sound(SOUND_UPGRADE_SOFTWARE);
#endif
    
    user_play_sound(SOUND_START_WALK);
}

//syn6658芯片音量设置, 0-10, 0为静音
void syn6658_set_volume(uint8_t level)
{
    char data[5] = {0};
    char len = 0;

    data[0] = '[';
    data[1] = 'v';
    
    // 不能超过最大输出声音 等级
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

//串口接收数据的处理回调函数
/*
芯片接收到控制命令帧，会向上位机发送1 个字节的状态回传，
上位机可根据这个回传来判断芯片目前的工作状态
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


//syn6658芯片初始化
static void syn6658_init(void)
{
    //[d], 恢复默认值
    //[m3], 选择发音人, 3 - 晓玲 (女声)
    //[s5], 语速, 0-10, 语速值越小, 语速越慢
    //[t5], 语调, 0-10, 语调值越小, 基频值越低
    //[v5], 播放音量, 0-10, 0为静音, 10 为最大输出声音
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

//合成语音并播放
static void syn6658_combine_play(SOUND_LIST list)
{
    char buf[128] = {0};
    uint8_t buf_len = 0;

    if(list >= SOUND_LIST_NUM)
    {
        return;
    }

    //开始播放语音时, 设置芯片忙
    syn6658_status = SYN6658_STATUS_BUSY;

    buf_len = strlen(TTS_list[list]);//不包括字符串结束符
    memcpy(buf, TTS_list[list], buf_len);

    //播放合成的语音
    syn6658_send_data(buf, buf_len);
    
}

//syn6658芯片其他命令(不知道为什么这指令返回0x45)
void syn6658_other_CMD(SYN6658_CMD cmd)
{
    char send_buf[5] = {0};
    uint8_t i = 0;
    
    if(cmd == VOICE_PLAY)
    {
        return;
    }

    //帧头
    send_buf[i++] = SYN6658_FRAME_HEAD;

    //数据区长度(2字节, 高字节在前), 目前还不知道数据区的长度
    send_buf[i++] = 0;
    send_buf[i++] = 0;

    //数据区(命令字, 命令参数, 发送文本)

    //命令字
    send_buf[i++] = (char)cmd;

    //命令参数(这里是空, 没有任何东西)

    //需要发送的文本信息(这里是空, 没有任何东西)

    //数据区长度(2字节, 高字节在前), 不包括帧头(1 字节) 和数据区长度(2字节)
    send_buf[DATA_LEN_HIGH_BYTE_INDEX] = ((i - 3) >> 8);
    send_buf[DATA_LEN_LOW_BYTE_INDEX]  = ((i - 3) >> 0); 

    //串口发送数据
    uart6_dma_send_data((uint8_t*)send_buf, i);

}


//syn6658芯片播放语音函数
static void syn6658_send_data(const char* data, uint8_t len)
{
    //格式: 帧头, 数据区长度(2字节, 高字节在前), 数据区(命令字, 命令参数, 发送文本)
    static char send_buf[128] = {0};
    uint8_t i = 0;

    if(len == 0)
    {
        return;
    }

    //帧头
    send_buf[i++] = SYN6658_FRAME_HEAD;

    //数据区长度(2字节, 高字节在前), 目前还不知道数据区的长度
    send_buf[i++] = 0;
    send_buf[i++] = 0;

    //数据区(命令字, 命令参数, 发送文本)

    //命令字
    send_buf[i++] = VOICE_PLAY;

    //命令参数
    send_buf[i++] = SYN6658_CMD_PARAM;

    //需要发送的文本信息
    memcpy(&send_buf[i], data, len);
    i += len;

    //数据区长度(2字节, 高字节在前), 不包括帧头(1 字节) 和数据区长度(2字节)
    send_buf[DATA_LEN_HIGH_BYTE_INDEX] = ((i - 3) >> 8);
    send_buf[DATA_LEN_LOW_BYTE_INDEX]  = ((i - 3) >> 0); 

    //串口发送数据
    uart6_dma_send_data((uint8_t*)send_buf, i);
    
}

//加入队列函数
uint8_t syn6658_queue_in(SYN6658_QUEUE* qu, uint8_t data) //在多任务和中断的情况下需要实现原子操作，才能保证数据操作的正确性。
{
    if(((qu->tail + 1) % SYN6658_QUEUE_SIZE) == qu->head)  //队列满了, 数据丢弃
    {
        return 0; 								  	
    }

    qu->qBuf[qu->tail] = data; //数据放入队列尾部
    qu->tail = (qu->tail + 1) % SYN6658_QUEUE_SIZE;  //更新后的tail指示可以写入的区域

    return 1;
}

//取出队列函数
uint8_t syn6658_queue_out(SYN6658_QUEUE* qu ,uint8_t* data) //在多任务和中断的情况下需要实现原子操作，才能保证数据操作的正确性。
{	  
    if(((qu->tail) % SYN6658_QUEUE_SIZE) == qu->head) //队列空，没有数据可供返回
    {
        return 0;
    }

    *data = qu->qBuf[qu->head]; //从队列头部读取数据
    qu->head = (qu->head + 1) % SYN6658_QUEUE_SIZE; //更新后的head指示下一个可读的区域

    return 1;
}


