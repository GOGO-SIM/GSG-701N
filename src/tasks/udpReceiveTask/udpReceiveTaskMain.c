#include <stdint.h>
#include <string.h>

#define FALSE 0
#define TRUE 1
#define ERROR -1

typedef struct sGsmpMH
{
	uint8_t startBit;
	uint8_t startflag;
	uint8_t msgId;
	uint8_t srcId;
	uint8_t destId;
	uint8_t msgStat;
	uint16_t msgLen;

} tGsmpMH;

unsigned char gUdpBuffer[100];
int8_t gUdpFailCount;
int8_t gStatus;

int checkCrc()
{
	int res;
	uint16_t crc;

	// 1. 헤더크기와 페이로드 뒤 crc를 검사
	if (getCrc() == crc)
		res = TRUE;
	else
		res = FALSE;
	return res;
}

void activateFuze()
{
	printf("BOOM!");
}

void handleImuMsg()
{
	// 1. IMU payload에 memcpy
	tImuPayload cur;
	memcpy(&cur, msg + sizeof(tGsmpMH), sizeof(tImuPayload));
	// 2. 값 범위 체크
	if (checkImuData(cur) == ERROR)
	{
		gImuDataFailCount += 1;
		if (gImuDataFailCount > 10)
			activateFuze();
	}
	else
	{
		gImuDataFailCount = 0;
		// 3. 데이터 저장
		gImuData = cur; // TODO: 필요한 경우 TICK을 함께 저장하기
	}
	// 4. 항법 태스크 resume
	taskResume(navigationTaskHandle);
}

void handleSeekerMsg()
{
	// 1. Seeker payload에 memcpy
	tSeekerPayload cur;
	memcpy(&cur, msg + sizeof(tGsmpMH), sizeof(tSeekerPayload));
	// 2. 값 범위 체크
	if (checkSeekerData(cur) == ERROR)
	{
		gSeekerDataFailCount += 1;
		if (gSeekerDataFailCount > 10)
			activateFuze();
	}
	else
	{
		gSeekerDataFailCount = 0;
		// 3. 데이터 저장
		SeekerData = cur;
	}
}

void udpReceiveTaskMain( void *pvParameters )
{
	tGsmpMH curHeader;
	// 1. 메시지 헤더에 저장
	memcpy(&curHeader, gUdpBuffer, sizeof(curHeader));
	// 2. CRC 체크
	if (checkCrc() == FALSE)
	{
		gUdpFailCount += 1;
		if (gUdpFailCount > 10)
		{
			// 2-1. 신관 작동
			activateFuze();
		}
	}
	else
	{
		gUdpFailCount = 0;
		// 3. message_id 체크
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
