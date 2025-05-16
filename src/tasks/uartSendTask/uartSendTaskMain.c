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
		// 1. gIsUartReceive�� True���� Ȯ���Ѵ�.
		if (recvFlag != FALSE)
		{

			uint8_t txBuffer[sizeof(tGsmpMessageHeader) + sizeof(gControlCmd) + 2]; // CRC ����

			// test data
			gControlCmd.x = 1.02;
			gControlCmd.y = 2.03;
			gControlCmd.z = 3.04;

			// ��� ����
			targetMsg.header.startflag = START_FLAG;
			targetMsg.header.msgId = ACB_MSG_ID;
			targetMsg.header.srcId = GCU_ID;
			targetMsg.header.destId = ACB_ID;
			targetMsg.header.msgStat = OK;
			targetMsg.header.msgLen = sizeof(gControlCmd);
			targetMsg.payload = &gControlCmd;

			// CRC ���� �ӽ� ����
			uint8_t tempBuf[sizeof(tGsmpMessageHeader) + sizeof(gControlCmd)];
			memcpy(tempBuf, (const uint8_t*) &targetMsg.header, sizeof(tGsmpMessageHeader));
			memcpy(tempBuf + sizeof(tGsmpMessageHeader), (const uint8_t*) targetMsg.payload, sizeof(gControlCmd));
			uint16_t crc = calcCrc(tempBuf, sizeof(tempBuf));

			// ���� ���� ����
			memcpy(txBuffer, &targetMsg.header, (const uint8_t*) sizeof(tGsmpMessageHeader));
			memcpy(txBuffer + sizeof(tGsmpMessageHeader), (const uint8_t*) targetMsg.payload, sizeof(gControlCmd));
			txBuffer[sizeof(tGsmpMessageHeader) + sizeof(gControlCmd) + 0] = crc & 0xFF;        // LSB
			txBuffer[sizeof(tGsmpMessageHeader) + sizeof(gControlCmd) + 1] = (crc >> 8) & 0xFF; // MSB


			// ����
			sendData(txBuffer, sizeof(txBuffer));
			vTaskDelay(100);
		}
	}
}

int initUartPs()
{

    XUartPs_Config *Config;
    int Status;
    // ����̹��� ����� ���� ���� ���� ��ȯ, device ID �� �ش��ϴ� ���� ����ü�� ã�Ƽ� ��ȯ
    Config = XUartPs_LookupConfig(XPAR_PS7_UART_1_DEVICE_ID);
    if (Config == NULL) return XST_FAILURE;

    // �ν��Ͻ� �ʱ�ȭ
    Status = XUartPs_CfgInitialize(&Uart_Ps, Config, Config->BaseAddress);
    if (Status != XST_SUCCESS) return XST_FAILURE;

    // �ν��Ͻ� �ʱ�ȭ ��, default ����19200bps�̱� ������ �ʿ� �� ���
    XUartPs_SetBaudRate(&Uart_Ps, UART_BAUD);
    return XST_SUCCESS;
}


void sendData(uint8_t* buffer, uint16_t len)
{
    // len ����Ʈ��ŭ UART�� ����
    XUartPs_Send(&Uart_Ps, buffer, len);
    //xil_printf("TX: 0x%02X %02X %02X %02X\n", buffer[0], buffer[1], buffer[2], buffer[3]);

}
