#include "gsmpWrapper.h"
#define xil_printf(...)  do {} while(0)
/*
 * tGsmpMsg* gsmpWrapper(int messageId, int messageStatus, void* pPayload, int destId)
 * GSMP(GoSim Message Protocol)의 전송을 지원한다.
 * return : X
 *
 **/
void gsmpWrapper(int messageId, int messageStatus, void* pPayload)
{

	uint8_t headerSize = sizeof(tGsmpMessageHeader);
	uint8_t payloadSize;
	uint8_t crcSize = 2;
	tGsmpMsg* pDestMsg = NULL;
	uint8_t destId;
	/**
	 * [데이터의 포맷을 wrap한다.]
	 * 송수신하는 모든 시스템은 리틀 엔디안 형식을 사용한다고 가정한다.
	 * 1. 인자값을 바탕으로 바이트 스트림을 생성한다.
	 * 2. 생성한 바이트스트림을 반환한다.
	 */
	// 헤더 설정
	//msg.header.startflag = START_FLAG;
	//msg.header.msgId = messageId;

	switch(messageId)
	{
	case ACB_SEND_MSG_ID :
	{
		destId = ACB_ID;
		payloadSize = sizeof(tAcbPayload);
		//gAcbSendMsg
		pDestMsg = &gAcbSendMsg;
		xil_printf("ACB_SEND_MSG\r\n");
		break;
	}
	case ACB_ECHO_SEND_MSG_ID :
	{
		destId = ACB_ID;
		payloadSize = sizeof(uint8_t);
		//gAcbEchoSendMsg
		pDestMsg = &gAcbEchoSendMsg;
		break;
	}
	case TELEMETRY_MSG_ID :
	{
		destId = TELMETRY_ID;
		payloadSize = sizeof(tTelemetryData);
		//gTelemetryMsg
		pDestMsg = &gTelemetryMsg;
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
	// payload 저장
	if (pDestMsg != NULL && pPayload != NULL)
	{
		// header 저장
		pDestMsg->header.startflag = START_FLAG;
		pDestMsg->header.srcId = GCU_ID;
		pDestMsg->header.destId = destId;
		pDestMsg->header.msgId = messageId;
		pDestMsg->header.msgStat = messageStatus;
		pDestMsg->header.msgLen = payloadSize;

		memcpy(pDestMsg->payload, pPayload, payloadSize);
		xil_printf("WRAPPER : payloadSize = %d bytes\r\n", payloadSize);

		// CRC를 계산하여 tGsmpMsg에 저장
		// CRC 계산용 임시 버퍼를 지정하여 crc 연산을 수행한다.
		uint8_t tmpBuf[headerSize+payloadSize];
		//헤더 복사
		memcpy(tmpBuf, (const uint8_t*)&pDestMsg->header, headerSize);
		// payload 내용 복사 (payload가 가리키는 메모리 내용)
		memcpy(tmpBuf + headerSize, (const uint8_t*)pDestMsg->payload, payloadSize);
		// CRC 계산
		pDestMsg->CRC = calcCrc(tmpBuf, headerSize + payloadSize);
	}

	return;
}
