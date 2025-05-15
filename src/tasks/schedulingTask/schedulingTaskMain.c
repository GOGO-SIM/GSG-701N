#include "FreeRTOS.h"
#include "global.h"
/**
 * [�� �½�ũ�� ���� �ð� ����]
 */
// UDP_RECEIVE_DEADLINE: IMU ������ ó������ �ɸ��� ���� �ð�
const static int UDP_RECEIVE_DEADLINE = 20;
const static int NAVIGATION_DEADLINE = 10;
const static int GUIDANCE_DEADLINE = 10;
const static int CONTOL_DEADLINE = 10;
const static int UART_SEND_DEADLINE = 10;
const static int CBIT_DEADLINE = 10;
const static int TELEMETRY_DEADLINE = 10;

void schedulingTaskMain(void *pvParameters)
{
    TickType_t       last;

    for(;;)
    {
    	// 1. UDP ������(IMU ������ ����) ó�� �� notify give�� ���
    	ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(UDP_RECEIVE_DEADLINE));
    	// 2. �����ٸ� �½�ũ�� ���� �ð�(���� ƽ) ����
    	last = xTaskGetTickCount();
    	// 3. �׹� �½�ũ give && delay
    	xTaskNotifyGive(xNavigationTaskHandle);
    	vTaskDelayUntil(&last, pdMS_TO_TICKS(NAVIGATION_DEADLINE));
    	// 4. ���� �½�ũ give && delay
		xTaskNotifyGive(xGuidanceTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(GUIDANCE_DEADLINE));
		// 5. ���� �½�ũ give && delay
		xTaskNotifyGive(xControlTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(CONTOL_DEADLINE));
		// 6. UART ���� �½�ũ give && delay
		xTaskNotifyGive(xUartSendTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(UART_SEND_DEADLINE));
		// 7. CBIT �½�ũ give && delay
		xTaskNotifyGive(xCbitTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(CBIT_DEADLINE));
		// 8. Telemetry �½�ũ give && delay
		xTaskNotifyGive(xTelemetryTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(TELEMETRY_DEADLINE));
    }
}

