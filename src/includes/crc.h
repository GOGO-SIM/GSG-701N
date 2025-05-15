#ifndef CRC_UTILS_H
#define CRC_UTILS_H

#include <stdint.h>
#include <string.h>  // for memcpy
#include "global.h"

// 다항식, x^16 + x^12 + x^5 + 1
static const uint16_t polynomial = 0x1021;
// 초기값
static const uint16_t defaultVal = 0x0000;


// CRC 계산 함수 (CRC-16/XMODEM)
uint16_t calcCrc(const void* data, uint16_t length);

// CRC 검사 함수
int checkCrc(void* recvData);

#endif
