#include "global.h"
#include "gsgTypes.h"
#include "xil_printf.h"
#include "crc.h"
#include "xuartps.h"
#include "xparameters.h"
#include "uartSendTaskMain.h"
// blocking 무효화
#define xil_printf(...)  do {} while(0)

void uartSendTaskMain(void *pvParameters) {
	for(;;)
	{
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
        runUartSend();
	}
}

void runUartSend()
{
	tGsmpMsg uartSendMsg;
	// 1. gIsUartReceive가 True인지 확인한다.
	if(gRecvFlag == FALSE)
	{
		if(++gFailCount[UART_FAIL] >= 10)
		{
			explode();
		}
	}
	else
	{
		gFailCount[UART_FAIL] = 0;
	}
	gRecvFlag = FALSE;

	// gsmp 프로토콜에 맞춰 메세지를 감싼다.
	gsmpWrapper(ACB_SEND_MSG_ID, OK, gC, destId)

	// 전송을 수행한다.
	sendData(uartSendMsg, sizeof(uartSendMsg));
}


void sendData(tGsmpMsg msg, uint16_t len)
{
	// Msg를 버퍼로  변환
	uint8_t txBuffer[bufferSizeNoCRC + crcSize]; // CRC 포함
	memcpy(txBuffer, &gSendMsg.header, headerSize);
	memcpy(txBuffer + headerSize, (const uint8_t*) msg.payload, msg.payload.size());
	txBuffer[bufferSizeNoCRC + 0] = msg.crc & 0xFF;        // LSB
	txBuffer[bufferSizeNoCRC + 1] = (msg.crc >> 8) & 0xFF; // MSB
    // 버퍼에 들어간 바이트의 수만큼 UART로 전송
    XUartPs_Send(&gUartPs, buffer, len);
    //xil_printf("\r\n");
    //xil_printf("TX: 0x%02X %02X %02X %02X\n", buffer[0], buffer[1], buffer[2], buffer[3]);
}
