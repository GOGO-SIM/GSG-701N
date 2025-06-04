#include "gsmpUnwrapper.h"
/*
 * void gsmpUnwrapper(void* recvByte)
 * GSMP(GoSim Message Protocol) 해석 함수.
 * 메시지 타입에 따라서 각 payload의 전역 변수에 값이 세팅 된다.
 * TODO: void 반환에서 특정 메시지를 반환할 수 있도록 만들기
 **/

void gsmpUnwrapper(uint8_t* buffer, tGsmpMsg* msg)
{
	// header copy
	memcpy(&msg->header, buffer, sizeof(tGsmpMessageHeader));
  
	switch(msg->header.msgId)
	{
		case IMU_MSG_ID:
		{
			memcpy(&gImuPayload, buffer + sizeof(tGsmpMessageHeader), sizeof(tImuPayload));
			break;
		}
		case SEEKER_MSG_ID:
		{
			memcpy(&gSeekerPayload, buffer + sizeof(tGsmpMessageHeader), sizeof(tSeekerPayload));
			break;
		}
		case ACB_RECV_MSG_ID:
		{
			memcpy(&gAcbRecvPayload, buffer + sizeof(tGsmpMessageHeader), sizeof(tAcbRecvPayload));
			break;
		}
		case ACB_ECHO_RECV_MSG_ID:
		{
			memcpy(&gEchoPayload, buffer + sizeof(tGsmpMessageHeader), sizeof(tEchoPayload));
			break;
		}
		default :
		{
			// TODO: Error handling
			xil_printf("ERROR : HEADER MSG ID NOT FOUND - %d\r\n", msg->header.msgId);
		}
	}
}
