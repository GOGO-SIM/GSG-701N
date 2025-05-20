#include "global.h"
#include "gsgTypes.h"

// Ethernet 체크시 PHY 주소

#define PHY_ADDR   0x0
#define PHY_BASIC_STATUS    0x1

// Ethernet 체크 위해 존재하는 명령

#define MDIO_CMD_START (1 << 31)
#define MDIO_CMD_READ (1 << 30)
#define MDIO_PHY_0 ((PHY_ADDR & 0x1F) << 23)
#define MDIO_PHY_BASIC_STATUS ((PHY_BASIC_STATUS & 0x1F) << 18)

// OCM 메모리 체크 주소

#define OCM_PARITY_CTRL_ADDR   0xF800C000U //OCM 컨트롤 용 주소
#define OCM_IRQ_STS_ADDR       0xF800C008U //OCM 인터럽트 상태 주소

// OCM 메모리 체크 발생 여부 판단

#define OCM_SINGLE_ERR   (1 << 0) // OCM 인터럽트 상태 0번자리가 1이면 Single Bit Error
#define OCM_MULTI_ERR    (1 << 1) // OCM 인터럽트 상태 1번자리가 1이면 Multiple Bit Error

// ADC 변환 Raw 값

static u16 sRawVccInt;
static u16 sRawVccAux;
static u16 sRawVccRam;
static u16 sRawTemperture;

// 레지스터를 통한 상태점검 변수

static u32 sOcmStatus; //ocm = on chip memory
static u32 sUartStatus;
static u32 sEthernetStatus;
static u32 sEthStatReadCmd;

void debug(int err)
{
    printf("VCCINT: %.3lf V | VCCAUX: %.3lf V | VCCBRAM: %.3lf V | TEMP: %.3lf'C | ErrCNT: %d\r\n",
            gVoltageInt,gVoltageAux,gVoltageBram ,gCelcius,err);
}

static void checkPower()
{
   // 각 측정 데이터 Raw 값으로 받아오기

   sRawVccInt = XSysMon_GetAdcData(&sysMonInst, XSM_CH_VCCINT);
   sRawVccAux = XSysMon_GetAdcData(&sysMonInst, XSM_CH_VCCAUX);
   sRawVccRam = XSysMon_GetAdcData(&sysMonInst, XSM_CH_VBRAM);
   sRawTemperture = XSysMon_GetAdcData(&sysMonInst, XSM_CH_TEMP);

   // Raw 데이터를 실제 전압,온도로 변환

   gVoltageInt = XSysMon_RawToVoltage(sRawVccInt);
   gVoltageBram = XSysMon_RawToVoltage(sRawVccRam);
   gVoltageAux = XSysMon_RawToVoltage(sRawVccAux);
   gCelcius = XSysMon_RawToTemperature(sRawTemperture);

   // 비정상 전압 혹은 온도 감지시 gPassCbitFlag False로 전환

   if ( gVoltageInt < 0.95 || gVoltageInt > 1.05 )
   {
      gPassCbitFlag = FALSE;
   }
   if ( gVoltageBram < 0.95 || gVoltageBram > 1.05 )
   {
      gPassCbitFlag = FALSE;
   }
   if ( gVoltageAux < 1.7 || gVoltageAux > 1.9 )
   {
      gPassCbitFlag = FALSE;
   }
   if ( gCelcius > 95 )
   {
      gPassCbitFlag = FALSE;
   }
}

static void checkUart()
{
   sUartStatus = XUartPs_ReadReg(gUartConfig->BaseAddress,XUARTPS_ISR_OFFSET);

   if (sUartStatus & XUARTPS_IXR_PARITY)
   {
	   gPassCbitFlag = FALSE; // Uart 패리티 에러 Set
   }
   if (sUartStatus & XUARTPS_IXR_FRAMING)
   {
	   gPassCbitFlag = FALSE; // Uart 프레이밍 에러 Set
   }
   if (sUartStatus & XUARTPS_IXR_OVER)
   {
	   gPassCbitFlag = FALSE; // Uart 버퍼 오버런 에러 Set
   }

   XUartPs_WriteReg(gUartConfig->BaseAddress, XUARTPS_ISR_OFFSET, sUartStatus); // Error Reset
}

static void checkMemory()
{
	sOcmStatus = Xil_In32(OCM_IRQ_STS_ADDR); // OCM 인터럽트 정보를 받아온다.

    if (sOcmStatus & (OCM_SINGLE_ERR | OCM_MULTI_ERR))
    {
    	printf("Memory Failed  | ");
    	gPassCbitFlag = FALSE;
    	Xil_Out32(OCM_IRQ_STS_ADDR,sOcmStatus & (OCM_SINGLE_ERR | OCM_MULTI_ERR) );
    }
    else
    {
    	printf("Memory Passed | "); //디버깅용 나중에 else는 삭제
    }
}

static void checkEthernet()
{
	    sEthStatReadCmd = MDIO_CMD_START |  // MDIO 명령 시작
	    		  	  	  MDIO_CMD_READ | // MDIO 읽어오기 명령
						  MDIO_PHY_0 | // 읽어올 위치
						  MDIO_PHY_BASIC_STATUS; // 읽어올 정보

	    Xil_Out32(XPAR_PS7_ETHERNET_0_BASEADDR + XEMACPS_PHYMNTNC_OFFSET, sEthStatReadCmd);

	    while ( (Xil_In32(XPAR_PS7_ETHERNET_0_BASEADDR + XEMACPS_NWSR_OFFSET ) & XEMACPS_NWSR_MDIOIDLE_MASK) == 0);

	    sEthernetStatus = Xil_In32(XPAR_PS7_ETHERNET_0_BASEADDR + XEMACPS_PHYMNTNC_OFFSET) & 0xFFFF; //하위 16비트 읽기

	    if ( (sEthernetStatus & 0x4) == FALSE ) // 이더넷 연결 FALSE
	     {
	    	 printf("Ethernet Failed | ");
	         gPassCbitFlag = FALSE;
	     }
	    else
	    {
	    	 printf("Ethernet Passed | "); // 디버깅용
	    }
}

static void checkRegister()
{
   checkUart();
   checkEthernet();
   checkMemory();
}

static void runCbit(void)
{
   static int sErrorCount = 0;
   while(1)
   {
      if(sErrorCount >= 5)
      {
    	  xil_printf("explode();\n");
         //explode(); 5번 연속 에러시 자폭
      }
      checkPower();
      if ( gPassCbitFlag == TRUE )
      {
         checkRegister();
      }
      if ( gPassCbitFlag == FALSE )
      {
         sErrorCount += 1;
      }
      else if ( gPassCbitFlag == TRUE )
      {
         sErrorCount = 0;
      }
      gPassCbitFlag = TRUE;
      //디버깅용------------
      debug(sErrorCount);
      //디버깅용------------
      vTaskDelay(20);
   }
}

void cbitTaskMain( void *pvParameters )
{
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));
		runCbit();
    }
}
