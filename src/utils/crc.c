#include <stdio.h>
#include <string.h>
#include "gsgTypes.h"
#include "global.h"
#include "crc.h"

// CRC���� �Լ�
// ��ȯ�� CRC �� return
uint16_t calcCrc(const void* data, uint16_t length)
{
	uint16_t crc = defaultVal;
	// void* -> uint8_t* �� ��ȯ
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

// CRC �˻� �Լ�
// CRC �˻� ���� �� ����|���� ��ȯ
int checkCrc(void* recvData)
{
    if (!recvData) return FALSE;

    // 1. ��� ���� (�޸� �������� ���� ����)
    tGsmpMessageHeader header;
    memcpy(&header, recvData, sizeof(tGsmpMessageHeader));

    // 2. �˻� ��� ���� ��� (CRC ����)
    uint16_t dataLen = sizeof(tGsmpMessageHeader) + header.msgLen;

    // 3. CRC ��� (CRC ���� ������ ���)
    uint16_t crc = calcCrc(recvData, dataLen);

    // 4. ���ŵ� CRC �� ���� (MSB ����)
    uint8_t* bytes = (uint8_t*)recvData;
    uint16_t recvCrc = bytes[dataLen] | (bytes[dataLen + 1] << 8);

    // 5. ��
    return (crc == recvCrc) ? TRUE : FALSE;
}
