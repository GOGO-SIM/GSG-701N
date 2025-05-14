static int sPassCbitFlag = TRUE; // ��� == TRUE, ���� == FALSE

static void checkPower()
{
	/*
	 * 1. XADC ��ȯ ���
	 * 2. �� �б�
	 * 3. �Ӱ谪 ��
	 * �ʱ�ȭ ������ �̹� PBIT���� �����Ͽ������� �� ���� ����
	 */
	/*****************************[�ӽ� ����]*****************************/
	//1. XADC ��ȯ ���
	while (!xADC_IsConversionDone());

	//2. �� �б�
	int rawValue = xADC_GetData();
    double measuredVoltage = xADC_ConvertToVoltage(rawValue);

	//3.�Ӱ谪 ��
	if (measuredVoltage < Ư�� ��)
	{
		sPassCbitFlag == FALSE;
	}
}

static void checkRegister()
{
	/*
	 * 1. ��緹������ �˻�
	 */
	/*****************************[�ӽ� ����]*****************************/

	// 1. ��緹������ �˻�
	if (���������� �ϳ��� �̻��ϸ�)

	// �������Ͱ� �̻��Ҷ� set �Ǵ���, �����϶� set �Ǵ��� ���� �Ǵ� �Ұ�

	{
		sPassCbitFlag = FALSE
	}
}

void cbitTaskMain( void *pvParameters )
{
	/*
	 * 1. ���� �˻�
	 * 2. �������� �˻�
	 * 3. �÷��� �˻� �� ����
	 */
	/*****************************[�ӽ� ����]*****************************/

	vTaskSuspend();
	while(1)
	{
		// 1. ���� �˻�
		checkPower();
		if ( sPassCbitFlag == TRUE )
		{
		// 2. �������� �˻�
			checkRegister();
		}
		// 3. �÷��� �˻� �� ����
		if ( sPassCbitFlag == FALSE )
		{
			//�����Ѵ�.
		}
		vTaskResume(���� �½�ũ);
		vTaskSuspend();
	}

}
