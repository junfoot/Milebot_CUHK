#ifndef __UTILS_H
#define __UTILS_H

#include "stdint.h"
#include "arm_math.h"

/* -----------------------EMG--------------------------*/
#define NUM_CHANNELS 4
#define SAMPLE_RATE_HZ 1000

// ÂË²¨Æ÷²ÎÊý
#define NUM_TAPS_1 41
#define NUM_TAPS_2 41

void init_all_channels(void);
float32_t full_wave_rectify(float32_t value);
float32_t process_channel_sample(int channel_idx, float32_t input_sample);


/* -----------------------EMG--------------------------*/

void set_mode(int cmd);
float kalman_filter(float y_last, float x, int num);
void set_admittance(int K, int B, int M);
void QuickSort(float *arr, int low, int high);
void BubbleSort(float *arr, int size);
float bpnn(float input[4]);
float bpnn_G(float input[2]);
float Hex2Float(uint8_t *pMem1);

#endif
