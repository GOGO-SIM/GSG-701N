#include <stdio.h>
#include<string.h>

// ���׽�, x^16 + x^12 + x^5 + 1
const uint16_t polynomial = 0x1021;
// �ʱⰪ
const uint16_t defaultVal = 0x0000;

// CRC ���� �Լ�
// ��ȯ�� CRC �� return
uint16_t setCrc(const void* data, uint16_t length)
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
int getCrc(void* recvData)
{
}
