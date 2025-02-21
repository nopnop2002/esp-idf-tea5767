/*
	BSD Socket UDP Broadcast Server

	This example code is in the Public Domain (or CC0 licensed, at your option.)

	Unless required by applicable law or agreed to in writing, this
	software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
	CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "cJSON.h"

#include "lwip/sockets.h"
#include "status.h"

static const char *TAG = "UDP-LISTENER";

extern QueueHandle_t xQueueStatus;

void udp_server(void *pvParameters)
{
	ESP_LOGI(TAG, "Start CONFIG_UDP_LISTEN_PORT=%d", CONFIG_UDP_LISTEN_PORT);

	// set up address to recvfrom
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(CONFIG_UDP_LISTEN_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY); /* senderInfo message from ANY */

	// create the socket
	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP ); // Create a UDP socket.
	LWIP_ASSERT("sock >= 0", sock >= 0);

#if 0
	// set option
	int broadcast=1;
	int ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast);
	LWIP_ASSERT("ret >= 0", ret >= 0);
#endif

	// bind socket
	int ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	LWIP_ASSERT("ret >= 0", ret >= 0);

	// senderInfo data
	struct sockaddr_in senderInfo;
	socklen_t senderInfoLen = sizeof(senderInfo);
	
	while(1) {
		char buffer[128];
		int len = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&senderInfo, &senderInfoLen);
		ESP_LOGI(TAG, "recvfrom len=%d errno=%d", len, errno);
		if (len < 0) {
			ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
			break;
		}
		if (len > 0) {
			ESP_LOGD(TAG, "recvfrom buffer=\n%.*s", len, buffer);
			buffer[len] = 0;
			char senderstr[16];
			inet_ntop(AF_INET, &senderInfo.sin_addr, senderstr, sizeof(senderstr));
			ESP_LOGI(TAG, "recvfrom : %s, port=%d", senderstr, ntohs(senderInfo.sin_port));

			// Deserialize
			STATUS_t status;
			cJSON *root2 = cJSON_Parse(buffer);
			int currentFrequence = cJSON_GetObjectItem(root2,"currentFrequence")->valueint;
			ESP_LOGD(TAG, "currentFrequence=%d",currentFrequence);
			status.currentFrequence = (currentFrequence/10.0);
			ESP_LOGD(TAG, "currentFrequence=%f",status.currentFrequence);
			status.stereoMode = cJSON_GetObjectItem(root2,"stereoMode")->valueint;
			ESP_LOGD(TAG, "stereoMode=%d",status.stereoMode);
			status.signalLevel = cJSON_GetObjectItem(root2,"signalLevel")->valueint;
			ESP_LOGD(TAG, "signalLevel=%d",status.signalLevel);
			status.muteStatus = cJSON_GetObjectItem(root2,"muteStatus")->valueint;
			ESP_LOGD(TAG, "muteStatus=%d",status.muteStatus);
			cJSON_Delete(root2);
			xQueueOverwrite(xQueueStatus, &status);
		}
	} // end while

	// close socket
	close(sock);
	vTaskDelete(NULL);
}

