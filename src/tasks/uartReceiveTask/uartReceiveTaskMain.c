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

void explode();

void uartReceiveTaskMain() 
{
	/*
	 * 1. recvFlag�� true�� �����Ѵ�.
	 * 2. gsmpMsgHeader ������ memcpy�Ѵ�.
	 * 3. CRC �˻縦 �����Ѵ�.
	 * 3.1. CRC �˻簡 ��ġ�ϴ��� �˻��Ѵ�.
	 * 3.1.1. CRC�� ��ġ���� �ʰų�, ���� �ڵ尡 OK�� �ƴ� ��� gUartFailCount += 1
	 * 3.1.2. gUartFailCount > 10 �̶�� ���� �����Ѵ�.
	 * 4. gUartFailCount = 0
	 * 5. ����
	 * */
	int isCRCSuccess;
	int messageStatus;
	sMessageHeader gsmpMsgHeader;

	// 1. recvFlag�� true�� �����Ѵ�.
	recvFlag = TRUE;
	messageStatus = TRUE;

	// 2. gsmpMsgHeader ������ memcpy�Ѵ�.
	memcpy(&gsmpMsgHeader, gUartBuffer, sizeof(gsmpMsgHeader));
	// getUartMsg(gsmpMsgHeader); -> TODO: �ʱ�ȭ �� UART RX interrupt�� �����ؾ� �Ѵ�.

	// 3. CRC �˻縦 �����Ѵ�.
	isCRCSuccess = checkCRC(gsmpMsgHeader);
	// 3.1. CRC �˻簡 ��ġ�ϴ��� �˻��Ѵ�.
	// 3.1.1. ��ġ���� �ʴ� ��� gUartFailCount += 1

	/**
	 * [������ message ���� �˻�]:
	 * CRC�� ������ ���: GCU���� ����� ����� ����ġ
	 * message ���°� OK�� �ƴ� ���: ���� ��(ACB)�� CRC ���� �Ǵ� ��Ʈ��ũ ����
	 */
	if (isCRCSuccess == FALSE || messageStatus != OK)
	{
		gUartFailCount += 1;
		// 3.1.2. gUartFailCount > 10 �̶�� ���� �����Ѵ�.
		if (gUartFailCount > 10)
		{
			explode();
		}
	}
	// 4. gUartFailCount = 0
	gUartFailCount = 0;
	// 5. ����
	return;
}


void explode()
{
	printf("explode\n");
	return ;
}
