#include "utils.h"
#include <stdlib.h>

//extern float K_admittance;
//extern float B_admittance;
//extern float M_inv_admittance;
//extern float x_desire[4];

//extern int ukey;

/* -----------------------EMG--------------------------*/

// 滤波器系数（两个滤波器）
const float32_t firCoeffs1[NUM_TAPS_1] = {
   -0.01553044678961,  -0.1574881842497, -0.06252866664882,    0.144302672573,
     0.0563003902109, 0.005470245967427,  0.08275675617697, 0.001408848605359,
    0.01125803387079,   0.0375744199072,   -0.045651174781,  0.00944232089461,
   -0.02571478397562, -0.08027771592911, 0.002019796156794,  -0.1107409408889,
   -0.07949935175631, -0.00666737646521,  -0.2514877440275,   0.1280810005249,
     0.6561543831268,   0.1280810005249,  -0.2514877440275, -0.00666737646521,
   -0.07949935175631,  -0.1107409408889, 0.002019796156794, -0.08027771592911,
   -0.02571478397562,  0.00944232089461,   -0.045651174781,   0.0375744199072,
    0.01125803387079, 0.001408848605359,  0.08275675617697, 0.005470245967427,
     0.0563003902109,    0.144302672573, -0.06252866664882,  -0.1574881842497,
   -0.01553044678961
};
const float32_t firCoeffs2[NUM_TAPS_2] = {
    0.02242078953255, 0.002690874444448, 0.002853792743583, 0.003017961111788,
   0.003138677618728, 0.003283512046493, 0.003418707276559, 0.003551691091357,
   0.003676484917277, 0.003796357224287, 0.003899321398543, 0.004000004317616,
   0.004084508184643, 0.004164665908178, 0.004232639427175, 0.004295557731147,
    0.00434444667927, 0.004389144439665, 0.004416859583134, 0.004437967219595,
   0.004442434058889, 0.004437967219595, 0.004416859583134, 0.004389144439665,
    0.00434444667927, 0.004295557731147, 0.004232639427175, 0.004164665908178,
   0.004084508184643, 0.004000004317616, 0.003899321398543, 0.003796357224287,
   0.003676484917277, 0.003551691091357, 0.003418707276559, 0.003283512046493,
   0.003138677618728, 0.003017961111788, 0.002853792743583, 0.002690874444448,
    0.02242078953255
};

// 通道数据结构
typedef struct {
    // 第一个滤波器的状态
    float32_t fir1_state[NUM_TAPS_1];
    arm_fir_instance_f32 fir1_instance;
    
    // 第二个滤波器的状态
    float32_t fir2_state[NUM_TAPS_2];
    arm_fir_instance_f32 fir2_instance;
    
    // 中间结果（整流后的值）
    float32_t rectified_value;
} channel_data_t;

// 所有通道的数据
channel_data_t channels[NUM_CHANNELS];

// 初始化所有通道的滤波器
void init_all_channels(void) {
    for (int ch = 0; ch < NUM_CHANNELS; ch++) {
        // 清零状态缓冲区
        memset(channels[ch].fir1_state, 0, sizeof(channels[ch].fir1_state));
        memset(channels[ch].fir2_state, 0, sizeof(channels[ch].fir2_state));
        
        // 初始化第一个滤波器
        arm_fir_init_f32(&channels[ch].fir1_instance, 
                                 NUM_TAPS_1, 
                                 (float32_t*)firCoeffs1, 
                                 channels[ch].fir1_state, 
                                 1); // 逐样本处理

        
        // 初始化第二个滤波器
        arm_fir_init_f32(&channels[ch].fir2_instance, 
                                 NUM_TAPS_2, 
                                 (float32_t*)firCoeffs2, 
                                 channels[ch].fir2_state, 
                                 1); // 逐样本处理
        
        // 初始化中间结果
        channels[ch].rectified_value = 0;
    }
}

// 整流函数 - 全波整流
float32_t full_wave_rectify(float32_t value) {
    return fabsf(value);
}

// 处理单个通道的单个样本
float32_t process_channel_sample(int channel_idx, float32_t input_sample) {
    float32_t output1, output2;
    channel_data_t* ch = &channels[channel_idx];
    
    // 应用第一个FIR滤波器
    arm_fir_f32(&ch->fir1_instance, &input_sample, &output1, 1);
    
    // 应用整流操作
    ch->rectified_value = full_wave_rectify(output1);
    
    // 应用第二个FIR滤波器
    arm_fir_f32(&ch->fir2_instance, &ch->rectified_value, &output2, 1);
    
    return output2;
}

/* -----------------------EMG--------------------------*/


float bpnn(float input[4])   //神经网络计算函数
{       
/* -----------------------PARAMETER--------------------------*/ 
    #define N 5
    float xmin[4] = {-119.7285,  -24.3589,   -1.3137,  -67.1579};
    float xrange_inv[4] = {237.7285,   49.9736,    3.3387,  132.7863};
    float ymin = -10.2439;
    float yrange = 20.5007;

    float iw_data[4*N] = {2.8634,-1.569907,-1.802670,4.093281,-6.011202,
     9.661440,-8.936080,-6.072055,2.559126,-1.347665,
     -1.599983,3.603834,-0.199129,0.637812,-0.710828,
     -0.661065,-2.463922,2.164462,0.287311,-2.333859};
    
    float lw_data[N] = {-4.4509,-0.109176,4.959380,1.928764,-0.746005};
    
    float b1[N] = {-1.6923,-0.960280,-1.494521,-0.455537,-2.116924};
    
    float b2 = 0.1417;
/* -----------------------PARAMETER--------------------------*/    
    float h_data[N];
    float o_data;
    arm_matrix_instance_f32 iw={N,4,iw_data};
    arm_matrix_instance_f32 lw={1,N,lw_data};
    arm_matrix_instance_f32 h={N,1,h_data};
    arm_matrix_instance_f32 output={1,1,&o_data};
    
    // x normalization
    int i = 0;
    for(i=0;i<4;i++)
    {
        input[i] = (input[i]-xmin[i]) * xrange_inv[i] * 2 - 1;
    }
    arm_matrix_instance_f32 inputM={4,1,input};
    
    arm_mat_mult_f32(&iw,&inputM,&h);  //DSP指令，矩阵相乘
    for(i=0;i<N;i++)
    {
        h_data[i] = 2/(1+exp(-2*(h_data[i]+b1[i])))-1;
    }
    arm_mat_mult_f32(&lw,&h,&output);
    o_data = (o_data + b2 + 1) * 0.5f * yrange + ymin;
//    o_data = o_data + b2;
    
    return o_data;
}

float bpnn_G(float input[2])   //神经网络计算函数
{       
/* --------------------------------------------------------------------PARAMETER------------------------------------------------------------*/
    float xmin[2] = {-1,-11.896000000000000};
    float xrange_inv[2] = {0.5, 0.0326};
    float ymin = -7.9060;
    float yrange = 16.8970;

    float iw_data[4] = {1.3324,0.028933,1.296005,0.108403};
    
    float lw_data[2] = {-0.59519,-0.775453};
    
    float b1[2] = {-0.91891,1.000259};
    
    float b2 = 0.0429;
/* --------------------------------------------------------------------PARAMETER------------------------------------------------------------*/    
    float h_data[2];
    float o_data;
    arm_matrix_instance_f32 iw={2,2,iw_data};
    arm_matrix_instance_f32 lw={1,2,lw_data};
    arm_matrix_instance_f32 h={2,1,h_data};
    arm_matrix_instance_f32 output={1,1,&o_data};
    
    // x normalization
    int i = 0;
    for(i=0;i<2;i++)
    {
        input[i] = (input[i]-xmin[i]) * xrange_inv[i] * 2 - 1;
    }
    arm_matrix_instance_f32 inputM={2,1,input};
    
    arm_mat_mult_f32(&iw,&inputM,&h);  //DSP指令，矩阵相乘
    for(i=0;i<2;i++)
    {
        h_data[i] = 2/(1+exp(-2*(h_data[i]+b1[i])))-1;
    }
    arm_mat_mult_f32(&lw,&h,&output);
    o_data = (o_data + b2 + 1) * 0.5f * yrange + ymin;
//    o_data = o_data + b2;
    
    return o_data;
}

/* ----------------------------------------------------指令函数---------------------------------------------------------------- */
//void set_mode(int cmd)
//{
//    ukey = cmd;
//}

///* usmart不能传float */
//void set_admittance(int K, int B, int M)
//{
//    K_admittance = K * 0.001f;
//    B_admittance = B * 0.001f;
//    M_inv_admittance = M * 1.0f;
//}
/* ----------------------------------------------------指令函数---------------------------------------------------------------- */


float kalman_filter(float y_last, float x, int num)
{
    static float P_vel = 100, P_acc = 100, P_cur = 100, P_force = 100, P_pos = 100, P_pcpos = 100, P_pck = 100;
    float Q, R;
    float *P;
    switch(num)
    {
        case(1): // vel
            Q = 0.1;
            R = 10;
            P = &P_vel;
            break;
        case(2): // acc
            Q = 0.1;
            R = 10;
            P = &P_acc;
            break;
        case(3): // cur
            Q = 0.1;
            R = 10;
            P = &P_cur;
            break;
        case(4): // tor
            Q = 0.1;
            R = 4;
            P = &P_force;
            break;
        case(5): // pos
            Q = 2;
            R = 1;
            P = &P_pos;
            break;
        case(6): // pcpos
            Q = 0.1;
            R = 2;
            P = &P_pcpos;
            break;
        case(7): // pck
            Q = 0.1;
            R = 1;
            P = &P_pck;
            break;
    }
    *P = *P + Q;
    float K = *P / (*P + R);
    float y = y_last + (x - y_last) * K;
    *P = (1 - K) * *P;
    return y;
}


/************************************
 *函数名：QuickSort
 *作用：快速排序算法
 *参数：
 *返回值：无
 ************************************/
void QuickSort(float *arr, int low, int high)
{
    if (low < high)
    {
        int i = low;
        int j = high;
        float k = arr[low];
        while (i < j)
        {
            while(i < j && arr[j] >= k)     // 从右向左找第一个小于k的数
            {
                j--;
            }
 
            if(i < j)
            {
                arr[i++] = arr[j];
            }
 
            while(i < j && arr[i] < k)      // 从左向右找第一个大于等于k的数
            {
                i++;
            }
 
            if(i < j)
            {
                arr[j--] = arr[i];
            }
        }
 
        arr[i] = k;
 
        // 递归调用
        QuickSort(arr, low, i - 1);     // 排序k左边
        QuickSort(arr, i + 1, high);    // 排序k右边
    }
}


void BubbleSort(float *arr, int size)  
{  
    int i, j; 
    float tmp;  
    for (i = 0; i < size - 1; i++) {  
        for (j = 0; j < size - i - 1; j++) {  
            if (arr[j] > arr[j+1]) {  
                tmp = arr[j];  
                arr[j] = arr[j+1];  
                arr[j+1] = tmp;  
            }  
        }  
    }  
} 

float Hex2Float(uint8_t *pMem1)
{
    uint8_t i;
    uint32_t number=0;
    uint8_t sign;
    uint8_t exponent;
    float mantissa;
    for(i=0;i<4;i++)
    {
        number = number<<8;
        number = number+ pMem1[i];
        
    }
    sign = (number & 0x80000000) ? -1 : 1;
    exponent = ((number >> 23) & 0xff) - 127;
    mantissa = 1 + ((float)(number & 0x7fffff) / 0x7fffff);
    return sign * mantissa * pow(2, exponent);
}



