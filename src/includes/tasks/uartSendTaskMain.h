// uartSendTaskMain.h
#ifndef UART_SEND_TASK_MAIN_H
#define UART_SEND_TASK_MAIN_H


// This typedef contains configuration information for the device.
int initUartPs();
void sendData(uint8_t* buffer, uint16_t len);

void uartSendTaskMain(void *pvParameters);

#endif
