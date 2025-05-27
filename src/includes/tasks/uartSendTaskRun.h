// uartSendTaskMain.h
#ifndef UART_SEND_TASK_MAIN_H
#define UART_SEND_TASK_MAIN_H
#include "global.h"
#include "gsgTypes.h"
#include "xil_printf.h"
#include "crc.h"
#include "xuartps.h"
#include "xparameters.h"
#include "gsmpWrapper.h"

// blocking 무효화
#define xil_printf(...)  do {} while(0)

// This typedef contains configuration information for the device.
void runUartSend();
void explode();		// TODO : 삭제
void sendUartData(tGsmpMsg* msg);

#endif
