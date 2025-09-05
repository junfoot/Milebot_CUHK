
#include "json_app.h"
#include "cJSON.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "normal_lib.h"

/*

调用cJSON_CreateObject() 太需要heap 空间了, 而且float 类型有6 位小数
所以: 
发送数据决定不用cJSON 提供的库函数
接收并解析JSON 数时，还是使用cJSON 提供的库函数


打印并保存json
通过main_json , 把整个JSON 转化为普通字符串, 并返回指针
cJSON_PrintUnformatted(): 是没有格式的, 即是转换出的字符串中间不会有"\n" "\t"之类
的东西存在, 在串传输的时候使用"无格式", 可以有效减少传输内容.
cJSON_Print(): 打印出来是人看起来舒服、明了的JSON格式
char *buf = cJSON_PrintUnformatted(main_json);//减少传输内容
//char *buf = cJSON_Print(main_json);
sprintf(json_buf, "%s", buf); //把JSON 转化后为普通字符串, 复制到自定义的缓冲区json_buf中

//必须要清理工作, 释放内存
free(buf);//前面cJSON_Print() 会调用malloc()函数, 故要释放内存
cJSON_Delete(main_json);//cJSON_Delete函数用于释放JSON所占的内存空间(参数是根节点的json)


demo:

{
    uint16_t index = 0;
    char* ptr = (char*)uart6_send_buf;
    uint8_t len = 0;

    //开头, 左大括号'{'
    ptr[index++] = '{';


    //------------------------------------int 类型------------------------------------
    //"score": 100
    len = json_add_int(&ptr[index], "score", 100);
    index += len;
    ptr[index++] = ',';

    //"score-2": [100, 99, 98]
    int64_t int_buf[3] = {100, 99, 98};
    len = json_add_int_array(&ptr[index], "score-2", int_buf, 3);
    index += len;
    ptr[index++] = ',';


    //------------------------------------float 类型------------------------------------
    //"height": 1.68123
    len = json_add_float(&ptr[index], "height", 1.68123);
    index += len;
    ptr[index++] = ',';

    //"height-2": [1.6811111,1.70,1.8, 1.988888, 1.6666666]
    float float_buf[5] = {1.6811111, 1.70, 1.8, 1.988888, 1.6666666};
    len = json_add_float_array(&ptr[index], "height-2", float_buf, 5);
    index += len;
    ptr[index++] = ',';


    //------------------------------------bool 类型------------------------------------
    //"rich": true
    len = json_add_bool(&ptr[index], "rich", 1);
    index += len;
    ptr[index++] = ',';

    //"rich-2": [true,true,false]
    uint8_t bool_buf[3] = {1, 1, 0};
    len = json_add_bool_array(&ptr[index], "rich-2", bool_buf, 3);
    index += len;
    ptr[index++] = ',';


    //------------------------------------string 类型------------------------------------
    //"Country": "China"
    len = json_add_string(&ptr[index], "Country", "China");
    index += len;
    ptr[index++] = ',';

    //"Country": ["China", "USA", "UK", "Germany", "France"]
    char* string_buf[5] = {"China", "USA", "UK", "Germany", "France"};
    len = json_add_string_array(&ptr[index], "Country-2", string_buf, 5);
    index += len;
//    ptr[index++] = ',';    //最后不需要逗号



    //结尾, 右大括号'}'
    ptr[index++] = '}';

    //字符串结束符, 因为strlen()
    ptr[index++] = '\0';

    uart6_send_buf_len = index;

}






*/


/**** Definition of types ****/ 



Server2Client_DATA s2c_data = 
{
    .mode = 1,
    .assist = 0,
    .hmp_side = 0,
    .lift_height = 1,
    .lift_side = 0,
    .speed = 1,
    .walk_ctrl = 0,
    .device_SN_buf = {0},
};

//JSON 数据的索引表
volatile JSON_DATA_MASS JSON_index_buf[JSON_TOTAL_END] = 
{
    {"mode", JSON_DATA_TYPE_uint8, (void*)(&s2c_data.mode)},
    {"assist", JSON_DATA_TYPE_uint8, (void*)(&s2c_data.assist)},
    {"hmp_side", JSON_DATA_TYPE_uint8, (void*)(&s2c_data.hmp_side)},
    {"lift_height", JSON_DATA_TYPE_uint8, (void*)(&s2c_data.lift_height)},
    {"lift_side", JSON_DATA_TYPE_uint8, (void*)(&s2c_data.lift_side)},
    {"speed", JSON_DATA_TYPE_uint8, (void*)(&s2c_data.speed)},
    {"walk_ctrl", JSON_DATA_TYPE_uint8, (void*)(&s2c_data.walk_ctrl)},
    {"SN", JSON_DATA_TYPE_string, (void*)(s2c_data.device_SN_buf)}, 
};



/* Private variables ---------------------------------------------------------*/
//完整的解析前的JSON 数据包的buf
#define pre_JSON_LEN    1024
static char pre_JSON_buf[pre_JSON_LEN] = {0x00};


/* Extern variables ---------------------------------------------------------*/


/* functions ---------------------------------------------------------*/

static void get_JSON_value_via_key(char* buf);


//解析JSON 数据
uint8_t analyze_JSON_data(uint8_t *buf, uint16_t len)
{
    char* search_front_ptr = NULL;
    char* search_back_ptr = NULL;
    uint16_t valid_JSON_data_len = 0;
    char* buf_ptr = NULL;
    uint8_t analyze_flag = 0;

    
    //从buf 中的'{' 和'}' 作为完整解析包
    //查找'{' 和'}'并copy后,解析JSON前要清除ble_raw_data_rx_len = 0;
    
    buf[len] = '\0';//添加字符串结束符, 防止被查找的字符串没有结束符'\0'
    buf_ptr = (char*)buf;

    do 
    {
        //防止出现{xxx{xxx{xxxxx}这种情况
        //不处理{xxx{xxx{xxxxx}xx}xxx}嵌套的情况
        //strrchr() 查找字符c在一个字符串s中<最后一次>出现的位置
        //search_front_ptr = strchr(buf_ptr, '{');//strchr()
        search_front_ptr = strrchr(buf_ptr, '{');//strrchr()
        if(search_front_ptr != NULL)
        {
            //strchr() 查找字符串s 中<首次>出现c 字符的位置
            search_back_ptr = strchr((search_front_ptr+1), '}');
            if(search_back_ptr != NULL)
            {
                valid_JSON_data_len = search_back_ptr - search_front_ptr + 1;
                if(valid_JSON_data_len < pre_JSON_LEN)
                {
                    memcpy(pre_JSON_buf, search_front_ptr, valid_JSON_data_len);
                    pre_JSON_buf[valid_JSON_data_len] = '\0';//添加字符串结束符, 防止字符串没有结束符'\0'
                
                    //根据JSON 数据, 通过key, 获取value
                    get_JSON_value_via_key(pre_JSON_buf);
                    analyze_flag = 1;
                }
            }
        }

        //继续查找最新的JSON
        buf_ptr = search_back_ptr + 1;

    } while((search_front_ptr != NULL) && (search_back_ptr != NULL));

    //清除旧的JSON buf
    memset(buf, 0x00, len);//清除旧的JSON buf

    return analyze_flag;

}



//根据JSON 数据, 通过key, 获取value
static void get_JSON_value_via_key(char* buf)
{
    cJSON* main_json = NULL;
    cJSON* temp_key = NULL;
    RECEIVE_JSON_INDEX JSON_index = JSON_mode;
    
    //解析json 数据的函数
    main_json = cJSON_Parse((char *)buf);//从缓冲区中解析出JSON结构
    if(main_json == NULL)
    {
        return;
    }

    //cJOSN_GetObjectItem 根据key来查找json节点, 若有则返回非空
    for(JSON_index = JSON_mode; JSON_index < JSON_TOTAL_END; JSON_index++)
    {
        temp_key = cJSON_GetObjectItem(main_json, (JSON_index_buf[JSON_index].key_ptr));
        if(temp_key != NULL)
        {
            if(temp_key->type != cJSON_Array)//----------------不是数组----------------
            {
                switch(JSON_index_buf[JSON_index].data_type)
                {
//                    case JSON_DATA_TYPE_bool:
//                        (*((STATUS_FLAG*)(JSON_index_buf[JSON_index].data_ptr))) = (STATUS_FLAG)(temp_key->valueint);//bool
//                        break;

                    case JSON_DATA_TYPE_uint8:
                        (*((uint8_t*)(JSON_index_buf[JSON_index].data_ptr))) = (uint8_t)(temp_key->valueint);//uint8_t
                        break;

//                    case JSON_DATA_TYPE_uint16:
//                        (*((uint16_t*)(JSON_index_buf[JSON_index].data_ptr))) = (uint16_t)(temp_key->valueint);//uint16_t
//                        break;

//                    case JSON_DATA_TYPE_uint32:
//                        (*((uint32_t*)(JSON_index_buf[JSON_index].data_ptr))) = (uint32_t)(temp_key->valueint);//uint32_t
//                        break;

//                    case JSON_DATA_TYPE_float:
//                        (*((float*)(JSON_index_buf[JSON_index].data_ptr))) = (float)(temp_key->valuedouble);//float
//                        break;

                    case JSON_DATA_TYPE_string:
                        //((char*)(JSON_index_buf[JSON_index].data_ptr)) = (char *)(temp_key->valuestring);//string

                        if(JSON_SN == JSON_index)
                        {
                            if(strlen(temp_key->valuestring) == DEVICE_SN_LEN)
                            {
                                memcpy(s2c_data.device_SN_buf, (temp_key->valuestring), DEVICE_SN_LEN);
                                s2c_data.device_SN_buf[DEVICE_SN_LEN] = '\0';//字符串结束符
                            }
                        }

                        break;

                    default:
                        break;
                }
            }
//            else//----------------是数组----------------
//            {
//                uint8_t i = 0;
//                cJSON *tnode = NULL;
//                uint8_t size = cJSON_GetArraySize(temp_key);
//            
//                switch(JSON_index_buf[JSON_index].data_type)
//                {
//                    case JSON_DATA_TYPE_uint16:
//            
//                        for(i = 0; i < size; i++)
//                        {
//                            tnode = cJSON_GetArrayItem(temp_key, i);
//                            if(tnode != NULL)
//                            {
//                                (((uint16_t*)(JSON_index_buf[JSON_index].data_ptr))[i]) = (uint16_t)(tnode->valueint);//uint16_t
//                            }
//                        }
//            
//                        break;
//            
//                    case JSON_DATA_TYPE_float:
//            
//                        for(i = 0; i < size; i++)
//                        {
//                            tnode = cJSON_GetArrayItem(temp_key, i);
//                            if(tnode != NULL)
//                            {
//                                (((float*)(JSON_index_buf[JSON_index].data_ptr))[i]) = (float)(tnode->valuedouble);//float
//                            }
//                        }
//            
//                        break;
//            
//                    default:
//                        break;
//                }
//            }
        }
    }

    //必须要清理工作, 释放内存
    cJSON_Delete(main_json);//cJSON_Delete函数用于释放JSON所占的内存空间(参数是根节点的json)

}




//增加int 类型(不包括最后的逗号,), 返回增加的字节数据长度
//例如: "score": 100
uint8_t json_add_int(char* json_buf, char* key, int64_t value)
{
    uint8_t len = 0;
    uint8_t i = 0;

    //key
    len = json_add_key_string(&json_buf[i], key);
    i += len;

    //:
    json_buf[i++] = ':';

    //int
    len = json_add_value_int(&json_buf[i], value);
    i += len;

    return i;
}

//增加int 类型数组(不包括最后的逗号,), 返回增加的字节数据长度
//例如数组: "score": [100, 99, 98]
uint8_t json_add_int_array(char* json_buf, char* key, int64_t* value, uint8_t num)
{
    uint8_t len = 0;
    uint8_t i = 0;
    uint8_t loop = 0;

    //key
    len = json_add_key_string(&json_buf[i], key);
    i += len;

    //:
    json_buf[i++] = ':';

    //int array
    json_buf[i++] = '[';
    for(loop = 0; loop < num; loop++)
    {
        len = json_add_value_int(&json_buf[i], value[loop]);
        i += len;
        if(loop != (num - 1))
        {
            json_buf[i++] = ',';
        }
    }    
    json_buf[i++] = ']';

    return i;
}


//增加float 类型(不包括最后的逗号,), 返回增加的字节数据长度
//例如: "height": 1.68
uint8_t json_add_float(char* json_buf, char* key, float value)
{
    uint8_t len = 0;
    uint8_t i = 0;

    //key
    len = json_add_key_string(&json_buf[i], key);
    i += len;

    //:
    json_buf[i++] = ':';

    //float
    len = json_add_value_float(&json_buf[i], value);
    i += len;

    return i;
}


//增加float 类型数组(不包括最后的逗号,), 返回增加的字节数据长度
//例如数组: "height": [1.68,1.71,1.80]
uint8_t json_add_float_array(char* json_buf, char* key, float* value, uint8_t num)
{
    uint8_t len = 0;
    uint8_t i = 0;
    uint8_t loop = 0;

    //key
    len = json_add_key_string(&json_buf[i], key);
    i += len;

    //:
    json_buf[i++] = ':';

    //float array
    json_buf[i++] = '[';
    for(loop = 0; loop < num; loop++)
    {
        len = json_add_value_float(&json_buf[i], value[loop]);
        i += len;
        if(loop != (num - 1))
        {
            json_buf[i++] = ',';
        }
    }    
    json_buf[i++] = ']';

    return i;
}


//增加bool 类型(不包括最后的逗号,), 返回增加的字节数据长度
//例如: "rich": true
uint8_t json_add_bool(char* json_buf, char* key, uint8_t value)
{
    uint8_t len = 0;
    uint8_t i = 0;

    //key
    len = json_add_key_string(&json_buf[i], key);
    i += len;

    //:
    json_buf[i++] = ':';

    //bool
    len = json_add_value_bool(&json_buf[i], value);
    i += len;

    return i;
}

//增加bool 类型数组(不包括最后的逗号,), 返回增加的字节数据长度
//例如数组: "rich": [true,true,false]
uint8_t json_add_bool_array(char* json_buf, char* key, uint8_t* value, uint8_t num)
{
    uint8_t len = 0;
    uint8_t i = 0;
    uint8_t loop = 0;

    //key
    len = json_add_key_string(&json_buf[i], key);
    i += len;

    //:
    json_buf[i++] = ':';

    //bool array
    json_buf[i++] = '[';
    for(loop = 0; loop < num; loop++)
    {
        len = json_add_value_bool(&json_buf[i], value[loop]);
        i += len;
        if(loop != (num - 1))
        {
            json_buf[i++] = ',';
        }
    }    
    json_buf[i++] = ']';

    return i;
}


//增加string 类型(不包括最后的逗号,), 返回增加的字节数据长度
//例如: "Country": "China"
uint8_t json_add_string(char* json_buf, char* key, char* value)
{
    uint8_t len = 0;
    uint8_t i = 0;

    //key
    len = json_add_key_string(&json_buf[i], key);
    i += len;

    //:
    json_buf[i++] = ':';

    //string
    len = json_add_value_string(&json_buf[i], value);
    i += len;
    
    return i;
}


//增加string 类型数组(不包括最后的逗号,), 返回增加的字节数据长度
//例如数组: "Country": ["China", "USB", "UK"]
uint8_t json_add_string_array(char* json_buf, char* key, char** value, uint8_t num)
{
    uint8_t len = 0;
    uint8_t i = 0;
    uint8_t loop = 0;

    //key
    len = json_add_key_string(&json_buf[i], key);
    i += len;

    //:
    json_buf[i++] = ':';

    //string array
    json_buf[i++] = '[';
    for(loop = 0; loop < num; loop++)
    {
        len = json_add_value_string(&json_buf[i], value[loop]);
        i += len;
        if(loop != (num - 1))
        {
            json_buf[i++] = ',';
        }
    }    
    json_buf[i++] = ']';

    return i;
}







//----------------下面函数一般不需要直接调用----------------

//增加json 的key (string), 返回增加的字节数据长度
uint8_t json_add_key_string(char* json_buf, char* value)
{
    uint8_t len = 0;
    uint8_t i = 0;

    json_buf[i++] = '"';
    len = strlen(value);
    memcpy(&json_buf[i], value, len);
    i += len;
    json_buf[i++] = '"';

    return i;
}


//增加json 的value (int64_t), 返回增加的字节数据长度
uint8_t json_add_value_int(char* json_buf, int64_t value)
{
    uint8_t len = 0;
    uint8_t i = 0;

    len = int2str(value, &json_buf[i]);
    i += len;

    return i;
}


//增加json 的value (float), 返回增加的字节数据长度
uint8_t json_add_value_float(char* json_buf, float value)
{
    uint8_t len = 0;
    uint8_t i = 0;

    len = float2str(value, &json_buf[i]);
    i += len;

    return i;
}


//增加json 的value (bool), 返回增加的字节数据长度
uint8_t json_add_value_bool(char* json_buf, uint8_t value)
{
    uint8_t i = 0;

    //bool
    if(value == 0)
    {
        json_buf[i++] = 'f';
        json_buf[i++] = 'a';
        json_buf[i++] = 'l';
        json_buf[i++] = 's';
        json_buf[i++] = 'e';
    }
    else
    {
        json_buf[i++] = 't';
        json_buf[i++] = 'r';
        json_buf[i++] = 'u';
        json_buf[i++] = 'e';
    }

    return i;
}


//增加json 的value (string), 返回增加的字节数据长度
uint8_t json_add_value_string(char* json_buf, char* value)
{
    uint8_t len = 0;
    uint8_t i = 0;

    json_buf[i++] = '"';
    len = strlen(value);
    memcpy(&json_buf[i], value, len);
    i += len;
    json_buf[i++] = '"';

    return i;
}


