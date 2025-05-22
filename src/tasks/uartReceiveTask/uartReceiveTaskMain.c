#include <stdio.h>
#include <string.h>
#include "global.h"
#include "gsgTypes.h"
#include "crc.h"
#include "xparameters.h"
#include "uartReceiveTaskRun.h"
/* TODO : 테스트 데이터 예시 크기이므로 추후 리팩토링 필요 #56
 * RECEIVE_SIZE 재정의
 * exampleMsg -> recvUartMsg, 전역변수화
 * */

void uartReceiveTaskMain() 
{
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
        runUartReceive();

    }
}

