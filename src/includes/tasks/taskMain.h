#pragma once
#ifndef _TASK_MAIN_H_
#define _TASK_MAIN_H_

void USER_THREADS(void);
void taskMain(void *pvParameters);
void initTaskMain(void *pvParameters);
void pbitTaskMain(void *pvParameters);
void stanbyIgnitionTaskMain(void *pvParameters);
void uartReceiveTaskMain(void *pvParameters);
void udpReceiveTaskMain(void *pvParameters);
void schedulingTaskMain(void *pvParameters);
void navigationTaskMain(void *pvParameters);
void guidanceTaskMain(void *pvParameters);
void controlTaskMain(void *pvParameters);
void uartSendTaskMain(void *pvParameters);
void cbitTaskMain(void *pvParameters);
void telemetryTaskMain(void *pvParameters);
void pbitFailTaskMain(void *pvParameters);
void explodeTaskMain(void *pvParameters);

#endif
