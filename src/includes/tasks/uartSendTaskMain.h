// uartSendTaskMain.h
#ifndef UART_SEND_TASK_MAIN_H
#define UART_SEND_TASK_MAIN_H


// This typedef contains configuration information for the device.
void runUartSend();
void explode();		// TODO : 삭제
void sendUartData();
void uartSendTaskMain(void *pvParameters);

static const uint8_t headerSize = sizeof(tGsmpMessageHeader);
static const uint8_t cmdSize = sizeof(tAcbPayload);
static const uint8_t crcSize = 2;
static const uint8_t bufferSizeNoCRC = sizeof(tGsmpMessageHeader) + sizeof(tAcbPayload);

#endif
