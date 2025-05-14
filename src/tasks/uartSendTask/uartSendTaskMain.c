#include "includes/gsgTypes.h"
#include "includes/taskMain.h"

typedef struct _sMessageHeader {
	uint8_t startflag;
	uint8_t msgId;
	uint8_t srcId;
	uint8_t destId;
	uint8_t msgStat;
	uint16_t msgLen;
} sMessageHeader;

static int recvFlag;
extern int gRecvMissCount;

void explode();
void setCrc(sMessageHeader* acbMsg, sMessageHeader* fullMsgBuffer);
void sendData(uint32_t buffer);

void uartSendTaskMain(void *pvParameters) {
	sMessageHeader acbMsg;
	recvMissCount = 0;
	uint32_t buffer;

	// 1. gIsUartReceive가 True인지 확인한다.
	if (recvFlag == False) {
		// 1.1. gIsUartReceive == False인 경우 recvMissCount +=1;
		// 1.1.1. recvMissCount >= 10 이라면 폭파한다.
		if (++gRecvMissCount >= 10) {
			explode();
		}
	}
	// 2. recvMissCount = 0;
	gRecvMissCount = 0;
	// 3. messageStatus를 0으로 설정한다.
	acbMsg.msgStat = OK;
	// 4. CRC-16/XMODEM 세팅을 진행한다.
	setCrc(acbMsg, &buffer);
	// 5. UART 송신을 시작한다.
	sendData(buffer);
}

void setCrc(sMessageHeader acbMsg, sMessageHeader* fullMsgBuffer) {
	// CRC는 마지막 2바이트에 저장될 예정
	uint16_t totalLength = sizeof(sMessageHeader) + acbMsg.msgLen;
	// CRC 계산 (CRC 자체는 포함하지 않음)
	uint16_t crc = crc16_xmodem(fullMsgBuffer, totalLength);
	fullMsgBuffer[totalLength] = crc & 0xFF;
	fullMsgBuffer[totalLength + 1] = (crc >> 8) & 0xFF;

}
