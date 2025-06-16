
#include <stdio.h>
#include <string.h>
#include "global.h"
#include "gsgTypes.h"
#include "crc.h"
#include "xparameters.h"
#include "uartReceiveTaskRun.h"
#include "xtime_l.h"

/* TODO : 테스트 데이터 예시 크기이므로 추후 리팩토링 필요 #56
 * RECEIVE_SIZE 재정의
 * exampleMsg -> recvUartMsg, 전역변수화
 * */
void uartReceiveTaskMain() 
{
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        //xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));

        XTime start, end;
        XTime_GetTime(&start);  // 시작 시간 기록
        runUartReceive();
        XTime_GetTime(&end);    // 종료 시간 기록
        // 시간 차이 계산 (us 단위)
        uint64_t elapsed_ticks = end - start;
        uint64_t elapsed_us = elapsed_ticks / (COUNTS_PER_SECOND / 1000000);

        //xil_printf("start: %llu, end: %llu, elapsed: %llu ticks\n", start, end, elapsed_ticks);
        xil_printf("UartRecv: %llu\r\n", elapsed_ticks);


    }
}
