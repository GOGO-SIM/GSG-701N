#include <stdio.h>
#include <string.h>
#include "global.h"
#include "gsgTypes.h"

extern int gUartFailCount;


static int recvFlag;

void explode();
int checkCRC(tGsmpMessageHeader examp, uint16_t size);

void uartReceiveTaskMain() 
{
	/*
	 * 1. recvFlag는 true로 설정한다.
	 * 2. gsmpMsgHeader 변수에 memcpy한다.
	 * 3. CRC 검사를 진행한다.
	 * 3.1. CRC 검사가 일치하는지 검사한다.
	 * 3.1.1. CRC가 일치하지 않거나, 응답 코드가 OK가 아닌 경우 gUartFailCount += 1
	 * 3.1.2. gUartFailCount > 10 이라면 폭파 진행한다.
	 * 4. gUartFailCount = 0
	 * 5. 종료
	 * */
	int isCRCSuccess;
	int messageStatus;
	tGsmpMessageHeader gsmpMsgHeader;
	uint8_t* gUartBuffer;

	// 1. recvFlag는 true로 설정한다.
	recvFlag = TRUE;
	messageStatus = TRUE;

	// 2. gsmpMsgHeader 변수에 memcpy한다.
	memcpy(&gsmpMsgHeader, gUartBuffer, sizeof(gsmpMsgHeader));
	// getUartMsg(gsmpMsgHeader); -> TODO: 초기화 시 UART RX interrupt를 설정해야 한다.

	// 3. CRC 검사를 진행한다.
	isCRCSuccess = checkCRC(gsmpMsgHeader, sizeof(gsmpMsgHeader));
	// 3.1. CRC 검사가 일치하는지 검사한다.
	// 3.1.1. 일치하지 않는 경우 gUartFailCount += 1

	/**
	 * [수신한 message 에러 검사]:
	 * CRC를 실패한 경우: GCU에서 계산한 결과와 불일치
	 * message 상태가 OK가 아닌 경우: 수신 측(ACB)의 CRC 오류 또는 네트워크 오류
	 */
	if (isCRCSuccess == FALSE || messageStatus != OK)
	{
		gFailCount[UART_FAIL] += 1;
		// 3.1.2. gUartFailCount > 10 이라면 폭파 진행한다.
		if (gFailCount[UART_FAIL] > 10)
		{
			explode();
		}
	}
	// 4. gUartFailCount = 0
	gFailCount[UART_FAIL] = 0;
	// 5. 종료
	return;
}

int checkCRC(tGsmpMessageHeader examp, uint16_t size)
{
	return 1;
}


void explode()
{
	printf("explode\n");
	return ;
}
