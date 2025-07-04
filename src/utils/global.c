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
	xPbitFailTaskHandle,
	xExplodeTaskHandle;


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
XEmacPs gXemacPsInst;
XEmacPs_Config *gXemacConfig;
XGpioPs gGpioPs;
XGpioPs_Config *gGpioConfig;

// 에코잉 받아온 값 저장

tEchoPayload gSeekerEchoRecvData = 19980398;
tEchoPayload gAcbEchoRecvData = 19980398;

// 연속 실패 횟수 저장

uint32_t gFailCount[4];

// ACB, ECHO, TELEMETRY SendTask 및 wrapper

uint8_t gSendBuffer[TELEMETRY_MSG_SIZE];

// GCU의 현재 모드
int gModeStatus;

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
tDVector3 gForward;
/**
 * gControlCommand
 * ���� �ڼ� double vector3 (x, y, z)
 * ��ǥ ���ӵ� -> Guidance  Task return�ϴ� ��
 */
tDVector3 gControlCmd;


/*
 * [Receive Payload]
 */
tAcbRecvPayload gAcbRecvPayload;
tEchoPayload gAcbEchoPayload;
tEchoPayload gSeekerEchoPayload;
tImuPayload gImuPayload;
tSeekerPayload gSeekerPayload;
uint8_t gSeekerInfo;

/**
 * gGcuStatus
 * - WAIT
 * - ENGAGE
 * - SAFE
 * - RE-EXPLORATION
 */

uint32_t gGcuStatus = WAIT;

/*=====CBIT&PBIT Standard=====*/

double gVoltageInt = 0.0f;
double gVoltageBram = 0.0f;
double gVoltageAux = 0.0f;
double gCelcius = 0.0f;

u32 gPassCbitFlag = TRUE;
u32 gPassPbitFlag = TRUE;

/*=======GNC=======*/

volatile BaseType_t gSeekerUpdated = pdFALSE;
tDVector3 gLdot;

#endif
