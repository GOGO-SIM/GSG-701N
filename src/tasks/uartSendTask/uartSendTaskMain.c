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

	gControlCmd.x = 1.42;
	gControlCmd.y = 3.14;
	gControlCmd.z = 2.38;

	// example tAcbPayload
	tAcbPayload examplePayload;
	examplePayload.canadCmd = gControlCmd;
	examplePayload.currentStatus = ENGAGE;

	// gsmp 프로토콜에 맞춰 메세지를 감싼다.
	gsmpWrapper(ACB_SEND_MSG_ID, OK, &examplePayload);

	// 전송을 수행한다.
	sendUartData();
}


void sendUartData()
{
    // 5. 전송
    XUartPs_Send(&gUartPs, gSendBuffer, ACB_SEND_MSG_SIZE);
}
