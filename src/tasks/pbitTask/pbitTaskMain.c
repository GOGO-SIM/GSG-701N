

static int sPbitFail;
//static ??? gImuData;
//static ??? gSeekerData;

static void imuRxCallback(){
	//imu 데이터 받아오기
}

static void seekerRxCallback(){
	//seeker 데이터 받아오기
}

static void timerOvfCallback(){
	sPbitFail |= setErrorFlag;
}

static int checkPower()
{
	/**
	 * [checkPower]
	 *
	 * 1. XADC 초기화
	 * 2. XADC 채널 선택
	 * 3. XADC 변환 대기
	 * 4. 값 읽기
	 * 5. 임계값 비교
	 *
	 */
	/*****************************[임시 구현]*****************************/

	//1. XADC 초기화

	//2. XADC 채널 선택

	//3. XADC 변환 대기
	while (!xADC_IsConversionDone());

	//4. 값 읽기
	int rawValue = xADC_GetData();
    double measuredVoltage = xADC_ConvertToVoltage(rawValue);

	//5.임계값 비교
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
	 * 1. UART 초기화
	 * 2. 송신 버퍼 준비
	 * 3. 데이터 송신
	 * 4. 데이터 수신 대기
	 * 5. 수신된 데이터 읽기
	 * 6. 송신 데이터와 수신 데이터 비교
	 *
	 */
	/*****************************[임시 구현]*****************************/

	// 1. UART 초기화
	UartInit();

	// 2. 송신 버퍼 준비
	char txData = 'A';

	// 3. 데이터 송신
	UartSend(txData);

	// 4. 데이터 수신 대기
	while (!UartIsDataAvailable());

	// 5. 수신된 데이터 읽기
	char rxData = UartReceive();

	// 6. 송신 데이터와 수신 데이터 비교
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
	 * 2. 데이터 수신 대기
	 * 3. Imu / Seeker 데이터 도착시 인터럽트 발생 후 수신
	 * 4. 데이터 수신 후 비교
	 * 5. 타이머 종료 및 초기화
	 *
	 */
	/*****************************[임시 구현]*****************************/

	//1.Timer Start
	//타이머 초기화 및 시작

	//2.데이터 수신 대기

	while(); //둘다 도착하거나, 에러 발생시 까지 대기 설계하기 나름
	//timerOvfCallback()발생시 에러 발생 flag set

	//3.Imu / Seeker 데이터 도착시 인터럽트 발생 후 수신
	//imuRxCallback() 과 seekerRxCallback()에 정의

	//4.데이터 수신 후 비교
	//-> 받아야 할 값이 왔는지 확인

	//5.타이머 종료 및 초기화
}

void pbitTaskMain( void *pvParameters )
{
	/**
	 * [PBIT 검사 수행]
	 *
	 * 1. 전압을 체크한다.
	 * 2. Uart 상태를 체크한다.
	 * 3. 연결된 기기의 네트워크를 검사한다.
	 * 4. 다음 task를 깨운다.
	 * 5. PBIT task를 삭제한다.
	 *
	 */

	/*****************************[임시 구현]*****************************/

	// 1. 전압을 체크한다.
	checkPower();

	if (!sPbitFail)
	{
	// 2. Uart 상태를 체크한다
		checkUart();

	// 3. 연결된 기기의 네트워크를 검사한다.
		checkNetwork();
	}

	// 4. 다음task를 깨운다.
	if (!sPbitFail)
	{
	// xTaskResume(콜드런치 점화 대기 상태 태스크);
	}
	else
	{
	// xTaskResume(PBIT 실패 제어 차단 상태 태스크);
	}

	// 5. pbit task를 삭제한다.
	// xTaskDelete(xHandlePbit);
}
