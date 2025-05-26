#include <stdint.h>
#include <string.h>
#include "crc.h"
#include "xuartps.h"
#include "xparameters.h"
#include "gsgTypes.h"
#include "global.h"

/*
 * void* gsmpWrapper(int messageId, int messageStatus, void* pPayload)
 * GSMP(GoSim Message Protocol)의 전송을 지원한다.
 * return : ByteStream 반환
 **/
void gsmpWrapper(int messageId, int messageStatus, void* pPayload);
