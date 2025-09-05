
#include "json_app.h"
#include "cJSON.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "normal_lib.h"

/*

����cJSON_CreateObject() ̫��Ҫheap �ռ���, ����float ������6 λС��
����: 
�������ݾ�������cJSON �ṩ�Ŀ⺯��
���ղ�����JSON ��ʱ������ʹ��cJSON �ṩ�Ŀ⺯��


��ӡ������json
ͨ��main_json , ������JSON ת��Ϊ��ͨ�ַ���, ������ָ��
cJSON_PrintUnformatted(): ��û�и�ʽ��, ����ת�������ַ����м䲻����"\n" "\t"֮��
�Ķ�������, �ڴ������ʱ��ʹ��"�޸�ʽ", ������Ч���ٴ�������.
cJSON_Print(): ��ӡ�������˿�������������˵�JSON��ʽ
char *buf = cJSON_PrintUnformatted(main_json);//���ٴ�������
//char *buf = cJSON_Print(main_json);
sprintf(json_buf, "%s", buf); //��JSON ת����Ϊ��ͨ�ַ���, ���Ƶ��Զ���Ļ�����json_buf��

//����Ҫ������, �ͷ��ڴ�
free(buf);//ǰ��cJSON_Print() �����malloc()����, ��Ҫ�ͷ��ڴ�
cJSON_Delete(main_json);//cJSON_Delete���������ͷ�JSON��ռ���ڴ�ռ�(�����Ǹ��ڵ��json)


demo:

{
    uint16_t index = 0;
    char* ptr = (char*)uart6_send_buf;
    uint8_t len = 0;

    //��ͷ, �������'{'
    ptr[index++] = '{';


    //------------------------------------int ����------------------------------------
    //"score": 100
    len = json_add_int(&ptr[index], "score", 100);
    index += len;
    ptr[index++] = ',';

    //"score-2": [100, 99, 98]
    int64_t int_buf[3] = {100, 99, 98};
    len = json_add_int_array(&ptr[index], "score-2", int_buf, 3);
    index += len;
    ptr[index++] = ',';


    //------------------------------------float ����------------------------------------
    //"height": 1.68123
    len = json_add_float(&ptr[index], "height", 1.68123);
    index += len;
    ptr[index++] = ',';

    //"height-2": [1.6811111,1.70,1.8, 1.988888, 1.6666666]
    float float_buf[5] = {1.6811111, 1.70, 1.8, 1.988888, 1.6666666};
    len = json_add_float_array(&ptr[index], "height-2", float_buf, 5);
    index += len;
    ptr[index++] = ',';


    //------------------------------------bool ����------------------------------------
    //"rich": true
    len = json_add_bool(&ptr[index], "rich", 1);
    index += len;
    ptr[index++] = ',';

    //"rich-2": [true,true,false]
    uint8_t bool_buf[3] = {1, 1, 0};
    len = json_add_bool_array(&ptr[index], "rich-2", bool_buf, 3);
    index += len;
    ptr[index++] = ',';


    //------------------------------------string ����------------------------------------
    //"Country": "China"
    len = json_add_string(&ptr[index], "Country", "China");
    index += len;
    ptr[index++] = ',';

    //"Country": ["China", "USA", "UK", "Germany", "France"]
    char* string_buf[5] = {"China", "USA", "UK", "Germany", "France"};
    len = json_add_string_array(&ptr[index], "Country-2", string_buf, 5);
    index += len;
//    ptr[index++] = ',';    //�����Ҫ����



    //��β, �Ҵ�����'}'
    ptr[index++] = '}';

    //�ַ���������, ��Ϊstrlen()
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

//JSON ���ݵ�������
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
//�����Ľ���ǰ��JSON ���ݰ���buf
#define pre_JSON_LEN    1024
static char pre_JSON_buf[pre_JSON_LEN] = {0x00};


/* Extern variables ---------------------------------------------------------*/


/* functions ---------------------------------------------------------*/

static void get_JSON_value_via_key(char* buf);


//����JSON ����
uint8_t analyze_JSON_data(uint8_t *buf, uint16_t len)
{
    char* search_front_ptr = NULL;
    char* search_back_ptr = NULL;
    uint16_t valid_JSON_data_len = 0;
    char* buf_ptr = NULL;
    uint8_t analyze_flag = 0;

    
    //��buf �е�'{' ��'}' ��Ϊ����������
    //����'{' ��'}'��copy��,����JSONǰҪ���ble_raw_data_rx_len = 0;
    
    buf[len] = '\0';//����ַ���������, ��ֹ�����ҵ��ַ���û�н�����'\0'
    buf_ptr = (char*)buf;

    do 
    {
        //��ֹ����{xxx{xxx{xxxxx}�������
        //������{xxx{xxx{xxxxx}xx}xxx}Ƕ�׵����
        //strrchr() �����ַ�c��һ���ַ���s��<���һ��>���ֵ�λ��
        //search_front_ptr = strchr(buf_ptr, '{');//strchr()
        search_front_ptr = strrchr(buf_ptr, '{');//strrchr()
        if(search_front_ptr != NULL)
        {
            //strchr() �����ַ���s ��<�״�>����c �ַ���λ��
            search_back_ptr = strchr((search_front_ptr+1), '}');
            if(search_back_ptr != NULL)
            {
                valid_JSON_data_len = search_back_ptr - search_front_ptr + 1;
                if(valid_JSON_data_len < pre_JSON_LEN)
                {
                    memcpy(pre_JSON_buf, search_front_ptr, valid_JSON_data_len);
                    pre_JSON_buf[valid_JSON_data_len] = '\0';//����ַ���������, ��ֹ�ַ���û�н�����'\0'
                
                    //����JSON ����, ͨ��key, ��ȡvalue
                    get_JSON_value_via_key(pre_JSON_buf);
                    analyze_flag = 1;
                }
            }
        }

        //�����������µ�JSON
        buf_ptr = search_back_ptr + 1;

    } while((search_front_ptr != NULL) && (search_back_ptr != NULL));

    //����ɵ�JSON buf
    memset(buf, 0x00, len);//����ɵ�JSON buf

    return analyze_flag;

}



//����JSON ����, ͨ��key, ��ȡvalue
static void get_JSON_value_via_key(char* buf)
{
    cJSON* main_json = NULL;
    cJSON* temp_key = NULL;
    RECEIVE_JSON_INDEX JSON_index = JSON_mode;
    
    //����json ���ݵĺ���
    main_json = cJSON_Parse((char *)buf);//�ӻ������н�����JSON�ṹ
    if(main_json == NULL)
    {
        return;
    }

    //cJOSN_GetObjectItem ����key������json�ڵ�, �����򷵻طǿ�
    for(JSON_index = JSON_mode; JSON_index < JSON_TOTAL_END; JSON_index++)
    {
        temp_key = cJSON_GetObjectItem(main_json, (JSON_index_buf[JSON_index].key_ptr));
        if(temp_key != NULL)
        {
            if(temp_key->type != cJSON_Array)//----------------��������----------------
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
                                s2c_data.device_SN_buf[DEVICE_SN_LEN] = '\0';//�ַ���������
                            }
                        }

                        break;

                    default:
                        break;
                }
            }
//            else//----------------������----------------
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

    //����Ҫ������, �ͷ��ڴ�
    cJSON_Delete(main_json);//cJSON_Delete���������ͷ�JSON��ռ���ڴ�ռ�(�����Ǹ��ڵ��json)

}




//����int ����(���������Ķ���,), �������ӵ��ֽ����ݳ���
//����: "score": 100
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

//����int ��������(���������Ķ���,), �������ӵ��ֽ����ݳ���
//��������: "score": [100, 99, 98]
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


//����float ����(���������Ķ���,), �������ӵ��ֽ����ݳ���
//����: "height": 1.68
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


//����float ��������(���������Ķ���,), �������ӵ��ֽ����ݳ���
//��������: "height": [1.68,1.71,1.80]
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


//����bool ����(���������Ķ���,), �������ӵ��ֽ����ݳ���
//����: "rich": true
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

//����bool ��������(���������Ķ���,), �������ӵ��ֽ����ݳ���
//��������: "rich": [true,true,false]
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


//����string ����(���������Ķ���,), �������ӵ��ֽ����ݳ���
//����: "Country": "China"
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


//����string ��������(���������Ķ���,), �������ӵ��ֽ����ݳ���
//��������: "Country": ["China", "USB", "UK"]
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







//----------------���溯��һ�㲻��Ҫֱ�ӵ���----------------

//����json ��key (string), �������ӵ��ֽ����ݳ���
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


//����json ��value (int64_t), �������ӵ��ֽ����ݳ���
uint8_t json_add_value_int(char* json_buf, int64_t value)
{
    uint8_t len = 0;
    uint8_t i = 0;

    len = int2str(value, &json_buf[i]);
    i += len;

    return i;
}


//����json ��value (float), �������ӵ��ֽ����ݳ���
uint8_t json_add_value_float(char* json_buf, float value)
{
    uint8_t len = 0;
    uint8_t i = 0;

    len = float2str(value, &json_buf[i]);
    i += len;

    return i;
}


//����json ��value (bool), �������ӵ��ֽ����ݳ���
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


//����json ��value (string), �������ӵ��ֽ����ݳ���
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


