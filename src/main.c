#include <stdio.h>
#include "xparameters.h"
#include "netif/xadapter.h"
#include "platform_config.h"
#include "xil_printf.h"
#include "unistd.h"

/* Tasks */
#define PRIO_SCHED    7
#define PRIO_CTRL     6
#define PRIO_GUID     5
#define PRIO_NAV      4
#define PRIO_UDP      3
#define PRIO_TEL	  2

TaskHandle_t hMain = NULL;
TaskHandle_t hSched = NULL;
TaskHandle_t hUDP   = NULL;
TaskHandle_t hNav   = NULL;
TaskHandle_t hGuid  = NULL;
TaskHandle_t hCtrl  = NULL;
TaskHandle_t hTel = NULL;

void TaskMain(void *pvParameters);
void TaskScheduler(void *pvParameters);
void TaskUDP(void *pvParameters);
void TaskNav(void *pvParameters);
void TaskGuid(void *pvParameters);
void TaskCtrl(void *pvParameters);
void TaskTel(void *pvParameters);


void lwip_init();
void USER_THREADS( void );
#define THREAD_STACKSIZE 1024

static struct netif server_netif;
struct netif *echo_netif;

int main()
{
	USER_THREADS();

	sleep(3);

	xTaskCreate(TaskMain, "MAIN", 1024, NULL, 2, &hMain);
	vTaskStartScheduler();
	while(1);
	return 0;

}

void USER_THREADS(void)
{
    xTaskCreate(TaskScheduler, "SCHED", 256, NULL, PRIO_SCHED, &hSched);
    xTaskCreate(TaskUDP,       "UDP",   1024, NULL, PRIO_UDP,   &hUDP);
    xTaskCreate(TaskNav,       "NAV",   1024, NULL, PRIO_NAV,   &hNav);
    xTaskCreate(TaskGuid,      "GUID",  1024, NULL, PRIO_GUID,  &hGuid);
    xTaskCreate(TaskCtrl,      "CTRL",  1024, NULL, PRIO_CTRL,  &hCtrl);
    xTaskCreate(TaskTel,      "TEL",  1024, NULL, PRIO_TEL,  &hTel);
}

void TaskMain(void *pvParameters)
{
	xil_printf("TaskMain START!\n");

	ip_addr_t ipaddr, netmask, gw;
	unsigned char mac_addr[] = { 0x00, 0x18, 0x3E, 0x04, 0x50, 0x91 };

	/* Initialize lwIP stack */
	lwip_init();
	/* Static IPv4 settings */
	IP4_ADDR(&ipaddr,  192,168,1,10);
	IP4_ADDR(&netmask, 255,255,255, 0);
	IP4_ADDR(&gw,      192,168,1, 1);

	/* Register and bring up interface */
	xemac_add(&server_netif, &ipaddr, &netmask, &gw,
			  mac_addr, PLATFORM_EMAC_BASEADDR);
	netif_set_default(&server_netif);
	netif_set_up(&server_netif);

	/* Start Ethernet RX thread */
	sys_thread_new("xemacif_input",
				   (void(*)(void*))xemacif_input_thread,
				   &server_netif,
				   THREAD_STACKSIZE,
				   2);

	/* Delete main_thread after startup */
//	vTaskResume(hSched);
	xil_printf("Netwrok configration END! TaskMain DELETE!\n");
	vTaskDelete(NULL);

}


