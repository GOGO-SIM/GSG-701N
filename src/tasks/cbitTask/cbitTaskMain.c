#include <stdio.h>
#include "FreeRTOS.h"
#include "gsgTypes.h"
#include "global.h"

//static int sPassCbitFlag = TRUE; // 占쏙옙占� == TRUE, 占쏙옙占쏙옙 == FALSE
//
//int xADC_IsConversionDone();
//
//static void checkPower()
//{
//	/*
//	 * 1. XADC 占쏙옙환 占쏙옙占�
//	 * 2. 占쏙옙 占싻깍옙
//	 * 3. 占쌈계값 占쏙옙
//	 * 占십깍옙화 占쏙옙占쏙옙占쏙옙 占싱뱄옙 PBIT占쏙옙占쏙옙 占쏙옙占쏙옙占싹울옙占쏙옙占쏙옙占쏙옙 占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙
//	 */
//	/*****************************[占쌈쏙옙 占쏙옙占쏙옙]*****************************/
//	//1. XADC 占쏙옙환 占쏙옙占�
//	while (!xADC_IsConversionDone());
//
//	//2. 占쏙옙 占싻깍옙
//	int rawValue = xADC_GetData();
//    double measuredVoltage = xADC_ConvertToVoltage(rawValue);
//
//	//3.占쌈계값 占쏙옙
//	if (measuredVoltage < gVoltage1)
//	{
//		sPassCbitFlag == FALSE;
//	}
//}
//
//int checkAllRegister()
//{
//	return TRUE;
//}
//
//static void checkRegister()
//{
//	/*
//	 * 1. 占쏙옙渶뱄옙占쏙옙占쏙옙占� 占싯삼옙
//	 */
//	/*****************************[占쌈쏙옙 占쏙옙占쏙옙]*****************************/
//
//	// 1. 占쏙옙渶뱄옙占쏙옙占쏙옙占� 占싯삼옙
//	if (checkAllRegister() == TRUE)
//	{
//		// 占쏙옙占쏙옙占쏙옙占싶곤옙 占싱삼옙占쌀띰옙 set 占실댐옙占쏙옙, 占쏙옙占쏙옙占싹띰옙 set 占실댐옙占쏙옙 占쏙옙占쏙옙 占실댐옙 占쌀곤옙
//		sPassCbitFlag = FALSE;
//	}
//}

static void run(void)
{

}

void cbitTaskMain( void *pvParameters )
{
	/*
	 * 1. 占쏙옙占쏙옙 占싯삼옙
	 * 2. 占쏙옙占쏙옙占쏙옙占쏙옙 占싯삼옙
	 * 3. 占시뤄옙占쏙옙 占싯삼옙 占쏙옙 占쏙옙占쏙옙
	 */
	/*****************************[占쌈쏙옙 占쏙옙占쏙옙]*****************************/
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
		run();
    }
//	while(1)
//	{
//		// 1. 占쏙옙占쏙옙 占싯삼옙
//		checkPower();
//		if ( sPassCbitFlag == TRUE )
//		{
//		// 2. 占쏙옙占쏙옙占쏙옙占쏙옙 占싯삼옙
//			checkRegister();
//		}
//		// 3. 占시뤄옙占쏙옙 占싯삼옙 占쏙옙 占쏙옙占쏙옙
//		if ( sPassCbitFlag == FALSE )
//		{
//			//占쏙옙占쏙옙占싼댐옙.
//		}
//		vTaskResume(xTelemetryTaskHandle);
//		vTaskSuspend(xCbitTaskHandle);
//	}

}


int xADC_IsConversionDone()
{
	return 1;
}
