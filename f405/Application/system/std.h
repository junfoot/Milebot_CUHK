/*******************************************************************************
*   Copyright (C) Shenzhen MileBot Robotics Tech Co., Ltd.
*                  All Rights Reserved.
*   Department : R&D SW
********************************************************************************
* File Name   : std.h
* Author      : wb
* Version     : v0.01
* Date        : 2017/6/1
* Description : common macros defines
*******************************************************************************/
/*-History----------------------------------------------------------------------
* Version   Date      Name        Changes and comments
* v0.01     2017/6/1  wb    initial version
*=============================================================================*/
#ifndef _STD_H_
#define _STD_H_

#include <inttypes.h>
#include <math.h>

/* some convenience macros to print debug/config messages at compile time */
#include "msg_pragmas.h"

/* stringify a define, e.g. one that was not quoted */
#define _STRINGIFY(s) #s
#define STRINGIFY(s) _STRINGIFY(s)

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

/* Boolean values */
typedef uint8_t bool_t;

/* Unit (void) values */
typedef uint8_t unit_t;

#ifndef bool
#define bool uint8_t
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_4
#define M_PI_4 (M_PI/4)
#endif

#ifndef M_PI_2
#define M_PI_2 (M_PI/2)
#endif


#ifndef bit_is_set
#define bit_is_set(x, b) ((x >> b) & 0x1)
#endif

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

#define SetBit(a, n) a |= (1 << n)
#define ClearBit(a, n) a &= ~(1 << n)

#define NormRadAngle(x) { \
    while (x > M_PI) x -= 2 * M_PI; \
    while (x < -M_PI) x += 2 * M_PI; \
  }
#define DegOfRad(x) ((x) * (180. / M_PI))
#define DeciDegOfRad(x) ((x) * (1800./ M_PI))
#define RadOfDeg(x) ((x) * (M_PI/180.))

#define Min(x,y) (x < y ? x : y)
#define Max(x,y) (x > y ? x : y)

#ifndef ABS
#define ABS(val) ((val) < 0 ? -(val) : (val))
#endif

#define BoundUpper(_x, _max) { if (_x > (_max)) _x = (_max);}


#define Bound(_x, _min, _max) { if (_x > (_max)) _x = (_max); else if (_x < (_min)) _x = (_min); }
#define BoundInverted(_x, _min, _max) {           \
    if ((_x < (_min)) && (_x > (_max))) {         \
      if (abs(_x - (_min)) < abs(_x - (_max)))    \
        _x = (_min);                              \
      else                                        \
        _x = (_max);                              \
    }                                             \
  }
#define BoundWrapped(_x, _min, _max) {            \
    if ((_max) > (_min))                          \
      Bound(_x, _min, _max)                       \
    else                                          \
      BoundInverted(_x, _min, _max)               \
  }
#define BoundAbs(_x, _max) Bound(_x, -(_max), (_max))
#define Chop(_x, _min, _max) ( (_x) < (_min) ? (_min) : (_x) > (_max) ? (_max) : (_x) )
#define ChopAbs(x, max) Chop(x, -(max), (max))

#define DeadBand(_x, _v) {                        \
    if (_x > (_v))                              \
      _x = _x -(_v);                            \
    else if  (_x < -(_v))                       \
      _x = _x +(_v);                            \
    else                                        \
      _x = 0;                                   \
  }

#define Blend(a, b, rho) (((rho)*(a))+(1-(rho))*(b))

#define RunOnceEvery(_prescaler, _code) {        \
    static uint16_t prescaler = 0;            \
    prescaler++;                    \
    if (prescaler >= _prescaler) {            \
      prescaler = 0;                    \
      _code;                        \
    }                            \
  }

#define RunXTimesEvery(_jumpstart, _prescaler, _interval, _xtimes, _code) {        \
  static uint16_t prescaler = _jumpstart;            \
  static uint16_t xtimes = 0;                   \
  prescaler++;                    \
  if (prescaler >= _prescaler + _interval*xtimes && xtimes < _xtimes) {            \
    _code;                        \
    xtimes++;                        \
    }                            \
  if (xtimes >= _xtimes) {                \
    xtimes = 0;                    \
    prescaler = 0;                    \
    }                            \
}


#define PeriodicPrescaleBy5( _code_0, _code_1, _code_2, _code_3, _code_4) { \
    static uint8_t _50hz = 0;                        \
    _50hz++;                                \
    if (_50hz >= 5) _50hz = 0;                        \
    switch (_50hz) {                            \
    case 0:                                \
      _code_0;                                \
      break;                                \
    case 1:                                \
      _code_1;                                \
      break;                                \
    case 2:                                \
      _code_2;                                \
      break;                                \
    case 3:                                \
      _code_3;                                \
      break;                                \
    case 4:                                \
      _code_4;                                \
      break;                                \
    }                                    \
  }

#define PeriodicPrescaleBy10( _code_0, _code_1, _code_2, _code_3, _code_4, _code_5, _code_6, _code_7, _code_8, _code_9) { \
    static uint8_t _cnt = 0;                        \
    _cnt++;                                \
    if (_cnt >= 10) _cnt = 0;                        \
    switch (_cnt) {                            \
    case 0:                                \
      _code_0;                                \
      break;                                \
    case 1:                                \
      _code_1;                                \
      break;                                \
    case 2:                                \
      _code_2;                                \
      break;                                \
    case 3:                                \
      _code_3;                                \
      break;                                \
    case 4:                                \
      _code_4;                                \
      break;                                \
    case 5:                                \
      _code_5;                                \
      break;                                \
    case 6:                                \
      _code_6;                                \
      break;                                \
    case 7:                                \
      _code_7;                                \
      break;                                \
    case 8:                                \
      _code_8;                                \
      break;                                \
    case 9:                                \
    default:                                \
      _code_9;                                \
      break;                                \
    }                                    \
  }

static inline bool_t str_equal(const char * a, const char * b) {
  int i = 0;
  while (!(a[i] == 0 && b[i] == 0)) {
    if (a[i] != b[i]) return FALSE;
    i++;
  }
  return TRUE;
}
	#define BIG_ENDIAN_GET_BYTE4(buffer,startByte) ( (buffer[startByte]<<24)+(buffer[startByte+1]<<16)+(buffer[startByte+2]<<8)+buffer[startByte+3])
	#define BIG_ENDIAN_GET_BYTE3(buffer,startByte) ( (buffer[startByte]<<16)+(buffer[startByte+1]<<8)+(buffer[startByte+2]))
	#define BIG_ENDIAN_GET_BYTE2(buffer,startByte) ( (buffer[startByte]<<8)+buffer[startByte+1])
	#define BIG_ENDIAN_GET_BYTE1(buffer,startByte) (  buffer[startByte] )

	#define LITTLE_ENDIAN_GET_BYTE4(buffer,startByte) (buffer[startByte]+(buffer[startByte+1]<<8)+(buffer[startByte+2]<<16)+(buffer[startByte+3]<<24))
	#define LITTLE_ENDIAN_GET_BYTE3(buffer,startByte) (buffer[startByte]+(buffer[startByte+1]<<8)+(buffer[startByte+2]<<16))
	#define LITTLE_ENDIAN_GET_BYTE2(buffer,startByte) (buffer[startByte]+(buffer[startByte+1]<<8))
	#define LITTLE_ENDIAN_GET_BYTE1(buffer,startByte) (buffer[startByte] )


#endif /* _STD_H_ */
