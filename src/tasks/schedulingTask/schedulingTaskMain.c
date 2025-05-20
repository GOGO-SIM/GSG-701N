#include "FreeRTOS.h"
#include "global.h"
/**
 * [�� �½�ũ�� ���� �ð� ����]
 */
// UDP_RECEIVE_DEADLINE: IMU ������ ó������ �ɸ��� ���� �ð�
const static int UDP_RECEIVE_DEADLINE = 1000;
const static int NAVIGATION_DEADLINE = 1000;
const static int GUIDANCE_DEADLINE = 1000;
const static int CONTOL_DEADLINE = 1000;
const static int UART_SEND_DEADLINE = 1000;
const static int CBIT_DEADLINE = 1000;
const static int TELEMETRY_DEADLINE = 1000;

void schedulingTaskMain(void *pvParameters)
{
    TickType_t       last;

    xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));

    for(;;)
    {

    	last = xTaskGetTickCount();
    	// 1. UDP ������(IMU ������ ����) ó�� �� notify give�� ���
    	xil_printf("WAKE - UDP_RECEIVE\r\n", pcTaskGetName(NULL));
    	xTaskNotifyGive(xUdpReceiveTaskHandle);
    	vTaskDelayUntil(&last, pdMS_TO_TICKS(UDP_RECEIVE_DEADLINE));
    	// 2. �����ٸ� �½�ũ�� ���� �ð�(���� ƽ) ����
    	// 3. �׹� �½�ũ give && delay
    	xil_printf("WAKE - Nav\r\n", pcTaskGetName(NULL));
    	xTaskNotifyGive(xNavigationTaskHandle);
    	vTaskDelayUntil(&last, pdMS_TO_TICKS(NAVIGATION_DEADLINE));
    	// 4. ���� �½�ũ give && delay
    	xil_printf("WAKE - gui\r\n", pcTaskGetName(NULL));
		xTaskNotifyGive(xGuidanceTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(GUIDANCE_DEADLINE));
		// 5. ���� �½�ũ give && delay
    	xil_printf("WAKE - cont\r\n", pcTaskGetName(NULL));
		xTaskNotifyGive(xControlTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(CONTOL_DEADLINE));
		// 6. UART ���� �½�ũ give && delay
    	xil_printf("WAKE - uart send\r\n", pcTaskGetName(NULL));
		xTaskNotifyGive(xUartSendTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(UART_SEND_DEADLINE));
		// 7. UART get && delay
    	xil_printf("WAKE - uart recv\r\n", pcTaskGetName(NULL));
		xTaskNotifyGive(xUartReceiveTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(UART_RECV_DEADLINE));
		// 8. CBIT �½�ũ give && delay
    	xil_printf("WAKE - cbit\r\n", pcTaskGetName(NULL));
		xTaskNotifyGive(xCbitTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(CBIT_DEADLINE));
		// 9. Telemetry �½�ũ give && delay
    	xil_printf("WAKE - tele\r\n", pcTaskGetName(NULL));
		xTaskNotifyGive(xTelemetryTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(TELEMETRY_DEADLINE));
    }
}

