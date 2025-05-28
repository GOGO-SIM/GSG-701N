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

#include "global.h"
#include <math.h>
#include "taskFunc.h"

extern TaskHandle_t hMain;
extern TaskHandle_t hSched;
extern TaskHandle_t hUDP;
extern TaskHandle_t hNav;
extern TaskHandle_t hGuid;
extern TaskHandle_t hCtrl;
extern TaskHandle_t hTel;

#define UDP_ECHO_PORT    1234
#define RECV_BUF_SIZE 2048

#define PN_GAIN   2.0       /* N */
#define MISSILE_V 10.0      /* V_m (m/s) : 고정속도 가정 */
#define INV_MISSILE_V    (1.0 / MISSILE_V)

#define KP_OMEGA  0.5
#define KD_OMEGA  0.25

#define MAX_ALPHA_CMD    5.0

static char buf[RECV_BUF_SIZE];
static char bufSend[RECV_BUF_SIZE];
static char msg[64];
static char dbg[96];
static char dbc[256];

void TaskScheduler(void *pvParameters)
{
    const TickType_t slot = pdMS_TO_TICKS(20);  // 50 ms slot per stage
    TickType_t       last;

    xil_printf("TaskScheduler START AND Delay!\n");
    vTaskDelay(pdMS_TO_TICKS(10000)); // 10초 대기

    for (;;)
    {
        /* Record cycle start */
//    	xil_printf("TaskScheduler RUNING!\n");
        last = xTaskGetTickCount();
        TickType_t start = last;

        /* UDP slot */
        vTaskResume(hUDP);
        vTaskDelayUntil(&last, slot);
        vTaskSuspend(hUDP);

        /* Navigation slot */
        vTaskResume(hNav);
        vTaskDelayUntil(&last, slot);
        vTaskSuspend(hNav);

        /* Guidance slot */
        vTaskResume(hGuid);
        vTaskDelayUntil(&last, slot);
        vTaskSuspend(hGuid);

        /* Control slot */
        vTaskResume(hCtrl);
        vTaskDelayUntil(&last, slot);
        vTaskSuspend(hCtrl);

        vTaskResume(hTel);
		vTaskDelayUntil(&last, slot);
		vTaskSuspend(hTel);

        // 주기
        TickType_t end = xTaskGetTickCount();
        end = end - start;
        xil_printf("TaskScheduler: cycle time = %lu ms\n", end * portTICK_PERIOD_MS);

    }
}

/*=== Pipeline Stage Tasks ===*/
void TaskUDP(void *pvParameters)
{
    xil_printf("TaskUDP START AND SUSPEND!\n");
    vTaskSuspend(NULL);

    struct sockaddr_in remote;
    socklen_t addr_len = sizeof(remote);
    int len;

    xil_printf(">> UDP echo server started @ port %d\r\n", UDP_ECHO_PORT);

    // 1) 소켓 생성 및 저장
    udpSock = lwip_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSock < 0) {
        xil_printf("UDP socket creation failed\r\n");
        vTaskDelete(NULL);
        return;
    }

    // 2) 바인드
    struct sockaddr_in addr = {0};
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(UDP_ECHO_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (lwip_bind(udpSock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        xil_printf("UDP bind failed\r\n");
        lwip_close(udpSock);
        vTaskDelete(NULL);
        return;
    }

    // 3) 논블로킹 모드
    {
        int on = 1;
        lwip_ioctl(udpSock, FIONBIO, &on);
    }
    xil_printf("TaskUDP init COMPLETE!\n");

    for (;;)
    {
        vTaskSuspend(NULL);
        xil_printf("[UDP Task]\n");

        // 큐에 남은 모든 패킷을 처리
        while (1) {
            len = lwip_recvfrom(udpSock,
                                buf, RECV_BUF_SIZE, 0,
                                (struct sockaddr *)&remote,
                                &addr_len);
            if (len <= 0) break;  // EWOULDBLOCK 또는 에러

            // 마지막 송신자 주소 저장
            pcAddr    = remote;
            pcAddrLen = addr_len;

            buf[len] = '\0';
            double a, b, c, d;
            int cnt = sscanf(buf, "%lf %lf %lf %lf", &a, &b, &c, &d);

            if (cnt == 3) {
                // IMU 데이터
                gImuData.gyro.x = a;
                gImuData.gyro.y = b;
                gImuData.gyro.z = c;
                snprintf(msg, sizeof(msg),
                         "IMU = [%.6f, %.6f, %.6f]\n",
                         a, b, c);
            }
            else if (cnt == 4) {
                // LOS 데이터
                gSeekerData.los.x    = a;
                gSeekerData.los.y    = b;
                gSeekerData.los.z    = c;
                gSeekerData.distance = d;
                snprintf(msg, sizeof(msg),
                         "LOS = [%.6f, %.6f, %.6f], dist=%.6f\n",
                         a, b, c, d);
            }
            else {
                xil_printf("Parse error: %s\n", buf);
                continue;
            }

            xil_printf("%s", msg);
        }

        xil_printf("\n");
    }

}

void TaskNav(void *pvParameters)
{
	xil_printf("TaskNAV START AND SUSPEND!\n");
	vTaskSuspend(NULL);

	//초기 자세
	gAttitude.w = 0.70710678;
	gAttitude.x = 0.0;
	gAttitude.y = 0.70710678;
	gAttitude.z = 0.0;

	//바디프레임 기준 미사일 머리 방향
//	gForward.x = 1.0;
//	gForward.y = 0.0;
//	gForward.z = 0.0;

	// 초기 틱 값 저장
	TickType_t xPrevTick = xTaskGetTickCount();

	xil_printf("TaskNAV init COMPLETE!\n");

    for (;;)
    {
        vTaskSuspend(NULL);
        xil_printf("[NAV Task]\n");

        // dt 계산 (초 단위)
        TickType_t xCurrTick = xTaskGetTickCount();
        TickType_t tickDiff  = xCurrTick - xPrevTick;
        xPrevTick = xCurrTick;               // 갱신은 한 번만

        double    dt   = (double)tickDiff * portTICK_PERIOD_MS * 1e-3;

        uint32_t  dt_ms= tickDiff * portTICK_PERIOD_MS;
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

		//UART 출력 (쿼터니언)
		snprintf(msg, sizeof(msg),"quternion = [%.6f, %.6f, %.6f, %.6f]",
				gAttitude.w, gAttitude.x, gAttitude.y, gAttitude.z);
				xil_printf("%s", msg);

		// 전방 벡터 계산
		gForward = rotateVectorByQuat(q, (tDVector3){1.0, 0.0, 0.0});

		// UART 출력 (전방 벡터)
		snprintf(msg, sizeof(msg),
				 " forward = [%.6f, %.6f, %.6f]\n",
				 gForward.x, gForward.y, gForward.z);
		xil_printf("%s\n", msg);


    }
}

void TaskGuid(void *pvParameters)
{
	xil_printf("TaskGUI START AND SUSPEND!\n");
	vTaskSuspend(NULL);

    /* 초기화 */
	TickType_t lastTick  = xTaskGetTickCount();
	tDVector3  prevLOS   = {0};         /* 첫 회차는 각속도 0 */
	BaseType_t firstRun  = pdTRUE;


	xil_printf("TaskGUI init COMPLETE!\n");
	for (;;)
	{
		vTaskSuspend(NULL);
		xil_printf("[GUID Task]\n");

		/* (1) Δt 계산 (초) */
		TickType_t now = xTaskGetTickCount();
		double dt = (now - lastTick) * portTICK_PERIOD_MS * 1e-3;
		lastTick  = now;
		if (dt < 1e-6) dt = 1e-6;          // ★ wrap 보호

		/* (2) LOS 단위벡터 (시뮬레이터가 정규화해 전송) */
		tDVector3 currLOS = gSeekerData.los;

		/* (3) LOS 변화율 Ldot 계산 */
		tDVector3 Ldot = {0};
		if (!firstRun) {
			Ldot.x = (currLOS.x - prevLOS.x) / dt;
			Ldot.y = (currLOS.y - prevLOS.y) / dt;
			Ldot.z = (currLOS.z - prevLOS.z) / dt;
		}
		firstRun = pdFALSE;
		prevLOS  = currLOS;

		/* (4) PN 가속도 (관성계) a_cmd = N·V·( (L × Ldot) × L ) */
		 tDVector3 omega = {
		     currLOS.y*Ldot.z - currLOS.z*Ldot.y,
		     currLOS.z*Ldot.x - currLOS.x*Ldot.z,
		     currLOS.x*Ldot.y - currLOS.y*Ldot.x
		 };
		tDVector3 a_cmd = {   /* (ω × L) */
			omega.y*currLOS.z - omega.z*currLOS.y,
			omega.z*currLOS.x - omega.x*currLOS.z,
			omega.x*currLOS.y - omega.y*currLOS.x
		};
		a_cmd.x *= PN_GAIN * MISSILE_V;    // 스칼라 곱
		a_cmd.y *= PN_GAIN * MISSILE_V;
		a_cmd.z *= PN_GAIN * MISSILE_V;

		/* (5) 관성계 → Body 기준 가속도로 변환 */
		tDVector3 a_body =
			rotateVectorByQuat(quatConjugate(gAttitude), a_cmd);

		/* (6) (미사일Body 기준) 횡 가속도 명령 */
		gAccCmd = a_body;

		/* (7) 디버그 출력 (전역 기준 라벨 명시) */
		//acc_cmd
		snprintf(dbg, sizeof(dbg),
				 "acc_I=[% .4f % .4f % .4f]  acc_B=[% .4f % .4f % .4f]\n",
				 a_cmd.x, a_cmd.y, a_cmd.z,
				 a_body.x, a_body.y, a_body.z);
		xil_printf("%s\n", dbg);


	}
}

void TaskCtrl(void *pvParameters)
{
	 xil_printf("TaskCTL START AND SUSPEND!\n");
	vTaskSuspend(NULL);

	const tDVector3 bodyX = (tDVector3){1.0, 0.0, 0.0};
	tDVector3 prev_err    = {0.0, 0.0, 0.0};

	TickType_t lastTick = xTaskGetTickCount();
	xil_printf("TaskCTL init COMPLETE!\n");

	for (;;)
	{
		vTaskSuspend(NULL);                 /* 주기 동기화 */
		xil_printf("[CTRL Task]\n");

		/* 1) Δt 계산 */
		TickType_t now = xTaskGetTickCount();
		double dt = (now - lastTick) * portTICK_PERIOD_MS * 1e-3;
		lastTick = now;
		if (dt < 1e-6) dt = 1e-6;

		/* (2) Body 기준 가속도 바로 사용 */
		tDVector3 a_body = gAccCmd;

		/* 3) Body 가속도 → 목표 각속도 (omega_cmd) */
		tDVector3 omega_cmd = {
			(bodyX.y * a_body.z - bodyX.z * a_body.y) * INV_MISSILE_V ,
			(bodyX.z * a_body.x - bodyX.x * a_body.z) * INV_MISSILE_V ,
			(bodyX.x * a_body.y - bodyX.y * a_body.x) * INV_MISSILE_V
		};

		/* 4) 측정 각속도 (Unity IMU) */
		const tDVector3 omega_act = gImuData.gyro;

		/* 5) PD 제어 → 각가속도 명령 */
		tDVector3 err = {
			omega_cmd.x - omega_act.x,
			omega_cmd.y - omega_act.y,
			omega_cmd.z - omega_act.z
		};
		tDVector3 derr = {
			(err.x - prev_err.x) / dt,
			(err.y - prev_err.y) / dt,
			(err.z - prev_err.z) / dt
		};
		prev_err = err;

		tDVector3 alpha_cmd = {
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

		/* 8) 디버그 출력  -> '목표 가속도' 및 '명령 각가속도' */
		snprintf(dbc, sizeof(dbc),
				 "w_cmd=[% .4f,% .4f,% .4f]  "
				 "acc_cmd=[% .4f,% .4f,% .4f]\n",
				 omega_cmd.x, omega_cmd.y, omega_cmd.z,
				 alpha_cmd.x, alpha_cmd.y, alpha_cmd.z);
		xil_printf("%s\n", dbc);
	}
}

void TaskTel(void *pvParameters){

    xil_printf("TaskTEL START AND SUSPEND!\n");
    vTaskSuspend(NULL);

    xil_printf("TaskTEL init COMPLETE!\n");

    for (;;)
    {
        vTaskSuspend(NULL);
        xil_printf("[TEL Task]\n");

        int len = snprintf(bufSend, sizeof(bufSend),
                           "%.6f %.6f %.6f\n",
						   gAngAccelCmd.x,
						   gAngAccelCmd.y,
						   gAngAccelCmd.z);

        // 마지막 저장된 PC 주소로 전송
        int ret = lwip_sendto(udpSock,
                    bufSend, len, 0,
                    (struct sockaddr *)&pcAddr,
                    pcAddrLen);

        if (ret < 0)
            xil_printf("sendto err=%d\n", ret);
    }

}
