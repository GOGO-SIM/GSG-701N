#include <stdint.h>
#include <string.h>
#include "gsgTypes.h"
#include "global.h"

void standyIgnitionTaskMain( void *pvParameters )
{
	/*
	 * 1. ���� Ŀ�ǵ� ������ payload�� current Status�� STANDBY�� ����
	 * 2. ACB CMD �۽� �½�ũ ����
	 * */
	// 1. ���� Ŀ�ǵ� ������ payload�� current Status�� STANDBY�� ����
	gGcuStatus = STANDBY;
	// 2. ACB CMD �۽� �½�ũ ����
	xTaskNotifyGive(xUartSendTaskHandle);

}
