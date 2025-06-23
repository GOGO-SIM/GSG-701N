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

// #define NAVIGATION_PERIOD_S (0.020)  // 더 이상 사용하지 않으므로 삭제 또는 주석 처리

// --- 수정된 부분 시작 ---
// 이전 Tick 시간을 저장하기 위한 static 변수
static TickType_t sPrevTick = 0;
// --- 수정된 부분 종료 ---

static void navigationRun()
{
    // --- 수정된 부분 시작 ---
    // 1. 현재 Tick을 가져와 실제 경과 시간 dt (초 단위)를 계산합니다.
    TickType_t currentTick = xTaskGetTickCount();
    double dt;

    if (sPrevTick == 0) {
        // 태스크가 시작되고 처음 호출될 때, dt는 기본값(20ms)을 사용합니다.
        dt = 0.020;
    } else {
        // (현재 Tick - 이전 Tick) * Tick당 시간(ms) / 1000.0 => 초(s)로 변환
        dt = (double)(currentTick - sPrevTick) * portTICK_PERIOD_MS / 1000.0;
    }
    // 다음 계산을 위해 현재 Tick 값을 저장합니다.
    sPrevTick = currentTick;

    // 2. dt가 비정상적인 값(0 또는 너무 큰 값)일 경우를 대비한 안전장치를 추가합니다.
    if (dt <= 0 || dt > 0.040) { // 2주기(40ms) 이상 벌어지면 이상상황으로 간주
        dt = 0.020;
    }
    // const double dt = NAVIGATION_PERIOD_S; // 기존의 고정값 dt 라인을 삭제합니다.
    // --- 수정된 부분 종료 ---

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
//   printf("forward = [%.6f, %.6f, %.6f]\r\n", gForward.x, gForward.y, gForward.z);
}

void navigationTaskMain( void *pvParameters )
{
	// TODO: move to init Task
	gAttitude.w = 1.0;
	gAttitude.x = 0.0;
	gAttitude.y = 0.0;
	gAttitude.z = 0.0;

    // --- 수정된 부분 시작 ---
    // sPrevTick을 0으로 초기화하여 navigationRun()의 첫 실행을 감지할 수 있도록 합니다.
	sPrevTick = 0;
    // --- 수정된 부분 종료 ---

    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

//        xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
        navigationRun();
        if (gSeekerUpdated) {          // ★ 새 Seeker 데이터가 있으면
            gSeekerUpdated = pdFALSE;  //   소비 후 클리어
            xTaskNotifyGive(xGuidanceTaskHandle);  // 항법→유도→제어
        } else {
            xTaskNotifyGive(xControlTaskHandle);   // 항법→제어
        }
    }
}
