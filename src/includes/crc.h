#ifndef CRC_UTILS_H
#define CRC_UTILS_H

#include <stdint.h>
#include <string.h>  // for memcpy

// TRUE / FALSE 매크로 정의
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// CRC-16/XMODEM 관련 상수
extern const uint16_t polynomial;   // 다항식: x^16 + x^12 + x^5 + 1
extern const uint16_t defaultVal;   // 초기값

// 메시지 헤더 구조체 정의
typedef struct _tGsmpMessageHeader
{
	uint8_t startBit;
	uint8_t startflag;
	uint8_t msgId;
	uint8_t srcId;
	uint8_t destId;
	uint8_t msgStat;
	uint16_t msgLen;  // payload 길이
} tGsmpMessageHeader;

// CRC 계산 함수 (CRC-16/XMODEM)
uint16_t calcCrc(const void* data, uint16_t length);

// CRC 검사 함수
int checkCrc(void* recvData);

#endif
