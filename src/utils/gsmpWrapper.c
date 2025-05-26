#include "gsmpWrapper.h"

/*
 * tGsmpMsg* gsmpWrapper(int messageId, int messageStatus, void* pPayload, int destId)
 * GSMP(GoSim Message Protocol)의 전송을 지원한다.
 * return : gsmpMsg 반환
 *
 **/
void gsmpWrapper(int messageId, int messageStatus, void* pPayload, int destId)
{

	uint8_t headerSize = sizeof(tGsmpMessageHeader);
	uint8_t payloadSize;
	uint8_t crcSize = 2;

	/**
	 * [데이터의 포맷을 wrap한다.]
	 * 송수신하는 모든 시스템은 리틀 엔디안 형식을 사용한다고 가정한다.
	 * 1. 인자값을 바탕으로 바이트 스트림을 생성한다.
	 * 2. 생성한 바이트스트림을 반환한다.
	 */
	tGsmpMsg msg;
	// 헤더 설정
	msg.header.startflag = START_FLAG;
	msg.header.msgId = messageId;
	switch(messageId)
	{
	case ACB_SEND_MSG_ID :
	{
		msg.header.destId = ACB_ID;
		payloadSize = sizeof(tAcbPayload);
		break;
	}
	case ACB_ECHO_SEND_MSG_ID :
	{
		msg.header.destId = ACB_ID;
		payloadSize = sizeof(uint8_t);
		break;
	}
	case TELEMETRY_MSG_ID :
	{
		msg.header.destId = TELMETRY_ID;
		payloadSize = sizeof(tTelemetryData);
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

	uint8_t bufferSizeNoCRC = sizeof(tGsmpMessageHeader) + payloadSize;

	msg.header.srcId = GCU_ID;
	msg.header.destId = ACB_ID;
	msg.header.msgStat = messageStatus;
	msg.header.msgLen = payloadSize;
	// pPayload와 메세지 형식 내 지정된 사이즈 동일한지 검사
	msg.payload = &pPayload;

	// CRC를 계산하여 tGsmpMsg에 저장한다.
	// CRC 계산용 임시 버퍼를 지정하여 crc 연산을 수행한다.
	uint8_t tempBuf[bufferSizeNoCRC];
	memcpy(tempBuf, (const uint8_t*) &msg.header, headerSize);
	memcpy(tempBuf + headerSize, (const uint8_t*) msg.payload, sizeof(pPayload));
	uint16_t crc = calcCrc(tempBuf, sizeof(tempBuf));

	msg.CRC = crc;
	memcpy(msg, gSendMsg, sizeof(gSendMsg));
	return;

}
