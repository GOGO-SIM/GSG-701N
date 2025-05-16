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
uint32_t gFailCount[4];


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

// ERR : voltage ���� �̸� ������ �ʿ�
/*=====CBIT&PBIT ���� �� �µ�  üũ  ����=====*/
const double gVoltage1 = 3.0f;
const double gVoltage2 = 4.0f;
const double gVoltage3 = 5.0f;

const double gCelcius = 30.0f;

#endif
