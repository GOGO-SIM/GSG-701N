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

typedef struct _tAccCmd
{
	int currentStatus;
	tDVector3 canadCmd;
} tAccCmd;

typedef struct _tTelemetryData
{
	uint8_t bitFlags;
	int cnt;
	double dist;
	tImuPayload imuPayload;
	tSeekerPayload seekerPayload;
	tDVector4 Quarternion;
	tDVector3 accCmd;
	tAcbPayload ctlCmd;
} tTelemetryData;

#endif
