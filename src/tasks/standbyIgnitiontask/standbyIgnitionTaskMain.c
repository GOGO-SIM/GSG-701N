#include <stdint.h>
#include <string.h>
#include "gsgTypes.h"
#include "global.h"

void stanbyIgnitionTaskMain( void *pvParameters )
{
	/*
	 * 1. 전역 커맨드 변수의 payload의 current Status를 STANDBY로 설정
	 * 2. ACB CMD 송신 태스크 깨움
	 * */
	// 1. 전역 커맨드 변수의 payload의 current Status를 STANDBY로 설정
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
	gGcuStatus = STANDBY;
	// 2. ACB CMD 송신 태스크 깨움
	xTaskNotifyGive(xUartSendTaskHandle);
	xil_printf("DEL -- %s\r\n", pcTaskGetName(NULL));
	vTaskDelete(NULL);
}
