#include "global.h"
#include "gsgTypes.h"

static int recvFlag;
extern int gRecvMissCount;

void explode();
void setCrc(tGsmpMessageHeader* acbMsg, tGsmpMessageHeader* fullMsgBuffer);
void sendData(uint32_t* buffer);

static void run()
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
	// 1. gIsUartReceive占쏙옙 True占쏙옙占쏙옙 확占쏙옙占싼댐옙.
	if (recvFlag == FALSE) {
		// 1.1. gIsUartReceive == False占쏙옙 占쏙옙占� recvMissCount +=1;
		// 1.1.1. recvMissCount >= 10 占싱띰옙占� 占쏙옙占쏙옙占싼댐옙.
		if (++gFailCount[UART_FAIL] >= 10) {
			explode();
		}
	}
	// 2. recvMissCount = 0;
	gFailCount[UART_FAIL] = 0;
	// 3. messageStatus占쏙옙 0占쏙옙占쏙옙 占쏙옙占쏙옙占싼댐옙.
	acbMsg.msgStat = OK;
	// 4. CRC-16/XMODEM 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占싼댐옙.
	// ERR : not declared -> declared in crc.h
	//calcCrc(acbMsg, &buffer);
	// 5. UART 占쌜쏙옙占쏙옙 占쏙옙占쏙옙占싼댐옙.
	sendData(buffer);

}

void sendData(uint32_t* buffer)
{
	return;
}
