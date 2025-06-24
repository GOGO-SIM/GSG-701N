#include <stdint.h>
#include <string.h>
#include "gsgTypes.h"
#include "global.h"

void stanbyIgnitionTaskMain( void *pvParameters )
{
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
	while (gDistanceFromStart < 400)
	{
		xTaskNotifyGive(xUdpReceiveTaskHandle); // udp 태스크 폴링
        vTaskDelay(pdMS_TO_TICKS(10));
	}
	gGcuStatus = ENGAGE;
	xTaskNotifyGive(xSchedulingTaskHandle);
	xil_printf("delete -- %s\r\n", pcTaskGetName(NULL));
	vTaskDelete(NULL);
}
