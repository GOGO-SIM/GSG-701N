#include <stdio.h>
#include <string.h>
#include "global.h"
#include "gsgTypes.h"

extern int gUartFailCount;


static int recvFlag;

void explode();
int checkCRC(tGsmpMessageHeader examp, uint16_t size);

static void uartReceiverun()
{

}

void uartReceiveTaskMain() 
{
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
		uartReceiverun();
    }
	/*
	 * 1. recvFlag占쏙옙 true占쏙옙 占쏙옙占쏙옙占싼댐옙.
	 * 2. gsmpMsgHeader 占쏙옙占쏙옙占쏙옙 memcpy占싼댐옙.
	 * 3. CRC 占싯사를 占쏙옙占쏙옙占싼댐옙.
	 * 3.1. CRC 占싯사가 占쏙옙치占싹댐옙占쏙옙 占싯삼옙占싼댐옙.
	 * 3.1.1. CRC占쏙옙 占쏙옙치占쏙옙占쏙옙 占십거놂옙, 占쏙옙占쏙옙 占쌘드가 OK占쏙옙 占싣댐옙 占쏙옙占� gUartFailCount += 1
	 * 3.1.2. gUartFailCount > 10 占싱띰옙占� 占쏙옙占쏙옙 占쏙옙占쏙옙占싼댐옙.
	 * 4. gUartFailCount = 0
	 * 5. 占쏙옙占쏙옙
	 * */
//	int isCRCSuccess;
//	int messageStatus;
//	tGsmpMessageHeader gsmpMsgHeader;
//	uint8_t* gUartBuffer;
//
//	// 1. recvFlag占쏙옙 true占쏙옙 占쏙옙占쏙옙占싼댐옙.
//	recvFlag = TRUE;
//	messageStatus = TRUE;
//
//	// 2. gsmpMsgHeader 占쏙옙占쏙옙占쏙옙 memcpy占싼댐옙.
//	memcpy(&gsmpMsgHeader, gUartBuffer, sizeof(gsmpMsgHeader));
//	// getUartMsg(gsmpMsgHeader); -> TODO: 占십깍옙화 占쏙옙 UART RX interrupt占쏙옙 占쏙옙占쏙옙占쌔억옙 占싼댐옙.
//
//	// 3. CRC 占싯사를 占쏙옙占쏙옙占싼댐옙.
//	isCRCSuccess = checkCRC(gsmpMsgHeader, sizeof(gsmpMsgHeader));
//	// 3.1. CRC 占싯사가 占쏙옙치占싹댐옙占쏙옙 占싯삼옙占싼댐옙.
//	// 3.1.1. 占쏙옙치占쏙옙占쏙옙 占십댐옙 占쏙옙占� gUartFailCount += 1
//
//	/**
//	 * [占쏙옙占쏙옙占쏙옙 message 占쏙옙占쏙옙 占싯삼옙]:
//	 * CRC占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占�: GCU占쏙옙占쏙옙 占쏙옙占쏙옙占� 占쏙옙占쏙옙占� 占쏙옙占쏙옙치
//	 * message 占쏙옙占승곤옙 OK占쏙옙 占싣댐옙 占쏙옙占�: 占쏙옙占쏙옙 占쏙옙(ACB)占쏙옙 CRC 占쏙옙占쏙옙 占실댐옙 占쏙옙트占쏙옙크 占쏙옙占쏙옙
//	 */
//	if (isCRCSuccess == FALSE || messageStatus != OK)
//	{
//		gFailCount[UART_FAIL] += 1;
//		// 3.1.2. gUartFailCount > 10 占싱띰옙占� 占쏙옙占쏙옙 占쏙옙占쏙옙占싼댐옙.
//		if (gFailCount[UART_FAIL] > 10)
//		{
//			explode();
//		}
//	}
//	// 4. gUartFailCount = 0
//	gFailCount[UART_FAIL] = 0;
//	// 5. 占쏙옙占쏙옙
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

