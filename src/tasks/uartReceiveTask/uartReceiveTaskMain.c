#include <stdio.h>
#include <string.h>
#include "global.h"
#include "gsgTypes.h"

extern int gUartFailCount;


static int recvFlag;

void explode();
int checkCRC(tGsmpMessageHeader examp, uint16_t size);

void run()
{
    
}

void uartReceiveTaskMain() 
{
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
		run();
    }
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
//	int isCRCSuccess;
//	int messageStatus;
//	tGsmpMessageHeader gsmpMsgHeader;
//	uint8_t* gUartBuffer;
//
//	// 1. recvFlag�� true�� �����Ѵ�.
//	recvFlag = TRUE;
//	messageStatus = TRUE;
//
//	// 2. gsmpMsgHeader ������ memcpy�Ѵ�.
//	memcpy(&gsmpMsgHeader, gUartBuffer, sizeof(gsmpMsgHeader));
//	// getUartMsg(gsmpMsgHeader); -> TODO: �ʱ�ȭ �� UART RX interrupt�� �����ؾ� �Ѵ�.
//
//	// 3. CRC �˻縦 �����Ѵ�.
//	isCRCSuccess = checkCRC(gsmpMsgHeader, sizeof(gsmpMsgHeader));
//	// 3.1. CRC �˻簡 ��ġ�ϴ��� �˻��Ѵ�.
//	// 3.1.1. ��ġ���� �ʴ� ��� gUartFailCount += 1
//
//	/**
//	 * [������ message ���� �˻�]:
//	 * CRC�� ������ ���: GCU���� ����� ����� ����ġ
//	 * message ���°� OK�� �ƴ� ���: ���� ��(ACB)�� CRC ���� �Ǵ� ��Ʈ��ũ ����
//	 */
//	if (isCRCSuccess == FALSE || messageStatus != OK)
//	{
//		gFailCount[UART_FAIL] += 1;
//		// 3.1.2. gUartFailCount > 10 �̶�� ���� �����Ѵ�.
//		if (gFailCount[UART_FAIL] > 10)
//		{
//			explode();
//		}
//	}
//	// 4. gUartFailCount = 0
//	gFailCount[UART_FAIL] = 0;
//	// 5. ����
//	return;
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
