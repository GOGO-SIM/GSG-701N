static int sPassCbitFlag = TRUE; // 통과 == TRUE, 실패 == FALSE

static void checkPower()
{
	/*
	 * 1. XADC 변환 대기
	 * 2. 값 읽기
	 * 3. 임계값 비교
	 * 초기화 관련은 이미 PBIT에서 진행하였음으로 더 하지 않음
	 */
	/*****************************[임시 구현]*****************************/
	//1. XADC 변환 대기
	while (!xADC_IsConversionDone());

	//2. 값 읽기
	int rawValue = xADC_GetData();
    double measuredVoltage = xADC_ConvertToVoltage(rawValue);

	//3.임계값 비교
	if (measuredVoltage < 특정 값)
	{
		sPassCbitFlag == FALSE;
	}
}

static void checkRegister()
{
	/*
	 * 1. 모든레지스터 검사
	 */
	/*****************************[임시 구현]*****************************/

	// 1. 모든레지스터 검사
	if (레지스터중 하나라도 이상하면)

	// 레지스터가 이상할때 set 되는지, 정상일때 set 되는지 아직 판단 불가

	{
		sPassCbitFlag = FALSE
	}
}

void cbitTaskMain( void *pvParameters )
{
	/*
	 * 1. 전압 검사
	 * 2. 레지스터 검사
	 * 3. 플래그 검사 및 폭발
	 */
	/*****************************[임시 구현]*****************************/

	vTaskSuspend();
	while(1)
	{
		// 1. 전압 검사
		checkPower();
		if ( sPassCbitFlag == TRUE )
		{
		// 2. 레지스터 검사
			checkRegister();
		}
		// 3. 플래그 검사 및 폭발
		if ( sPassCbitFlag == FALSE )
		{
			//폭발한다.
		}
		vTaskResume(다음 태스크);
		vTaskSuspend();
	}

}
