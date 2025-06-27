#include "global.h"
#include "gncUtil.h"

static TickType_t lastTick = 0;
static tDVector3  prevLOS;
static BaseType_t firstRun;
static double prevDist = 0.0;

static void guidanceRun()
{
	TickType_t now = xTaskGetTickCount();
	double dt;

	if (firstRun == pdTRUE) {
		dt = 0.040;
	} else {
		dt = (double)(now - lastTick) * portTICK_PERIOD_MS / 1000.0;
	}
	lastTick = now;

    if (dt <= 0 || dt > 0.040) {
        dt = 0.020;
    }

	tDVector3 currLOS = normalize(gSeekerData.los);
	double currDist = gSeekerData.distance;

	/* 폐쇠속도 V_C 계산 */
	double V_C = 0.0;
	if(!firstRun){
		V_C = -(currDist - prevDist)/dt;
	}else {
		V_C = MISSILE_V;
	}

	prevDist = currDist;

	/* 시선 벡터 변화율 LDOT 계산 */
	tDVector3 Ldot = {0};
	if (!firstRun)
	{
		Ldot.x = (currLOS.x - prevLOS.x) / dt;
		Ldot.y = (currLOS.y - prevLOS.y) / dt;
		Ldot.z = (currLOS.z - prevLOS.z) / dt;
	}
	firstRun = pdFALSE;
	prevLOS  = currLOS;

	/* 횡가속도 계산*/
	tDVector3 omega =
	{
		currLOS.y*Ldot.z - currLOS.z*Ldot.y,
		currLOS.z*Ldot.x - currLOS.x*Ldot.z,
		currLOS.x*Ldot.y - currLOS.y*Ldot.x
	};

	tDVector3 a_cmd =
	{
		omega.y*currLOS.z - omega.z*currLOS.y,
		omega.z*currLOS.x - omega.x*currLOS.z,
		omega.x*currLOS.y - omega.y*currLOS.x
	};

	a_cmd.x *= PN_GAIN * V_C;
	a_cmd.y *= PN_GAIN * V_C;
	a_cmd.z *= PN_GAIN * V_C;

	gAccCmd = a_cmd;
	gLdot = Ldot; // 시선벡터 변화율 (텔레메트리용)
}

void guidanceTaskMain(void *pvParameters)
{
	lastTick  = 0;
	firstRun  = pdTRUE;
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        guidanceRun();
        xTaskNotifyGive(xControlTaskHandle);
    }
}
