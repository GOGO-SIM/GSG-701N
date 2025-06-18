#include "global.h"
#include "xgpiops.h"

static int sOutPin = 12;

const static int UART_SEND_TASK_PRIO = 19;
const static int EXPLODE_TASK_PRIO = 24;

void explodeTaskMain(void *param)
{
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    vTaskSuspend(xUdpReceiveTaskHandle);
    vTaskSuspend(xNavigationTaskHandle);
    vTaskSuspend(xGuidanceTaskHandle);
    vTaskSuspend(xControlTaskHandle);
    vTaskSuspend(xControlTaskHandle);
    vTaskSuspend(xUartReceiveTaskHandle);
    vTaskSuspend(xCbitTaskHandle);
    vTaskSuspend(xTelemetryTaskHandle);

    xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));

    gModeStatus = EXPLODE;

    XGpioPs_WritePin(&gGpioPs, 11, GPIO_OFF);
    XGpioPs_WritePin(&gGpioPs, 10, GPIO_OFF);
    //-------------------------------------------

   vTaskPrioritySet(xUartSendTaskHandle, EXPLODE_TASK_PRIO);
   xTaskNotifyGive(xUartSendTaskHandle);
   vTaskDelay(pdMS_TO_TICKS(100));
   vTaskSuspend(xUdpReceiveTaskHandle);

   //-------------------------------------------

   while(1)
   {
		XGpioPs_WritePin(&gGpioPs, 12, GPIO_ON);
		usleep(200000);
		XGpioPs_WritePin(&gGpioPs, 12, GPIO_OFF);
		usleep(200000);
   }

 }
