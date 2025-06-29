#pragma once
#ifndef TASKFUNC_H
#define TASKFUNC_H
#include "global.h"
#include <math.h>

#define UDP_ECHO_PORT    1234
#define RECV_BUF_SIZE 2048

#define MISSILE_V 500.0      /* V_m (m/s) : 고정속도 가정 */
#define INV_MISSILE_V    (1.0 / MISSILE_V)

#define PN_GAIN   2.98 // 3.78 (0.6댐핑 , 1.3토크 , 0.1제어스피드 )   // 3.48
#define KP_OMEGA  1.45// 2.88            // 1.20
#define KD_OMEGA  0.22 // 0.48             // 0.65

#define MAX_ALPHA_CMD    10.0

tDVector4 quatMultiply(const tDVector4 a, const tDVector4 b);

void normalizeQuat(tDVector4 *q);

tDVector4 quatConjugate(const tDVector4 q);

tDVector4 vecToQuat(const tDVector3 v);

tDVector3 rotateVectorByQuat(const tDVector4 q, const tDVector3 v);

double clamp(double v, double lim);

#endif
