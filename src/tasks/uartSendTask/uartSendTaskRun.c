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

	tAcbPayload acbSendPayload;
	acbSendPayload.canadCmd = gControlCmd;
	acbSendPayload.currentStatus = gModeStatus;

#ifdef TEST
	gControlCmd.x = 1.02;
	gControlCmd.y = 3.04;
	gControlCmd.z = 5.06;
	// example tAcbPayload
	acbSendPayload.canadCmd = gControlCmd;
	acbSendPayload.currentStatus = ENGAGE;
#endif
	gsmpWrapper(ACB_SEND_MSG_ID, OK, &acbSendPayload);
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


