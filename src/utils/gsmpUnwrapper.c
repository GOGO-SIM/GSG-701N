#include "gsmpUnwrapper.h"
/*
 * void gsmpUnwrapper(void* recvByte)
 * GSMP(GoSim Message Protocol)�� ������ �����Ѵ�.
 * return : dataPayload ��ȯ
 *
 **/
void* gsmpUnWrapper(uint8_t* buffer, tGsmpMsg* msg)
{
	// header copy
	memcpy(&msg->header, buffer, sizeof(tGsmpMessageHeader));

	// payload copy
	switch(msg->header.msgId)
	{
	case IMU_MSG_ID:
	{
		msg->payload = pvPortMalloc(sizeof(tDVector3));
		break;
	}
	case SEEKER_MSG_ID:
	{
		msg->payload = pvPortMalloc(sizeof(tDVector3));
		break;
	}
	case ACB_MSG_ID:
	{
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
		msg->payload = pvPortMalloc(sizeof(int32_t));
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

	memcpy(msg->payload, buffer + sizeof(tGsmpMessageHeader), msg->header.msgLen);

	// copy CRC
	msg->CRC = (buffer[sizeof(tGsmpMessageHeader) + msg->header.msgLen] << 8) | buffer[sizeof(tGsmpMessageHeader) + msg->header.msgLen + 1];
}
