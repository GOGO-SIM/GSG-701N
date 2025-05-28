#ifndef TASKFUNC_H
#define TASKFUNC_H

#include "gsgTypes.h"   // tDVector3, tDVector4 µî Á¤ÀÇ
#include <math.h>

tDVector4 quatMultiply(const tDVector4 a, const tDVector4 b);

void normalizeQuat(tDVector4 *q);

tDVector4 quatConjugate(const tDVector4 q);

tDVector4 vecToQuat(const tDVector3 v);

tDVector3 rotateVectorByQuat(const tDVector4 q, const tDVector3 v);

double clamp(double v, double lim);

#endif
