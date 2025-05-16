#include "FreeRTOS.h"
#include "global.h"
/**
 * [각 태스크의 마감 시간 설정]
 */
// UDP_RECEIVE_DEADLINE: IMU 데이터 처리까지 걸리는 마감 시간
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
    	// 1. UDP 데이터(IMU 데이터 수신) 처리 후 notify give를 대기
    	xil_printf("WAKE - UDP_RECEIVE\r\n", pcTaskGetName(NULL));
    	xTaskNotifyGive(xUdpReceiveTaskHandle);
    	vTaskDelayUntil(&last, pdMS_TO_TICKS(UDP_RECEIVE_DEADLINE));
    	// 2. 스케줄링 태스크의 시작 시간(시작 틱) 설정
    	// 3. 항법 태스크 give && delay
    	xil_printf("WAKE - Nav\r\n", pcTaskGetName(NULL));
    	xTaskNotifyGive(xNavigationTaskHandle);
    	vTaskDelayUntil(&last, pdMS_TO_TICKS(NAVIGATION_DEADLINE));
    	// 4. 유도 태스크 give && delay
    	xil_printf("WAKE - gui\r\n", pcTaskGetName(NULL));
		xTaskNotifyGive(xGuidanceTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(GUIDANCE_DEADLINE));
		// 5. 조종 태스크 give && delay
    	xil_printf("WAKE - cont\r\n", pcTaskGetName(NULL));
		xTaskNotifyGive(xControlTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(CONTOL_DEADLINE));
		// 6. UART 전송 태스크 give && delay
    	xil_printf("WAKE - uart send\r\n", pcTaskGetName(NULL));
		xTaskNotifyGive(xUartSendTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(UART_SEND_DEADLINE));
		// 7. CBIT 태스크 give && delay
    	xil_printf("WAKE - cbit\r\n", pcTaskGetName(NULL));
		xTaskNotifyGive(xCbitTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(CBIT_DEADLINE));
		// 8. Telemetry 태스크 give && delay
    	xil_printf("WAKE - tele\r\n", pcTaskGetName(NULL));
		xTaskNotifyGive(xTelemetryTaskHandle);
		vTaskDelayUntil(&last, pdMS_TO_TICKS(TELEMETRY_DEADLINE));
    }
}

