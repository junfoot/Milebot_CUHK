#ifndef __UTILS_H
#define __UTILS_H

#include "stdint.h"

void set_mode(int cmd);
float kalman_filter(float y_last, float x, int num);
void set_admittance(int K, int B, int M);
void QuickSort(float *arr, int low, int high);
void BubbleSort(float *arr, int size);
float bpnn(float input[4]);
float bpnn_G(float input[2]);
float Hex2Float(uint8_t *pMem1);

#endif
