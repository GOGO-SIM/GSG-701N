#include "FreeRTOS.h"
#include "global.h"
/**
 * [각 태스크의 마감 시간 설정]
 */
// UDP_RECEIVE_DEADLINE: IMU 데이터 처리까지 걸리는 마감 시간
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
    	// 1. UDP 데이터(IMU 데이터 수신) 처리 후 notify give를 대기
    	ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(UDP_RECEIVE_DEADLINE));
    	// 2. 스케줄링 태스크의 시작 시간(시작 틱) 설정
    	last = xTaskGetTickCount();
    	// 3. 항법 태스크 give && delay
    	xTaskNotifyGive(xNavigationTaskHandle);
    	vTaskDelayUntil(&last, pdMS_TO_TICKS(NAVIGATION_DEADLINE));
    	// 4. 유도 태스크 give && delay
		xTaskNotifyGive(xGuidanceTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(GUIDANCE_DEADLINE));
		// 5. 조종 태스크 give && delay
		xTaskNotifyGive(xControlTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(CONTOL_DEADLINE));
		// 6. UART 전송 태스크 give && delay
		xTaskNotifyGive(xUartSendTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(UART_SEND_DEADLINE));
		// 7. CBIT 태스크 give && delay
		xTaskNotifyGive(xCbitTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(CBIT_DEADLINE));
		// 8. Telemetry 태스크 give && delay
		xTaskNotifyGive(xTelemetryTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(TELEMETRY_DEADLINE));
    }
}

