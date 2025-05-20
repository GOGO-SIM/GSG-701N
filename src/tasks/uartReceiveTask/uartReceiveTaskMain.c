#include <stdio.h>
#include <string.h>
#include "global.h"
#include "gsgTypes.h"
#include "xparameters.h"

extern int gUartFailCount;

uint8_t rxBuffer[13];

int recvUartBytes(uint8_t* buffer, int len);
static void runUartReceive();
void explode();

static int isSuccess = FALSE;

void uartReceiveTaskMain() 
{
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
        runUartReceive();
    }
}

// len개의 바이트를 받을 때 까지 데이터를 수신받는다.
int recvUartBytes(uint8_t* buffer, int len)
{
	int received = 0;
	while(received < len)
	{
		received += XUartPs_Recv(&gUartPs, &buffer[received], len - received);
	}
	return TRUE;
}

static void runUartReceive()
{
	int received = 0;
	while(received < 13)
	{
		received += XUartPs_Recv(&gUartPs, &rxBuffer[received], 13 - received);
	}

	uint16_t calculatedCrc = calcCrc(rxBuffer, 11);
	uint16_t receivedCrc = rxBuffer[11] | (rxBuffer[12] << 8);

	if(calculatedCrc != receivedCrc)
	{
		xil_printf("CRC Error! calculated CRC : %x received CRC : %x\r\n", calculatedCrc, receivedCrc);
		return;
	}

	xil_printf("\r\ntotal size is %d\r\n", sizeof(rxBuffer));
	xil_printf("Received CRC is %x && Calculated CRC is %x\r\n", receivedCrc, calculatedCrc);
	tGsmpMessageHeader header;
	memcpy(&header, rxBuffer, sizeof(header));

	uint8_t* ptr = rxBuffer + sizeof(tGsmpMessageHeader);  // 포인터 위치 조정

	int32_t responseMsg = (int32_t)(ptr[0]) |
	                      (int32_t)(ptr[1] << 8) |
	                      (int32_t)(ptr[2] << 16) |
	                      (int32_t)(ptr[3] << 24);
	isSuccess = TRUE;

	//xil_printf("size = %d\r\n", sizeof(payload));
	xil_printf(" startFlag: %x\r\n msgId: %x\r\n destId: %x\r\n srcId: %x\r\n msgLen: %d\r\n", header.startflag, header.msgId, header.destId, header.srcId, header.msgLen);
	for(int i = 0; i < sizeof(rxBuffer); ++i)
	{
		xil_printf("%02x ", rxBuffer[i]);
	}
	xil_printf("\r\n");

	xil_printf("responseMsg: %d\r\n", responseMsg);
	xil_printf("\r\n*----------------*\r\n");
}

void explode()
{
	printf("explode\n");
	return ;
}
