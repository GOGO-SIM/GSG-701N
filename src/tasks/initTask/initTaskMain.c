#include "global.h"


void initUdpServer()
{
	// lwip�� udp ���� �ʱ�ȭ ��� 3���� �� ������ ������ �� �����Ͽ� ����
}

void initTaskMain( void *pvParameters )
{
	/**
	 * 1. UDP server open
	 * 2. delete task
	 */
	initUdpServer();
	vTaskDelete(xInitTaskHandle);
}
