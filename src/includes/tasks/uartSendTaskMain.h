// uartSendTaskMain.h
#ifndef UART_SEND_TASK_MAIN_H
#define UART_SEND_TASK_MAIN_H


// This typedef contains configuration information for the device.
void runUartSend();
void explode();		// TODO : 삭제
void sendData(uint8_t* buffer, uint16_t len);
void uartSendTaskMain(void *pvParameters);

static const uint8_t headerSize = sizeof(tGsmpMessageHeader);
static const uint8_t cmdSize = sizeof(gControlCmd);
static const uint8_t crcSize = 2;
static const uint8_t bufferSizeNoCRC = sizeof(tGsmpMessageHeader) + sizeof(gControlCmd);

#endif
