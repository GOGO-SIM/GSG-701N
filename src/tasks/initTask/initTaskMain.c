#include "global.h"


void initUdpServer()
{
	// lwip의 udp 서버 초기화 방식 3가지 중 무엇을 선택할 지 결정하여 구현
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
