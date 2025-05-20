#include "uartReceiveTaskRun.h"

/* TODO : 테스트 데이터 예시 크기이므로 추후 리팩토링 필요 #56
 * RECEIVE_SIZE 재정의
 * exampleMsg -> recvUartMsg, 전역변수화
 * */
uint8_t rxBuffer[DATA_RECEIVE_SIZE];
tGsmpMsg exampleMsg;

// TODO : dev merge 후 삭제 필요
int gRecvFlag;
//extern int gRecvFlag;

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

void runUartReceive()
{
	gRecvFlag = TRUE;
	// 1. 수신받은 데이터 패킷의 크기가 맞지 않다면 종료한다.
	// 2. CRC 검사를 수행한다.
	if(!recvUartBytes(rxBuffer, DATA_RECEIVE_SIZE) || !checkCrc(rxBuffer))
	{
		//xil_printf("calced CRC : 0X%x\r\n", calcCrc(rxBuffer, DATA_RECEIVE_SIZE-2));
		// 2.1. CRC 실패 시, gUartMissCount를 증가햔다. 10 이상이면 폭파한다.
		if(++gFailCount[UART_FAIL] >= 10)
		{
			explode();
		}
		return;
	}


	// 3. 기존의 데이터에 payload가 지정되어있다면, 풀어준다.
	if(exampleMsg.payload)
	{
		vPortFree(exampleMsg.payload);
		exampleMsg.payload = NULL;
	}
	// 4. 데이터 파싱을 진행한다.
	//storeParsedUartPacket(rxBuffer, &exampleMsg);
	gsmpUnWrapper(rxBuffer, &exampleMsg);
	if(exampleMsg.header.msgStat == OK)
	{
		gFailCount[UART_FAIL] = 0;
	}
	else
	{
		if(++gFailCount[UART_FAIL] >= 10)
		{
			explode();
		}
	}

	if (exampleMsg.payload && exampleMsg.header.msgId == ACB_ECHO_RECV_MSG_ID) {
		// TODO : 테스트 데이터 예시 크기이므로 추후 리팩토링 필요 #56
		int32_t response = *((int32_t*)exampleMsg.payload);
		xil_printf("responseMsg: %d\r\n", response);
	}
}

void explode()
{
	printf("explode\r\n");
	return ;
}
