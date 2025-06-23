#include "global.h"
#include "gncUtil.h"

// #define GUIDANCE_PERIOD_S (0.040) // 잘못된 값이므로 삭제 또는 주석 처리

// --- 수정된 부분 시작 ---
// 이전 Tick 시간을 저장하기 위한 static 변수 (기존 변수 재활용)
static TickType_t lastTick = 0;
// --- 수정된 부분 종료 ---
static tDVector3  prevLOS;
static BaseType_t firstRun;
static double prevDist = 0.0;

static void guidanceRun()
{
	// --- 수정된 부분 시작 ---
	// 1. 현재 Tick을 가져와 실제 경과 시간 dt (초 단위)를 계산합니다.
	TickType_t now = xTaskGetTickCount();
	double dt;

	if (firstRun == pdTRUE) {
		// 태스크가 시작되고 처음 호출될 때, dt는 기본값(20ms)을 사용합니다.
		dt = 0.020;
	} else {
		// (현재 Tick - 이전 Tick) * Tick당 시간(ms) / 1000.0 => 초(s)로 변환
		dt = (double)(now - lastTick) * portTICK_PERIOD_MS / 1000.0;
	}
	// 다음 계산을 위해 현재 Tick 값을 저장합니다.
	lastTick = now;

	// 2. dt가 비정상적인 값(0 또는 너무 큰 값)일 경우를 대비한 안전장치를 추가합니다.
    if (dt <= 0 || dt > 0.040) { // 2주기(40ms) 이상 벌어지면 이상상황으로 간주
        dt = 0.020;
    }
    // --- 수정된 부분 종료 ---

	/* (1) Δt 계산 (초) */
	// const double dt = GUIDANCE_PERIOD_S; // 기존의 고정값 dt 라인을 삭제합니다.

	tDVector3 currLOS = gSeekerData.los;
	double currDist = gSeekerData.distance;

	/* 폐쇠속도 Vc 계산 */
	double V_C = 0.0;
	if(!firstRun){
		V_C = -(currDist - prevDist)/dt;
	}else {
		V_C = MISSILE_V;
	}

	prevDist = currDist;

	/* (3) LOS 변화율 Ldot 계산 */
	tDVector3 Ldot = {0};
	if (!firstRun)
	{
		Ldot.x = (currLOS.x - prevLOS.x) / dt;
		Ldot.y = (currLOS.y - prevLOS.y) / dt;
		Ldot.z = (currLOS.z - prevLOS.z) / dt;
	}
	firstRun = pdFALSE; // firstRun 플래그는 기존 로직 그대로 활용
	prevLOS  = currLOS;

	/* (4) PN 가속도 (관성계) a_cmd = N·V·( (L × Ldot) × L ) */
	tDVector3 omega =
	{
		currLOS.y*Ldot.z - currLOS.z*Ldot.y,
		currLOS.z*Ldot.x - currLOS.x*Ldot.z,
		currLOS.x*Ldot.y - currLOS.y*Ldot.x
	};

	tDVector3 a_cmd =
	{   /* (ω × L) */
		omega.y*currLOS.z - omega.z*currLOS.y,
		omega.z*currLOS.x - omega.x*currLOS.z,
		omega.x*currLOS.y - omega.y*currLOS.x
	};

	a_cmd.x *= PN_GAIN * V_C;
	a_cmd.y *= PN_GAIN * V_C;
	a_cmd.z *= PN_GAIN * V_C;

//	const double GRAVITY_ACCELERATION = 9.81;
//	a_cmd.z -= GRAVITY_ACCELERATION;  // <--- 이 부분이 주석 처리되어 있습니다!

	gAccCmd = a_cmd;
}

void guidanceTaskMain(void *pvParameters)
{
	// lastTick은 0으로 초기화하고, firstRun 플래그를 사용하여 첫 실행을 처리합니다.
	lastTick  = 0;
	firstRun  = pdTRUE;
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        guidanceRun();
        xTaskNotifyGive(xControlTaskHandle);
    }
}
