/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*				  All Rights Reserved.
*   Department: R&D SW
********************************************************************************
* File Name   : mcl_interpolation_if.h
* Author      : hg
* Version     : v0.01
* Date        : 2017/9/11
* Description : Motion Control Lib -- interpolathion.
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/9/11  hg          initial version
*=============================================================================*/

#ifndef _MCL_INTERPOLATION_IF_H
#define _MCL_INTERPOLATION_IF_H

/* cubic poly interpolation struct */
typedef struct _mcl_cubic_poly_intpol_st {
	unsigned char intpol_completed;                 //Interpolation complete flag, 1: undone, 0: done
	float intpol_ts;                                //Interpolation interval, equal to control cycle
	float intpol_time;                              //Interpolation time
	float total_intpol_time;                        //Total interpolation time
	float start;                                    //Start value
	float end;                                      //End value
	float coefficients[4];                          //Interpolation coefficient
	float next_value;                               //Next interpolation value
}mcl_cubic_poly_intpol_st;


/* line poly interpolation struct */
typedef struct _mcl_line_poly_intpol_st {
	unsigned char intpol_completed;                 //Interpolation complete flag, 1: undone, 0: done
	float intpol_ts;                                //Interpolation interval, equal to control cycle
	float intpol_time;                              //Interpolation time
	float total_intpol_time;                        //Total interpolation time
	float start;                                    //Start value
	float end;                                      //End value
	float next_value;                               //Next interpolation value
}mcl_line_poly_intpol_st;

/* parabola poly interpolation struct */
typedef struct _mcl_parabola_poly_intpol_st {
	unsigned char intpol_completed;                 //Interpolation complete flag, 1: undone, 0: done
	float intpol_ts;                                //Interpolation interval, equal to control cycle
	float intpol_time;                              //Interpolation time
	float intpol_time2;                              //Interpolation time
	float total_time1;                              //Acceleration time
	float total_time2;                              //deceleration time
	float peak_time;
	float peak;                                     //Peak value
	float acc_coefficients[4];                      //Acceleration coefficient
	float dec_coefficients[4];                      //Deceleration coefficient
	float next_value;                               //Next interpolation value
}mcl_parabola_poly_intpol_st;

/* cubic polynomial interpolation */
extern void mcl_cubic_poly_intpol_st_init(mcl_cubic_poly_intpol_st *cubic_poly_intpol, float intpol_ts);
extern unsigned char mcl_cubic_poly_intpol(mcl_cubic_poly_intpol_st *cubic_poly_intpol, float init_value, float final_value, float time);
extern unsigned char mcl_ppp_cubic_poly_intpol(mcl_cubic_poly_intpol_st *cubic_poly_intpol, 
	                                           float init_value, float final_value, float time, float init_velocity, float final_velocity);
extern float mcl_cubic_poly_intpol_updata(mcl_cubic_poly_intpol_st *cubic_poly_intpol);

/* linear interpolation */
extern void mcl_line_poly_intpol_st_init(mcl_line_poly_intpol_st *line_poly_intpol, float intpol_ts);
extern unsigned char mcl_line_poly_intpol(mcl_line_poly_intpol_st *line_poly_intpol, float init_value, float final_value, float time);
extern float mcl_line_poly_intpol_updata(mcl_line_poly_intpol_st *line_poly_intpol);

/* parabola interpolation */
extern void mcl_parabola_poly_intpol_st_init(mcl_parabola_poly_intpol_st *parabola_poly_intpol, float intpol_ts);
extern unsigned char mcl_parabola_poly_intpol(mcl_parabola_poly_intpol_st *parabola_poly_intpol, float peak, float time1, float time2, float peak_time);
extern unsigned char mcl_parabola_poly_intpol_1(mcl_parabola_poly_intpol_st *parabola_poly_intpol, float start, float peak, float last, float time1, float time2, float peak_time);
extern unsigned char mcl_parabola_poly_intpol_2(mcl_parabola_poly_intpol_st *parabola_poly_intpol, float peak, float time1);
extern unsigned char mcl_parabola_poly_intpol_3(mcl_parabola_poly_intpol_st *parabola_poly_intpol, float peak, float time2);
extern unsigned char mcl_parabola_poly_intpol_4(mcl_parabola_poly_intpol_st *parabola_poly_intpol, float init_value, float peak, float time1);

extern float mcl_parabola_poly_intpol_updata(mcl_parabola_poly_intpol_st *parabola_poly_intpol);
extern float mcl_parabola_poly_intpol_updata_2(mcl_parabola_poly_intpol_st *parabola_poly_intpol);
extern float mcl_parabola_poly_intpol_updata_3(mcl_parabola_poly_intpol_st *parabola_poly_intpol);



#endif


/****************************** END OF FILE ***********************************/

