#include "global.h"
#include "gsgTypes.h"
#include "gsmpWrapper.h"
#include "lwip/err.h"
#include "lwip/api.h"

#define ECHOING_VALUE 19980398
static u32 sEthernetStatus;
static u32 sButtonLevel;

// Ethernet 체크시 PHY 주소

#define PHY_ADDR   0x0
#define PHY_BASIC_STATUS    0x1

// ADC 변환 Raw 값

static u16 sRawVccInt;
static u16 sRawVccAux;
static u16 sRawVccRam;
static u16 sRawTemperture;
static u16 sTimeOut;
static u32 sUartStatus;
static int sEchoMsgLen = sizeof(tGsmpMessageHeader) +  sizeof(tEchoPayload) + 2;
static int sEchoMsgLength = sizeof(tEchoPayload);

struct netbuf *SeekerEchoSendBuf;

const static int PBIT_TASK_PRIO = 27;
const static int UDP_RECEIVE_TASK_PRIO = 15;
const static int UART_RECEIVE_TASK_PRIO = 11;
const static int UDP_RECEIVE_DEADLINE = 100;
const static int UART_SEND_DEADLINE = 100;
const static int UART_RECV_DEADLINE = 100;

static tEchoPayload sSeekerEchoPayload = ECHOING_VALUE;
static tEchoPayload sAcbEchoPayload = ECHOING_VALUE;

static void delay_ms(u32 ms)
{
    TickType_t start = xTaskGetTickCount();
    while ( (xTaskGetTickCount() - start) < pdMS_TO_TICKS(ms) );
}

static void waitInputHigh()
{
	while(!sButtonLevel)
	{
		sButtonLevel= XGpioPs_ReadPin(&gGpioPs, DESTRUCT_STATUS_POS);
	}
}

static void waitInputLow()
{
	static int sInputCount = 0;
	while(1)
	{
		sButtonLevel = XGpioPs_ReadPin(&gGpioPs, DESTRUCT_STATUS_POS);

		if (sButtonLevel != 1)
		{
			sInputCount += 1;
		}
		if (sInputCount > 5)
		{
			break;
		}
	}
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

   if ( gVoltageBram < 0.97 || gVoltageBram > 1.03 )
   {
      gPassPbitFlag = FALSE;
   }
   printf("| 0.97V < %.3lfV < 1.03V | PassFlag: %u |\n",gVoltageBram,gPassPbitFlag);

   if ( gVoltageAux < 1.7 || gVoltageAux > 1.88 )
   {
	  gPassPbitFlag = FALSE;
   }
   printf("| 1.70V < %.3lfV < 1.88V | PassFlag: %u |\n",gVoltageAux,gPassPbitFlag);

   if ( gCelcius >  90 )
   {
      gPassPbitFlag = FALSE;
   }
   printf("| %.3lf'C < 90'C | PassFlag: %u |\n",gCelcius,gPassPbitFlag);

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

	delay_ms(50);

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
    if (sUartStatus & XUARTPS_IXR_FRAMING)
    {
	   gPassPbitFlag = FALSE; // Uart 프레이밍 에러 Set
    }
    else
    {
	   xil_printf(" Uart Framing Error Not Occured.\r\n\n");
    }
    if (sUartStatus & XUARTPS_IXR_OVER)
    {
	   gPassPbitFlag = FALSE; // Uart 버퍼 오버런 에러 Set
    }
    else
    {
	   xil_printf(" Uart Overrun Error Not Occured.\r\n\n");
    }
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

static void checkEthernet()
{
   XEmacPs_PhyRead(&gXemacPsInst, PHY_ADDR,PHY_BASIC_STATUS,&sEthernetStatus);

   if (((sEthernetStatus & 0x0004) == 0)) // 이더넷 연결 FALSE
   {
	  gSeekerEchoRecvData = FALSE;
      gPassPbitFlag = FALSE;
   }
}

static void checkNetwork()
{
	// UDP Receive

	checkEthernet();

	vTaskPrioritySet(xUdpReceiveTaskHandle, PBIT_TASK_PRIO);
	xTaskNotifyGive(xUdpReceiveTaskHandle);
	vTaskDelay(pdMS_TO_TICKS(UDP_RECEIVE_DEADLINE));
	vTaskPrioritySet(xUdpReceiveTaskHandle, UDP_RECEIVE_TASK_PRIO);

	xil_printf("\n");

	// Seeker Check
	// Seeker는 자체적 PBIT을 마치고 지속적으로 GCU에  PBIT정보에 대한 데이터를 보냄
	// Seeker가 GCU에게 PBIT 실패 정보를 보내면, gPassPbitFlag를 False로 전환

	xil_printf(" GSG-701N / [PBIT] : Check for Seeker Connection\r\n\n");

	if (gSeekerInfo == PBIT_FAIL)
	{
		gPassPbitFlag = FALSE;
	}

	// Seeker가  GCU에 PBIT 성공 UDP 정보를 보내는 경우, GCU는 Seeker에 에코잉 값 19980398을 보냄

	gsmpWrapper(SEEKER_ECHO_SEND_MSG_ID,OK,&sSeekerEchoPayload);
	SeekerEchoSendBuf = netbuf_new();
	netbuf_ref(SeekerEchoSendBuf, gSendBuffer,sEchoMsgLen);
	netconn_send(gpUdpClientConn, SeekerEchoSendBuf);
	xil_printf(" UDP Seeker Echo Transmitted : %d\r\n\n",sSeekerEchoPayload);

	delay_ms(100);

	// Seeker는 GCU에 19980398을 되돌려줌

	vTaskPrioritySet(xUdpReceiveTaskHandle, PBIT_TASK_PRIO);
	xTaskNotifyGive(xUdpReceiveTaskHandle);
	vTaskDelay(pdMS_TO_TICKS(UDP_RECEIVE_DEADLINE));
	vTaskPrioritySet(xUdpReceiveTaskHandle, UDP_RECEIVE_TASK_PRIO);

	xil_printf("\n");

	delay_ms(100);

	xil_printf(" UDP Seeker Echo Recieved : %d\r\n\n",gSeekerEchoRecvData);

	if(gSeekerEchoRecvData != ECHOING_VALUE)
	{
		gPassPbitFlag = FALSE;
	}

	// GCU는 19980398을 보내고 받은 값이 같은지 확인하고, 같다면 PBIT 통과로 간주

	xil_printf(" Check for ACB Connection\r\n");

	gsmpWrapper(ACB_ECHO_SEND_MSG_ID,OK,&sAcbEchoPayload);
	XUartPs_Send(&gUartPs, gSendBuffer, sEchoMsgLen);

	xil_printf("\nUart ACB Echo Transmitted : %d\r\n\n",sAcbEchoPayload);

	// UART Send

	delay_ms(100);

	vTaskPrioritySet(xUartReceiveTaskHandle, PBIT_TASK_PRIO);
	xTaskNotifyGive(xUartReceiveTaskHandle);
	vTaskDelay(pdMS_TO_TICKS(UART_RECV_DEADLINE));
	vTaskPrioritySet(xUartReceiveTaskHandle, UART_RECEIVE_TASK_PRIO);

	xil_printf("\n");

	delay_ms(100);

	xil_printf(" Uart ACB Echo Recieved : %d\r\n\n",gAcbEchoRecvData);

	if(gAcbEchoRecvData != ECHOING_VALUE)
	{
		gPassPbitFlag = FALSE;
	}
}

static void runPbit(void)
{
	xil_printf(" || GSG-701N PowerON Built In Test ||\r\n\n");
    delay_ms(50);
    checkPower();
    delay_ms(50);
    checkUart();
    delay_ms(50);
    checkNetwork();
    delay_ms(50);
}

void pbitTaskMain( void *pvParameters )
{
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	xil_printf("RUN -- %s\r\n\n", pcTaskGetName(NULL));
	runPbit();
	xil_printf("DEL -- %s\r\n", pcTaskGetName(NULL));

	if(gPassPbitFlag == TRUE)
	{
		xil_printf("\n GSG-701N / [PBIT] : PBIT Success\r\n\n");
		XGpioPs_WritePin(&gGpioPs, OPERATION_STATUS_POS, GPIO_ON);

		waitInputHigh();
		waitInputLow();

		XGpioPs_WritePin(&gGpioPs, PREPARE_STATUS_POS, GPIO_OFF);
		xTaskNotifyGive(xStanbyIgnitionTaskHandle);
	}

	else if(gPassPbitFlag == FALSE)
	{
	    xil_printf("\n GSG-701N / [PBIT] : PBIT Failed\r\n\n");
	    XGpioPs_WritePin(&gGpioPs, PREPARE_STATUS_POS, GPIO_OFF);
	    XGpioPs_WritePin(&gGpioPs, EXPLODE_STATUS_POS, GPIO_ON);

	    xTaskNotifyGive(xPbitFailTaskHandle);
	}
	xTaskNotifyGive(xSchedulingTaskHandle);
	vTaskDelete(NULL);
}
