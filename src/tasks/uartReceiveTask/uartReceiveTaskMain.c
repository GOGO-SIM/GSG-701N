#include <stdio.h>

const int FALSE = 0;
const int TRUE = 1;

extern int gUartFailCount;

typedef struct _sMessageHeader
{
	uint8_t startflag;
	uint8_t msgId;
	uint8_t srcId;
	uint8_t destId;
	uint8_t msgStat;
	uint16_t msgLen;
}sMessageHeader;

static int recvFlag;

void uartReceiveTaskMain() 
{
	/*
	 * 1. recvFlag는 true로 설정한다.
	 * 2. gsmpMsgHeader 변수에 memcpy한다.
	 * 3. CRC 검사를 진행한다.
	 * 3.1. CRC 검사가 일치하는지 검사한다.
	 * 3.1.1. 일치하지 않는 경우 gUartFailCount += 1
	 * 3.1.2. gUartFailCount > 10 이라면 폭파 진행한다.
	 * 4. messageStatus == OK를 검사한다.
	 * 4.1. messageStatus != OK인 경우 gUartFailCount += 1
	 * 4.1.2. gUartFailCount > 10 이라면 폭파 진행한다.
	 * 5. gUartFailCount = 0
	 * 6. 종료
	 * */
	int isCRCSuccess;
	int messageStatus;
	sMessageHeader gsmpMsgHeader;

	// 1. recvFlag는 true로 설정한다.
	recvFlag = TRUE;
	messageStatus = TRUE;

	// 2. gsmpMsgHeader 변수에 memcpy한다.
	getUartMsg(gsmpMsgHeader);

	// 3. CRC 검사를 진행한다.
	isCRCSuccess = checkCRC(gsmpMsgHeader);
	// 3.1. CRC 검사가 일치하는지 검사한다.
	// 3.1.1. 일치하지 않는 경우 gUartFailCount += 1
	if(isCRCSuccess == FALSE)
	{
		gUartFailCount += 1;
	}
	else
	{
		// 4. messageStatus == OK를 검사한다.
		// 4.1. messageStatus != OK인 경우 gUartFailCount += 1
		if(messageStatus == FALSE)
			{
				gUartFailCount += 1;
			}
	}
	// 3.1.2. gUartFailCount > 10 이라면 폭파 진행한다.
	// 4.1.2. gUartFailCount > 10 이라면 폭파 진행한다.
	if(gUartFailCount > 10)
	{
		explode();
	}
	// 5. gUartFailCount = 0
	gUartFailCount = 0;
	// 6. 종료
	return;
}
