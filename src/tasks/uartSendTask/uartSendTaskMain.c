#include "uartSendTaskRun.h"
#include "xtime_l.h"

void uartSendTaskMain(void *pvParameters) {
	for(;;)
	{
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        //xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
        XTime start, end;
        XTime_GetTime(&start);  // 시작 시간 기록
        runUartSend();
        XTime_GetTime(&end);    // 종료 시간 기록
        // 시간 차이 계산 (us 단위)
        uint64_t elapsed_ticks = end - start;
        uint64_t elapsed_us = elapsed_ticks / (COUNTS_PER_SECOND / 1000000);

        //xil_printf("start: %llu, end: %llu, elapsed: %llu ticks\n", start, end, elapsed_ticks);
        xil_printf("UartSend: %llu\r\n", elapsed_ticks);
	}
}
