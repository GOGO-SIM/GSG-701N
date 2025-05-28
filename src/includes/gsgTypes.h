#pragma once
#ifndef _GSG_TYPES_H_
#define _GSG_TYPES_H_

#include <stdint.h>

typedef struct _tDVector3
{
	double x;
	double y;
	double z;
} tDVector3;

typedef struct _tDVector4
{
	double w;
	double x;
	double y;
	double z;
} tDVector4;

typedef struct _tGsmpMessageHeader
{
	uint8_t startflag;
	uint8_t msgId;
	uint8_t srcId;
	uint8_t destId;
	uint8_t msgStat;
	uint16_t msgLen;

} tGsmpMessageHeader;

typedef struct sGsmpMsg
{
	tGsmpMessageHeader header;
	void* payload;
	uint16_t CRC;

} tGsmpMsg;

typedef struct _tImuPayload
{
	tDVector3 acc;
	tDVector3 gyro;
} tImuPayload;

typedef tImuPayload tImuData;

typedef struct _tSeekerPayload
{
	tDVector3 los;
	double distance;
} tSeekerPayload;

typedef tSeekerPayload tSeekerData;

typedef struct _tACBPayload
{
	int currentStatus;
	tDVector3 canadCmd;
} tAcbPayload;

// controlTask input
typedef struct _tAccCmd
{
	int currentStatus;
	tDVector3 canadCmd;
} tAccCmd;

typedef struct _tTelemetryData
{
	int cnt;
	uint8_t bitFlags;
	tImuData imuData;
	tSeekerData seekerData;
	tDVector3 accCmd;
	tDVector4 quarternion;
	tDVector3 controlCmd;

} tTelemetryData;

#endif
