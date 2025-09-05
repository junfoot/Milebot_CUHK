#ifndef __gait_H__
#define __gait_H__
#include "stdint.h"



//extern float hip_flexion[450], knee_flexion[450], knee_flexion_RE40[450];
#define GAIT_N 500
extern double hip[500], knee[500];
extern double r_hip[500], r_knee[500];
extern double L_hip[500], L_knee[500];

#define N_test 616
extern float hip_test[N_test];
extern float knee_test[N_test];

#define ArrayMaxSize 10
typedef struct {
	int * ArrayName[ArrayMaxSize];
	uint16_t RunNumber;
}trajectory;


void firstPos(uint32_t * pos);

#endif
