
#include <stdint.h>
#include "normal_lib.h"

//Mem_Copy
void Mem_Copy_Ex(uint8_t *psdata, uint8_t *pddata, uint16_t num)
{
    uint16_t i;

    for(i=0; i<num; i++)
    {
        *pddata++ = *psdata++;
    }
}

//Mem_Copy
void Mem_Copy_N(uint8_t *psdata, uint8_t *pddata, uint16_t len, uint16_t loc, uint16_t num)
{
    uint16_t i;

    for(i=0; i<num; i++)
    {
        *pddata++ = *(psdata + loc);
        loc = (loc + 1) % len;
    }
}

//Mem_Set
void Mem_Set_Ex(uint8_t *pdata, uint8_t data, uint16_t num)
{
    uint16_t i;

    for(i=0; i<num; i++)
    {
        *pdata++ = data;
    }
}

//U32Abs
uint32_t U32Abs(uint32_t a, uint32_t b)
{
    uint32_t result;

    if(a >= b)
    {
        return(a - b);
    }
    else
    {
        result = 0xFFFFFFFF - b;
        result = a + result + 1;
        
        return result;
    }
}

//Xor
uint8_t Xor_Byte(uint8_t pdata[], uint16_t num)
{
    uint8_t result;
    uint16_t i;

    result = pdata[0];
    for(i=1; i<num; i++)
    {
        result ^= pdata[i];
    }

    return result;
}

/*比较字符串前N位字符：0：相等；1：大于；-1：小于*/
uint8_t StrCmp_N(uint8_t str1[], uint8_t str2[], uint8_t n)
{
    int i;
    //uint8_t result = 0;

    for(i=0; i<n; i++)
    {
        if(str1[i] > str2[i])
        {
            return 1;
        }
        else if(str1[i] < str2[i])
        {
            return 0xFF;
        }
    }

    return 0;
}

/*比较字符串：0：相等；1：大于；-1：小于*/
uint8_t strcmp_Ex(uint8_t str1[], uint8_t str2[])
{
    //int i;
    //uint8_t result = 0;
    while((*str1) && (*str2))
    {
        if(*str1 > *str2)
        {
            return 1;
        }
        else if(*str1 < *str2)
        {
            return 0xFF;
        }

        str1++;
        str2++;
    }

    if(*str1)
    {
        return 1;
    }
    else if(*str2)
    {
        return 0xFF;
    }

    return 0;

}

/*求字符串长度*/
uint8_t Str_Len_Ex(uint8_t str[])
{
    uint8_t len = 0;

    while(*str)
    {
        len++;
        str++;
    }

    return len;
}

/*字符串查找:0：没有匹配字符串；1：有匹配子字符串*/
uint8_t Str_Find(uint8_t str1[], uint8_t str2[])
{
    uint8_t i, len1, len2;

    len1 = Str_Len_Ex(str1);
    len2 = Str_Len_Ex(str2);

    if(len1 < len2)
    {
        return 0;
    }

    for(i=0; i<=(len1-len2); i++)
    {
        if(!StrCmp_N(&str1[i], &str2[0], len2))
        {
            return i+1;
        }
    }

    return 0;
}


//num为64bit 要被转换的数字
// str 为存储转换后的字符串
//把数字转换为字符串, 返回字符串的有效长度(不包括\0)
//sprintf()函数可能存在问题, 所以不使用
char int2str(int64_t num, char* str)
{
    char i = 0;
    char len = 0;
    int64_t temp = num;

    if(num == 0)
    {
        str[0] = '0';
        str[1]='\0';//字符串结束符
        
        return 1;
    }

    //正负数
    if(num < 0)
    {
        num = -num;
        len++;
        str[0] = '-';
    }
    temp = num;

    //长度
    while(num > 0)
    {
        len++;
        num /= 10;
    }

    //转换
    num = temp;
    do
    {
        str[len - 1 - i] = ((num % 10) + '0');
        num /= 10;
        i++;
    }while(num);
    str[len]='\0';//字符串结束符

    return len;
}


//num为要被转换的浮点型数字, 保留小数点后2 位有效数据
// str 为存储转换后的字符串
//把数字转换为字符串, 返回字符串的有效长度(不包括\0)
//sprintf()函数可能存在问题, 所以不使用
char float2str(float num, char* str)
{
    char i = 0;
    char len = 0;
    char sign = 0;
    int int_num = 0;
    int temp = 0;

    if((num > -0.001) && (num < 0.001))
    {
        str[0] = '0';
        str[1]='\0';//字符串结束符
        
        return 1;
    }

    //把float 转换为int, 保留2 位小数
    int_num = (int)(num*100);

    //正负数
    if(int_num < 0)
    {
        sign = 1;
        int_num = -int_num;
        len++;
        str[0] = '-';
    }
    temp = int_num;

    //特殊处理, 如0.05 或-0.50
    if(int_num < 10)
    {
        if(sign == 0)
        {
            str[0] = '0';
            str[1] = '.';
            str[2] = '0';
            str[3] = int_num + '0';
            str[4]='\0';//字符串结束符
            return 4;
        }
        else
        {
            str[0] = '-';
            str[1] = '0';
            str[2] = '.';
            str[3] = '0';
            str[4] = int_num + '0';
            str[5]='\0';//字符串结束符
            return 5;
        }
    }
    else if(int_num < 100)
    {
        if(sign == 0)
        {
            str[0] = '0';
            str[1] = '.';
            str[2] = int_num/10 + '0';
            str[3] = int_num%10 + '0';
            str[4]='\0';//字符串结束符
            return 4;
        }
        else
        {
            str[0] = '-';
            str[1] = '0';
            str[2] = '.';
            str[3] = int_num/10 + '0';
            str[4] = int_num%10 + '0';
            str[5]='\0';//字符串结束符
            return 5;
        }
    }

    //长度
    while(int_num > 0)
    {
        len++;
        int_num /= 10;
    }
    len++;//小数点占用1 个字节的长度

    //转换
    int_num = temp;
    do
    {
        str[len - 1 - i] = ((int_num % 10) + '0');
        int_num /= 10;
        i++;
        if(i == 2)//小数点
        {
            str[len - 1 - i] = '.';
            i++;
        }
    }while(int_num);
    str[len]='\0';//字符串结束符


    return len;
}


