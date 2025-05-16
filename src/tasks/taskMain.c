#include "global.h"
#include "taskMain.h"

/*
 * Starts all the other tasks, then starts the scheduler.
 */

/**
 * [快急鉴困]
 */
const static int INIT_TASK_PRIO = 28;
const static int PBIT_TASK_PRIO = 27;
const static int STANBY_IGNITION_TASK_PRIO = 24;
const static int PBIT_FAIL_TASK_PRIO = 24;
const static int UART_RECEIVE_TASK_PRIO = 11;
// 胶纳临傅 怕胶农狼 快急 鉴困绰 怕胶农 眉牢 吝 啊厘 臭酒具 茄促.
const static int SCHEDULING_TASK_PRIO = 22;
const static int UDP_RECEIVE_TASK_PRIO = 15;
const static int NAVIGATION_TASK_PRIO = 16;
const static int GUIDANCE_TASK_PRIO = 17;
const static int CONTROL_TASK_PRIO = 18;
const static int UART_SEND_TASK_PRIO = 19;
const static int CBIT_TASK_PRIO = 20;
const static int TELEMETRY_TASK_PRIO = 21;
// ERR : main task priority -> TBD
const static int MAIN_TASK_PRIO = 29;

//void taskMain(void *pvParameters);

void USER_THREADS( void )
{
	/* Create one of the two tasks. */
	xil_printf("-----USER THREAD> Task Main try Create------\r\n");
	xTaskCreate(	(TaskFunction_t)taskMain,
					"TaskMain",
					256,
					NULL,
					MAIN_TASK_PRIO,
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
	BaseType_t xReturned;

	xil_printf("----- TRY CREATE Init Task------\r\n");
	xReturned = xTaskCreate(	(TaskFunction_t)initTaskMain,
					"Init Task",
					256,
					NULL,
					INIT_TASK_PRIO,
					&xInitTaskHandle);
	if (xReturned == pdPASS)
	{
		xil_printf("----- CREATED Init Task------\r\n");
	}
	else
	{
		xil_printf("----- FAILED CREATING Init Task------\r\n");
	}
//	 * 2. pbit Task 积己
	xil_printf("----- TRY CREATE PBIT Task------\r\n");
	xReturned = xTaskCreate(	(TaskFunction_t)pbitTaskMain,
					"PBIT Task",
					256,
					NULL,
					PBIT_TASK_PRIO,
					&xPbitTaskHandle);
	if (xReturned == pdPASS)
	{
		xil_printf("----- CREATED PBIT Task------\r\n");
	}
	else
	{
		xil_printf("----- FAILED CREATING PBIT Task------\r\n");
	}
//	 * 3. standbyIgnition Task 积己
	xil_printf("----- TRY CREATE Standby Ignition Task------\r\n");
	xReturned = xTaskCreate(	(TaskFunction_t)stanbyIgnitionTaskMain,
					"Standby Ignition Task",
					256,
					NULL,
					STANBY_IGNITION_TASK_PRIO,
					&xStanbyIgnitionTaskHandle);
	if (xReturned == pdPASS)
	{
		xil_printf("----- CREATED Task------\r\n");
	}
	else
	{
		xil_printf("----- FAILED CREATING Task------\r\n");
	}
//	 * 4. uartReceive Task 积己
	xil_printf("----- TRY CREATE UART Receive Task------\r\n");
	xReturned = xTaskCreate(	(TaskFunction_t)uartReceiveTaskMain,
					"UART Receive Task",
					256,
					NULL,
					UART_RECEIVE_TASK_PRIO,
					&xUartReceiveTaskHandle);
	if (xReturned == pdPASS)
	{
		xil_printf("----- CREATED Task------\r\n");
	}
	else
	{
		xil_printf("----- FAILED CREATING Task------\r\n");
	}
//	 * 5. udpReceive Task 积己
	xil_printf("----- TRY CREATE UDP Receive Task------\r\n");
	xTaskCreate(	(TaskFunction_t)udpReceiveTaskMain,
					"UDP Receive Task",
					256,
					NULL,
					UDP_RECEIVE_TASK_PRIO,
					&xUdpReceiveTaskHandle);
	if (xReturned == pdPASS)
	{
		xil_printf("----- CREATED Task------\r\n");
	}
	else
	{
		xil_printf("----- FAILED CREATING Task------\r\n");
	}
//	 * 6. scheduling Task 积己
	xil_printf("----- TRY CREATE Scheduling Task------\r\n");
	xTaskCreate(	(TaskFunction_t)schedulingTaskMain,
					"Scheduling Task",
					256,
					NULL,
					SCHEDULING_TASK_PRIO,
					&xSchedulingTaskHandle);
	if (xReturned == pdPASS)
	{
		xil_printf("----- CREATED Task------\r\n");
	}
	else
	{
		xil_printf("----- FAILED CREATING Task------\r\n");
	}
//	 * 6. navigation Task 积己
	xil_printf("----- TRY CREATE Navigation Task------\r\n");
	xReturned = xTaskCreate(	(TaskFunction_t)navigationTaskMain,
					"Navigation Task",
					256,
					NULL,
					NAVIGATION_TASK_PRIO,
					&xNavigationTaskHandle);
	if (xReturned == pdPASS)
	{
		xil_printf("----- CREATED Task------\r\n");
	}
	else
	{
		xil_printf("----- FAILED CREATING Task------\r\n");
	}
//	 * 7. guidance Task 积己
	xil_printf("----- TRY CREATE Guidance Task------\r\n");
	xReturned = xTaskCreate(	(TaskFunction_t)guidanceTaskMain,
					"Guidance Task",
					256,
					NULL,
					GUIDANCE_TASK_PRIO,
					&xGuidanceTaskHandle);
	if (xReturned == pdPASS)
	{
		xil_printf("----- CREATED Task------\r\n");
	}
	else
	{
		xil_printf("----- FAILED CREATING Task------\r\n");
	}
//	 * 8. control Task 积己
	xil_printf("----- TRY CREATE Control Task------\r\n");
	xReturned = xTaskCreate(	(TaskFunction_t)controlTaskMain,
					"Control Task",
					256,
					NULL,
					CONTROL_TASK_PRIO,
					&xControlTaskHandle);
	if (xReturned == pdPASS)
	{
		xil_printf("----- CREATED Task------\r\n");
	}
	else
	{
		xil_printf("----- FAILED CREATING Task------\r\n");
	}
//	 * 9. uartSend Task 积己
	xil_printf("----- TRY CREATE Uart Send Task------\r\n");
	xReturned = xTaskCreate(	(TaskFunction_t)uartSendTaskMain,
					"Uart Send Task",
					256,
					NULL,
					UART_SEND_TASK_PRIO,
					&xUartSendTaskHandle);
	if (xReturned == pdPASS)
	{
		xil_printf("----- CREATED Task------\r\n");
	}
	else
	{
		xil_printf("----- FAILED CREATING Task------\r\n");
	}
//	 * 10. cbit Task 积己
	xil_printf("----- TRY CREATE CBIT Task------\r\n");
	xReturned = xTaskCreate(	(TaskFunction_t)cbitTaskMain,
					"CBIT Task",
					256,
					NULL,
					CBIT_TASK_PRIO,
					&xCbitTaskHandle);
	if (xReturned == pdPASS)
	{
		xil_printf("----- CREATED Task------\r\n");
	}
	else
	{
		xil_printf("----- FAILED CREATING Task------\r\n");
	}
//	 * 11. telemetry Task 积己
	xil_printf("----- TRY CREATE Telemetry Task------\r\n");
	xReturned = xTaskCreate(	(TaskFunction_t)telemetryTaskMain,
					"Telemetry Task",
					256,
					NULL,
					TELEMETRY_TASK_PRIO,
					&xTelemetryTaskHandle);
	if (xReturned == pdPASS)
	{
		xil_printf("----- CREATED Task------\r\n");
	}
	else
	{
		xil_printf("----- FAILED CREATING Task------\r\n");
	}
//	 * 12. pbit fail Task 积己
	xil_printf("----- TRY CREATE PBIT Fail Task------\r\n");
	xReturned = xTaskCreate(	(TaskFunction_t)pbitFailTaskMain,
					"PBIT Fail Task",
					256,
					NULL,
					PBIT_FAIL_TASK_PRIO,
					&xPbitFailTaskHandle);
	if (xReturned == pdPASS)
	{
		xil_printf("----- CREATED Task------\r\n");
	}
	else
	{
		xil_printf("----- FAILED CREATING Task------\r\n");
	}
}

void taskMain(void *pvParameters)
{
	xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
	xil_printf("----- Task Main start------\r\n");
	// 1. 葛电 怕胶农甫 积己茄促.
	createAllTask();
	xil_printf("----- created all tasks ------\r\n");
	// 2. 磊脚阑 昏力窍哥 辑厚胶 胶纳临傅阑 矫累茄促.
	xil_printf("DEL -- %s\r\n", pcTaskGetName(NULL));
	vTaskDelete (NULL);	// == vTaskDelete (NULL)
}
