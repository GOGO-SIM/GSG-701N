#include "gsmpUnwrapper.h"
/*
 * void gsmpUnwrapper(void* recvByte)
 * GSMP(GoSim Message Protocol) 해석 함수.
 * return : dataPayload 데이터
 *
 **/

typedef union _uGsmpRecvPayload
{
	tImuPayload recvImuPayload;
	tSeekerPayload recvSeekerPayload;
	tAcbFeedbackPayload recvAcbFeedbackPayload;
	tAcbPayload recvAcbPayload;
	int32_t recvAcbEchoPayload;
	tTelemetryData recvTelemetryPayload;
} uGsmpRecvPayload;

static uGsmpRecvPayload payloadBuffer;

void gsmpUnWrapper(uint8_t* buffer, tGsmpMsg* msg)
{
	// header copy
	memcpy(&msg->header, buffer, sizeof(tGsmpMessageHeader));

	// payload copy

	uint8_t* payloadStart = buffer + sizeof(tGsmpMessageHeader);
	switch(msg->header.msgId)
	{
	case IMU_MSG_ID:
	{
		memcpy(&payloadBuffer.recvImuPayload, payloadStart, sizeof(tImuPayload));
		msg->payload = &payloadBuffer.recvImuPayload;
		break;
	}
	case SEEKER_MSG_ID:
	{
		memcpy(&payloadBuffer.recvSeekerPayload, payloadStart, sizeof(tSeekerPayload));
		msg->payload = &payloadBuffer.recvSeekerPayload;
		break;
	}
	case ACB_RECV_MSG_ID:
	{
		memcpy(&payloadBuffer.recvAcbEchoPayload, payloadStart, sizeof(int32_t));
		msg->payload = &payloadBuffer.recvAcbEchoPayload;
		break;
	}
	case ACB_SEND_MSG_ID:
	{
		// THIS WILL NEVER BE ENTERED
		xil_printf("ERROR : WRONG HEADER - MSG ID");
		break;
	}
	case ACB_ECHO_SEND_MSG_ID:
	{
		// THIS WILL NEVER BE ENTERED
		xil_printf("ERROR : WRONG HEADER - MSG ID");
		break;
	}
	case ACB_ECHO_RECV_MSG_ID:
	{
		memcpy(&payloadBuffer.recvAcbEchoPayload, payloadStart, sizeof(int32_t));
		msg->payload = &payloadBuffer.recvAcbEchoPayload;
		break;
	}
	case TELEMETRY_MSG_ID:
	{
		// THIS WILL NEVER BE ENTERED
		xil_printf("ERROR : WRONG HEADER - MSG ID");
		break;
	}
	default:
		xil_printf("ERROR : HEADER MSG ID NOT FOUND - %d\r\n", msg->header.msgId);
	}
	if (msg->payload == NULL)
	{
		xil_printf("Failed to allocate memory for payload\r\n");
		return;
	}
	// copy CRC
	//msg->CRC = (buffer[sizeof(tGsmpMessageHeader) + msg->header.msgLen] << 8) | buffer[sizeof(tGsmpMessageHeader) + msg->header.msgLen + 1];
}
