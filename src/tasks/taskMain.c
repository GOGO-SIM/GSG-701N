/*
 * Starts all the other tasks, then starts the scheduler.
 */

/**
 * [�켱���� ����]
 */
const static int INIT_TASK_PRIO = 44;
const static int PBIT_TASK_PRIO = 42;
const static int STANBY_IGNITION_TASK_PRIO = 22;
const static int PBIT_FAIL_TASK_PRIO = 21;
const static int UART_RECEIVE_TASK_PRIO = 24;
const static int UDP_RECEIVE_TASK_PRIO = 26;
// �����ٸ� �½�ũ�� �켱 ������ �½�ũ ü�� �� ���� ���ƾ� �Ѵ�.
const static int SCHEDULING_TASK_PRIO = 40;
const static int NAVIGATION_TASK_PRIO = 28;
const static int GUIDANCE_TASK_PRIO = 30;
const static int CONTROL_TASK_PRIO = 32;
const static int UART_SEND_TASK_PRIO = 34;
const static int CBIT_TASK_PRIO = 36;
const static int TELEMETRY_TASK_PRIO = 38;

/**
 * [task handlers]
 */
TaskHandle_t xMainTaskHandle,
	xSchedulingTaskHandle,
	xInitTaskHandle,
	xPbitTaskHandle,
	xStanbyIgnitionTaskHandle,
	xUartReceiveTaskHandle,
	xUdpReceiveTaskHandle,
	xNavigationTaskHandle,
	xGuidanceTaskHandle,
	xControlTaskHandle,
	xUartSendTaskHandle,
	xCbitTaskHandle,
	xTelemetryTaskHandle;


void USER_THREADS( void )
{
	/* Create one of the two tasks. */
	xTaskCreate(	(TaskFunction_t)TaskMain,
					"TaskMain",
					256,
					NULL,
					TASK_MAIN_PRIO,
					&xMainTaskHandle);
}

/**
 * [��� �½�ũ ����]
 * 1. init Task ����
 * 2. pbit Task ����
 * 3. standbyIgnition Task ����
 * 4. uartReceive Task ����
 * 5. udpReceive Task ����
 * 6. scheduing Task ����
 * 7. navigation Task ����
 * 8. guidance Task ����
 * 9. control Task ����
 * 10. uartSend Task ����
 * 11. cbit Task ����
 * 12. telemetry Task ����
 * 13. pbit fail task ����
 */
static void createAllTask(void)
{
//	 * 1. init Task ����
	xTaskCreate(	(TaskFunction_t)initTaskMain,
					"Init Task",
					256,
					NULL,
					INIT_TASK_PRIO,
					&xInitTaskHandle);
//	 * 2. pbit Task ����
	xTaskCreate(	(TaskFunction_t)pbitTaskMain,
					"PBIT Task",
					256,
					NULL,
					PBIT_TASK_PRIO,
					&xPbitTaskHandle);
//	 * 3. standbyIgnition Task ����
	xTaskCreate(	(TaskFunction_t)stanbyIgnitionTaskMain,
					"Standby Ignition Task",
					256,
					NULL,
					STANBY_IGNITION_TASK_PRIO,
					&xStanbyIgnitionTaskHandle);
//	 * 4. uartReceive Task ����
	xTaskCreate(	(TaskFunction_t)uartReceiveTaskMain,
					"UART Receive Task",
					256,
					NULL,
					UART_RECEIVE_TASK_PRIO,
					&xUartReceiveTaskHandle);
//	 * 5. udpReceive Task ����
	xTaskCreate(	(TaskFunction_t)udpReceiveTaskMain,
					"UDP Receive Task",
					256,
					NULL,
					UDP_RECEIVE_TASK_PRIO,
					&xUdpReceiveTaskHandle);
//	 * 6. scheduling Task ����
	xTaskCreate(	(TaskFunction_t)schedulingTaskMain,
					"Scheduling Task",
					256,
					NULL,
					SCHEDULING_TASK_PRIO,
					&xSchedulingTaskHandle);
//	 * 6. navigation Task ����
	xTaskCreate(	(TaskFunction_t)navigationTaskMain,
					"Navigation Task",
					256,
					NULL,
					NAVIGATION_TASK_PRIO,
					&xNavigationTaskHandle);
//	 * 7. guidance Task ����
	xTaskCreate(	(TaskFunction_t)guidanceTaskMain,
					"Guidance Task",
					256,
					NULL,
					GUIDANCE_TASK_PRIO,
					&xGuidanceTaskHandle);
//	 * 8. control Task ����
	xTaskCreate(	(TaskFunction_t)controlTaskMain,
					"Control Task",
					256,
					NULL,
					&xControlTaskHandle);
//	 * 9. uartSend Task ����
	xTaskCreate(	(TaskFunction_t)uartSendTaskMain,
					"Uart Send Task",
					256,
					NULL,
					UART_SEND_TASK_PRIO,
					&xUartSendTaskHandle);
//	 * 10. cbit Task ����
	xTaskCreate(	(TaskFunction_t)cbitTaskMain,
					"CBIT Task",
					256,
					NULL,
					CBIT_TASK_PRIO,
					&xCbitTaskHandle);
//	 * 11. telemetry Task ����
	xTaskCreate(	(TaskFunction_t)TelemetryTaskMain,
					"Telemetry Task",
					256,
					NULL,
					TELEMETRY_TASK_PRIO,
					&xTelemetryTaskHandle);
//	 * 12. pbit fail Task ����
	xTaskCreate(	(TaskFunction_t)pbitFailTaskMain,
					"PBIT Fail Task",
					256,
					NULL,
					PBIT_FAIL_TASK_PRIO,
					&xPbitFailTaskHandle);
}

void TaskMain( void *pvParameters )
{
	// 1. ��� �½�ũ�� �����Ѵ�.
	createAllTask();
	// 2. �ڽ��� �����ϸ� ���� �����ٸ��� �����Ѵ�.
	vTaskDelete (xMainTaskHandle);	// == vTaskDelete (NULL)
}
