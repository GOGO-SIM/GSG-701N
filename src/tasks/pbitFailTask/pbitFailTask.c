#include <stdint.h>
#include <string.h>
#include "gsgTypes.h"
#include "global.h"

void pbitFailTaskMain( void *pvParameters )
{
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
	/*
	 * 1. 전역 커맨드 변수의 payload의 Current Status를 PBIT_FAIL로 설정
	 * 2. ACB CMD 송신 태스크 깨움
	 * */
	// 1. 전역 커맨드 변수의 payload의 Current Status를 PBIT_FAIL로 설정
	gGcuStatus = PBIT_FAIL;
	// 2. ACB CMD 송신 태스크 깨움
	xTaskNotifyGive(xUartSendTaskHandle);
	// 3. pbit Task 태스크 휴면 시작
	xil_printf("infinity loop %s\r\n", pcTaskGetName(NULL));
	while (1) ;
}
