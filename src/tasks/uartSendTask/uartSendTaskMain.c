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

	// 1. gIsUartReceive�� True���� Ȯ���Ѵ�.
	if (recvFlag == False) {
		// 1.1. gIsUartReceive == False�� ��� recvMissCount +=1;
		// 1.1.1. recvMissCount >= 10 �̶�� �����Ѵ�.
		if (++gRecvMissCount >= 10) {
			explode();
		}
	}
	// 2. recvMissCount = 0;
	gRecvMissCount = 0;
	// 3. messageStatus�� 0���� �����Ѵ�.
	acbMsg.msgStat = OK;
	// 4. CRC-16/XMODEM ������ �����Ѵ�.
	setCrc(acbMsg, &buffer);
	// 5. UART �۽��� �����Ѵ�.
	sendData(buffer);
}

void setCrc(sMessageHeader acbMsg, sMessageHeader* fullMsgBuffer) {
	// CRC�� ������ 2����Ʈ�� ����� ����
	uint16_t totalLength = sizeof(sMessageHeader) + acbMsg.msgLen;
	// CRC ��� (CRC ��ü�� �������� ����)
	uint16_t crc = crc16_xmodem(fullMsgBuffer, totalLength);
	fullMsgBuffer[totalLength] = crc & 0xFF;
	fullMsgBuffer[totalLength + 1] = (crc >> 8) & 0xFF;

}
