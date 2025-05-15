#include <platform_config.h>
#include <sleep.h>
#include "netif/xadapter.h"
#include "xil_printf.h"
#include "lwip/init.h"
#include "lwip/inet.h"
#include "taskMain.h"
#pragma message("Using configMAX_PRIORITIES: " STRINGIFY(configMAX_PRIORITIES))
void initTaskMain( void *pvParameters );

int main()
{
	xil_printf("-----init Task Main try Create------\r\n");
	xTaskCreate((TaskFunction_t)initTaskMain,
						"Init Task",
						2048,
						NULL,
						50,
						&xInitTaskHandle);
	xil_printf("\n\r\n\r");
	xil_printf("-----init Task Main create complete------\r\n");
	vTaskStartScheduler();
	while(1);
	return 0;
}
