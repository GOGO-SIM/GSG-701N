/*
 * Starts all the other tasks, then starts the scheduler.
 */
void USER_THREADS( void )
{
	/* Create one of the two tasks. */
	xTaskCreate(	(TaskFunction_t)TaskMain,
					"TaskMain",
					256,
					NULL,
					TASK_MAIN_PRIO,
					&xHandleMain );
}

static void createAllTask(void)
{
	/**
	 * 1. Pbit Task 积己
	 * 2. Cbit Task 积己
	 * 3. Standby Task 积己
	 * 4. Seeker Task 积己
	 * 5. IMU Task 积己
	 * 6. ACB Task 积己
	 * 7. Telemetry Task 积己
	 * 8. navigation Task 积己
	 * 9. guidance Task 积己
	 * 10. control Task 积己
	 */

	xTaskCreate(	(TaskFunction_t)CbitTask,	/* Pointer to the function that implements the task. */
					"CbitTask",					/* Text name for the task.  This is to facilitate debugging only. */
					256,						/* Stack depth - most small microcontrollers will use much less stack than this. */
					NULL,						/* We are not using the task parameter. */
					CBIT_PRIO,					/* This task will run at this priority */
					&CbitHandle);				/* We are not using the task handle. */
}

void TaskMain( void *pvParameters )
{
	// 1. 葛电 怕胶农甫 积己茄促.
	createAllTask();
	// 2. 磊脚阑 昏力窍咯 辑厚胶 胶纳临傅阑 矫累茄促.
	vTaskDelete (xHandleMain);	// == vTaskDelete (NULL);
}
