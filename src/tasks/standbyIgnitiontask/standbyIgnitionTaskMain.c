#include <stdint.h>
#include <string.h>
#include "gsgTypes.h"
#include "taskMain.h"

void pbitFailTaskMain( void *pvParameters )
{
	/*
	 * 1. ���� Ŀ�ǵ� ������ payload�� current Status�� STANDBY�� ����
	 * 2. ACB CMD �۽� �½�ũ ����
	 * */
	// 1. ���� Ŀ�ǵ� ������ payload�� current Status�� STANDBY�� ����
	gGcuStatus = STANDBY;
	// 2. ACB CMD �۽� �½�ũ ����
	xTaskNotifyGive(uartSendTask);

}
