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
	tGsmpMsg targetMsg;
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

	uint8_t txBuffer[bufferSizeNoCRC + crcSize]; // CRC 포함
//	test data
//	gControlCmd.x = 1.02;
//	gControlCmd.y = 2.03;
//	gControlCmd.z = 3.04;
	// 헤더 설정
	targetMsg.header.startflag = START_FLAG;
	targetMsg.header.msgId = ACB_SEND_MSG_ID;
	targetMsg.header.srcId = GCU_ID;
	targetMsg.header.destId = ACB_ID;
	targetMsg.header.msgStat = OK;
	targetMsg.header.msgLen = cmdSize;
	targetMsg.payload = &gControlCmd;

	// CRC 계산용 임시 버퍼
	uint8_t tempBuf[bufferSizeNoCRC];
	memcpy(tempBuf, (const uint8_t*) &targetMsg.header, headerSize);
	memcpy(tempBuf + headerSize, (const uint8_t*) targetMsg.payload, sizeof(gControlCmd));
	uint16_t crc = calcCrc(tempBuf, sizeof(tempBuf));
//	xil_printf("tempBuf = %d\r\n", sizeof(tempBuf));
//	xil_printf("Header = %d\r\n", headerSize);   // 예상 7
//	xil_printf("Payload = %d\r\n", cmdSize);         // 예상 24
//	xil_printf("TX total = %d\r\n", bufferSize);           // 예상 33

	// 전송 버퍼 구성
	memcpy(txBuffer, &targetMsg.header, headerSize);
	memcpy(txBuffer + headerSize, (const uint8_t*) targetMsg.payload, cmdSize);
	txBuffer[bufferSizeNoCRC + 0] = crc & 0xFF;        // LSB
	txBuffer[bufferSizeNoCRC + 1] = (crc >> 8) & 0xFF; // MSB

	// 전송
	sendData(txBuffer, sizeof(txBuffer));
}


void sendData(uint8_t* buffer, uint16_t len)
{
    // len 바이트만큼 UART로 전송
    XUartPs_Send(&gUartPs, buffer, len);
//    xil_printf("\r\n");
    //xil_printf("TX: 0x%02X %02X %02X %02X\n", buffer[0], buffer[1], buffer[2], buffer[3]);
}
