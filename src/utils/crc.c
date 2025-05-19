#include <stdio.h>
#include <string.h>
#include "gsgTypes.h"
#include "global.h"
#include "crc.h"

// CRC설정 함수
// 변환된 CRC 값 return
uint16_t calcCrc(const void* data, uint16_t length)
{
	uint16_t crc = defaultVal;
	// void* -> uint8_t* 로 변환
	const uint8_t* bytes = (const uint8_t*) data;

	for(uint16_t i = 0; i < length; ++i)
	{
		crc ^= (bytes[i] << 8);

		for(uint8_t j = 0; j < 8; ++j)
		{
			if(crc & 0x8000)
			{
				crc = (crc << 1) ^ polynomial;
			}
			else
			{
				crc <<= 1;
			}
		}
	}
    return crc;

}

// CRC 검사 함수
// CRC 검사 성공 후 성공|실패 반환
int checkCrc(void* recvData)
{
    if (!recvData) return FALSE;

    // 1. 헤더 추출 (메모리 안정성을 위해 복사)
    tGsmpMessageHeader header;
    memcpy(&header, recvData, sizeof(tGsmpMessageHeader));

    // 2. 검사 대상 길이 계산 (CRC 제외)
    uint16_t dataLen = sizeof(tGsmpMessageHeader) + header.msgLen;

    // 3. CRC 계산 (CRC 제외 범위만 계산)
    uint16_t crc = calcCrc(recvData, dataLen);

    // 4. 수신된 CRC 값 추출 (MSB 먼저)
    uint8_t* bytes = (uint8_t*)recvData;
    uint16_t recvCrc = (bytes[dataLen] << 8) | bytes[dataLen + 1];

    // 5. 비교
    return (crc == recvCrc) ? TRUE : FALSE;
}
