#include "global.h"

static void run()
{
    
}

void controlTaskMain(void *pvParameters)
{
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
        run();
    }
	vTaskDelete (NULL);	// == vTaskDelete (NULL)
}
