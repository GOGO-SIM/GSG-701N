#include <stdio.h>
#include "global.h"
#include "gsgTypes.h"

#define DATA_RECEIVE_SIZE 13


int recvUartBytes(uint8_t* buffer, int len);
void runUartReceive();
void explode();
