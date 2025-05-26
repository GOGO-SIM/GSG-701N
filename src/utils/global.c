#pragma once
#ifndef _TASK_MAIN_H_
#define _TASK_MAIN_H_

#include <stdint.h>
#include "gsgTypes.h"
#include <global.h>

/**
 * [task handlers]
 */
TaskHandle_t xMainTaskHandle,
	xSchedulingTaskHandle,
	xInitTaskHandle,
	xPbitTaskHandle,
	xStanbyIgnitionTaskHandle,
	xUartReceiveTaskHandle,
	xUdpReceiveTaskHandle,
	xNavigationTaskHandle,
	xGuidanceTaskHandle,
	xControlTaskHandle,
	xUartSendTaskHandle,
	xCbitTaskHandle,
	xTelemetryTaskHandle,
	xPbitFailTaskHandle;


const uint8_t START_FLAG = 0x7E;

int gRecvFlag = TRUE;
int gRecvMissCount = 0;

/**
 * gFailCount[4]
 * [0] - udp
 * [1] - uart
 * [2] - imuData
 * [3] - SeekerData
 */

XUartPs gUartPs;
XUartPs_Config *gUartConfig;
XSysMon gSysMonInst;
XSysMon_Config *gXadcConfig;

uint32_t gFailCount[4];

tGSsmpMsg gSendMsg;

/**
 * gImuData
 * ���ӵ� double vector3(x, y, z)
 * ���ӵ� double vector3(x, y, z)
 */
tImuData gImuData;


/**
 * gSeekerData
 * los double vector 3(x, y, z)
 * distance double
 */
tSeekerData gSeekerData;

/**
 * gAccCommand
 * ��ǥ ���ӵ� double vector3 (x, y, z)
 */
tDVector3 gAccCmd;

/**
 * gAttitude
 * ���� �ڼ� double vector4 (w, x, y, z)
 */
tDVector4 gAttitude;

/**
 * gControlCommand
 * ���� �ڼ� double vector3 (x, y, z)
 * ��ǥ ���ӵ� -> Guidance  Task return�ϴ� ��
 */
tDVector3 gControlCmd;

/**
 * gGcuStatus
 * - NORMAL
 * - STANDBY
 * - PBIT_FAILED
 * - CBIT_FAILED
 */
uint32_t gGcuStatus;

/*=====CBIT&PBIT Standard=====*/

double gVoltageInt = 0.0f;
double gVoltageBram = 0.0f;
double gVoltageAux = 0.0f;
double gCelcius = 0.0f;

u32 gPassCbitFlag = TRUE;
u32 gPassPbitFlag = TRUE;

#endif
