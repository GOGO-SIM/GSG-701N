#include <stdio.h>
#include <string.h>
#include "global.h"
#include "gsgTypes.h"
#include "crc.h"
#include "xparameters.h"
#include "gsmpUnwrapper.h"


/* TODO : 테스트 데이터 예시 크기이므로 추후 리팩토링 필요 #56
 * RECEIVE_SIZE 재정의
 * exampleMsg -> recvUartMsg, 전역변수화
 * */
#define DATA_RECEIVE_SIZE 34
extern uint8_t rxBuffer[DATA_RECEIVE_SIZE];
extern tGsmpMsg exampleMsg;

int recvUartBytes(uint8_t* buffer, int len);
void runUartReceive();
void explode();
