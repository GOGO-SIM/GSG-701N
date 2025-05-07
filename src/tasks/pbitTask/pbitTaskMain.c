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
	 * 1. 전압 검사
	 * 2. 클럭 검사
	 */
	/*****************************[임시 구현]*****************************/
	// 1. 전압 검사
	if (checkPower())
	{
		// 2. 클럭 검사
		checkClock();
	}
	return ;
}

static void checkUart()
{
	// 1. physical loopback 검사
	// 2. echoing 검사
}

static void checkTcp()
{
	// 1. L1 검사
	// 2. L2 검사
	// 3. L3 검사
	// 4. L4 검사
	// 5. echoing 검사
}

static void inspectNetwork()
{
	/**
	 * 1. UART를 검사한다. (physical loopback, echoing)
	 * 2. TCP/IP와 UDP/IP를 검사한다. (L1, L2, L3, APP)
	 */

	/*****************************[임시 구현]*****************************/
	checkUart();
	checkTcp();

}

void pbitTaskMain( void *pvParameters )
{
	/**
	 * [PBIT 검사 수행]
	 * 1. 보드를 검사한다.
	 * 2. 연결된 기기의 네트워크를 검사한다.
	 * 3. 텔레메트리 데이터 생성
	 * 4. CBIT과 stanby, telemetry task를 깨운다.
	 * 5. PBIT task를 삭제한다.
	 */

	/*****************************[임시 구현]*****************************/
	// 1. 보드를 검사한다.
	inspectBoard();
	if (sPbitResultFlag == false)
	{
		// 2. 연결된 기기의 네트워크를 검사한다.
		inspectNetwork();
	}

	// 3. 텔레메트리 데이터 생성
	// pushPbitDataIntoQueue(sPbitResultFlag)

	// 4. CBIT과 stanby task를 깨운다.
	// xTaskResume(xHandleCbit);
	// xTaskResume(xHandleStanby);

	// 5. pbit task를 삭제한다.
	// xTaskDelete(xHandlePbit);
}
