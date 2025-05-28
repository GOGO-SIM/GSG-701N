#include "taskFunc.h"

//두 쿼터니언 a, b의 해밀토니안 곱
tDVector4 quatMultiply(const tDVector4 a, const tDVector4 b)
{
    tDVector4 r;
    r.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
    r.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
    r.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
    r.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w;
    return r;
}

//쿼터니언 q를 단위(quaternionic unit)로 정규화
void normalizeQuat(tDVector4 *q)
{
    double n = sqrt(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z);
    q->w /= n;  q->x /= n;
    q->y /= n;  q->z /= n;
}

tDVector4 quatConjugate(const tDVector4 q) {
    return (tDVector4){ q.w, -q.x, -q.y, -q.z };
}
tDVector4 vecToQuat(const tDVector3 v) {
    return (tDVector4){ 0.0, v.x, v.y, v.z };
}
tDVector3 rotateVectorByQuat(const tDVector4 q, const tDVector3 v) {
    tDVector4 vq   = vecToQuat(v);
    tDVector4 tmp  = quatMultiply(q, vq);
    tDVector4 qc   = quatConjugate(q);
    tDVector4 resQ = quatMultiply(tmp, qc);
    return (tDVector3){ resQ.x, resQ.y, resQ.z };
}

double clamp(double v, double lim)
{
    return (v >  lim) ?  lim :
           (v < -lim) ? -lim : v;
}
