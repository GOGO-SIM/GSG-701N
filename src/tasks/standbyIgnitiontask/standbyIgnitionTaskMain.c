#include <stdint.h>
#include <string.h>
#include "gsgTypes.h"
#include "global.h"

void stanbyIgnitionTaskMain( void *pvParameters )
{
	/*
	 * 1. ���� Ŀ�ǵ� ������ payload�� current Status�� STANDBY�� ����
	 * 2. ACB CMD �۽� �½�ũ ����
	 * */
	// 1. ���� Ŀ�ǵ� ������ payload�� current Status�� STANDBY�� ����
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
	gGcuStatus = STANDBY;
	// 2. ACB CMD �۽� �½�ũ ����
	xTaskNotifyGive(xUartSendTaskHandle);
	xil_printf("DEL -- %s\r\n", pcTaskGetName(NULL));
	vTaskDelete(NULL);
}
