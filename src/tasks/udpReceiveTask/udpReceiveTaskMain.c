#include <stdint.h>
#include <string.h>
#include "gsgTypes.h"
#include "taskMain.h"

uint8_t gUdpBuffer[100];

int checkCrc()
{
	int res;
	uint16_t crc;

	if (getCrc() == crc)
		res = TRUE;
	else
		res = FALSE;
	return res;
}

// explode 전역에서 관리
static void explode()
{
	printf("BOOM!");
}

void handleImuMsg()
{
	// 1. IMU payload memcpy
	tImuPayload cur;
	memcpy(&cur, msg + sizeof(tGsmpMessageHeader), sizeof(tImuPayload));
	// 2. check IMU Data
	if (checkImuData(cur) == ERROR)
	{
		gFailCount[IMU_DATA_FAIL] += 1;
		if (gFailCount[IMU_DATA_FAIL] > 10)
			explode();
	}
	else
	{
		gFailCount[IMU_DATA_FAIL] = 0;
		// 3. save valid IMU data
		gImuData = cur; // TODO: TIMER TICK can be added.
	}
	// 4. Resume navigation Task
	taskResume(navigationTaskHandle);
}

void handleSeekerMsg()
{
	// 1. Seeker payload  memcpy
	tSeekerPayload cur;
	memcpy(&cur, msg + sizeof(tGsmpMessageHeader), sizeof(tSeekerPayload));
	// 2. check Seeker Data
	if (checkSeekerData(cur) == ERROR)
	{
		gFailCount[SEEKER_DATA_FAIL] += 1;
		if (gFailCount[SEEKER_DATA_FAIL] > 10)
			explode();
	}
	else
	{
		gFailCount[SEEKER_DATA_FAIL] = 0;
		// 3. save valid Seeker data
		SeekerData = cur;
	}
}

void udpReceiveTaskMain( void *pvParameters )
{
	tGsmpMessageHeader curHeader;
	// 1. gsmp header memcpy
	memcpy(&curHeader, gUdpBuffer, sizeof(curHeader));
	// 2. check crc
	if (checkCrc() == FALSE)
	{
		gFailCount[UDP_FAIL] += 1;
		if (gFailCount[UDP_FAIL] > 10)
		{
			// 2-1. if failure accumulated 10 times, then explode
			explode();
		}
	}
	else
	{
		gFailCount[UDP_FAIL] = 0;
		// 3. process data based on message type
		if (curHeader.msgId == IMU_MSG_ID)
		{
			handleImuMsg();
		}
		else if (curHeader.msgId == SEEKER_MSG_ID)
		{
			handleSeekerMsg();
		}
	}
}
