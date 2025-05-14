/*
 * Starts all the other tasks, then starts the scheduler.
 */

/**
 * [快急鉴困 搬沥]
 */
const static int INIT_TASK_PRIO = 44;
const static int PBIT_TASK_PRIO = 42;
const static int STANBY_IGNITION_TASK_PRIO = 22;
const static int PBIT_FAIL_TASK_PRIO = 21;
const static int UART_RECEIVE_TASK_PRIO = 24;
const static int UDP_RECEIVE_TASK_PRIO = 26;
// 胶纳临傅 怕胶农狼 快急 鉴困绰 怕胶农 眉牢 吝 啊厘 臭酒具 茄促.
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
 * [葛电 怕胶农 积己]
 * 1. init Task 积己
 * 2. pbit Task 积己
 * 3. standbyIgnition Task 积己
 * 4. uartReceive Task 积己
 * 5. udpReceive Task 积己
 * 6. scheduing Task 积己
 * 7. navigation Task 积己
 * 8. guidance Task 积己
 * 9. control Task 积己
 * 10. uartSend Task 积己
 * 11. cbit Task 积己
 * 12. telemetry Task 积己
 * 13. pbit fail task 积己
 */
static void createAllTask(void)
{
//	 * 1. init Task 积己
	xTaskCreate(	(TaskFunction_t)initTaskMain,
					"Init Task",
					256,
					NULL,
					INIT_TASK_PRIO,
					&xInitTaskHandle);
//	 * 2. pbit Task 积己
	xTaskCreate(	(TaskFunction_t)pbitTaskMain,
					"PBIT Task",
					256,
					NULL,
					PBIT_TASK_PRIO,
					&xPbitTaskHandle);
//	 * 3. standbyIgnition Task 积己
	xTaskCreate(	(TaskFunction_t)stanbyIgnitionTaskMain,
					"Standby Ignition Task",
					256,
					NULL,
					STANBY_IGNITION_TASK_PRIO,
					&xStanbyIgnitionTaskHandle);
//	 * 4. uartReceive Task 积己
	xTaskCreate(	(TaskFunction_t)uartReceiveTaskMain,
					"UART Receive Task",
					256,
					NULL,
					UART_RECEIVE_TASK_PRIO,
					&xUartReceiveTaskHandle);
//	 * 5. udpReceive Task 积己
	xTaskCreate(	(TaskFunction_t)udpReceiveTaskMain,
					"UDP Receive Task",
					256,
					NULL,
					UDP_RECEIVE_TASK_PRIO,
					&xUdpReceiveTaskHandle);
//	 * 6. scheduling Task 积己
	xTaskCreate(	(TaskFunction_t)schedulingTaskMain,
					"Scheduling Task",
					256,
					NULL,
					SCHEDULING_TASK_PRIO,
					&xSchedulingTaskHandle);
//	 * 6. navigation Task 积己
	xTaskCreate(	(TaskFunction_t)navigationTaskMain,
					"Navigation Task",
					256,
					NULL,
					NAVIGATION_TASK_PRIO,
					&xNavigationTaskHandle);
//	 * 7. guidance Task 积己
	xTaskCreate(	(TaskFunction_t)guidanceTaskMain,
					"Guidance Task",
					256,
					NULL,
					GUIDANCE_TASK_PRIO,
					&xGuidanceTaskHandle);
//	 * 8. control Task 积己
	xTaskCreate(	(TaskFunction_t)controlTaskMain,
					"Control Task",
					256,
					NULL,
					&xControlTaskHandle);
//	 * 9. uartSend Task 积己
	xTaskCreate(	(TaskFunction_t)uartSendTaskMain,
					"Uart Send Task",
					256,
					NULL,
					UART_SEND_TASK_PRIO,
					&xUartSendTaskHandle);
//	 * 10. cbit Task 积己
	xTaskCreate(	(TaskFunction_t)cbitTaskMain,
					"CBIT Task",
					256,
					NULL,
					CBIT_TASK_PRIO,
					&xCbitTaskHandle);
//	 * 11. telemetry Task 积己
	xTaskCreate(	(TaskFunction_t)TelemetryTaskMain,
					"Telemetry Task",
					256,
					NULL,
					TELEMETRY_TASK_PRIO,
					&xTelemetryTaskHandle);
//	 * 12. pbit fail Task 积己
	xTaskCreate(	(TaskFunction_t)pbitFailTaskMain,
					"PBIT Fail Task",
					256,
					NULL,
					PBIT_FAIL_TASK_PRIO,
					&xPbitFailTaskHandle);
}

void TaskMain( void *pvParameters )
{
	// 1. 葛电 怕胶农甫 积己茄促.
	createAllTask();
	// 2. 磊脚阑 昏力窍哥 辑厚胶 胶纳临傅阑 矫累茄促.
	vTaskDelete (xMainTaskHandle);	// == vTaskDelete (NULL)
}
