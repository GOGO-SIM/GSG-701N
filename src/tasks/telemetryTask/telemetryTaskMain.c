#include "gsgTypes.h"
#include "global.h"

static tGsmpMsg telemetryMessage;
static tTelemetryData telemetryPayload;
static tGsmpMessageHeader telemetryHeader;
static int frameCount;

uint8_t taskFailedFlag; // 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占승쏙옙크占쏙옙 占쏙옙占싻울옙占싸울옙 占쏙옙占쏙옙 占쏙옙占쏙옙獵占�.

// 占쏙옙占싻쏙옙 占쏙옙占쏙옙 占쏙옙트占쏙옙 1占쏙옙 SET (TRUE)

// taskFailedFlag = || PBIT || - || CBIT || - || IMU || Seeker || ACB || - ||
//---------------------------------------------------------------------------
// taskFailedFlag = ||  7   || 6 ||  5   || 4 ||  3  ||   2    ||  1  || 0 ||

static void packMessage()
{
	telemetryMessage.payload = &telemetryPayload;
	telemetryMessage.header = telemetryHeader;
	//telemetryMessage.CRC = setCrc(const void* data, uint16_t length);
}

static void writeHeader()
{
	// 1. Start Flag Set
	telemetryHeader.startflag = 0x7E;

	// 2. 占쌘뤄옙占쏙옙트占쏙옙 占쌨쏙옙占쏙옙 0x07
	telemetryHeader.msgId = 0x07;

	// 3. 占쌜쏙옙占쏙옙占쏙옙 GCU 0x00
	telemetryHeader.srcId = 0x00;

	// 4. 占쏙옙占쏙옙占쏙옙占쏙옙 占쌘뤄옙占쏙옙트占쏙옙 0x04
	telemetryHeader.destId = 0x04;

	// 5. 占쌨쏙옙占쏙옙 占쏙옙占쏙옙 = 0
	telemetryHeader.msgStat = 0x00;

	// 6. 占쏙옙占싱로듸옙占쏙옙 占쏙옙占쏙옙  (1 + 4 + 24 + 24 + 24 + 32 + 24 + 24) = 157byte
	telemetryHeader.msgLen = 157;

}

static void writePayload()
{
	// 1. 占쏙옙占쏙옙占쏙옙 카占쏙옙트占쏙옙 write

	telemetryPayload.cnt = frameCount; // static占싱댐옙.

	// 2. 占쏙옙占쏙옙 占승쏙옙크占쏙옙 占쏙옙占쏙옙 占쏙옙占싸몌옙 write

	telemetryPayload.bitFlags = taskFailedFlag;

	// 3. 占쏙옙占쏙옙占쌔억옙占싹댐옙 占쏙옙占쏙옙占쏙옙 write
	telemetryPayload.imuData = gImuData;
	telemetryPayload.seekerData = gSeekerData;
	telemetryPayload.accCmd = gAccCmd;
	telemetryPayload.quarternion = gAttitude;
	telemetryPayload.controlCmd = gControlCmd;

	// 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占실곤옙 占쏙옙占쏙옙占쏙옙, 占쌕몌옙 Task占쏙옙占쏙옙 占쏙옙占쏙옙爭� 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙쨈占�.

	// 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占승뤄옙 Payload占쏙옙 占쏙옙占쏙옙占쏙옙占쌔댐옙.
}

static void sendViaUdp()
{

   // TBD

}

static void runTelemetry()
{
		/*
		 * 1. framecount += 1
		 * 2. 占쌨쏙옙占쏙옙 status占쏙옙 0占쏙옙占쏙옙 占쏙옙占쏙옙
		 * 3. telemetryPayload占쏙옙 占쏙옙占쏙옙 Set
		 * 4. telemetryHeader占쏙옙 占쏙옙占쏙옙 Set
		 * 5. 占쌨쏙옙占쏙옙 占쏙옙킹 占쏙옙 CRC占쏙옙占쏙옙
		 * 6. UDP 占쌜쏙옙
		 */
		/*****************************[占쌈쏙옙 占쏙옙占쏙옙]*****************************/

	// 1.framecount += 1;

	frameCount += 1;

	// 2.telemetryHeader占쏙옙 占쏙옙占쏙옙 Set

	writeHeader(); //

	// 3.telemetryPayload占쏙옙 占쏙옙占쏙옙 Set

	writePayload();

	// 4. 占쌨쏙옙占쏙옙 占쏙옙킹 占쏙옙 CRC占쏙옙占쏙옙

	packMessage();

	// 5. UDP 占쌜쏙옙

	sendViaUdp();

}

static void run()
{
    
}

void telemetryTaskMain(void *param)
{
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
		run();
    }
//    for(;;)
//    {
//        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
//        runTelemetry();
//    }
}
