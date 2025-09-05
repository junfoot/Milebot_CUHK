
#ifndef __NORMAL_LIB_H__
#define __NORMAL_LIB_H__

#include <stdint.h>

//Mem_Copy
void Mem_Copy_Ex(uint8_t *psdata, uint8_t *pddata, uint16_t num);

//Mem_Copy
void Mem_Copy_N(uint8_t *psdata, uint8_t *pddata, uint16_t len, uint16_t loc, uint16_t num);

//Mem_Set
void Mem_Set_Ex(uint8_t *pdata, uint8_t data, uint16_t num);

//U32Abs
uint32_t U32Abs(uint32_t a, uint32_t b);

//Xor
uint8_t Xor_Byte(uint8_t pdata[], uint16_t num);

/*比较字符串前N位字符：0：相等；1：大于；-1：小于*/
uint8_t StrCmp_N(uint8_t str1[], uint8_t str2[], uint8_t n);

/*比较字符串：0：相等；1：大于；-1：小于*/
uint8_t strcmp_Ex(uint8_t str1[], uint8_t str2[]);

/*求字符串长度*/
uint8_t Str_Len_Ex(uint8_t str[]);

/*字符串查找:0：没有匹配字符串；1：有匹配子字符串*/
uint8_t Str_Find(uint8_t str1[], uint8_t str2[]);

//num为要被转换的数字
// str 为存储转换后的字符串
//返回字符串的有效长度(不包括\0)
char my_itoa(int num, char* str);
char my_64_itoa(uint64_t num, char* str);
char int2str(int64_t num, char* str);
char float2str(float num, char* str);

#endif

