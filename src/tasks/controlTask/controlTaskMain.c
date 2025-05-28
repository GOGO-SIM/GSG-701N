#include "global.h"
#include "gncUtil.h"

tDVector3 gAngAccelCmd;
static TickType_t now;
static double dt;
static TickType_t lastTick;
const static tDVector3 bodyX = (tDVector3){1.0, 0.0, 0.0};
static tDVector3 prev_err = {0.0, 0.0, 0.0};

static void controlRun()
{
	now = xTaskGetTickCount();
	dt = (now - lastTick) * portTICK_PERIOD_MS * 1e-3;
	lastTick = now;
	tDVector3 a_body = gAccCmd;
	/* Body 가속도 → 목표 각속도 (omega_cmd) */
	tDVector3 omega_cmd =
	{
		(bodyX.y * a_body.z - bodyX.z * a_body.y) * INV_MISSILE_V ,
		(bodyX.z * a_body.x - bodyX.x * a_body.z) * INV_MISSILE_V ,
		(bodyX.x * a_body.y - bodyX.y * a_body.x) * INV_MISSILE_V
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
	lastTick = xTaskGetTickCount();
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
        controlRun();
    }
}
