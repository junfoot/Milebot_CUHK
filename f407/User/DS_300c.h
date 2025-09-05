/*
 * @Description: 
 * @Author: Sunqinglin
 * @Date: 2020-05-19 09:54:31
 * @LastEditTime: 2020-11-27 09:11:17
 * @LastEditors: Sunqinglin
 */ 
#ifndef _DS_300C_H
#define _DS_300C_H





// #include <unistd.h>
// #include <errno.h>
// #include <termios.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

//#include "usart.h"

//#define CHANGE_BYTE (1753.6)
#define CHANGE_BYTE (1809.26984)  // 编码器数据 -> 角度（°）

//位置控制模式，电机方向标识宏定义；H意为髋关节，K意为膝关节；（按外骨骼关节模组安装方式）
#define HLEFT_DIR 0
#define KLEFT_DIR 1
#define HRIGHT_DIR 1
#define KRIGHT_DIR 0

extern int ET_start;
extern char u2_buf[200];
extern int buf_num; 


typedef struct Driver_info
{
	int queue_index;
	int position ;
	int speed ;
	int queue_tail_index;
	int voltage;
	int complite_sign;
	int motor_alert_state;
}Driver_info;


uint16_t crc16(unsigned char *addr, unsigned int num, unsigned int crc);
unsigned short getCRC16(unsigned char *ptr,unsigned char len);
int can_send_usmart(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5);
int can_send(int* ptr_can,unsigned char* send_buf,int send_len);
int motor_enable(int *ptr_can,int motor_address);
int motor_disable(int *ptr_can,int motor_address);
int motor_set_max_v(int *ptr_can,int set_speed,int motor_address);
int motor_move_abs(int *ptr_can,double set_angle,int motor_address,int dir);
int motor_move_rel(int *ptr_can,double set_angle,int motor_address,int dir);
int motor_stop(int *ptr_can,int motor_address);
int motor_get_real_pos(int *ptr_can,int motor_address);
int motor_get_set_pos(int *ptr_can,int motor_address);
int motor_echo_test(int *ptr_can,int motor_address);
int motor_control_mode(int *ptr_can,int motor_address,unsigned char control_mode);
int motor_reset_enc(int *ptr_can,int motor_address);
int motor_set_Tcurve_max_a(int *ptr_can,int motor_address,short acceleration);
int motor_set_current_value(int *ptr_can,int motor_address,short current_value);
int motor_get_speed(int *ptr_can,int motor_address);
int motor_get_set_speed(int *ptr_can,int motor_address);
int motor_get_set_acc(int *ptr_can,int motor_address);
int motor_set_speed(int *ptr_can, int speed,int dir,int motor_address);
int clean_motor_alert_state(int *ptr_can,int motor_address);
int ds300c_motor_brake(int *ptr_can,int motor_address,unsigned short set_current);
int ds300c_motor_brake_quit(int *ptr_can,int motor_address);
int motor_alert_get(int *ptr_can,int motor_address);
int motor_get_move_state(int* ptr_can,int motor_address);
int motor_set_stop_accel(int* ptr_can, int motor_address,int accel);
int motor_continue_curve_control(int *ptr_can, int motor_address, int control_sign);

int motor_move_abs_usmart(int *ptr_can,int set_angle,int motor_address,int dir);
int motor_move_rel_usmart(int *ptr_can,int set_angle,int motor_address,int dir);
int motor_get_current(int *ptr_can,int motor_address);
#endif
