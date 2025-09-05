/**
* @file        walk.h
* @brief       the head file of walk.c
* @details     Including definitions
* @author      Huguang, any question please send mail to huguang108@163.com
* @date        2020-6-29
* @version     1.0
* @copyright   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd. All Rights Reserved.
**********************************************************************************
* @attention
* @par         Modify log:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/06/29  <td>1.0      <td>Huguang   <td>initial version
* </table>
*
**********************************************************************************
*/

#ifndef _WALK_H_
#define _WALK_H_

#include "algorithm.h"
#include "mcl_interpolation_if.h"
#include "common.h"

// output torque limit
#define T_LIMIT 3

// cut-ff frequency of force, angle and velocity
#define FORCE_CUT_OFF_FREQ 10	 
#define ANGLE_CUT_OFF_FREQ 10
#define VEL_CUT_OFF_FREQ 10
#define FFT_DATA_QUEUE_SIZE	380

#define STEP_COUNT 10
#define ANGLE_LENGTH 300

// Used to transform data types
union data_type
{
    float data_float;
    uint8_t data_buf[4];
};

// Joint information
typedef struct  
{
	filter_info f_actual;
    filter_info angle;
    filter_info vel;
}joint_info;

// struct of detect gait
typedef struct
{
	float gait_cycle[ALL_JOINTS];       			// gait cycle, s
	float gait_amplitude[ALL_JOINTS];  				// gait amplitude, degree, relative value
	float gait_phase[ALL_JOINTS];       			// gait phase
	uint8_t gait_stop_flag[ALL_JOINTS];             // gait stop flag, 0 - stopping, 1 - walkinging
}detect_gait_info;

typedef enum
{
	IDLE_LOOP = 0,             ///< 空循环
	WALK_LOOP,                 ///< 正常模式控制循环
	TEST_LOOP,                 ///< 测试模式控制循环
	PRE_WALK_LOOP,             ///< 正常模式之前的控制循环
	HEMIPLEGIA_LOOP,           ///< 偏瘫模式控制循环
	RESISTANCE_LOOP,             ///< 帕金森模式控制循环
	PASSIVE_LOOP,             ///< 被动模式控制循环
	EVALUATION_LOOP,          ///<
	SLIP_LOOP,                  ///< 摔倒模式控制循环
	PARKINSON_LOOP,
}CONTROL_LOOP;

typedef enum
{
	no_assist = 0,
	up_assist,
	down_assist,
	up_extension,
	down_extension,
	stable_assist,
}assist_type;

typedef enum
{
	slow_type = 0,
	normal_type,
	fast_type,
}customer_type;

// struct of walk control
typedef struct
{
	joint_info joint[ALL_JOINTS];
	mcl_parabola_poly_intpol_st torque_intel_p[ALL_JOINTS];
	ACTUATOR_CMD actuator_cmd[ALL_JOINTS];
	detect_gait_info detect_gait;

	uint8_t assist_level;              //助力等级 0~4 -- 1~5级
    uint8_t lift_level_left_hip;       //抬腿高度--左髋 0 1 2 -- 低 中 高
    uint8_t lift_level_right_hip;      //抬腿高度--右髋 0 1 2 -- 低 中 高
    uint8_t assist_side;               //偏瘫模式的患侧，或者帕金森模式的助力侧：0没有选择、1左侧、2右侧
    uint8_t lift_leg_flag;			   //帕金森模式抬腿点击标志：0->1 上升沿表示按下
    uint8_t speed_level;			   //被动模式运行速度

	uint16_t steps;                    //步数
	uint8_t slip_flag;                 //摔倒标志：0没摔倒 1摔倒
	uint8_t slip_detect_switch;
	uint8_t sitdown_detect_switch;
	uint8_t passive_press_flag;
	uint8_t mode_change_flag;
	float lift_vel_threshold;
	float assist_vel_threshold[ALL_JOINTS];
	float cancel_assist_vel_threshold[ALL_JOINTS];
	float up_time[ALL_JOINTS];
	float down_time[ALL_JOINTS];
	float peak_time[ALL_JOINTS];
	float power_coef[ALL_JOINTS];
	float power_coef_hemiplegia;
	float up_flexion_time;
	float torq_percent;
	float extension_ang_max[ALL_JOINTS];
	uint8_t exten_ang_mark_flag[ALL_JOINTS];
	float flexion_ang_max[ALL_JOINTS];
	assist_type torq_type[ALL_JOINTS];
	uint16_t flexion_cnt[ALL_JOINTS];
	uint16_t support_cnt[ALL_JOINTS];
	uint8_t para_plan_flag;
	float assist_vel_threshold_min[ALL_JOINTS];
	customer_type user_type;
	uint8_t sit_down_flag;
	uint8_t stand_up_flag;

	float healthy_start_lift_angle;
	float affected_start_lift_angle;

	uint16_t affected_detect_cnt;
	float vel_stop_limit;
	uint8_t affected_lift_flag; 
		
		
	float temp_ang[ALL_JOINTS];
	float temp_vel[ALL_JOINTS];
	float temp_torq[ALL_JOINTS];
	float pre_walk_start_angel;
	uint8_t pre_walk_status;

	uint8_t torq_smooth_flag;
	uint8_t torq_change;

	uint8_t temp_torq_down_flag[ALL_JOINTS];

    CONTROL_LOOP control_loop;
    float init_angle[ALL_JOINTS];
}walk_struct;

typedef struct
{
	float zero_angle[ALL_JOINTS];
	float max_extension_angle[ALL_JOINTS];
	float max_flexion_angle[ALL_JOINTS];
}FLASH_DATA;

typedef struct
{
    float qBuf[FFT_DATA_QUEUE_SIZE];
    uint16_t head;
    uint16_t tail;
} FFT_DATA_QUEUE;

typedef struct
{
	float flexion_angle_max[ALL_JOINTS];
	float extension_angle_max[ALL_JOINTS];
	float flexion_vel_max[ALL_JOINTS];
	uint16_t support_cnt[ALL_JOINTS];
	uint16_t swing_cnt[ALL_JOINTS];
	uint16_t flexion_angle_max_cnt[ALL_JOINTS];

	uint8_t valid[ALL_JOINTS];
	uint16_t step_count[ALL_JOINTS];

	float accel_y_temp[ALL_JOINTS];
	uint8_t imu_peak_flag[ALL_JOINTS];
	uint16_t exten_delay_cnt[ALL_JOINTS];
	uint16_t flex_delay_cnt[ALL_JOINTS];
	uint8_t flex_start_flag[ALL_JOINTS];

	uint8_t start_record_flag[ALL_JOINTS];
	uint8_t record_finish_flag[ALL_JOINTS];
	uint8_t invalid_flag[ALL_JOINTS];
	uint8_t count_cnt[ALL_JOINTS];
	uint8_t cycle_flag[ALL_JOINTS];

	uint16_t support_cnt_temp[ALL_JOINTS];
}assessment_data;

typedef struct
{
	float flexion_angle_max_max[ALL_JOINTS];
	float flexion_angle_max_min[ALL_JOINTS];
	float extension_angle_max_max[ALL_JOINTS];
	float extension_angle_max_min[ALL_JOINTS];
	float flexion_vel_max_max[ALL_JOINTS];
	float flexion_vel_max_min[ALL_JOINTS];
	float step_freq_max[ALL_JOINTS];
	float step_freq_min[ALL_JOINTS];
	float swing_per_max[ALL_JOINTS];
	float swing_per_min[ALL_JOINTS];
	float support_per_max[ALL_JOINTS];
	float support_per_min[ALL_JOINTS];
	float support_swing_per_max[ALL_JOINTS];
	float support_swing_per_min[ALL_JOINTS];
	
	float support_cnt_max[ALL_JOINTS];
	float support_cnt_min[ALL_JOINTS];
	float swing_cnt_max[ALL_JOINTS];
	float swing_cnt_min[ALL_JOINTS];

	float flexion_angle_max[ALL_JOINTS][STEP_COUNT];
	float extension_angle_max[ALL_JOINTS][STEP_COUNT];
	float flexion_vel_max[ALL_JOINTS][STEP_COUNT];
	uint16_t support_cnt[ALL_JOINTS][STEP_COUNT];
	uint16_t swing_cnt[ALL_JOINTS][STEP_COUNT];

	float flexion_angle_max_avr[ALL_JOINTS];
	float extension_angle_max_avr[ALL_JOINTS];
	float flexion_vel_max_avr[ALL_JOINTS];
	float support_cnt_avr[ALL_JOINTS];
	float swing_cnt_avr[ALL_JOINTS];
	float step_freq_avr[ALL_JOINTS];
	float swing_per_avr[ALL_JOINTS];
	float support_per_avr[ALL_JOINTS];
	float support_swing_per_avr[ALL_JOINTS];

	float flexion_angle_max_var[ALL_JOINTS];
	float extension_angle_max_var[ALL_JOINTS];
	float flexion_vel_max_var[ALL_JOINTS];
	float support_cnt_var[ALL_JOINTS];
	float swing_cnt_var[ALL_JOINTS];
	float step_freq_var[ALL_JOINTS];
	float swing_per_var[ALL_JOINTS];
	float support_per_var[ALL_JOINTS];
	float support_swing_per_var[ALL_JOINTS];

	float angle_data[ALL_JOINTS][ANGLE_LENGTH];
	uint16_t angle_data_length[ALL_JOINTS];

	float diff_angle;
	float diff_vel;
	float diff_support_time;

	float complete_per;

	uint16_t step_count[ALL_JOINTS];
}gait_data;

typedef enum
{
	healthy_detect_lift_status = 0,
	healthy_lift_status,
	healthy_angle_max_status,
	healthy_foot_contact_status,
	affected_assist_flexion_status,
	affected_flexion_delay_status,
	affected_assist_lift_status,
	affected_foot_contact_status,
	init_status,
	assist_side_change_status,
}hemiplegia_status;

typedef enum
{
	para_init_status = 0,
	detect_lift_status,
	flexion_status,
	flexion_angle_max_status,
	foot_contact_status,
	extension_status,
	extension_angle_max_status,
}motion_status;

typedef enum
{
	pre_walk_detect_lift_status = 0,
	pre_walk_left_lift_status,
	pre_walk_left_pja_status,
	pre_walk_left_fc_status,
	pre_walk_right_lift_status,
	pre_walk_right_pja_status,
	pre_walk_right_fc_status,
}pre_walk_status;

typedef enum
{
	assist_init_status = 0,
	start_walk_detect_status,
	left_detect_lift_status,
	left_lift_status,
	left_pja_status,
	left_fc_status,
	right_detect_lift_status, // =6
	right_lift_status,
	right_pja_status,
	right_fc_status,
	sit_down_status,
	stand_up_status,
}assist_status;


static void detect_gait(float vel_l, float vel_r, int *flag1, int *flag2);
static void actuator_state_walk_cb(uint8_t __attribute__((unused)) sender_id, ACTUATOR_INFO *msg);
static void walk_control(walk_struct *walk);
static void hemiplegia_control(walk_struct *walk);
static void resistance_control(walk_struct *walk);
static void parkinson_control(walk_struct *walk);
static void passive_control(walk_struct *walk);
static void assessment_control(walk_struct *walk);
static void test_control(walk_struct *walk);
static void torq_smooth_control(walk_struct *walk);
void fft_data_init(FFT_DATA_QUEUE* queue);
void fft_data_in(FFT_DATA_QUEUE* queue, float data);
void fft_data_out(FFT_DATA_QUEUE* queue, float* buffpoint);
void hemiplegia_status_detect(walk_struct* walk, hemiplegia_status* status, uint8_t assist_side, uint8_t healthy_side, float* t_e);
void hemiplegia_assist_update(walk_struct* walk, hemiplegia_status* status, float* t_e, uint8_t assist_side, uint8_t healthy_side);
void detect_motion_info(walk_struct* walk, motion_status* motion_status, assessment_data* motion_data, int detect_side, float accel);
void gait_assessment_cal(void);
void detect_motion_lift(walk_struct* walk, motion_status* motion_status, assessment_data* motion_data, int detect_side, uint16_t* cnt);
void detect_flexion_max(walk_struct* walk, motion_status* motion_status, assessment_data* motion_data, int detect_side, uint16_t* cnt);
void detect_motion_contact(walk_struct* walk, motion_status* motion_status, assessment_data* motion_data, int detect_side, float accel, uint16_t* cnt);
void detect_extension_max(walk_struct* walk, motion_status* motion_status, assessment_data* motion_data, int detect_side);
void gait_cycle_data_record(walk_struct* walk, motion_status* motion_status, assessment_data* motion_data, int detect_side);
void gait_motion_save(assessment_data* motion_data, int detect_side);
void step_data_check(assessment_data* motion_data, int detect_side);
void gait_data_update(assessment_data* motion_data, int detect_side);
void step_data_clean(assessment_data* motion_data, int detect_side);
uint8_t walk_detect_left_lift_state();
uint8_t walk_detect_right_lift_state();

extern void walk_task(void *argument);

#endif /*_WALK_H_*/

/****************************** END OF FILE ***********************************/

