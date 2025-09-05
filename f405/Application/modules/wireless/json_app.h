
#ifndef __JSON_APP_H__
#define __JSON_APP_H__

#include <stdint.h>
#include "internal_flash.h"


/**** Definition of types ****/ 

//JSON 数据的类型
typedef enum
{
    JSON_DATA_TYPE_bool = 0,
    JSON_DATA_TYPE_uint8,
    JSON_DATA_TYPE_uint16,
    JSON_DATA_TYPE_uint32,
    JSON_DATA_TYPE_float,
    JSON_DATA_TYPE_string,
} JSON_DATA_TYPE;

//JSON 数据属性集
typedef struct
{
    char* key_ptr;
    JSON_DATA_TYPE data_type;//JSON 数据的类型
    void* data_ptr;
} JSON_DATA_MASS;


//接收到的JSON 的索引
typedef enum
{
    JSON_mode = 0,
    JSON_assist,
    JSON_hmp_side,
    JSON_lift_height,
    JSON_lift_side,
    JSON_speed,
    JSON_walk_ctrl,
    JSON_SN,
    
    //end
    JSON_TOTAL_END,
} RECEIVE_JSON_INDEX;

//服务器发送给客户端的JSON 解析后的数据
typedef struct
{
    uint8_t mode;
    uint8_t assist;
    uint8_t hmp_side;
    uint8_t lift_height;
    uint8_t lift_side;
    uint8_t speed;
    uint8_t walk_ctrl;
    uint8_t device_SN_buf[DEVICE_SN_LEN + 1];
} Server2Client_DATA;

//external
extern Server2Client_DATA s2c_data;


//函数接口
uint8_t analyze_JSON_data(uint8_t *buf, uint16_t len);

uint8_t json_add_int(char* json_buf, char* key, int64_t value);
uint8_t json_add_int_array(char* json_buf, char* key, int64_t* value, uint8_t num);
uint8_t json_add_float(char* json_buf, char* key, float value);
uint8_t json_add_float_array(char* json_buf, char* key, float* value, uint8_t num);
uint8_t json_add_bool(char* json_buf, char* key, uint8_t value);
uint8_t json_add_bool_array(char* json_buf, char* key, uint8_t* value, uint8_t num);
uint8_t json_add_string(char* json_buf, char* key, char* value);
uint8_t json_add_string_array(char* json_buf, char* key, char** value, uint8_t num);

//----------------下面函数一般不需要直接调用----------------
uint8_t json_add_key_string(char* json_buf, char* value);
uint8_t json_add_value_int(char* json_buf, int64_t value);
uint8_t json_add_value_float(char* json_buf, float value);
uint8_t json_add_value_bool(char* json_buf, uint8_t value);
uint8_t json_add_value_string(char* json_buf, char* value);


#endif

