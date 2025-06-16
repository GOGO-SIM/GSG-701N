#include "gsgTypes.h"
#include "global.h"
#include "gsmpWrapper.h"
#include "lwip/err.h"
#include "lwip/api.h"
#include "xtime_l.h"

#define ACB_ERROR_POS 1
#define SEEKER_ERROR_POS 2
#define IMU_ERROR_POS 3
#define CBIT_ERROR_POS 4
#define PBIT_ERROR_POS 6

#define TELEMETRY_IP  "192.168.1.4"
#define TELEMETRY_PORT    1234
extern struct netconn *gpUdpClientConn;
struct netbuf *teleSendBuf;

static int sTeleMsgLen = sizeof(tGsmpMessageHeader) +  sizeof(tTelemetryData) + 2;
static tTelemetryData sTelemetryPayload;
static int sframeCount;
static u8 sTaskStatusFlags;


static void writeFlag()
 {
     sTaskStatusFlags = 0x00; // Clear All

     if(gFailCount[UART_FAIL] != FALSE)
     {
    	 // ACB_ERROR_POS = 1
        sTaskStatusFlags |= ( TRUE << ACB_ERROR_POS );
     }
     if(gFailCount[IMU_DATA_FAIL] != FALSE)
     {
    	 // SEEKER_ERROR_POS = 2
        sTaskStatusFlags |= ( TRUE << SEEKER_ERROR_POS );
     }
     if(gFailCount[SEEKER_DATA_FAIL] != FALSE)
     {
    	 // IMU_ERROR_POS = 3
        sTaskStatusFlags |= ( TRUE << IMU_ERROR_POS );
     }
     // CBIT_ERROR_POS = 1
     sTaskStatusFlags |= ( !gPassCbitFlag << CBIT_ERROR_POS );
     // PBIT_ERROR_POS = 1
     sTaskStatusFlags |= ( !gPassPbitFlag << PBIT_ERROR_POS );
 }

 static void writePayload()
 {
   sTelemetryPayload.cnt = sframeCount;  // Write Current FrameCount
   sTelemetryPayload.bitFlags = sTaskStatusFlags; // Write Current StatusFlag;
   sTelemetryPayload.imuData = gImuData;
   sTelemetryPayload.seekerData = gSeekerData;
   sTelemetryPayload.accCmd = gAccCmd;
   sTelemetryPayload.quarternion = gAttitude;
   sTelemetryPayload.controlCmd = gAngAccelCmd;
 }

 static void sendViaUdp()
 {
     err_t err;

     teleSendBuf = netbuf_new();
     if (!teleSendBuf)
     {
        xil_printf("Failed to Create nebuf\r\n");
     }
     netbuf_ref(teleSendBuf, gSendBuffer, sTeleMsgLen);
     err = netconn_send(gpUdpClientConn, teleSendBuf);
     if (err != ERR_OK)
     {
        xil_printf("\nFailed to send UDP packet: %d\r\n\n", err);
     }
     else
     {
        //xil_printf("\nSent Tele UDP packet to %s:%d\r\n\n", TELEMETRY_IP, TELEMETRY_PORT);
     }
     netbuf_delete(teleSendBuf);
 }

 static void runTelemetry()
 {
	 sframeCount += 1;
	 writeFlag();
	 writePayload();
	 gsmpWrapper(TELEMETRY_MSG_ID,OK,&sTelemetryPayload);
	 sendViaUdp();
 }


 void telemetryTaskMain(void *param)
 {
     for(;;)
     {
       ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
       //xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));

       XTime start, end;
       XTime_GetTime(&start);  // 시작 시간 기록
       runTelemetry();
       XTime_GetTime(&end);    // 종료 시간 기록
       // 시간 차이 계산 (us 단위)
       uint64_t elapsed_ticks = end - start;
       uint64_t elapsed_us = elapsed_ticks / (COUNTS_PER_SECOND / 1000000);

       xil_printf("Telemetry: %llu\r\n", elapsed_ticks);


     }
 }
