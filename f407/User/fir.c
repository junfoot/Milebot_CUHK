
#include "arm_math.h"

#define TEST_LENGTH_SAMPLES  1024    /* 采样点数 */
#define BLOCK_SIZE           1         /* 调用一次arm_fir_f32处理的采样点个数 */
#define NUM_TAPS             29      /* 滤波器系数个数 */

uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = TEST_LENGTH_SAMPLES/BLOCK_SIZE;            /* 需要调用arm_fir_f32的次数 */

static float32_t testInput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES]; /* 采样点 */
static float32_t testOutput[TEST_LENGTH_SAMPLES];               /* 滤波后的输出 */
static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1];        /* 状态缓存，大小numTaps + blockSize - 1*/


/* 低通滤波器系数 通过fadtool获取*/
const float32_t firCoeffs32LP[NUM_TAPS] = {
  -0.001822523074f,  -0.001587929321f,  1.226008847e-18f,  0.003697750857f,  0.008075430058f,
  0.008530221879f,   -4.273456581e-18f, -0.01739769801f,   -0.03414586186f,  -0.03335915506f,
  8.073562366e-18f,  0.06763084233f,    0.1522061825f,     0.2229246944f,    0.2504960895f,
  0.2229246944f,     0.1522061825f,     0.06763084233f,    8.073562366e-18f, -0.03335915506f,
  -0.03414586186f,   -0.01739769801f,   -4.273456581e-18f, 0.008530221879f,  0.008075430058f,
  0.003697750857f,   1.226008847e-18f,  -0.001587929321f,  -0.001822523074f
};


/*
*********************************************************************************************************
*    函 数 名: arm_fir_f32_lp
*    功能说明: 调用函数arm_fir_f32_lp实现低通滤波器
*    形    参：无
*    返 回 值: 无
*********************************************************************************************************
*/
static void arm_fir_f32_lp(void)
{
    uint32_t i;
    arm_fir_instance_f32 S;
    float32_t  *inputF32, *outputF32;

    /* 初始化输入输出缓存指针 */
    inputF32 = &testInput_f32_50Hz_200Hz[0];
    outputF32 = &testOutput[0];

    /* 初始化结构体S */
    arm_fir_init_f32(&S,                            
                     NUM_TAPS, 
                    (float32_t *)&firCoeffs32LP[0], 
                     &firStateF32[0], 
                     blockSize);

    /* 实现FIR滤波，这里每次处理1个点 */
    for(i=0; i < numBlocks; i++)
    {
        arm_fir_f32(&S, inputF32 + (i * blockSize),  outputF32 + (i * blockSize),  blockSize);
    }
    

//    /* 打印滤波后结果 */
//    for(i=0; i<TEST_LENGTH_SAMPLES; i++)
//    {
//        printf("%f, %f\r\n", testOutput[i], inputF32[i]);
//    }

}