#include "global.h"

static void run()
{
    
}

void navigationTaskMain( void *pvParameters )
{

    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
		run();
    }
	// 1. task suspend

	for (;;)
	{
		// 2. 占쌓뱄옙 占쏙옙占�
		// 3. guidance task resume
		// task suspend
	}
}
