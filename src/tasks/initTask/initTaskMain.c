
#include "FreeRTOS.h"
#include "lwip/tcpip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/api.h"
#include "netif/xadapter.h"
#include "lwip/netbuf.h"
#include "lwip/ip_addr.h"
#include "netif/xemacpsif.h"
#include "xparameters.h"
#include "taskMain.h"
#include "platform_config.h"

#define INIT_FAIL 4
#define LWIP_NETCONN 1

static struct netbuf *recvBuf;
static ip_addr_t *clientAddr;
static u16_t clientPort;
struct netif myNetif;
struct netconn *udpConn;
// 콜백 함수
static void tcpip_init_done(void *arg) {
    ip4_addr_t ipaddr, netmask, gw;
    unsigned char macAddr[] = { 0x00, 0x18, 0x3E, 0x04, 0x50, 0x84 };
    err_t err;

    lwip_init();

    IP4_ADDR(&ipaddr, 192, 168, 1, 10);
    IP4_ADDR(&netmask, 255, 255, 255, 0);
    IP4_ADDR(&gw, 192, 168, 1, 1);

//    netif_add(&myNetif, &ipaddr, &netmask, &gw, (void *)XPAR_XEMACPS_0_BASEADDR, xemacpsif_init, tcpip_input);
//    xil_printf("netif_add done\r\n");
    if (!xemac_add(&myNetif, &ipaddr, &netmask, &gw, macAddr,
    		PLATFORM_EMAC_BASEADDR)) {
    		xil_printf("Error adding N/W interface\r\n");
    		return;
	}
	xil_printf("xemac_add done \r\n");

    netif_set_default(&myNetif);
    xil_printf("set_default done\r\n");

    netif_set_up(&myNetif);
    xil_printf("set_up done\r\n");


    sys_thread_new("xemacif_input_thread",
                   xemacif_input_thread,
                   &myNetif,
                   1024,
                   4);

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
    xil_printf("UDP Server initialized on port 5001\r\n");

}

void initUdpServer() {
    xil_printf("-----lwIP netconn UDP Test Application------\r\n");

    tcpip_init(tcpip_init_done, NULL);
}

TaskHandle_t xtest;

// UDP 에코서버 예제
void testUdp( void *pvParameters )
{
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
	xil_printf("-----init Task Main------\r\n");

	initUdpServer();

	xil_printf("-----test main------\r\n");
	xTaskCreate((TaskFunction_t)testUdp,
						"test",
						2048,
						NULL,
						1,
						&xtest);


	vTaskDelete(NULL);
}
