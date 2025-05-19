#include "global.h"
#include "gsgTypes.h"
#include "xil_printf.h"
#include "crc.h"
#include "xuartps.h"
#include "xparameters.h"

static int recvFlag = TRUE;
extern int gRecvMissCount;


static XUartPs Uart_Ps;

void uartSendDataRun();
void explode();		// TODO : ����
int initUartPs();
void sendData(uint8_t* buffer, uint16_t len);

void uartSendTaskMain(void *pvParameters) {
	// TODO : initTask #50
	//xil_printf("1");
	int uartStatus = initUartPs();
	if(uartStatus == XST_FAILURE)
	{
		xil_printf("UART send task failed!\n");
	}
	for(;;)
	{
		//xil_printf("2");
		//ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		uartSendDataRun();
	}
}

void uartSendDataRun()
{
	//xil_printf("3");
	tGsmpMsg targetMsg;
	// 1. gIsUartReceive�� True���� Ȯ���Ѵ�.
	if (recvFlag != FALSE)
	{
		//xil_printf("4");
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
		//xil_printf("5");
		// CRC ���� �ӽ� ����
		uint8_t tempBuf[sizeof(tGsmpMessageHeader) + sizeof(gControlCmd)];
		memcpy(tempBuf, (const uint8_t*) &targetMsg.header, sizeof(tGsmpMessageHeader));
		memcpy(tempBuf + sizeof(tGsmpMessageHeader), (const uint8_t*) targetMsg.payload, sizeof(gControlCmd));
		uint16_t crc = calcCrc(tempBuf, sizeof(tempBuf));
		//xil_printf("6");
		xil_printf("Header = %d\n", sizeof(tGsmpMessageHeader));   // ���� 7
		xil_printf("Payload = %d\n", sizeof(gControlCmd));         // ���� 24
		xil_printf("TX total = %d\n", sizeof(txBuffer));           // ���� 33

		// ���� ���� ����
		memcpy(txBuffer, &targetMsg.header, sizeof(tGsmpMessageHeader));
		memcpy(txBuffer + sizeof(tGsmpMessageHeader), (const uint8_t*) targetMsg.payload, sizeof(gControlCmd));
		txBuffer[sizeof(tGsmpMessageHeader) + sizeof(gControlCmd) + 0] = crc & 0xFF;        // LSB
		txBuffer[sizeof(tGsmpMessageHeader) + sizeof(gControlCmd) + 1] = (crc >> 8) & 0xFF; // MSB
		xil_printf("%d\n", sizeof(txBuffer));
		// ����
		sendData(txBuffer, sizeof(txBuffer));
		vTaskDelay(100);
		}
}

int initUartPs()
{
	// UART ���� ������ ���� ����ü ������ ����
    XUartPs_Config *Config;
    int Status;

    // UART ����̽� ID�� �ش��ϴ� ���� ������ �˻��ؼ� Config�� ����
    // �� �������� BaseAddress, BaudRate ���� �ϵ���� ������ ���Ե�
    Config = XUartPs_LookupConfig(XPAR_PS7_UART_1_DEVICE_ID);
    if (Config == NULL)
    {
    	xil_printf("Config is NULL\n");
    	return XST_FAILURE;
    }

    Status = XUartPs_CfgInitialize(&Uart_Ps, Config, Config->BaseAddress);
    if (Status != XST_SUCCESS)
    {
    	xil_printf("State is FAIL\n");
    	return XST_FAILURE;
   	}

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
