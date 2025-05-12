

static int sPbitFail;
//static ??? gImuData;
//static ??? gSeekerData;

static void imuRxCallback(){
	//imu ������ �޾ƿ���
}

static void seekerRxCallback(){
	//seeker ������ �޾ƿ���
}

static void timerOvfCallback(){
	sPbitFail |= setErrorFlag;
}

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
	while (!xADC_IsConversionDone());

	//4. �� �б�
	int rawValue = xADC_GetData();
    double measuredVoltage = xADC_ConvertToVoltage(rawValue);

	//5.�Ӱ谪 ��
	if (measuredVoltage < 4.5)
	{
		sPbitFail |= setErrorFlag;
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

	// 2. �۽� ���� �غ�
	char txData = 'A';

	// 3. ������ �۽�
	UartSend(txData);

	// 4. ������ ���� ���
	while (!UartIsDataAvailable());

	// 5. ���ŵ� ������ �б�
	char rxData = UartReceive();

	// 6. �۽� �����Ϳ� ���� ������ ��
	if (rxData != txData)
	{
	   sPbitFail |= setErrorFlag;
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

	while(); //�Ѵ� �����ϰų�, ���� �߻��� ���� ��� �����ϱ� ����
	//timerOvfCallback()�߻��� ���� �߻� flag set

	//3.Imu / Seeker ������ ������ ���ͷ�Ʈ �߻� �� ����
	//imuRxCallback() �� seekerRxCallback()�� ����

	//4.������ ���� �� ��
	//-> �޾ƾ� �� ���� �Դ��� Ȯ��

	//5.Ÿ�̸� ���� �� �ʱ�ȭ
}

void pbitTaskMain( void *pvParameters )
{
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

	// 1. ������ üũ�Ѵ�.
	checkPower();

	if (!sPbitFail)
	{
	// 2. Uart ���¸� üũ�Ѵ�
		checkUart();

	// 3. ����� ����� ��Ʈ��ũ�� �˻��Ѵ�.
		checkNetwork();
	}

	// 4. ����task�� �����.
	if (!sPbitFail)
	{
	// xTaskResume(�ݵ己ġ ��ȭ ��� ���� �½�ũ);
	}
	else
	{
	// xTaskResume(PBIT ���� ���� ���� ���� �½�ũ);
	}

	// 5. pbit task�� �����Ѵ�.
	// xTaskDelete(xHandlePbit);
}
