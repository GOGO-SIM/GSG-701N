#pragma once
#ifndef _GSG_TYPES_H_
#define _GSG_TYPES_H_

#include <stdint.h>

#pragma pack(push, 1)
typedef struct _tDVector3
{
	double x;
	double y;
	double z;
} tDVector3;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _tDVector4
{
	double w;
	double x;
	double y;
	double z;
} tDVector4;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _tGsmpMessageHeader
{
	uint8_t startflag;
	uint8_t msgId;
	uint8_t srcId;
	uint8_t destId;
	uint8_t msgStat;
	uint16_t msgLen;

} tGsmpMessageHeader;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct sGsmpMsg
{
	tGsmpMessageHeader header;
	void* payload;
	uint16_t CRC;

} tGsmpMsg;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _tImuPayload
{
	tDVector3 acc;
	tDVector3 gyro;
} tImuPayload;
#pragma pack(pop)

typedef tImuPayload tImuData;

#pragma pack(push, 1)
typedef struct _tSeekerPayload
{
	tDVector3 los;
	double distance;
} tSeekerPayload;
#pragma pack(pop)

typedef tSeekerPayload tSeekerData;

#pragma pack(push, 1)
typedef struct _tACBPayload
{
	int currentStatus;
	tDVector3 canadCmd;
} tAcbPayload;
#pragma pack(pop)

// ACB Feedback Message
#pragma pack(push, 1)
typedef struct _tAcbFeedbackPayload
{
	tDVector3 canadCmd;
} tAcbFeedbackPayload;
#pragma pack(pop)

// controlTask input
#pragma pack(push, 1)
typedef struct _tAccCmd
{
	int currentStatus;
	tDVector3 canadCmd;
} tAccCmd;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _tTelemetryData
{
	int cnt;
	//int status;
	uint8_t bitFlags;
	tImuData imuData;
	tSeekerData seekerData;
	tDVector3 accCmd;
	tDVector4 quarternion;
	tDVector3 controlCmd;

} tTelemetryData;
#pragma pack(pop)

/*
 * [Receive Payload]
 */
typedef tDVector3 tAcbRecvPayload;
typedef uint32_t tEchoPayload;

#endif
