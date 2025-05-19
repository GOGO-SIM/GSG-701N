#include "global.h"
#include "gsgTypes.h"
#include "xil_printf.h"
#include "crc.h"
#include "xuartps.h"
#include "xparameters.h"

extern int gRecvMissCount;


static XUartPs Uart_Ps;
static const uint8_t headerSize = sizeof(tGsmpMessageHeader);
static const uint8_t cmdSize = sizeof(gControlCmd);
static const uint8_t crcSize = 2;
static const uint8_t bufferSizeNoCRC = sizeof(tGsmpMessageHeader) + sizeof(gControlCmd);


void uartSendDataRun();
void explode();		// TODO : 삭제
int initUartPs();
void sendData(uint8_t* buffer, uint16_t len);

static void run()
{
    
}

void uartSendTaskMain(void *pvParameters) {
	// TODO : initTask #50

	int uartStatus = initUartPs();
	if(uartStatus == XST_FAILURE)
	{
		xil_printf("UART send task failed!\n");
	}
	for(;;)
	{
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
		uartSendDataRun();
	}
}

void uartSendDataRun()
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
	gControlCmd.x = 1.02;
	gControlCmd.y = 2.03;
	gControlCmd.z = 3.04;
	// 헤더 설정
	targetMsg.header.startflag = START_FLAG;
	targetMsg.header.msgId = ACB_MSG_ID;
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

//	xil_printf("Header = %d\r\n", headerSize);   // 예상 7
//	xil_printf("Payload = %d\r\n", cmdSize);         // 예상 24
//	xil_printf("TX total = %d\r\n", bufferSize);           // 예상 33

	// 전송 버퍼 구성
	memcpy(txBuffer, &targetMsg.header, headerSize);
	memcpy(txBuffer + headerSize, (const uint8_t*) targetMsg.payload, cmdSize);
	txBuffer[bufferSizeNoCRC + 0] = crc & 0xFF;        // LSB
	txBuffer[bufferSizeNoCRC + 1] = (crc >> 8) & 0xFF; // MSB

	xil_printf("TX total = %d\r\n", sizeof(txBuffer));

	// 전송
	sendData(txBuffer, sizeof(txBuffer));
}

int initUartPs()
{
	// UART 설정 정보를 담을 구조체 포인터 선언
    XUartPs_Config *Config;
    int Status;

    // UART 디바이스 ID에 해당하는 설정 정보를 검색해서 Config에 저장
    // 이 정보에는 BaseAddress, BaudRate 등 하드웨어 정보 포함
    Config = XUartPs_LookupConfig(XPAR_PS7_UART_1_DEVICE_ID);
    if (Config == NULL)
    {
    	xil_printf("Config is NULL\r\n");
    	return XST_FAILURE;
    }

    Status = XUartPs_CfgInitialize(&Uart_Ps, Config, Config->BaseAddress);
    if (Status != XST_SUCCESS)
    {
    	xil_printf("State is FAIL\r\n");
    	return XST_FAILURE;
   	}

    // 인스턴스 초기화 시, default 값이19200bps이기 때문에 필요 시 명시
    XUartPs_SetBaudRate(&Uart_Ps, UART_BAUD);
    return XST_SUCCESS;
}


void sendData(uint8_t* buffer, uint16_t len)
{
    // len 바이트만큼 UART로 전송
    XUartPs_Send(&Uart_Ps, buffer, len);
    xil_printf("\r\n");
    //xil_printf("TX: 0x%02X %02X %02X %02X\n", buffer[0], buffer[1], buffer[2], buffer[3]);
}
