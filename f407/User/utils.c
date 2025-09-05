#include "arm_math.h"
#include "utils.h"
#include <stdlib.h>

//extern float K_admittance;
//extern float B_admittance;
//extern float M_inv_admittance;
//extern float x_desire[4];

//extern int ukey;


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



