#include <stdint.h>
#include <string.h>
#include "gsgTypes.h"
#include "taskMain.h"

void pbitFailTaskMain( void *pvParameters )
{
	/*
	 * 1. ���� Ŀ�ǵ� ������ payload�� Current Status�� PBIT_FAIL�� ����
	 * 2. ACB CMD �۽� �½�ũ ����
	 * */
	// 1. ���� Ŀ�ǵ� ������ payload�� Current Status�� PBIT_FAIL�� ����
	gGcuStatus = PBIT_FAIL;
	// 2. ACB CMD �۽� �½�ũ ����
	xTaskNotifyGive(uartSendTask);

}
