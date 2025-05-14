#include <stdint.h>
#include <string.h>
#include "gsgTypes.h"
#include "taskMain.h"

void pbitFailTaskMain( void *pvParameters )
{
	/*
	 * 1. 전역 커맨드 변수의 payload의 Current Status를 PBIT_FAIL로 설정
	 * 2. ACB CMD 송신 태스크 깨움
	 * */
	// 1. 전역 커맨드 변수의 payload의 Current Status를 PBIT_FAIL로 설정
	gGcuStatus = PBIT_FAIL;
	// 2. ACB CMD 송신 태스크 깨움
	xTaskNotifyGive(uartSendTask);

}
