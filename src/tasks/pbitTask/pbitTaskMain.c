static int sPbitResultFlag;

static int checkPower()
{
	sPbitResultFlag |= POWER_ERROR;
}

static int checkClock()
{
	sPbitResultFlag |= CLOCK_ERROR;
}

static void inspectBoard()
{
	/**
	 * 1. ���� �˻�
	 * 2. Ŭ�� �˻�
	 */
	/*****************************[�ӽ� ����]*****************************/
	// 1. ���� �˻�
	if (checkPower())
	{
		// 2. Ŭ�� �˻�
		checkClock();
	}
	return ;
}

static void checkUart()
{
	// 1. physical loopback �˻�
	// 2. echoing �˻�
}

static void checkTcp()
{
	// 1. L1 �˻�
	// 2. L2 �˻�
	// 3. L3 �˻�
	// 4. L4 �˻�
	// 5. echoing �˻�
}

static void inspectNetwork()
{
	/**
	 * 1. UART�� �˻��Ѵ�. (physical loopback, echoing)
	 * 2. TCP/IP�� UDP/IP�� �˻��Ѵ�. (L1, L2, L3, APP)
	 */

	/*****************************[�ӽ� ����]*****************************/
	checkUart();
	checkTcp();

}

void pbitTaskMain( void *pvParameters )
{
	/**
	 * [PBIT �˻� ����]
	 * 1. ���带 �˻��Ѵ�.
	 * 2. ����� ����� ��Ʈ��ũ�� �˻��Ѵ�.
	 * 3. �ڷ���Ʈ�� ������ ����
	 * 4. CBIT�� stanby, telemetry task�� �����.
	 * 5. PBIT task�� �����Ѵ�.
	 */

	/*****************************[�ӽ� ����]*****************************/
	// 1. ���带 �˻��Ѵ�.
	inspectBoard();
	if (sPbitResultFlag == false)
	{
		// 2. ����� ����� ��Ʈ��ũ�� �˻��Ѵ�.
		inspectNetwork();
	}

	// 3. �ڷ���Ʈ�� ������ ����
	// pushPbitDataIntoQueue(sPbitResultFlag)

	// 4. CBIT�� stanby task�� �����.
	// xTaskResume(xHandleCbit);
	// xTaskResume(xHandleStanby);

	// 5. pbit task�� �����Ѵ�.
	// xTaskDelete(xHandlePbit);
}
