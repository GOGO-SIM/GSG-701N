#include "global.h"
#include "gsgTypes.h"

#define ECHOING_VALUE 19980398

// ADC 변환 Raw 값

static u16 sRawVccInt;
static u16 sRawVccAux;
static u16 sRawVccRam;
static u16 sRawTemperture;
static u16 sTimeOut;
static u32 sUartStatus;

static void delay_ms(u32 ms)
{
    TickType_t start = xTaskGetTickCount();
    while ( (xTaskGetTickCount() - start) < pdMS_TO_TICKS(ms) );
}

static void checkPower()
{
   // 각 측정 데이터 Raw 값으로 받아오기

   sRawVccInt = XSysMon_GetAdcData(&gSysMonInst, XSM_CH_VCCINT);
   sRawVccAux = XSysMon_GetAdcData(&gSysMonInst, XSM_CH_VCCAUX);
   sRawVccRam = XSysMon_GetAdcData(&gSysMonInst, XSM_CH_VBRAM);
   sRawTemperture = XSysMon_GetAdcData(&gSysMonInst, XSM_CH_TEMP);

   // Raw 데이터를 실제 전압,온도로 변환

   gVoltageInt = XSysMon_RawToVoltage(sRawVccInt);
   gVoltageBram = XSysMon_RawToVoltage(sRawVccRam);
   gVoltageAux = XSysMon_RawToVoltage(sRawVccAux);
   gCelcius = XSysMon_RawToTemperature(sRawTemperture);

   // 비정상 전압 혹은 온도 감지시 gPassPbitFlag False로 전환

   xil_printf(" GSG-701N / [PBIT] : Check for internal value...\r\n\n");

   if ( gVoltageInt < 0.97 || gVoltageInt > 1.03 )
   {
      gPassPbitFlag = FALSE;
   }
   printf("| 0.97V < %.3lfV < 1.03V | PassFlag: %u |\n",gVoltageInt,gPassPbitFlag);
   delay_ms(1000);

   if ( gVoltageBram < 0.97 || gVoltageBram > 1.03 )
   {
      gPassPbitFlag = FALSE;
   }
   printf("| 0.97V < %.3lfV < 1.03V | PassFlag: %u |\n",gVoltageBram,gPassPbitFlag);
   delay_ms(1000);

   if ( gVoltageAux < 1.7 || gVoltageAux > 1.88 )
   {
	  gPassPbitFlag = FALSE;
   }
   printf("| 1.70V < %.3lfV < 1.88V | PassFlag: %u |\n",gVoltageAux,gPassPbitFlag);
   delay_ms(1000);

   if ( gCelcius >  90 )
   {
      gPassPbitFlag = FALSE;
   }
   printf("| %.3lf'C < 90'C | PassFlag: %u |\n",gCelcius,gPassPbitFlag);
   delay_ms(1000);

   if(gPassPbitFlag == TRUE)
   {
	   xil_printf("\n GSG-701N / [PBIT] : Power Check Success\r\n\n");
   }
   else
   {
	   xil_printf("\n GSG-701N / [PBIT] : Power Check Failed\r\n\n");
   }
}

static void checkUartLoopback()
{
	char transmitBuffer[16], recieveBuffer[16];
	sTimeOut = 100000;

	// 루프백 모드로 변경
	XUartPs_SetOperMode(&gUartPs, XUARTPS_OPER_MODE_LOCAL_LOOP);

	int messageLength = snprintf(transmitBuffer,sizeof(transmitBuffer),"%u\r\n",ECHOING_VALUE);

	// 데이터 보내기
	XUartPs_Send(&gUartPs, (u8*)transmitBuffer, messageLength);

	// 값이 도착할동안 대기
	while (( (XUartPs_ReadReg(gUartConfig->BaseAddress, XUARTPS_SR_OFFSET) & XUARTPS_SR_RXEMPTY) != 0) && --sTimeOut);

    for (int i = 0; i < messageLength; ++i) {
        // 데이터가 오면 1바이트 리턴해서 받아오기
        recieveBuffer[i] = XUartPs_RecvByte(gUartConfig->BaseAddress);
    }

    recieveBuffer[messageLength] = '\0';

	XUartPs_SetOperMode(&gUartPs, XUARTPS_OPER_MODE_NORMAL);

	//디버깅용 -------------
	printf(" Uart Loopback Transmitted : %d\r\n\n",atoi(transmitBuffer));

	delay_ms(1000);

	printf(" Uart Loopback Recieved : %d\r\n\n",atoi(recieveBuffer));
	//디버깅용 -------------

	if(atoi(recieveBuffer) == ECHOING_VALUE)
	{
		//디버깅용 -------------
		xil_printf(" GSG-701N / [PBIT] : Uart Internal Loopback Check Passed\r\n\n");
		//디버깅용 -------------
	}
	else
	{
		//디버깅용 -------------
		xil_printf(" GSG-701N / [PBIT] : Uart Internal Loopback Check Failed\r\n\n");
		//디버깅용 -------------
		gPassPbitFlag = FALSE;
	}
}

static void checkUartRegister()
{
	   sUartStatus = XUartPs_ReadReg(gUartConfig->BaseAddress,XUARTPS_ISR_OFFSET);

	   if (sUartStatus & XUARTPS_IXR_PARITY)
	   {
		   gPassPbitFlag = FALSE; // Uart 패리티 에러 Set
	   }
	   else
	   {
		   xil_printf(" Uart Parity Error Not Occured.\r\n\n");
	   }
	   delay_ms(500);
	   if (sUartStatus & XUARTPS_IXR_FRAMING)
	   {
		   gPassPbitFlag = FALSE; // Uart 프레이밍 에러 Set
	   }
	   else
	   {
		   xil_printf(" Uart Framing Error Not Occured.\r\n\n");
	   }
	   delay_ms(500);
	   if (sUartStatus & XUARTPS_IXR_OVER)
	   {
		   gPassPbitFlag = FALSE; // Uart 버퍼 오버런 에러 Set
	   }
	   else
	   {
	 	   xil_printf(" Uart Overrun Error Not Occured.\r\n\n");
	   }
	   delay_ms(500);

	   //디버깅용------------
	   if (!(sUartStatus & (XUARTPS_IXR_PARITY|XUARTPS_IXR_FRAMING|XUARTPS_IXR_OVER)))
	   {
		   xil_printf(" GSG-701N / [PBIT] : Uart Register Check Passed\r\n\n");
	   }
	   //디버깅용------------
	   XUartPs_WriteReg(gUartConfig->BaseAddress, XUARTPS_ISR_OFFSET, sUartStatus); // Error Reset
}

static void checkUart()
{
	checkUartLoopback();
	checkUartRegister();
}

static void checkNetwork()
{
	xil_printf(" Check for Seeker Connection\r\n\n");

	// Seeker는 자체적 PBIT을 마치고 지속적으로 GCU에  PBIT정보에 대한 데이터를 보냄
	// Seeker가 GCU에세 PBIT UDP 실패 정보를 보내거나,데이터가 일정 시간 이내에 값이 도착하지 않으면 (타임아웃) gPassPbitFlag를 False로 전환
	// Seeker가  GCU에 PBIT 성공 UDP 정보를 보내는 경우, GCU는 Seeker에 에코잉 값 19980398을 보냄
	// Seeker는 GCU에 19980398을 되돌려줌
	// GCU는 19980398을 보내고 받은 값이 같은지 확인하고, 같다면 PBIT 통과로 간주

	xil_printf(" Check for ACB Connection\r\n\n");

	// TBD

	xil_printf(" Check for IMU Connection\r\n\n");

	// IMU는 계속 GCU에 자기가 보내줘야 할 데이터를 보냄
	// GCU는 값이 일정시간 이내에 값이 도착하지 않는다면 (타임아웃) gPassPbitFlag를 False로 전환
	// GCU가 IMU 값을 받을 시, IMU값이 유효한 값 (각속도 범위 < ABS(1.6)) 이면 PBIT 통과로 간주
}

static void runPbit(void)
{
	xil_printf(" || GSG-701N PowerON Built In Test ||\r\n\n");
    delay_ms(1000);
    checkPower();
    delay_ms(1000);
    checkUart();
    delay_ms(1000);
    checkNetwork();
    delay_ms(1000);
}

void pbitTaskMain( void *pvParameters )
{
	xil_printf("RUN -- %s\r\n\n", pcTaskGetName(NULL));
	runPbit();
	xil_printf("DEL -- %s\r\n", pcTaskGetName(NULL));

	if(gPassPbitFlag == TRUE)
	{
		xil_printf("\n GSG-701N / [PBIT] : PBIT Success\r\n\n");
		xTaskNotifyGive(xStanbyIgnitionTaskHandle);
	}
	else if(gPassPbitFlag == FALSE)
	{
	    xil_printf("\n GSG-701N / [PBIT] : PBIT Failed\r\n\n");
	    xTaskNotifyGive(xPbitFailTaskHandle);
	}
	vTaskDelete(NULL);
}
