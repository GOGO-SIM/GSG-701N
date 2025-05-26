#include "global.h"
#include "gsgTypes.h"
#include "xil_printf.h"
#include "crc.h"
#include "xuartps.h"
#include "xparameters.h"
#include "uartSendTaskMain.h"
#include "gsmpWrapper.h"
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
	//tGsmpMsg uartSendMsg;
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

	gControlCmd.x = 1.02;
	gControlCmd.y = 3.04;
	gControlCmd.z = 5.06;

	// example tAcbPayload
	tAcbPayload examplePayload;
	examplePayload.canadCmd = gControlCmd;
	examplePayload.currentStatus = ENGAGE;

	// gsmp 프로토콜에 맞춰 메세지를 감싼다.
	gsmpWrapper(ACB_SEND_MSG_ID, OK, &examplePayload);

	// 전송을 수행한다.
	sendUartData(&gAcbSendMsg);
}


void sendUartData(tGsmpMsg* msg)
{
    const uint8_t headerSize = sizeof(tGsmpMessageHeader);
    const uint8_t payloadSize = msg->header.msgLen;
    const uint8_t crcSize = sizeof(uint16_t);
    const uint8_t totalLen = headerSize + payloadSize + crcSize;

    uint8_t txBuffer[totalLen];

    // 1. 헤더 복사
    memcpy(txBuffer, &msg->header, headerSize);
    // 2. payload 복사
    memcpy(txBuffer + headerSize, (const uint8_t*) msg->payload, payloadSize);
    // 3. CRC 복사 (Little Endian)
    txBuffer[headerSize + payloadSize + 0] = msg->CRC & 0xFF;
    txBuffer[headerSize + payloadSize + 1] = (msg->CRC >> 8) & 0xFF;

    // 4. 디버깅용 출력
        xil_printf("UART TX (%d bytes): ", totalLen);
        for (int i = 0; i < totalLen; ++i) {
            xil_printf("%02X ", txBuffer[i]);
        }
        xil_printf("\r\n");

    // 5. 전송
    XUartPs_Send(&gUartPs, txBuffer, totalLen);
}
