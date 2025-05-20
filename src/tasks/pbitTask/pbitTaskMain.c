#include "global.h"

#define ECHOING_VALUE 19980398

static int sPassPbitFlag; // ok == TRUE, false == FALSE
//static ??? gImuData;
//static ??? gSeekerData;

void UartInit();

static void imuRxCallback(){
	//imu ������ �޾ƿ���
}

static void seekerRxCallback(){
	//seeker ������ �޾ƿ���
}

static void timerOvfCallback(){
	// ERR : pbit Fail Success check
	//gGcuStatus |= setErrorFlag;
}

void UartSend(int txData);
int UartReceive();
int UartIsDataAvailable();
int xADC_GetData();
double xADC_ConvertToVoltage(int rawValue);

static int checkPower()
{
	/**
	 * [checkPower]
	 *
	 * 1. XADC �ʱ�ȭ
	 * 2. XADC ä�� ����
	 * 3. XADC ��ȯ ���
	 * 4. �� �б�
	 * 5. �Ӱ谪 ��
	 *
	 */
	/*****************************[�ӽ� ����]*****************************/

	//1. XADC �ʱ�ȭ

	//2. XADC ä�� ����

	//3. XADC ��ȯ ���
	//while (!xADC_IsConversionDone());

	//4. �� �б�
	int rawValue = xADC_GetData();
    double measuredVoltage = xADC_ConvertToVoltage(rawValue);

	//5.�Ӱ谪 ��
	if (measuredVoltage < 4.5)
	{
		// ERR : pbit Fail Success check
		//sPbitFail |= setErrorFlag;
	}
}

static void checkUart()
{
	/**
	 * [checkPower]
	 *
	 * 1. UART �ʱ�ȭ
	 * 2. �۽� ���� �غ�
	 * 3. ������ �۽�
	 * 4. ������ ���� ���
	 * 5. ���ŵ� ������ �б�
	 * 6. �۽� �����Ϳ� ���� ������ ��
	 *
	 */
	/*****************************[�ӽ� ����]*****************************/

	// 1. UART �ʱ�ȭ
	UartInit();

	// TODO: loop back �˻� �߰��ϱ�(optional)

	// 2. �۽� ���� �غ�
	int txData = ECHOING_VALUE;

	// 3. ������ �۽�
	UartSend(txData);

	// 4. ������ ���� ���
	while (!UartIsDataAvailable());

	// 5. ���ŵ� ������ �б�
	int rxData = UartReceive();

	// 6. �۽� �����Ϳ� ���� ������ ��
	if (rxData != txData)
	{
		// ERR : pbit Fail Success check
	   //sPbitFail |= setErrorFlag;
	}
}

static void checkNetwork(){

	/**
	 * [checkNetwork]
	 *
	 * 1. Timer Start
	 * 2. ������ ���� ���
	 * 3. Imu / Seeker ������ ������ ���ͷ�Ʈ �߻� �� ����
	 * 4. ������ ���� �� ��
	 * 5. Ÿ�̸� ���� �� �ʱ�ȭ
	 *
	 */
	/*****************************[�ӽ� ����]*****************************/

	//1.Timer Start
	//Ÿ�̸� �ʱ�ȭ �� ����

	//2.������ ���� ���

	for(;;){}
	//�Ѵ� �����ϰų�, ���� �߻��� ���� ��� �����ϱ� ����
	//timerOvfCallback()�߻��� ���� �߻� flag set
	// -> UDP rx �Լ��� timeout �� �� ���� �� ����.

	//3.Imu / Seeker ������ ������ ���ͷ�Ʈ �߻� �� ����
	//imuRxCallback() �� seekerRxCallback()�� ����

	//4.������ ���� �� ��
	//-> �޾ƾ� �� ���� �Դ��� Ȯ��

	//5.Ÿ�̸� ���� �� �ʱ�ȭ

	// [���� ����] TODO: ������ �� �½�ũ���� ��������, PBIT �ÿ��� polling ������� ó������
	// ERR : IMU_RX & SEEKER_RX ����
	/*
	while (!IMU_RX && !SEEKER_RX)
	{
		// receive(buffer, timeout);
		// �޽��� ��� �˻�(CRC����)
		// ���̷ε� ���� �� Ȯ��
		// pbit ��� ����
		//	-> timeout �� ��Ʈ��ũ ������ ����
	}
	*/
}

void pbitTaskMain( void *pvParameters )
{

	xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));

	/**
	 * [PBIT �˻� ����]
	 *
	 * 1. ������ üũ�Ѵ�.
	 * 2. Uart ���¸� üũ�Ѵ�.
	 * 3. ����� ����� ��Ʈ��ũ�� �˻��Ѵ�.
	 * 4. ���� task�� �����.
	 * 5. PBIT task�� �����Ѵ�.
	 *
	 */

	/*****************************[�ӽ� ����]*****************************/

//	// 1. ������ üũ�Ѵ�.
//	checkPower();
//
//	if (sPassPbitFlag == TRUE)
//	{
//	// 2. Uart ���¸� üũ�Ѵ�
//		checkUart();
//
//	// 3. ����� ����� ��Ʈ��ũ�� �˻��Ѵ�.
//		checkNetwork();
//	}
//
//	// 4. ����task�� �����.
//	if (sPassPbitFlag == TRUE)
//	{
//	// xTaskResume(�ݵ己ġ ��ȭ ��� ���� �½�ũ);
//	}
//	else
//	{
//	// xTaskResume(PBIT ���� ���� ���� ���� �½�ũ);
//	}
//
//	// 5. pbit task�� �����Ѵ�.
	xil_printf("DEL -- %s\r\n", pcTaskGetName(NULL));
	xTaskNotifyGive(xStanbyIgnitionTaskHandle);
	vTaskDelete(NULL);
}

void UartInit()
{
	return;
}

void UartSend(int txData)
{
	return;
}

int UartReceive()
{
	return TRUE;
}

int UartIsDataAvailable()
{
	return TRUE;
}

int xADC_GetData()
{
	return 1;
}

double xADC_ConvertToVoltage(int rawValue)
{
	return 0.0f;
}
