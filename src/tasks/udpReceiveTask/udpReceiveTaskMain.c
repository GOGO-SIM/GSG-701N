#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "gsgTypes.h"
#include "global.h"
#include "lwip/tcpip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/api.h"
#include "netif/xadapter.h"
#include "lwip/netbuf.h"
#include "lwip/ip_addr.h"
#include "netif/xemacpsif.h"
#include "xemacps_hw.h"
#include "crc.h"
#include "gsmpUnWrapper.h"
#include "xtime_l.h"

static void explode()
{
	printf("explode\r\n");
	for (;;) ;
	return ;
}

int checkImuData(void)
{
	// 라디안 범위
	return TRUE;
}

int checkSeekerData(void)
{
	// TODO: Los vector(절대 좌표 VS 상대 좌표)
	return TRUE;
}

void handleImuMsg(tGsmpMsg* msg)
{
	if (checkImuData() == ERROR || msg->header.msgStat != OK)
	{
		gFailCount[IMU_DATA_FAIL] += 1;
		if (gFailCount[IMU_DATA_FAIL] > 10)
			explode();
	}
	else
	{
		gFailCount[IMU_DATA_FAIL] = 0;
		// 3. save valid IMU data
		gImuData = gImuPayload;

		// printf("\t[IMU data] x:%f y:%f z:%f || x:%f y:%f z:%f\r\n",
		// 		gImuData.acc.x,
		// 		gImuData.acc.y,
		// 		gImuData.acc.z,
		// 		gImuData.gyro.x,
		// 		gImuData.gyro.y,
		// 		gImuData.gyro.z);
	}
}

void handleSeekerMsg(tGsmpMsg* msg)
{
	if (checkSeekerData() == ERROR || msg->header.msgStat != OK)
	{
		gFailCount[SEEKER_DATA_FAIL] += 1;
		if (gFailCount[SEEKER_DATA_FAIL] > 10)
		{
			explode();
		}
	}
	else
	{

		gFailCount[SEEKER_DATA_FAIL] = 0;
		// 3. save valid Seeker data
		gSeekerData = gSeekerPayload;

		// printf("\t[Seeker data] x:%f y:%f z:%f dist:%f\r\n",
		// 		gSeekerData.los.x,
		// 		gSeekerData.los.y,
		// 		gSeekerData.los.z,
		// 		gSeekerData.distance);
	}
}

static void handleEchoMsg(tGsmpMsg* msg)
{
	// TODO: echoing msg가 이상할 경우 PBIT 태스크에 알려야 함.
}

static void udpReceiveRun()
{
	static tGsmpMsg msg;
	struct netbuf *recvBuf;
	void* udpBuffer;
	err_t err;
	u16_t len;

	// 논블로킹 수신
	while (1)
	{
		err = netconn_recv(gpUdpServerConn, &recvBuf);
		netbuf_data(recvBuf, &udpBuffer, &len);
		if (err == ERR_WOULDBLOCK)
		{
			// xil_printf("there is no more data\r\n");
			break ;
		}
		// 2. check crc
		else if (err != ERR_OK || checkCrc(udpBuffer) == FALSE)
		{
			// xil_printf("something went wrong, it might be CRC Error\r\n");
			gFailCount[UDP_FAIL] += 1;
			if (gFailCount[UDP_FAIL] > 10)
			{
				// 2-1. if failure accumulated 10 times, then explode
				explode();
			}
		}
		else
		{
			gsmpUnwrapper(udpBuffer, &msg);
			gFailCount[UDP_FAIL] = 0;
			// 3. process data based on message type
			// TODO: refactoring into function pointer array
			if (msg.header.msgId == IMU_MSG_ID)
			{
				handleImuMsg(&msg);
			}
			else if (msg.header.msgId == SEEKER_MSG_ID)
			{
				handleSeekerMsg(&msg);
			}
			else if (msg.header.msgId == ACB_ECHO_RECV_MSG_ID)
			{
				handleEchoMsg(&msg);
			}
		}
		netbuf_delete(recvBuf);
	}
}

void udpReceiveTaskMain( void *pvParameters )
{
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        //xil_printf("RUN -- %s\r\n", pcTaskGetName(NULL));

        XTime start, end;
        XTime_GetTime(&start);  // 시작 시간 기록
        udpReceiveRun();
        XTime_GetTime(&end);    // 종료 시간 기록
        // 시간 차이 계산 (us 단위)
        uint64_t elapsed_ticks = end - start;
        uint64_t elapsed_us = elapsed_ticks / (COUNTS_PER_SECOND / 1000000);

        //xil_printf("start: %llu, end: %llu, elapsed: %llu ticks\n", start, end, elapsed_ticks);
        xil_printf("UdpRecv: %llu\r\n", elapsed_ticks);


    }

}
