#include <stdio.h>
#include <string.h>
#include "global.h"
#include "gsgTypes.h"
#include "xparameters.h"

extern int gUartFailCount;

uint8_t rxBuffer[33];


static void runUartReceive();
void explode();
int checkCRC(tGsmpMessageHeader examp, uint16_t size);

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

static void runUartReceive()
{
	int received = 0;
	while(received < 33)
	{
		received += XUartPs_Recv(&gUartPs, &rxBuffer[received], 33 - received);
	}

	uint16_t calculatedCrc = calcCrc(rxBuffer, 31);
	uint16_t receivedCrc = rxBuffer[31] | (rxBuffer[32] << 8);

	if(calculatedCrc != receivedCrc)
	{
		xil_printf("CRC Error! calculated CRC : %x received CRC : %x\r\n", calculatedCrc, receivedCrc);
		return;
	}
	xil_printf("Received CRC is %x && Calculated CRC is %x\r\n", receivedCrc, calculatedCrc);
	tGsmpMessageHeader header;
	memcpy(&header, rxBuffer, sizeof(header));

	tDVector3 payload;
	memcpy(&payload, rxBuffer + sizeof(header), sizeof(payload));
	isSuccess = TRUE;

	xil_printf("size = %d\r\n", sizeof(payload));
	xil_printf("X = %f, Y = %f, Z = %f\r\n", payload.x, payload.y, payload.z);

}


void explode()
{
	printf("explode\n");
	return ;
}
