#include "global.h"
#include "gncUtil.h"

// #define CONTROL_PERIOD_S (0.020) // 더 이상 사용하지 않으므로 삭제 또는 주석 처리

tDVector3 gAngAccelCmd;

// --- 수정된 부분 시작 ---
// 이전 Tick 시간을 저장하기 위한 static 변수
static TickType_t lastTick = 0;
// --- 수정된 부분 종료 ---

const static tDVector3 bodyX = (tDVector3){1.0, 0.0, 0.0};
static tDVector3 prev_err = {0.0, 0.0, 0.0};

static void proximityFuze()
{
	if (gSeekerData.distance < 100)
	{
		xTaskNotifyGive(xExplodeTaskHandle);
	}
}

static void controlRun()
{
	// --- 수정된 부분 시작 ---
	// 1. 현재 Tick을 가져와 실제 경과 시간 dt (초 단위)를 계산합니다.
	TickType_t now = xTaskGetTickCount();
	double dt;

  // 근접신관 동작
  proximityFuze();
 
	if (lastTick == 0) {
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
    // const double dt = CONTROL_PERIOD_S; // 기존의 고정값 dt 라인을 삭제합니다.
    // --- 수정된 부분 종료 ---


	/* 관성계 → Body 기준 가속도로 변환 */
	tDVector3 Acc_body = rotateVectorByQuat(quatConjugate(gAttitude), gAccCmd);

	tDVector3 omega_cmd =
	{
		(bodyX.y * Acc_body.z - bodyX.z * Acc_body.y) * INV_MISSILE_V ,
		(bodyX.z * Acc_body.x - bodyX.x * Acc_body.z) * INV_MISSILE_V ,
		(bodyX.x * Acc_body.y - bodyX.y * Acc_body.x) * INV_MISSILE_V
	};
	/* 측정 각속도 (Unity IMU) */
	const tDVector3 omega_act = gImuData.gyro;

	/* PD 제어 → 각가속도 명령 */
	tDVector3 err =
	{
		omega_cmd.x - omega_act.x,
		omega_cmd.y - omega_act.y,
		omega_cmd.z - omega_act.z
	};

	tDVector3 derr =
	{
		(err.x - prev_err.x) / dt,
		(err.y - prev_err.y) / dt,
		(err.z - prev_err.z) / dt
	};
	prev_err = err;

	tDVector3 alpha_cmd =
	{
		KP_OMEGA * err.x + KD_OMEGA * derr.x,
		KP_OMEGA * err.y + KD_OMEGA * derr.y,
		KP_OMEGA * err.z + KD_OMEGA * derr.z
	};

	/* 6) 제한 */
	alpha_cmd.x = clamp(alpha_cmd.x, MAX_ALPHA_CMD);
	alpha_cmd.y = clamp(alpha_cmd.y, MAX_ALPHA_CMD);
	alpha_cmd.z = clamp(alpha_cmd.z, MAX_ALPHA_CMD);

	/* 7) 전역 출력 */
	gAngAccelCmd = alpha_cmd;
}

void controlTaskMain(void *pvParameters)
{
	// lastTick을 0으로 초기화하여 controlRun()의 첫 실행을 감지하도록 합니다.
	lastTick = 0;
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        controlRun();
    }
}
