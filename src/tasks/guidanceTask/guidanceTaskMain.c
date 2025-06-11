#include "global.h"
#include "gncUtil.h"

static TickType_t lastTick;
static tDVector3  prevLOS;         /* 첫 회차는 각속도 0 */
static BaseType_t firstRun;

static void guidanceRun()
{
	/* (1) Δt 계산 (초) */
	TickType_t now = xTaskGetTickCount();
	double dt = (now - lastTick) * portTICK_PERIOD_MS * 1e-3;
	lastTick  = now;
	if (dt < 1e-6)
		dt = 1e-6;
	tDVector3 currLOS = gSeekerData.los;
	/* (3) LOS 변화율 Ldot 계산 */
	tDVector3 Ldot = {0};
	if (!firstRun)
	{
		Ldot.x = (currLOS.x - prevLOS.x) / dt;
		Ldot.y = (currLOS.y - prevLOS.y) / dt;
		Ldot.z = (currLOS.z - prevLOS.z) / dt;
	}
	firstRun = pdFALSE;
	prevLOS  = currLOS;

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

	a_cmd.x *= PN_GAIN * MISSILE_V;    // 스칼라 곱
	a_cmd.y *= PN_GAIN * MISSILE_V;
	a_cmd.z *= PN_GAIN * MISSILE_V;

	/* (5) 관성계 → Body 기준 가속도로 변환 */
	tDVector3 a_body = rotateVectorByQuat(quatConjugate(gAttitude), a_cmd);

	/* (6) (미사일Body 기준) 횡 가속도 명령 */
	gAccCmd = a_cmd;
}

void guidanceTaskMain(void *pvParameters)
{
	lastTick  = xTaskGetTickCount();
	firstRun  = pdTRUE;
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
        guidanceRun();
    }
}
