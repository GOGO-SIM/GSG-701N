#include "uartSendTaskRun.h"

void uartSendTaskMain(void *pvParameters) {
	for(;;)
	{
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
        runUartSend();
	}
}
