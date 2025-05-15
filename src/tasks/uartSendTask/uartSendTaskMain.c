#include "global.h"
#include "gsgTypes.h"

static int recvFlag;
extern int gRecvMissCount;

void explode();
void setCrc(tGsmpMessageHeader* acbMsg, tGsmpMessageHeader* fullMsgBuffer);
void sendData(uint32_t* buffer);

void uartSendTaskMain(void *pvParameters) {
	tGsmpMessageHeader acbMsg;
	uint32_t* buffer;

	// 1. gIsUartReceive가 True인지 확인한다.
	if (recvFlag == FALSE) {
		// 1.1. gIsUartReceive == False인 경우 recvMissCount +=1;
		// 1.1.1. recvMissCount >= 10 이라면 폭파한다.
		if (++gFailCount[UART_FAIL] >= 10) {
			explode();
		}
	}
	// 2. recvMissCount = 0;
	gFailCount[UART_FAIL] = 0;
	// 3. messageStatus를 0으로 설정한다.
	acbMsg.msgStat = OK;
	// 4. CRC-16/XMODEM 세팅을 진행한다.
	// ERR : not declared -> declared in crc.h
	//calcCrc(acbMsg, &buffer);
	// 5. UART 송신을 시작한다.
	sendData(buffer);
}

void sendData(uint32_t* buffer)
{
	return;
}
