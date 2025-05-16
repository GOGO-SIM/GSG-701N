#include "global.h"
#include "gsgTypes.h"
#include "xil_printf.h"
#include "crc.h"
#include "xuartps.h"
#include "xparameters.h"

static int recvFlag = TRUE;
extern int gRecvMissCount;


static XUartPs Uart_Ps;

void explode();
int initUartPs();
void sendData(uint8_t* buffer, uint16_t len);

void uartSendTaskMain(void *pvParameters) {
	//xil_printf("uart send task ready");
	if(initUartPs == FALSE)
	{
		printf("uart send task failed!\n");
	}
	initUartPs();
	for(;;)
	{
		//xil_printf("1\n");
		tGsmpMsg targetMsg;
		// 1. gIsUartReceive가 True인지 확인한다.
		if (recvFlag != FALSE)
		{

			uint8_t txBuffer[sizeof(tGsmpMessageHeader) + sizeof(gControlCmd) + 2]; // CRC 포함

			// test data
			gControlCmd.x = 1.02;
			gControlCmd.y = 2.03;
			gControlCmd.z = 3.04;

			// 헤더 설정
			targetMsg.header.startflag = START_FLAG;
			targetMsg.header.msgId = ACB_MSG_ID;
			targetMsg.header.srcId = GCU_ID;
			targetMsg.header.destId = ACB_ID;
			targetMsg.header.msgStat = OK;
			targetMsg.header.msgLen = sizeof(gControlCmd);
			targetMsg.payload = &gControlCmd;

			// CRC 계산용 임시 버퍼
			uint8_t tempBuf[sizeof(tGsmpMessageHeader) + sizeof(gControlCmd)];
			memcpy(tempBuf, (const uint8_t*) &targetMsg.header, sizeof(tGsmpMessageHeader));
			memcpy(tempBuf + sizeof(tGsmpMessageHeader), (const uint8_t*) targetMsg.payload, sizeof(gControlCmd));
			uint16_t crc = calcCrc(tempBuf, sizeof(tempBuf));

			// 전송 버퍼 구성
			memcpy(txBuffer, &targetMsg.header, (const uint8_t*) sizeof(tGsmpMessageHeader));
			memcpy(txBuffer + sizeof(tGsmpMessageHeader), (const uint8_t*) targetMsg.payload, sizeof(gControlCmd));
			txBuffer[sizeof(tGsmpMessageHeader) + sizeof(gControlCmd) + 0] = crc & 0xFF;        // LSB
			txBuffer[sizeof(tGsmpMessageHeader) + sizeof(gControlCmd) + 1] = (crc >> 8) & 0xFF; // MSB


			// 전송
			sendData(txBuffer, sizeof(txBuffer));
			vTaskDelay(100);
		}
	}
}

int initUartPs()
{

    XUartPs_Config *Config;
    int Status;
    // 드라이버가 사용할 정적 구성 정보 반환, device ID 에 해당하는 설정 구조체를 찾아서 반환
    Config = XUartPs_LookupConfig(XPAR_PS7_UART_1_DEVICE_ID);
    if (Config == NULL) return XST_FAILURE;

    // 인스턴스 초기화
    Status = XUartPs_CfgInitialize(&Uart_Ps, Config, Config->BaseAddress);
    if (Status != XST_SUCCESS) return XST_FAILURE;

    // 인스턴스 초기화 시, default 값이19200bps이기 때문에 필요 시 명시
    XUartPs_SetBaudRate(&Uart_Ps, UART_BAUD);
    return XST_SUCCESS;
}


void sendData(uint8_t* buffer, uint16_t len)
{
    // len 바이트만큼 UART로 전송
    XUartPs_Send(&Uart_Ps, buffer, len);
    //xil_printf("TX: 0x%02X %02X %02X %02X\n", buffer[0], buffer[1], buffer[2], buffer[3]);

}
