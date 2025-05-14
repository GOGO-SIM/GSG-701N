#include "gsgTypes.h"

static tGsmpMsg telemetryMessage;
static tTelemetryData telemetryPayload;
static tGsmpMessageHeader telemetryHeader;
static int frameCount;

uint8_t taskFailedFlag; // 전역 변수로 각각의 태스크의 실패여부에 대해 들어있다.

// 실패시 각각 비트가 1로 SET (TRUE)

// taskFailedFlag = || PBIT || - || CBIT || - || IMU || Seeker || ACB || - ||
//---------------------------------------------------------------------------
// taskFailedFlag = ||  7   || 6 ||  5   || 4 ||  3  ||   2    ||  1  || 0 ||

static void packMessage()
{
	telemetryMessage.payload = telemetryPayload;
	telemetryMessage.header = telemetryHeader;
	//telemetryMessage.CRC = setCrc(const void* data, uint16_t length);
}

static void writeHeader()
{
	// 1. Start Flag Set
	telemetryHeader.startflag = 0x7E;

	// 2. 텔레메트리 메시지 0x07
	telemetryHeader.msgId = 0x07;

	// 3. 송신지는 GCU 0x00
	telemetryHeader.srcId = 0x00;

	// 4. 수신지는 텔레메트리 0x04
	telemetryHeader.destId = 0x04;

	// 5. 메시지 스탯 = 0
	telemetryHeader.msgStat = 0x00;

	// 6. 페이로드의 길이  (1 + 4 + 24 + 24 + 24 + 32 + 24 + 24) = 157byte
	telemetryHeader.msgLen = 157;

}

static void writePayload()
{
	// 1. 프레임 카운트를 write

	telemetryPayload.cnt = frameCount; // static이다.

	// 2. 각각 태스크의 실패 여부를 write

	telemetryPayload.bitFlags = taskFailedFlag;

	// 3. 전송해야하는 값들을 write

	telemetryPayload.ctlCmd = ctlCmd;
	telemetryPayload.seekerPayload = seekerPayload;
	telemetryPayload.imuPayload = imuPayload;
	telemetryPayload.accCmd = accCmd;
	telemetryPayload.losVector = losVector;
	telemetryPayload.Quarternion = Quarternion;

	// 각각의 값들은 전역 변수로 공유되고 있으며, 다른 Task에서 만들어낸 전역변수 값을 들고온다.

	// 위와 같은 형태로 Payload를 구성해준다.
}

static void sendViaUdp()
{

   // TBD

}

static void runTelemetry()
{
		/*
		 * 1. framecount += 1
		 * 2. 메시지 status를 0으로 설정
		 * 3. telemetryPayload에 내용 Set
		 * 4. telemetryHeader에 내용 Set
		 * 5. 메시지 패킹 및 CRC설정
		 * 6. UDP 송신
		 */
		/*****************************[임시 구현]*****************************/

	// 1.framecount += 1;

	framecount += 1;

	// 2.telemetryHeader에 내용 Set

	writeHeader(); //

	// 3.telemetryPayload에 내용 Set

	writePayload();

	// 4. 메시지 패킹 및 CRC설정

	packMessage();

	// 5. UDP 송신

	sendViaUdp();

}

void telemetryTaskMain(void *param)
{
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        runTelemetry();
    }
}
