// File: src/tasks/initTask/initTaskMain.c

#include "global.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

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
#include "xparameters.h"
#include "xemacps.h"

#include "xil_printf.h"
#include "xuartps.h"
#include "xsysmon.h"
#include "xil_io.h"

#define SERVER_IP_ADDR       "192.168.1.4"
#define SERVER_PORT          1234

#define INIT_FAIL            4

// OCM parity‐check addresses
#define OCM_PARITY_CTRL_ADDR 0xF800C000U
#define OCM_IRQ_STS_ADDR     0xF800C008U

#define OCM_PARITY_EN            (0 << 0)
#define OCM_PARITY_EN_IRQ_SINGLE (1 << 2)
#define OCM_PARITY_EN_IRQ_MULTI  (1 << 3)

// MDIO PHY polling parameters
#define PHY_MDIO_ADDR         0    // 보드상의 PHY MDIO 주소 (대개 0)
#define PHY_BSR_REG           1    // Basic Status Register (MDIO 1)
#define PHY_LINK_STATUS_BIT   (1U << 2)

struct netif      gGsgNetif;
struct netconn  *gpUdpClientConn;
struct netconn  *gpUdpServerConn;
SemaphoreHandle_t xLinkReadySem = NULL;

// Forward declarations
static void tcpipInitDone(void *arg);
void testUdp(void *pvParameters);

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

// XADC(System Monitor) 초기화
void initXsysMon(void)
{
    XSysMon_Config *xadcCfg = XSysMon_LookupConfig(XPAR_SYSMON_0_DEVICE_ID);
    XSysMon_CfgInitialize(&gSysMonInst, xadcCfg, xadcCfg->BaseAddress);
    XSysMon_SetSequencerMode(&gSysMonInst, XSM_SEQ_MODE_CONTINPASS);
}

// OCM 패리티 체크 초기화
void initMemoryCheck(void)
{
    u32 ctrl = OCM_PARITY_EN
            | OCM_PARITY_EN_IRQ_SINGLE
            | OCM_PARITY_EN_IRQ_MULTI;
    Xil_Out32(OCM_PARITY_CTRL_ADDR, ctrl);
    Xil_Out32(OCM_IRQ_STS_ADDR, 0x7);  // clear all interrupts
}

// lwIP + EMAC bring-up 완료 후 호출되는 콜백
static void tcpipInitDone(void *arg)
{
    ip4_addr_t ipaddr, netmask, gw, serverIp;
    u16        phyData;
    LONG       mdioStatus;
    err_t      err;
    unsigned char macAddr[6] = {0x00,0x18,0x3E,0x04,0x50,0x84};

    // 1) EMAC-PS 드라이버 초기화
    XEmacPs_Config *emacCfg = XEmacPs_LookupConfig(XPAR_PS7_ETHERNET_0_DEVICE_ID);
    XEmacPs_CfgInitialize(&gXemacPsInst, emacCfg, emacCfg->BaseAddress);

    // 2) lwIP stack 초기화
    lwip_init();

    IP4_ADDR(&ipaddr,   192,168,1,10);
    IP4_ADDR(&netmask, 255,255,255,0);
    IP4_ADDR(&gw,      192,168,1, 1);
    ipaddr_aton(SERVER_IP_ADDR, &serverIp);

    // 3) netif 등록 및 up
    if (!xemac_add(&gGsgNetif, &ipaddr, &netmask, &gw,
                   macAddr, PLATFORM_EMAC_BASEADDR)) {
        xil_printf("Error adding netif\r\n");
        return;
    }
    netif_set_default(&gGsgNetif);
    netif_set_up(&gGsgNetif);
    xil_printf("EMAC netif up, polling PHY...\r\n");

    // 4) PHY Basic Status Register(1) 읽어서 링크 업 대기
    do {
        mdioStatus = XEmacPs_PhyRead(
            &gXemacPsInst,
            PHY_MDIO_ADDR,
            PHY_BSR_REG,
            &phyData
        );
        if (mdioStatus != XST_SUCCESS) {
            xil_printf("PHY MDIO Read error %ld\r\n", mdioStatus);
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }
    } while (!(phyData & PHY_LINK_STATUS_BIT));
    xil_printf(">>> PHY link is UP!\r\n");

    // 6) EMAC 입력 스레드 시작
    sys_thread_new("xemacif_input",
                   xemacif_input_thread,
                   &gGsgNetif, 1024, configMAX_PRIORITIES-1);

    // 7) UDP client/server conn 생성 및 설정
    gpUdpClientConn = netconn_new(NETCONN_UDP);
    if (!gpUdpClientConn) {
        xil_printf("netconn_new(client) failed\r\n");
        return;
    }
    err = netconn_connect(gpUdpClientConn, &serverIp, SERVER_PORT);
    xil_printf("Client connect: %d\r\n", err);

    gpUdpServerConn = netconn_new(NETCONN_UDP);
    if (!gpUdpServerConn) {
        xil_printf("netconn_new(server) failed\r\n");
        return;
    }
    err = netconn_bind(gpUdpServerConn, IP_ADDR_ANY, 5001);
    xil_printf("Server bind: %d\r\n", err);
    netconn_set_nonblocking(gpUdpServerConn, TRUE);
    xSemaphoreGive(xLinkReadySem);
}

// tcpip_init() 래핑
void initUdpServer(void)
{
    xil_printf("Starting lwIP...\r\n");
    tcpip_init(tcpipInitDone, NULL);
    xSemaphoreTake(xLinkReadySem, portMAX_DELAY);
}

// 초기화 태스크 진입점
void initTaskMain(void *pvParameters)
{
    xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));

    xLinkReadySem = xSemaphoreCreateBinary();
    if (!xLinkReadySem) {
        xil_printf("Failed to create semaphore\r\n");
        vTaskDelete(NULL);
    }

    initUartPs();
    xil_printf("UART initialized\r\n");

    initXsysMon();
    xil_printf("SysMon initialized\r\n");

    initMemoryCheck();
    xil_printf("OCM parity check initialized\r\n");

    initUdpServer();
    xil_printf("Network is ready\r\n");
    // 3) 링크 업 완료될 때까지 대기

    xTaskNotifyGive(xPbitTaskHandle);
    xil_printf("DEL -- %s\r\n", pcTaskGetName(NULL));
    vTaskDelete(NULL);
}
