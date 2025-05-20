#include "global.h"

#include "FreeRTOS.h"
#include "lwip/tcpip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/api.h"
#include "netif/xadapter.h"
#include "lwip/netbuf.h"
#include "lwip/ip_addr.h"
#include "netif/xemacpsif.h"
#include "platform_config.h"
#include "xemacps_hw.h"

#define INIT_FAIL 4
#define LWIP_NETCONN 1

// OCM 메모리 체크 주소

#define OCM_PARITY_CTRL_ADDR   0xF800C000U //OCM 컨트롤 용 주소
#define OCM_IRQ_STS_ADDR       0xF800C008U //OCM 인터럽트 상태 주소

// OCM 컨트롤 주소에  write해 설정해 줄 옵션들

#define OCM_PARITY_EN   (0 << 0) // 패리티 체크 Enable -> Parity Check Disable을 0으로 만들어 해제하기
#define OCM_PARITY_EN_IRQ_SINGLE  (1 << 2) // 싱글 비트 에러 인터럽트 Enable
#define OCM_PARITY_EN_IRQ_MULTI   (1 << 3) // 멀티 비트 에러 인터럽트 Enable

struct netbuf *recvBuf;
struct netif gsgNetif;
struct netconn *udpConn;

int initUartPs()
{
    // UART 설정 정보 구조체 포인터

    int Status;

    // UART 디바이스 ID에 해당하는 설정 정보를 검색해서 Config에 저장
    // BaseAddress, BaudRate 등 하드웨어 정보 포함
    gUartConfig = XUartPs_LookupConfig(XPAR_PS7_UART_1_DEVICE_ID);
    if (gUartConfig == NULL)
    {
    	xil_printf("Config is NULL\n");
    	return XST_FAILURE;
    }


    Status = XUartPs_CfgInitialize(&gUartPs, gUartConfig, gUartConfig->BaseAddress);
    if (Status != XST_SUCCESS)
    {
    	xil_printf("State is FAIL\n");
    	return XST_FAILURE;
   	}

    // 인스턴스 초기화 시, default 값이19200bps이기 때문에 필요 시 명시
    XUartPs_SetBaudRate(&gUartPs, UART_BAUD);
    return XST_SUCCESS;
}



void initXsysMon()
{
	gXadcConfig = XSysMon_LookupConfig(XPAR_SYSMON_0_DEVICE_ID);
    XSysMon_CfgInitialize(&gSysMonInst, gXadcConfig, gXadcConfig->BaseAddress);
    XSysMon_SetSequencerMode(&gSysMonInst, XSM_SEQ_MODE_CONTINPASS);
}

void initMemoryCheck(){

	u32 OcmControl = 0x00;
	OcmControl = OCM_PARITY_EN | OCM_PARITY_EN_IRQ_SINGLE | OCM_PARITY_EN_IRQ_MULTI;
	Xil_Out32(OCM_PARITY_CTRL_ADDR,OcmControl);
	Xil_Out32(OCM_IRQ_STS_ADDR,0x7); // 모든 인터럽트 비트에 1을 써줌으로써 초기화 0x7 = 111이다.
}

// call back
static void tcpip_init_done(void *arg) {
    ip4_addr_t ipaddr, netmask, gw;
    unsigned char macAddr[] = { 0x00, 0x18, 0x3E, 0x04, 0x50, 0x84 };
    err_t err;

    lwip_init();

    IP4_ADDR(&ipaddr, 192, 168, 1, 10);
    IP4_ADDR(&netmask, 255, 255, 255, 0);
    IP4_ADDR(&gw, 192, 168, 1, 1);

    // TODO: tcp_init()의 우선 순위가 낮아서, init이 스케줄링 중간에 완료되는 현상이 생긴다.
    if (!xemac_add(&gsgNetif, &ipaddr, &netmask, &gw, macAddr,
    		PLATFORM_EMAC_BASEADDR))
    {
		xil_printf("Error adding N/W interface\r\n");
		return;
	}
	xil_printf("xemac_add done \r\n");

    netif_set_default(&gsgNetif);
    xil_printf("set_default done\r\n");

    netif_set_up(&gsgNetif);
    xil_printf("set_up done\r\n");


    sys_thread_new("xemacif_input_thread",
                   xemacif_input_thread,
                   &gsgNetif,
                   1024,
                   29);

    // netconn 생성
    udpConn = netconn_new(NETCONN_UDP);
    if (udpConn == NULL) {
        xil_printf("netconn_new failed!\r\n");
        gGcuStatus = INIT_FAIL;
        vTaskDelete(NULL);
    }

    xil_printf("netconn_new DONE\r\n");

    err = netconn_bind(udpConn, IP_ADDR_ANY, 5001);
    if (err != ERR_OK) {
        xil_printf("netconn_bind failed: %d\r\n", err);
        netconn_delete(udpConn);
        udpConn = NULL;
        gGcuStatus = INIT_FAIL;
        vTaskDelete(NULL);
    }

    //netconn_set_nonblocking(udpConn, TRUE);

}

void initUdpServer() {
    xil_printf("-----lwIP netconn UDP Test Application------\r\n");

    tcpip_init(tcpip_init_done, NULL);
}

TaskHandle_t xtest;

// Test: UDP echo server
void testUdp( void *pvParameters )
{
	static ip_addr_t *clientAddr;
	static u16_t clientPort;
    err_t err;

	while (1) {
		xil_printf("server is listening\r\n");
		err = netconn_recv(udpConn, &recvBuf);
		if (err == ERR_OK && recvBuf != NULL) {
			clientAddr = netbuf_fromaddr(recvBuf);
			clientPort = netbuf_fromport(recvBuf);

			void *data;
			u16_t len;
			netbuf_data(recvBuf, &data, &len);

			xil_printf("Received %d bytes from %s:%d\r\n",
					   netbuf_len(recvBuf),
					   ipaddr_ntoa(clientAddr),
					   clientPort);
			for (int i = 0; i < len; ++i) {
				xil_printf("%c", ((char*)data)[i]);
			}
			netconn_sendto(udpConn, recvBuf, clientAddr, clientPort);
		}

		if (recvBuf != NULL) {
			netbuf_delete(recvBuf);
			recvBuf = NULL;
		}
	}
}

void initTaskMain( void *pvParameters )
{
	/**
	 * 1. UDP server open
	 * 2. delete task
	 */
	xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));

	initUdpServer();
    xil_printf("UDP Server initialized on port 5001\r\n");
	initUartPs();
    xil_printf("UART successfully initialized\r\n");
    initXsysMon();
    xil_printf("system monitoring successfully initialized\r\n");

//	xil_printf("-----test main------\r\n");
//	xTaskCreate((TaskFunction_t)testUdp,
//						"test",
//						2048,
//						NULL,
//						1,
//						&xtest);

	xil_printf("DEL -- %s\r\n", pcTaskGetName(NULL));
	vTaskDelete(NULL);
}
