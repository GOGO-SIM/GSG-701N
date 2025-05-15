#pragma once
#ifndef _GLOBAL_H_
#define _GLOBAL_H_


#include <stdint.h>
#include "gsgTypes.h"
#include "FreeRTOS.h"
#include "task.h"

/* 매크로 정의 */
#define TRUE 1
#define FALSE 0
#define SUCCESS 1
#define FAIL 0
#define ERROR -1

/* 활용할 전역 변수 extern */
extern tImuData gImuData;
extern tSeekerData gSeekerData;
extern tDVector3 gAccCommand;
extern tDVector4 gAttitude;
extern uint32_t gGcuStatus;
extern uint32_t gFailCount[4];

/**
 * [task handlers]
 */
extern TaskHandle_t xMainTaskHandle;
extern TaskHandle_t	xSchedulingTaskHandle;
extern TaskHandle_t	xInitTaskHandle;
extern TaskHandle_t	xPbitTaskHandle;
extern TaskHandle_t	xStanbyIgnitionTaskHandle;
extern TaskHandle_t	xUartReceiveTaskHandle;
extern TaskHandle_t	xUdpReceiveTaskHandle;
extern TaskHandle_t	xNavigationTaskHandle;
extern TaskHandle_t	xGuidanceTaskHandle;
extern TaskHandle_t	xControlTaskHandle;
extern TaskHandle_t	xUartSendTaskHandle;
extern TaskHandle_t	xCbitTaskHandle;
extern TaskHandle_t	xTelemetryTaskHandle;
extern TaskHandle_t xPbitFailTaskHandle;

/* enum 클래스 정의 */
enum eGcuStatus
{
	NORMAL = 0,
	STANDBY = 1,
	PBIT_FAIL = 2,
	CBIT_FAIL = 3,
};

enum eFailedUnit
{
	UDP_FAIL = 0,
	UART_FAIL = 1,
	IMU_DATA_FAIL = 2,
	SEEKER_DATA_FAIL = 3,
};

extern const uint8_t START_FLAG;

enum eMessageId
{
	IMU_MSG_ID = 0,
	SEEKER_MSG_ID = 1,
	ACB_MSG_ID = 2,
	ACB_ECHO_SEND_MSG_ID = 3,
	ACB_ECHO_RECV_MSG_ID = 4,
	TELEMETRY_MSG_ID = 5,
};

enum eSourceId
{
	GCU_ID = 0,
	IMU_ID = 1,
	SEEKER_ID = 2,
	ACB_ID = 3,
	TELMETRY_ID = 4
};

enum eStatus
{
	OK = 0,
	CRC_ERROR = 1,
	INTERNAL_ERROR = 2,
};


/*=====CBIT&PBIT 전압 및 온도  체크  변수=====*/
extern const double gVoltage1;
extern const double gVoltage2;
extern const double gVoltage3;

extern const double gCelcius;


#endif
