#ifndef CRC_UTILS_H
#define CRC_UTILS_H

#include <stdint.h>
#include <string.h>  // for memcpy

// TRUE / FALSE ��ũ�� ����
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// CRC-16/XMODEM ���� ���
extern const uint16_t polynomial;   // ���׽�: x^16 + x^12 + x^5 + 1
extern const uint16_t defaultVal;   // �ʱⰪ

// �޽��� ��� ����ü ����
typedef struct _tGsmpMessageHeader
{
	uint8_t startBit;
	uint8_t startflag;
	uint8_t msgId;
	uint8_t srcId;
	uint8_t destId;
	uint8_t msgStat;
	uint16_t msgLen;  // payload ����
} tGsmpMessageHeader;

// CRC ��� �Լ� (CRC-16/XMODEM)
uint16_t calcCrc(const void* data, uint16_t length);

// CRC �˻� �Լ�
int checkCrc(void* recvData);

#endif
