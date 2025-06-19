#include <stdint.h>
#include <string.h>
#include "gsgTypes.h"
#include "global.h"

void pbitFailTaskMain( void *pvParameters )
{
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
	/*
	 * 1. ���� Ŀ�ǵ� ������ payload�� Current Status�� PBIT_FAIL�� ����
	 * 2. ACB CMD �۽� �½�ũ ����
	 * */
	// 1. ���� Ŀ�ǵ� ������ payload�� Current Status�� PBIT_FAIL�� ����
	gGcuStatus = PBIT_FAIL;
	// 2. ACB CMD �۽� �½�ũ ����
	xTaskNotifyGive(xUartSendTaskHandle);
	// 3. pbit Task �½�ũ �޸� ����
	xil_printf("\n Blocking All Task : %s\r\n", pcTaskGetName(NULL));
	while (1);
}
