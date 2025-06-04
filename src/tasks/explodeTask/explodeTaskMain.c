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
    xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));

    sGpioConfig = XGpioPs_LookupConfig(PS_GPIO_DEVICE_ID);
    XGpioPs_CfgInitialize(&sGpioPs, sGpioConfig, sGpioConfig->BaseAddr);

    XGpioPs_SetDirectionPin(&sGpioPs, sOutPin, GPIO_OUT);
    XGpioPs_SetOutputEnablePin(&sGpioPs, sOutPin, GPIO_OUT);

    gModeStatus = EXPLODE;

    //-------------------------------------------

	taskENTER_CRITICAL();

	vTaskPrioritySet(xUartSendTaskHandle, EXPLODE_TASK_PRIO);
	xTaskNotifyGive(xUartSendTaskHandle);
	vTaskDelay(pdMS_TO_TICKS(100));
	vTaskPrioritySet(xUartSendTaskHandle, UART_SEND_TASK_PRIO);

	taskEXIT_CRITICAL();

	//-------------------------------------------

	xil_printf("BOOM!\r\n");

    while (1)
    {

        XGpioPs_WritePin(&sGpioPs, sOutPin, GPIO_ON);
        //xil_printf("ON!\r\n");
        usleep(500000);
        XGpioPs_WritePin(&sGpioPs, sOutPin, GPIO_OFF);
        //xil_printf("OFF!\r\n");
        usleep(500000);
    }
 }
