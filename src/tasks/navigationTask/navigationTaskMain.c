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
#include "gncUtil.h"
#include "xtime_l.h"

tDVector3 gForward;
static TickType_t sPrevTick;

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
	// xil_printf("dt = %u ms\r\n", dt_ms);

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
	// printf("quternion = [%.6f, %.6f, %.6f, %.6f]\r\n", gAttitude.w, gAttitude.x, gAttitude.y, gAttitude.z);
	gForward = rotateVectorByQuat(q, (tDVector3){1.0, 0.0, 0.0});
	// printf("forward = [%.6f, %.6f, %.6f]\r\n",
	// 				 gForward.x, gForward.y, gForward.z);

}

void navigationTaskMain( void *pvParameters )
{
	// TODO: move to init Task
	gAttitude.w = 1.0;
	sPrevTick = xTaskGetTickCount();
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        //xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
        XTime start, end;
        XTime_GetTime(&start);  // 시작 시간 기록
        navigationRun();
        XTime_GetTime(&end);    // 종료 시간 기록
        // 시간 차이 계산 (us 단위)
        uint64_t elapsed_ticks = end - start;
        uint64_t elapsed_us = elapsed_ticks / (COUNTS_PER_SECOND / 1000000);

        xil_printf("Navigation: %llu\r\n", elapsed_ticks);

    }
}
