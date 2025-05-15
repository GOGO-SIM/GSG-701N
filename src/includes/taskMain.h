#pragma once
#ifndef _TASK_MAIN_H_
#define _TASK_MAIN_H_

#include <stdint.h>
#include "gsgTypes.h"

/**
 * gImuData
 * 가속도 double vector3(x, y, z)
 * 각속도 double vector3(x, y, z)
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
 * 목표 가속도 double vector3 (x, y, z)
 */
tDVector3 gAccCommand;

/**
 * gAttitude
 * 현재 자세 double vector4 (w, x, y, z)
 */
tDVector4 gAttitude;

/**
 * gGcuStatus
 * - NORMAL
 * - STANDBY
 * - PBIT_FAILED
 * - CBIT_FAILED
 */
uint32_t gGcuStatus;

enum eGcuStatus
{
	NORMAL = 0,
	STANDBY = 1,
	PBIT_FAIL = 2,
	CBIT_FAIL = 3,
};

/**
 * gFailCount[4]
 * [0] - udp
 * [1] - uart
 * [2] - imuData
 * [3] - SeekerData
 */
uint32_t gFailCount[4];

enum eFailedUnit
{
	UDP_FAIL = 0,
	UART_FAIL = 1,
	IMU_DATA_FAIL = 2,
	SEEKER_DATA_FAIL = 3,
};

const uint8_t START_FLAG = 0x7E;

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

#endif
