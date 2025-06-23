#include "gsmpWrapper.h"
#define xil_printf(...)  do {} while(0)
/*
 * tGsmpMsg* gsmpWrapper(int messageId, int messageStatus, void* pPayload)
 * GSMP(GoSim Message Protocol)의 전송을 지원한다. 데이터 스트림을 gSendData에 저장한다.
 * return : -
 *
 **/
void gsmpWrapper(int messageId, int messageStatus, void* pPayload)
{

	const uint8_t headerSize = sizeof(tGsmpMessageHeader);
	uint8_t payloadSize;
	tGsmpMessageHeader header;
	uint8_t destId;
	/**
	 * [데이터의 포맷을 wrap한다.]
	 * 송수신하는 모든 시스템은 리틀 엔디안 형식을 사용한다고 가정한다.
	 * 1. 인자값을 바탕으로 바이트 스트림을 생성하여 저장한다.
	 */
	switch(messageId)
	{
	case ACB_SEND_MSG_ID :
	{
		destId = ACB_ID;
		payloadSize = sizeof(tAcbPayload);
		break;
	}
	case ACB_ECHO_SEND_MSG_ID :
	{
		destId = ACB_ID;
		payloadSize = sizeof(tEchoPayload);
		break;
	}
	case TELEMETRY_MSG_ID :
	{
		destId = TELMETRY_ID;
		payloadSize = sizeof(tTelemetryData);
		break;
	}
	case SEEKER_ECHO_SEND_MSG_ID :
	{
		destId = SEEKER_ID;
		payloadSize = sizeof(tEchoPayload);
		break;
	}
	case IMU_MSG_ID :
	case SEEKER_MSG_ID :
	case ACB_RECV_MSG_ID :
	case ACB_ECHO_RECV_MSG_ID :
	{
		xil_printf("ERROR : WRONG MSGID - GSMPWRAPPER\r\n");
		break;
	}
	default :
	{
		break;
	}
	}
	if (pPayload != NULL)
	{
		// header 매핑
		header.startflag = START_FLAG;
		header.srcId = GCU_ID;
		header.destId = destId;
		header.msgId = messageId;
		header.msgStat = messageStatus;
		header.msgLen = payloadSize;

		// gSendBuffer 구성
	    // 1. 헤더 복사
	    memcpy(gSendBuffer, &header, headerSize);
	    // 2. payload 복사
	    memcpy(gSendBuffer + headerSize, (const uint8_t*) pPayload, payloadSize);

		// CRC 계산
		uint16_t crc = calcCrc(gSendBuffer, headerSize + payloadSize);

	    // 3. CRC 복사 (Little Endian)
	    gSendBuffer[headerSize + payloadSize + 0] = crc & 0xFF;
	    gSendBuffer[headerSize + payloadSize + 1] = (crc >> 8) & 0xFF;

	}

	return;
}
