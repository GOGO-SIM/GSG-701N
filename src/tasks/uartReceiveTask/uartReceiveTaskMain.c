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
	 * 1. recvFlag�� true�� �����Ѵ�.
	 * 2. gsmpMsgHeader ������ memcpy�Ѵ�.
	 * 3. CRC �˻縦 �����Ѵ�.
	 * 3.1. CRC �˻簡 ��ġ�ϴ��� �˻��Ѵ�.
	 * 3.1.1. ��ġ���� �ʴ� ��� gUartFailCount += 1
	 * 3.1.2. gUartFailCount > 10 �̶�� ���� �����Ѵ�.
	 * 4. messageStatus == OK�� �˻��Ѵ�.
	 * 4.1. messageStatus != OK�� ��� gUartFailCount += 1
	 * 4.1.2. gUartFailCount > 10 �̶�� ���� �����Ѵ�.
	 * 5. gUartFailCount = 0
	 * 6. ����
	 * */
	int isCRCSuccess;
	int messageStatus;
	sMessageHeader gsmpMsgHeader;

	// 1. recvFlag�� true�� �����Ѵ�.
	recvFlag = TRUE;
	messageStatus = TRUE;

	// 2. gsmpMsgHeader ������ memcpy�Ѵ�.
	getUartMsg(gsmpMsgHeader);

	// 3. CRC �˻縦 �����Ѵ�.
	isCRCSuccess = checkCRC(gsmpMsgHeader);
	// 3.1. CRC �˻簡 ��ġ�ϴ��� �˻��Ѵ�.
	// 3.1.1. ��ġ���� �ʴ� ��� gUartFailCount += 1
	if(isCRCSuccess == FALSE)
	{
		gUartFailCount += 1;
	}
	else
	{
		// 4. messageStatus == OK�� �˻��Ѵ�.
		// 4.1. messageStatus != OK�� ��� gUartFailCount += 1
		if(messageStatus == FALSE)
			{
				gUartFailCount += 1;
			}
	}
	// 3.1.2. gUartFailCount > 10 �̶�� ���� �����Ѵ�.
	// 4.1.2. gUartFailCount > 10 �̶�� ���� �����Ѵ�.
	if(gUartFailCount > 10)
	{
		explode();
	}
	// 5. gUartFailCount = 0
	gUartFailCount = 0;
	// 6. ����
	return;
}
