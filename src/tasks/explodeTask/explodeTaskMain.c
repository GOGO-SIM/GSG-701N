#include "global.h"
#include "xgpiops.h"

#define PS_GPIO_DEVICE_ID  XPAR_PS7_GPIO_0_DEVICE_ID

#define GPIO_IN 0
#define GPIO_OUT 1
#define GPIO_OFF 0
#define GPIO_ON 1

static int sOutPin = 12;
static XGpioPs sGpioPs;
static XGpioPs_Config *sGpioConfig;

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

    sGpioConfig = XGpioPs_LookupConfig(PS_GPIO_DEVICE_ID);
    XGpioPs_CfgInitialize(&sGpioPs, sGpioConfig, sGpioConfig->BaseAddr);

    XGpioPs_SetDirectionPin(&sGpioPs, sOutPin, GPIO_OUT);
    XGpioPs_SetOutputEnablePin(&sGpioPs, sOutPin, GPIO_OUT);

    XGpioPs_WritePin(&sGpioPs, sOutPin, GPIO_ON);

    gModeStatus = EXPLODE;

    //-------------------------------------------

	   vTaskPrioritySet(xUartSendTaskHandle, EXPLODE_TASK_PRIO);
	   xTaskNotifyGive(xUartSendTaskHandle);
	   vTaskDelay(pdMS_TO_TICKS(100));
	   vTaskSuspend(xUdpReceiveTaskHandle);

	   //-------------------------------------------

	   while(1)
	   {
		    XGpioPs_WritePin(&sGpioPs, sOutPin, GPIO_ON);
		    usleep(200000);
		    XGpioPs_WritePin(&sGpioPs, sOutPin, GPIO_OFF);
		    usleep(200000);

	   }
 }
