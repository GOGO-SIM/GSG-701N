#include "gsgTypes.h"
#include "global.h"

static tGsmpMsg telemetryMessage;
static tTelemetryData telemetryPayload;
static tGsmpMessageHeader telemetryHeader;
static int frameCount;

uint8_t taskFailedFlag; // ���� ������ ������ �½�ũ�� ���п��ο� ���� ����ִ�.

// ���н� ���� ��Ʈ�� 1�� SET (TRUE)

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

	// 2. �ڷ���Ʈ�� �޽��� 0x07
	telemetryHeader.msgId = 0x07;

	// 3. �۽����� GCU 0x00
	telemetryHeader.srcId = 0x00;

	// 4. �������� �ڷ���Ʈ�� 0x04
	telemetryHeader.destId = 0x04;

	// 5. �޽��� ���� = 0
	telemetryHeader.msgStat = 0x00;

	// 6. ���̷ε��� ����  (1 + 4 + 24 + 24 + 24 + 32 + 24 + 24) = 157byte
	telemetryHeader.msgLen = 157;

}

static void writePayload()
{
	// 1. ������ ī��Ʈ�� write

	telemetryPayload.cnt = frameCount; // static�̴�.

	// 2. ���� �½�ũ�� ���� ���θ� write

	telemetryPayload.bitFlags = taskFailedFlag;

	// 3. �����ؾ��ϴ� ������ write
	telemetryPayload.imuData = gImuData;
	telemetryPayload.seekerData = gSeekerData;
	telemetryPayload.accCmd = gAccCmd;
	telemetryPayload.quarternion = gAttitude;
	telemetryPayload.controlCmd = gControlCmd;

	// ������ ������ ���� ������ �����ǰ� ������, �ٸ� Task���� ���� �������� ���� ����´�.

	// ���� ���� ���·� Payload�� �������ش�.
}

static void sendViaUdp()
{

   // TBD

}

static void runTelemetry()
{
		/*
		 * 1. framecount += 1
		 * 2. �޽��� status�� 0���� ����
		 * 3. telemetryPayload�� ���� Set
		 * 4. telemetryHeader�� ���� Set
		 * 5. �޽��� ��ŷ �� CRC����
		 * 6. UDP �۽�
		 */
		/*****************************[�ӽ� ����]*****************************/

	// 1.framecount += 1;

	frameCount += 1;

	// 2.telemetryHeader�� ���� Set

	writeHeader(); //

	// 3.telemetryPayload�� ���� Set

	writePayload();

	// 4. �޽��� ��ŷ �� CRC����

	packMessage();

	// 5. UDP �۽�

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
