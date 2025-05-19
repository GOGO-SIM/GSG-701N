#include <stdio.h>
#include "FreeRTOS.h"
#include "gsgTypes.h"
#include "global.h"

//static int sPassCbitFlag = TRUE; // ��� == TRUE, ���� == FALSE
//
//int xADC_IsConversionDone();
//
//static void checkPower()
//{
//	/*
//	 * 1. XADC ��ȯ ���
//	 * 2. �� �б�
//	 * 3. �Ӱ谪 ��
//	 * �ʱ�ȭ ������ �̹� PBIT���� �����Ͽ������� �� ���� ����
//	 */
//	/*****************************[�ӽ� ����]*****************************/
//	//1. XADC ��ȯ ���
//	while (!xADC_IsConversionDone());
//
//	//2. �� �б�
//	int rawValue = xADC_GetData();
//    double measuredVoltage = xADC_ConvertToVoltage(rawValue);
//
//	//3.�Ӱ谪 ��
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
//	 * 1. ��緹������ �˻�
//	 */
//	/*****************************[�ӽ� ����]*****************************/
//
//	// 1. ��緹������ �˻�
//	if (checkAllRegister() == TRUE)
//	{
//		// �������Ͱ� �̻��Ҷ� set �Ǵ���, �����϶� set �Ǵ��� ���� �Ǵ� �Ұ�
//		sPassCbitFlag = FALSE;
//	}
//}

void run(void)
{

}

void cbitTaskMain( void *pvParameters )
{
	/*
	 * 1. ���� �˻�
	 * 2. �������� �˻�
	 * 3. �÷��� �˻� �� ����
	 */
	/*****************************[�ӽ� ����]*****************************/
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
		run();
    }
//	while(1)
//	{
//		// 1. ���� �˻�
//		checkPower();
//		if ( sPassCbitFlag == TRUE )
//		{
//		// 2. �������� �˻�
//			checkRegister();
//		}
//		// 3. �÷��� �˻� �� ����
//		if ( sPassCbitFlag == FALSE )
//		{
//			//�����Ѵ�.
//		}
//		vTaskResume(xTelemetryTaskHandle);
//		vTaskSuspend(xCbitTaskHandle);
//	}

}


int xADC_IsConversionDone()
{
	return 1;
}
