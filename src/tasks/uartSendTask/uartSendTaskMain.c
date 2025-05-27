#include "uartSendTaskRun.h"

void uartSendTaskMain(void *pvParameters) {
	for(;;)
	{
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
        runUartSend();
	}
}
<<<<<<< HEAD
=======

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
>>>>>>> d1d26eabd61b20933e5c4174cf00dbb25161c4e7
