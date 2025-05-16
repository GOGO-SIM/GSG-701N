#include "global.h"

void run()
{
    
}

void guidanceTaskMain(void *pvParameters)
{
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
        run();
    }
	vTaskDelete (NULL);	// == vTaskDelete (NULL)
}
