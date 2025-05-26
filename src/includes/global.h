#pragma once
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdint.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "xil_printf.h"
#include "xuartps.h"
#include "xsysmon.h"
#include "xparameters.h"
#include "xil_io.h"
#include "gsgTypes.h"
#include "xemacps_hw.h"

/* ��ũ�� ���� */
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef SUCCESS
#define SUCCESS 1
#endif
#ifndef FAIL
#define FAIL 0
#endif
#ifndef ERROR
#define ERROR -1
#endif
#ifndef UART_BAUD
#define UART_BAUD 115200
#endif

/* Ȱ���� ���� ���� extern */
extern int gRecvFlag;
extern uint32_t gFailCount[4];

// tGsmpMsg gSendMsg;
extern tGsmpMsg gAcbSendMsg;
extern tGsmpMsg gAcbEchoSendMsg;
extern tGsmpMsg gTelemetryMsg;

extern tImuData gImuData;
extern tSeekerData gSeekerData;
extern tDVector3 gAccCmd;
extern tDVector4 gAttitude;
extern tDVector3 gControlCmd;
extern uint32_t gGcuStatus;

/* from init */
extern XUartPs gUartPs;
extern XUartPs_Config *gUartConfig;
extern XSysMon gSysMonInst;
extern XSysMon_Config *gXadcConfig;

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

/* enum Ŭ���� ���� */
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
	ACB_SEND_MSG_ID = 2,
	ACB_RECV_MSG_ID = 3,
	ACB_ECHO_SEND_MSG_ID = 4,
	ACB_ECHO_RECV_MSG_ID = 5,
	TELEMETRY_MSG_ID = 6,
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

enum eModeStatus
{
	WAIT = 0,
	ENGAGE = 1,
	SAFE = 2,
	REEXPLORE = 3,
};

/*=====CBIT&PBIT ���� �� �µ�  üũ  ����=====*/
extern double gVoltageInt;
extern double gVoltageBram;
extern double gVoltageAux;

extern double gCelcius;

extern u32 gPassCbitFlag;
extern u32 gPassPbitFlag;

#endif
