#include "DS_300c.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "stdio.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"

#define delay2 1

extern uint8_t canbuf[8];
extern uint8_t flip;

extern uint8_t cmd[4][6];
extern int ifsend[4];

uint16_t crc16(unsigned char *addr, unsigned int num, unsigned int crc)
{
    int i;
    uint16_t Over_crc = 0;
    for(;num>0;num--)
    {
        crc = crc ^ (*addr++ << 8);
        for(i=0;i<8;i++)
        {
            if(crc & 0x8000)
                crc = (crc << 1) ^ 0x8005;
            else
                crc <<= 1;
        }
        crc = crc ^ Over_crc;
    }
    return crc;
}

/* JA110 关节模组驱动器； 电机DS300 */
unsigned short getCRC16(unsigned char *ptr,unsigned char len)
{
	unsigned char i;
	unsigned short crc = 0xFFFF;
	if(len==0)
	{
		len = 1;
	}
	while(len--)
	{
		crc ^= *ptr;
		for(i=0; i<8; i++)
		{
			if(crc&1)
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
			{
				crc >>= 1;
			}
		}
		ptr++;
	}
	return(crc);
}


int can_send(int* ptr_can,unsigned char* send_buf,int send_len)
{
    int address = send_buf[0] - 1;
    cmd[address][1] = send_buf[1];
    cmd[address][2] = send_buf[2];
    cmd[address][3] = send_buf[3];
    cmd[address][4] = send_buf[4];
    cmd[address][5] = send_buf[5];
    
    ifsend[0] = 1;
    ifsend[1] = 1;
    ifsend[2] = 1;
    ifsend[3] = 1;
    
    return 1;
}

/**
 * [电机使能通信]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:09:17+0800
 * @param    ptr_can               [串口文件描述符的指针]
 * @param    motor_address            [驱动器地址]
 * @return                            [命令发送结果,1 为发送成功,0 为发送失败]
 */
int motor_enable(int *ptr_can,int motor_address)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x15;
	send_buf[2] = 0x00;
	send_buf[3] = 0x00;
	send_buf[4] = 0x00;
	send_buf[5] = 0x00;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/**
 * [电机失能通信]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:13:04+0800
 * @param    ptr_can               [串口文件描述符的指针]
 * @param    motor_address            [驱动器地址]
 * @return                            [命令发送结果,1 为发送成功,0 为发送失败]
 */
int motor_disable(int *ptr_can,int motor_address)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x16;
	send_buf[2] = 0x00;
	send_buf[3] = 0x00;
	send_buf[4] = 0x00;
	send_buf[5] = 0x00;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/**
 * [设置位置模式下，最大电机转速]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:13:49+0800
 * @param    ptr_can               [串口文件描述符指针]
 * @param    set_speed                [要设置的最大电机转速,单位 RPM（转每分）]
 * @param    motor_address            [驱动器地址]
 * @return                            [命令发送结果,1 为发送成功,0 为发送失败]
 */
int motor_set_max_v(int *ptr_can,int set_speed,int motor_address)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255 || set_speed >3000 || set_speed <0)
	{
		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x2A;
	send_buf[2] = set_speed>>8;
	send_buf[3] = set_speed;
	send_buf[4] = 0x00;
	send_buf[5] = 0x00;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/**
 * [位置模式，绝对位置控制指令]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:29:21+0800
 * @param    ptr_can               [串口文件描述符指针]
 * @param    set_angle                [想要控制的，电机低速端的目标绝对角度]
 * @param    motor_address            [驱动器地址]
 * @param    dir                      [电机控制方向标识,只有0，1两个值；如参考外骨骼安装方式安装，具体方向参见宏定义]
 * @return                            [命令发送结果,1 为发送成功,0 为发送失败]
 */
int motor_move_abs(int *ptr_can,double set_angle,int motor_address,int dir)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255)
	{
		return 0;
	}
	//此处注意，根据外骨骼固定驱动地址，按左髋左膝右髋右膝，驱动地址为1，2，3，4
	//故根据驱动地址做了关节角度范围限制。如其他项目使用，极限角度不同，此处可酌情修改或删除
//	if(motor_address == 1 || motor_address == 3)
//	{
//		if(set_angle>100)
//		{
//			set_angle = 100;
//		}
//		if(set_angle<-30)
//		{
//			set_angle = -30;
//		}
//	}
//	if(motor_address == 2 || motor_address == 4)
//	{
//		if(set_angle<0)
//		{
//			set_angle = 0;
//		}
//		if(set_angle>95)
//		{
//			set_angle = 95;
//		}
//	}
	int position_value = 0;
	position_value = floor(set_angle * CHANGE_BYTE);
	if(dir == 0)
	{
		position_value *= -1;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x28;
	send_buf[2] = position_value>>24;
	send_buf[3] = position_value>>16;
	send_buf[4] = position_value>>8;
	send_buf[5] = position_value;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/**
 * [位置模式，相对位置控制指令]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:29:21+0800
 * @param    ptr_can               [串口文件描述符指针]
 * @param    set_angle                [想要控制的，电机低速端的目标相对角度]
 * @param    motor_address            [驱动地址]
 * @param    dir                      [电机控制方向标识,只有0，1两个值；如参考外骨骼安装方式安装，具体方向参见宏定义]
 * @return                            [命令发送结果,1 为发送成功,0 为发送失败]
 */
int motor_move_rel(int *ptr_can,double set_angle,int motor_address,int dir)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >4)
	{
		return 0;
	}
	int position_value = 0;

	position_value = floor(set_angle*CHANGE_BYTE);
	if(dir == 0)
	{
		position_value*=-1;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x29;
	send_buf[2] = position_value>>24;
	send_buf[3] = position_value>>16;
	send_buf[4] = position_value>>8;
	send_buf[5] = position_value;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * [电机运动停止指令]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:38:51+0800
 * @param    ptr_can               [串口文件描述符指针]
 * @param    motor_address            [驱动器地址]
 * @return                            [命令发送结果,1 为发送成功,0 为发送失败]
 */
int motor_stop(int *ptr_can,int motor_address)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255 )
	{
		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x3C;
	send_buf[2] = 0x00;
	send_buf[3] = 0x00;
	send_buf[4] = 0x00;
	send_buf[5] = 0x00;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * [获取电机实际位置指令]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:40:59+0800
 * @param    ptr_can               [串口设备文件描述符指针]
 * @param    motor_address            [驱动器地址]
 * @return                            [命令发送结果,1 为发送成功,0 为发送失败]
 */
int motor_get_real_pos(int *ptr_can,int motor_address)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255 )
	{
		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x05;
	send_buf[2] = 0x00;
	send_buf[3] = 0x00;
	send_buf[4] = 0x00;
	send_buf[5] = 0x00;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/**
 * [获取电机设定目标位置指令]
 * @author SunQinglin
 * @DateTime 2021-07-19T18:20:35+0800
 * @param    ptr_can               [串口设备文件描述符指针]
 * @param    motor_address            [驱动器地址]
 * @return                            [命令发送结果,1 为发送成功,0 为发送失败]
 */
int motor_get_set_pos(int *ptr_can,int motor_address)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255 )
	{
		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x04;
	send_buf[2] = 0x00;
	send_buf[3] = 0x00;
	send_buf[4] = 0x00;
	send_buf[5] = 0x00;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;

	if(can_send(ptr_can,send_buf,8))
	{

		return 1;
	}
	else
	{
		return 0;
	}
}
/**
 * [驱动通信测试指令]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:42:43+0800
 * @param    ptr_can               [串口设备文件描述符指针]
 * @param    motor_address            [驱动器地址]
 * @return                            [命令发送结果,1 为发送成功,0 为发送失败]
 */
int motor_echo_test(int *ptr_can,int motor_address)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x0E;
	send_buf[2] = 0x00;
	send_buf[3] = 0x00;
	send_buf[4] = 0x00;
	send_buf[5] = 0x00;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * [设置驱动控制模式（位置/速度/电流模式）]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:44:31+0800
 * @param    ptr_can               [串口设备文件描述符指针]
 * @param    motor_address            [驱动器地址]
 * @param    control_mode             [目标驱动控制模式标识 ,
 *                                    	0 为电流控制模式,
 *                                      1 为速度控制模式,
 *                                      2 为位置控制模式]
 * @return                            [命令发送结果,1 为发送成功,0 为发送失败]
 */
int motor_control_mode(int *ptr_can,int motor_address,unsigned char control_mode)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255)
	{

		return 0;
	}

	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x96;
	send_buf[2] = 0x00;
	send_buf[3] = 0x25;
	send_buf[4] = 0x00;
	send_buf[5] = control_mode;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/**
 * 设置驱动地址设置
 * @author 
 * @DateTime 
 * @param    ptr_can               [串口设备文件描述符指针]
 * @param    motor_address            [驱动器地址]
 * @param    control_mode             [目标驱动控制模式标识 ,
 *                                    	0 为电流控制模式,
 *                                      1 为速度控制模式,
 *                                      2 为位置控制模式]
 * @return                            [命令发送结果,1 为发送成功,0 为发送失败]
 */
int motor_adress_set(int *ptr_can,int old_motor_address, int new_address)
{
	if(ptr_can == NULL || old_motor_address <0 || old_motor_address >255 || new_address <0 || new_address >255)
	{
		return 0;
	}

	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = old_motor_address;
	send_buf[1] = 0x96;
	send_buf[2] = 0x00;
	send_buf[3] = 0x0B;
	send_buf[4] = 0x00;
	send_buf[5] = new_address;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * [电机运动加速度设置（位置/速度模式）]
 * @author SunQinglin
 * @DateTime 2021-07-19T19:03:14+0800
 * @param    ptr_can               [串口文件描述符指针]
 * @param    motor_address            [驱动器地址]
 * @param    acceleration             [目标加速度，单位为转每秒方（RPS2）]
 * @return                            [命令发送结果,1 为发送成功,0 为发送失败]
 */
int motor_set_Tcurve_max_a(int *ptr_can,int motor_address,short acceleration)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x96;
	send_buf[2] = 0x00;
	send_buf[3] = 0x53;
	send_buf[4] = acceleration>>8;
	send_buf[5] = acceleration;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}

}
/**
 * [驱动编码器清零指令]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:56:18+0800
 * @param    ptr_can               [串口设备文件描述符指针]
 * @param    motor_address            [驱动器地址]
 * @return                            [命令发送结果， 1为发送成功，0为发送失败]
 */
int motor_reset_enc(int *ptr_can,int motor_address)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x50;
	send_buf[2] = 0x00;
	send_buf[3] = 0x00;
	send_buf[4] = 0x00;
	send_buf[5] = 0x00;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * [电流模式，电流控制值设置指令]
 * @author SunQinglin
 * @DateTime 2018-07-13T15:48:24+0800
 * @param    ptr_can               [串口设备文件描述符指针]
 * @param    motor_address            [驱动器地址]
 * @param    current_value            [目标设置的电流值，单位为毫安（mA）]
 * @return                            [命令发送结果 ,1 为发送成功,0 为发送失败]
 */
int motor_set_current_value(int *ptr_can,int motor_address,short current_value)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255)
	{
		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x70;
	send_buf[2] = current_value>>8;
	send_buf[3] = current_value;
	send_buf[4] = 0x00;
	send_buf[5] = 0x00;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * [获取当前电机转动速度指令]
 * @author SunQinglin
 * @DateTime 2021-07-19T19:13:16+0800
 * @param    ptr_can               [串口设备文件描述]
 * @param    motor_address            [驱动器地址]
 * @return                            [命令发送结果， 1为发送成功， 0为发送失败]
 */
int motor_get_speed(int *ptr_can,int motor_address)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255)
	{
		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x3F;
	send_buf[2] = 0x00;
	send_buf[3] = 0x00;
	send_buf[4] = 0x00;
	send_buf[5] = 0x00;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int motor_get_set_speed(int *ptr_can,int motor_address)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255)
	{
		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x3E;
	send_buf[2] = 0x00;
	send_buf[3] = 0x00;
	send_buf[4] = 0x00;
	send_buf[5] = 0x00;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int motor_get_set_acc(int *ptr_can,int motor_address)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255)
	{
		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x95;
	send_buf[2] = 0x00;
	send_buf[3] = 0x53;
	send_buf[4] = 0x00;
	send_buf[5] = 0x00;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * [速度模式，目标速度控制下发]
 * @author SunQinglin
 * @DateTime 2021-07-19T19:17:59+0800
 * @param    ptr_can               [串口设备文件描述符指针]
 * @param    speed                    [目标速度，单位为0.1转每分(0.1RPM)]
 * @param    dir                      [目标速度方向]
 * @param    motor_address            [驱动器地址]
 * @return                            [命令发送结果，1为发送成功，0为发送失败]
 */
int motor_set_speed(int *ptr_can,int speed,int dir,int motor_address)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255)
	{
		return 0;
	}
	int real_speed = speed;
	if(dir == 0)
	{
		real_speed*=-1;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x6F;
	send_buf[2] = real_speed>>24;
	send_buf[3] = real_speed>>16;
	send_buf[4] = real_speed>>8;
	send_buf[5] = real_speed;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * [清除当前驱动报警指令]
 * @author SunQinglin
 * @DateTime 2021-07-19T19:23:04+0800
 * @param    ptr_can               [串口设备文件描述符指针]
 * @param    motor_address            [驱动器地址]
 * @return                            [命令发送结果，1为发送成功， 0为发送失败]
 */
int clean_motor_alert_state(int *ptr_can,int motor_address)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x17;
	send_buf[2] = 0x00;
	send_buf[3] = 0x00;
	send_buf[4] = 0x00;
	send_buf[5] = 0x00;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * [进入阻尼指令发送]
 * @author SunQinglin
 * @DateTime 2021-07-19T19:29:20+0800
 * @param    ptr_can               [串口设备文件描述符指针]
 * @param    motor_address            [驱动器地址]
 * @param    set_current              [目标阻力对应的电流，单位为A（不要大于10A，驱动允许最大电流为10A）]
 * @return                            [命令发送结果，1为发送成功， 0为发送失败]
 */
int ds300c_motor_brake(int *ptr_can,int motor_address,unsigned short set_current)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255 || set_current >10)
	{

		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x80;
	send_buf[2] = set_current >> 8;
	send_buf[3] = set_current;
	send_buf[4] = 0x00;
	send_buf[5] = 0x00;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * [退出阻尼指令发送]
 * @author SunQinglin
 * @DateTime 2021-07-19T19:34:44+0800
 * @param    ptr_can               [串口设备文件描述符指针]
 * @param    motor_address            [驱动器地址]
 * @return                            [命令发送结果，1为发送成功， 0为发送失败]
 */
int ds300c_motor_brake_quit(int *ptr_can,int motor_address)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255 )
	{

		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x81;
	send_buf[2] = 0x00;
	send_buf[3] = 0x00;
	send_buf[4] = 0x00;
	send_buf[5] = 0x00;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * [驱动报警情况获取]
 * @author SunQinglin
 * @DateTime 2021-07-19T19:35:41+0800
 * @param    ptr_can               [串口设备文件描述符]
 * @param    motor_address            [驱动器地址]
 * @return                            [命令发送结果，1为发送成功， 0为发送失败]
 */
int motor_alert_get(int *ptr_can,int motor_address)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255)
	{
		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x95;
	send_buf[2] = 0x00;
	send_buf[3] = 0xC8;
	send_buf[4] = 0X00;
	send_buf[5] = 0x00;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * [获取当前驱动状态指令]
 * @author SunQinglin
 * @DateTime 2021-07-19T19:36:32+0800
 * @param    ptr_can               [串口设备文件描述符指针]
 * @param    motor_address            [驱动器地址]
 * @return                            [命令发送结果， 1为发送成功， 0为发送失败]
 */
int motor_get_move_state(int* ptr_can,int motor_address)
{
    if(ptr_can == NULL || motor_address <0 || motor_address >255)
	{
		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x64;
	send_buf[2] = 0x00;
	send_buf[3] = 0x00;
	send_buf[4] = 0X00;
	send_buf[5] = 0x00;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * [电机停止减速度设置指令]
 * @author SunQinglin
 * @DateTime 2021-07-19T19:37:26+0800
 * @param    ptr_can               [串口设备文件描述符]
 * @param    motor_address            [驱动器地址]
 * @param    accel                    [目标减速度值，单位转每秒方（RPS2）]
 * @return                            [指令发送结果，1为发送成功， 0为发送失败]
 */
int motor_set_stop_accel(int* ptr_can, int motor_address,int accel)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255)
	{
		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x96;
	send_buf[2] = 0x00;
	send_buf[3] = 0x58;
	send_buf[4] = accel >> 8;
	send_buf[5] = accel;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * [位置模式，是否连续曲线控制发送]
 * @author SunQinglin
 * @DateTime 2021-07-19T19:39:32+0800
 * @param    ptr_can               [串口设备文件描述符指针]
 * @param    motor_address            [驱动器地址]
 * @param    control_sign             [控制标识，1为连续曲线， 0为单点控制（非连续曲线控制）]
 * @return                            [命令发送结果，1为发送成功，0为发送失败]
 */
int motor_continue_curve_control(int *ptr_can, int motor_address, int control_sign)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255 || control_sign < 0 || control_sign >1)
	{
		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x83;
	send_buf[2] = 0x00;
	send_buf[3] = control_sign;
	send_buf[4] = 0x00;
	send_buf[5] = 0x00;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/*-----------------------------------------------------------*/
int motor_move_abs_usmart(int *ptr_can,int set_angle,int motor_address,int dir)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255)
	{
		return 0;
	}
	int position_value = 0;
	position_value = floor(set_angle * CHANGE_BYTE);
	if(dir == 0)
	{
		position_value *= -1;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x28;
	send_buf[2] = position_value>>24;
	send_buf[3] = position_value>>16;
	send_buf[4] = position_value>>8;
	send_buf[5] = position_value;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int motor_move_rel_usmart(int *ptr_can,int set_angle,int motor_address,int dir)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >4)
	{
		return 0;
	}
	int position_value = 0;

	position_value = floor(set_angle*CHANGE_BYTE);
	if(dir == 0)
	{
		position_value*=-1;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x29;
	send_buf[2] = position_value>>24;
	send_buf[3] = position_value>>16;
	send_buf[4] = position_value>>8;
	send_buf[5] = position_value;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int motor_get_current(int *ptr_can,int motor_address)
{
	if(ptr_can == NULL || motor_address <0 || motor_address >255)
	{
		return 0;
	}
	unsigned char send_buf[8]={0};
	unsigned short crc_cal = 0;
	send_buf[0] = motor_address;
	send_buf[1] = 0x40;
	send_buf[2] = 0x00;
	send_buf[3] = 0x00;
	send_buf[4] = 0x00;
	send_buf[5] = 0x00;
	crc_cal = getCRC16(send_buf,6);
	send_buf[6] = crc_cal%0x100;
	send_buf[7] = crc_cal/0x100;
	if(can_send(ptr_can,send_buf,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

