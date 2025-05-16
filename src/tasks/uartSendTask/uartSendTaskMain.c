#include "global.h"
#include "gsgTypes.h"

static int recvFlag;
extern int gRecvMissCount;

void explode();
void setCrc(tGsmpMessageHeader* acbMsg, tGsmpMessageHeader* fullMsgBuffer);
void sendData(uint32_t* buffer);

void run()
{
    
}

void uartSendTaskMain(void *pvParameters) {
	tGsmpMessageHeader acbMsg;
	uint32_t* buffer;

    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
		run();
    }
	// 1. gIsUartReceive�� True���� Ȯ���Ѵ�.
	if (recvFlag == FALSE) {
		// 1.1. gIsUartReceive == False�� ��� recvMissCount +=1;
		// 1.1.1. recvMissCount >= 10 �̶�� �����Ѵ�.
		if (++gFailCount[UART_FAIL] >= 10) {
			explode();
		}
	}
	// 2. recvMissCount = 0;
	gFailCount[UART_FAIL] = 0;
	// 3. messageStatus�� 0���� �����Ѵ�.
	acbMsg.msgStat = OK;
	// 4. CRC-16/XMODEM ������ �����Ѵ�.
	// ERR : not declared -> declared in crc.h
	//calcCrc(acbMsg, &buffer);
	// 5. UART �۽��� �����Ѵ�.
	sendData(buffer);

}

void sendData(uint32_t* buffer)
{
	return;
}
