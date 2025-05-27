#include "uartSendTaskRun.h"


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

#ifdef TEST
	gControlCmd.x = 1.02;
	gControlCmd.y = 3.04;
	gControlCmd.z = 5.06;
	// example tAcbPayload
	tAcbPayload examplePayload;
	examplePayload.canadCmd = gControlCmd;
	examplePayload.currentStatus = ENGAGE;
	// gsmp 프로토콜에 맞춰 메세지를 감싼다.
	gsmpWrapper(ACB_SEND_MSG_ID, OK, &examplePayload);
#else
	//gsmpWrapper(ACB_SEND_MSG_ID, OK, &g);
#endif
	// 전송을 수행한다.
	sendUartData();
}

void sendUartData()
{
#ifdef DEBUG_UART_SEND
    // 4. 디버깅용 출력
//        xil_printf("UART TX (%d bytes): ", totalLen);
//        for (int i = 0; i < totalLen; ++i) {
//            xil_printf("%02X ", txBuffer[i]);
//        }
//        xil_printf("\r\n");
#endif
    // 5. 전송
    XUartPs_Send(&gUartPs, gSendBuffer, ACB_SEND_MSG_SIZE);
}


