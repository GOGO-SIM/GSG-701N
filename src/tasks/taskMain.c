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
	 * 1. Pbit Task ����
	 * 2. Cbit Task ����
	 * 3. Standby Task ����
	 * 4. Seeker Task ����
	 * 5. IMU Task ����
	 * 6. ACB Task ����
	 * 7. Telemetry Task ����
	 * 8. navigation Task ����
	 * 9. guidance Task ����
	 * 10. control Task ����
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
	// 1. ��� �½�ũ�� �����Ѵ�.
	createAllTask();
	// 2. �ڽ��� �����Ͽ� ���� �����ٸ��� �����Ѵ�.
	vTaskDelete (xHandleMain);	// == vTaskDelete (NULL);
}
