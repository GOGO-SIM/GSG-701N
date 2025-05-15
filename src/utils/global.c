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

// ERR : voltage 변수 이름 재정의 필요
/*=====CBIT&PBIT 전압 및 온도  체크  변수=====*/
const double gVoltage1 = 3.0f;
const double gVoltage2 = 4.0f;
const double gVoltage3 = 5.0f;

const double gCelcius = 30.0f;
