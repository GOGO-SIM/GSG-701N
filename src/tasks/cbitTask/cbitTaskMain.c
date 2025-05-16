// FreeRTOS ���� include

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

// �������� include

#include "global.h"

// Xilinx ���� include

#include "xsysmon.h"
#include "xil_printf.h"
#include "xil_io.h"
#include "xuartps.h"
#include "xparameters.h"

// Lwip ���� include

#include "lwip/tcpip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/api.h"
#include "netif/xadapter.h"
#include "lwip/netbuf.h"
#include "lwip/ip_addr.h"
#include "netif/xemacpsif.h"
#include "platform_config.h"

XSysMon sysMonInst;
XSysMon_Config *configPtr;

//-----------------------------( ���� üũ �뵵 )----------------------------------------

static u32 sEccStatus; // ���� �߻��� set Error Correcting Code
static u32 sAxiError;
static u32 sCacheError;

static u32 sUartStatus;
static u32 sTxStatusReg;
static u32 sRxStatusReg;
static u32 sPassCbitFlag = TRUE; // ��� == TRUE, ���� == FALSE

//----------------------------( ���� �� �µ� ������  )-------------------------------------

static u16 sRawVccInt;
static u16 sRawVccAux;
static u16 sRawVccRam;
static u16 sRawTemperture;

double gVoltageInt;
double gVoltageBram;
double gVoltageAux;
double gCelcius;

XUartPs_Config uartConfig; // (To do : ���߿� �����ּ���. -> Uart init�� �ܺο��� �Ϸ��. )

// -------------------( ���߿� Task ���� �� �ѹ� init ���־�� �Ѵ�.)-------------------------

void initXsysMon()
{
    configPtr = XSysMon_LookupConfig(XPAR_SYSMON_0_DEVICE_ID);
    XSysMon_CfgInitialize(&sysMonInst, configPtr, configPtr->BaseAddress);
    XSysMon_SetSequencerMode(&sysMonInst, XSM_SEQ_MODE_CONTINPASS);
}

//void debug()
//{
//	        int int_vccint = (int)gVoltageInt;
//	        int frac_vccint = (int)((gVoltageInt - int_vccint) * 1000);
//	        int int_vccaux = (int)gVoltageBram;
//	        int frac_vccaux = (int)((gVoltageAux - int_vccaux) * 1000);
//	        int int_vccbram = (int)gVoltageAux;
//	        int frac_vccbram = (int)((gVoltageBram - int_vccbram) * 1000);
//	        int int_temp = (int)gCelcius;
//	        int frac_temp = (int)((gCelcius - int_temp) * 1000);
//
//	        xil_printf("VCCINT: %d.%03d V, VCCAUX: %d.%03d V, VCCBRAM: %d.%03d V, TEMP: %d.%03d'C ErrCNT: %d\r\n",
//	                           int_vccint, frac_vccint,
//	                           int_vccaux, frac_vccaux,
//	                           int_vccbram, frac_vccbram,
//	                           int_temp, frac_temp,sErrorCount);
//}

// -------------------------------------------------------------------------------

static void checkPower()
{
	// �� ���� ������ Raw ������ �޾ƿ���

	sRawVccInt = XSysMon_GetAdcData(&sysMonInst, XSM_CH_VCCINT);
	sRawVccAux = XSysMon_GetAdcData(&sysMonInst, XSM_CH_VCCAUX);
	sRawVccRam = XSysMon_GetAdcData(&sysMonInst, XSM_CH_VBRAM);
	sRawTemperture = XSysMon_GetAdcData(&sysMonInst, XSM_CH_TEMP);

	// Raw �����͸� ���� ����,�µ��� ��ȯ

	gVoltageInt = XSysMon_RawToVoltage(sRawVccInt);
	gVoltageBram = XSysMon_RawToVoltage(sRawVccAux);
	gVoltageAux = XSysMon_RawToVoltage(sRawVccRam);
	gCelcius = XSysMon_RawToVoltage(sRawTemperture);

	// debug();

	// ������ ���� Ȥ�� �µ� ������ sPassCbitFlag False�� ��ȯ

	if ( gVoltageInt < 0.9 || gVoltageInt > 1.05 )
	{
		sPassCbitFlag = FALSE;
	}
	if ( gVoltageBram < 0.9 || gVoltageBram > 1.05 )
	{
		sPassCbitFlag = FALSE;
	}
	if ( gVoltageAux < 1.7 || gVoltageAux > 1.9 )
	{
		sPassCbitFlag = FALSE;
	}
	if ( gCelcius > 95 )
	{
		sPassCbitFlag = FALSE;
	}
}

static void checkUart()
{
	sUartStatus = XUartPs_ReadReg(uartConfig->BaseAddress,XUARTPS_ISR_OFFSET);

	if (sUartStatus & XUARTPS_IXR_PARITY)
	{
		sPassCbitFlag = FALSE; // Uart �и�Ƽ ���� Set
	}
	if (sUartStatus & XUARTPS_IXR_FRAMING)
	{
		sPassCbitFlag = FALSE; // Uart �����̹� ���� Set
	}
	if (sUartStatus & XUARTPS_IXR_OVER)
	{
		sPassCbitFlag = FALSE; // Uart ���� ������ ���� Set
	}
	XUartPs_WriteReg(uartConfig->BaseAddress, XUARTPS_ISR_OFFSET, sUartStatus); // Error Reset
}

static void checkEthernet()
{
	sTxStatusReg = XEmacPs_ReadReg(EMAC_BASEADDR, XEMACPS_TXSR_OFFSET);
	sRxStatusReg = XEmacPs_ReadReg(EMAC_BASEADDR, XEMACPS_RXSR_OFFSET);

    if (sTxStatusReg != 0) // 0�� �ƴ϶�� Error True False�� �����°� �ƴ�
    {
    	sPassCbitFlag = FALSE;
        XEmacPs_WriteReg(EMAC_BASEADDR, XEMACPS_TXSR_OFFSET, sTxStatusReg);
    }
    if ((sRxStatusReg & ~0x01) != 0) // ù ��Ʈ�� �����ϰ� 0���� �˻�
    {
    	sPassCbitFlag = FALSE;
        XEmacPs_WriteReg(EMAC_BASEADDR, XEMACPS_RXSR_OFFSET, sRxStatusReg);
    }
}

static void checkMemory()
{
    sEccStatus = Xil_In32(0xF800606C);
    sAxiError = Xil_In32(0xF8000F40);
    sCacheError = Xil_In32(0xF8F02100);

    if (sEccStatus == TRUE || sAxiError == TRUE || sCacheError == TRUE )
    {
    	sPassCbitFlag = FALSE;
    }

    Xil_Out32(0xF800606C, sEccStatus);
    Xil_Out32(0xF8000F40, sAxiError);
    Xil_Out32(0xF8F02100, sCacheError);
}

static void checkRegister()
{
	checkUart();
	checkEthernet();
	checkMemory();
}

void runCbit()
{
	static int sErrorCount;
	while(1)
	{
		if(sErrorCount > 5)
		{
			explode(); // 5�� ���� ������ ����
		}
		checkPower();
		if ( sPassCbitFlag == TRUE )
		{
			checkRegister();
		}
		if ( sPassCbitFlag == FALSE )
		{
			sErrorCount += 1;
		}
		else if ( sPassCbitFlag == TRUE )
		{
			sErrorCount = 0;
		}
	}
}

void cbitTaskMain(void *param)
{
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        runCbit();
    }
}
