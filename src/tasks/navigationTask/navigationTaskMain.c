#include "global.h"
#include "FreeRTOS.h"
#include <stdio.h>
#include "xparameters.h"
#include "netif/xadapter.h"
#include "platform_config.h"
#include "xil_printf.h"
#include "unistd.h"
#include <string.h>
#include "lwip/sockets.h"
#include "task.h"

#include <math.h>

#define UDP_ECHO_PORT    1234
#define RECV_BUF_SIZE 2048

#define PN_GAIN   2.0       /* N */
#define MISSILE_V 10.0      /* V_m (m/s) : 고정속도 가정 */
#define INV_MISSILE_V    (1.0 / MISSILE_V)

#define KP_OMEGA  0.5
#define KD_OMEGA  0.25

#define MAX_ALPHA_CMD    5.0

tDVector3 gForward;
static TickType_t sPrevTick;

static tDVector4 quatMultiply(const tDVector4 a, const tDVector4 b)
{
    tDVector4 r;
    r.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
    r.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
    r.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
    r.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w;
    return r;
}


static void normalizeQuat(tDVector4 *q)
{
    double n = sqrt(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z);
    q->w /= n;  q->x /= n;
    q->y /= n;  q->z /= n;
}

static void navigationRun()
{
	static double dt;
	static uint32_t  dt_ms;

	// dt 계산 (초 단위)
	TickType_t xCurrTick = xTaskGetTickCount();
	TickType_t tickDiff  = xCurrTick - sPrevTick;
	sPrevTick = xCurrTick;               // 갱신은 한 번만

	dt = (double)tickDiff * portTICK_PERIOD_MS * 1e-3;
	dt_ms= tickDiff * portTICK_PERIOD_MS;
	xil_printf("dt = %u ms\n", dt_ms);

    // IMU 데이터 읽기
    tImuData imu = gImuData;

	// 현재 자세 쿼터니언 복사
    tDVector4 q = gAttitude;

    // 각속도를 쿼터니언 형태로 변환 (OMEGA = [0, ωx, ωy, ωz])
	tDVector4 Omega = { 0.0, imu.gyro.x, imu.gyro.y, imu.gyro.z };

	// 쿼터니언 미분 계산: qDot = 0.5 * Ω⨂q
	tDVector4 qDot = quatMultiply(q, Omega);
	qDot.w *= 0.5;  qDot.x *= 0.5;
	qDot.y *= 0.5;  qDot.z *= 0.5;

	// 적분: q_new = q + qDot * dt
	q.w += qDot.w * dt;
	q.x += qDot.x * dt;
	q.y += qDot.y * dt;
	q.z += qDot.z * dt;

	// 정규화로 누적 오차 방지
	normalizeQuat(&q);

	// 전역 변수에 갱신
	gAttitude = q;

	normalizeQuat(&gAttitude);
	// 디버깅 용
	//gForward = rotateVectorByQuat(q, (tDVector3){1.0, 0.0, 0.0});
}

void navigationTaskMain( void *pvParameters )
{
	// TODO: move to init Task
	gAttitude.w = 1.0;
	sPrevTick = xTaskGetTickCount();
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
        navigationRun();
    }
}
