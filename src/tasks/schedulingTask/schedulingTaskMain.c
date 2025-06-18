#include "FreeRTOS.h"
#include "global.h"
/**
 * [�� �½�ũ�� ���� �ð� ����]
 */
// UDP_RECEIVE_DEADLINE: IMU ������ ó������ �ɸ��� ���� �ð�
const static int UDP_RECEIVE_DEADLINE = 30;
const static int NAVIGATION_DEADLINE = 10;
const static int GUIDANCE_DEADLINE = 10;
const static int CONTOL_DEADLINE = 10;
const static int UART_SEND_DEADLINE = 10;
const static int UART_RECV_DEADLINE = 10;
const static int CBIT_DEADLINE = 10;
const static int TELEMETRY_DEADLINE = 10;

void schedulingTaskMain(void *pvParameters)
{
    TickType_t       last;
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));

    for(;;)
    {

    	last = xTaskGetTickCount();

    	xil_printf("WAKE - UDP_RECEIVE\r\n", pcTaskGetName(NULL));
    	xTaskNotifyGive(xUdpReceiveTaskHandle);
    	vTaskDelayUntil(&last, pdMS_TO_TICKS(UDP_RECEIVE_DEADLINE));

    	xil_printf("WAKE - uart send\r\n", pcTaskGetName(NULL));
		xTaskNotifyGive(xUartSendTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(UART_SEND_DEADLINE));
    }
}

