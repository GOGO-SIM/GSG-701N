#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "gsgTypes.h"
#include "global.h"
#include "crc.h"

uint8_t gUdpBuffer[100];
int checkImuData(tImuPayload cur);
int checkSeekerData(tSeekerPayload cur);
uint16_t getCrc();
//int checkCrc()
//{
//	int res;
//	uint16_t crc;
//	crc = 0;
//
//	if (getCrc() == crc)
//		res = TRUE;
//	else
//		res = FALSE;
//	return res;
//}

// explode �������� ����
static void explode()
{
	printf("BOOM!");
}

void handleImuMsg()
{
	// 1. IMU payload memcpy
	tImuPayload cur;
	// ERR : msg undeclared
	//memcpy(&cur, msg + sizeof(tGsmpMessageHeader), sizeof(tImuPayload));
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
	vTaskResume(xNavigationTaskHandle);
}

void handleSeekerMsg()
{
	// 1. Seeker payload  memcpy
	tSeekerPayload cur;
	// ERR : msg undeclared
	//memcpy(&cur, msg + sizeof(tGsmpMessageHeader), sizeof(tSeekerPayload));
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
		gSeekerData = cur;
	}
}

void udpReceiveTaskMain( void *pvParameters )
{
	tGsmpMessageHeader curHeader;
	// 1. gsmp header memcpy
	memcpy(&curHeader, gUdpBuffer, sizeof(curHeader));
	// 2. check crc
	if (checkCrc(gUdpBuffer) == FALSE)
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

uint16_t getCrc()
{
	return 1;
}

int checkImuData(tImuPayload cur)
{
	return 1;
}

int checkSeekerData(tSeekerPayload cur)
{
	return 1;
}
