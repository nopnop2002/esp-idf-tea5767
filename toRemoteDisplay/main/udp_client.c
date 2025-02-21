/*	BSD Socket UDP Client

	This example code is in the Public Domain (or CC0 licensed, at your option.)

	Unless required by applicable law or agreed to in writing, this
	software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
	CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h" // IP2STR
#include "lwip/sockets.h"
#include "cJSON.h"

#include "status.h"

static const char *TAG = "UDP-CLIENT";

extern QueueHandle_t xQueueStatus;

void udp_client(void *pvParameters)
{
	ESP_LOGI(TAG, "Start UDP PORT=%d", CONFIG_UDP_PORT);

	/* Get the local IP address */
	esp_netif_ip_info_t ip_info;
	ESP_ERROR_CHECK(esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"), &ip_info));
	ESP_LOGI(TAG, "ip_info.ip="IPSTR, IP2STR(&ip_info.ip));

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(CONFIG_UDP_PORT);
#if CONFIG_UDP_LIMITED_BROADCAST
	addr.sin_addr.s_addr = htonl(INADDR_BROADCAST); /* send message to 255.255.255.255 */
	//addr.sin_addr.s_addr = inet_addr("255.255.255.255"); /* send message to 255.255.255.255 */
#elif CONFIG_UDP_DIRECTED_BROADCAST
	char directed[20];
	sprintf(directed, "%d.%d.%d.255", esp_ip4_addr1(&ip_info.ip), esp_ip4_addr2(&ip_info.ip), esp_ip4_addr3(&ip_info.ip));
	ESP_LOGI(TAG, "directed=[%s]", directed);
	//addr.sin_addr.s_addr = inet_addr("192.168.10.255"); /* send message to xxx.xxx.xxx.255 */
	addr.sin_addr.s_addr = inet_addr(directed); /* send message to xxx.xxx.xxx.255 */
#elif CONFIG_UDP_MULTICAST
	addr.sin_addr.s_addr = inet_addr(CONFIG_UDP_MULTICAST_ADDRESS);
#elif CONFIG_UDP_UNICAST
	addr.sin_addr.s_addr = inet_addr(CONFIG_UDP_UNICAST_ADDRESS);
	//addr.sin_addr.s_addr = inet_addr("192.168.10.46");
#endif

	// create the socket
	int sock = lwip_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP ); // Create a UDP socket.
	LWIP_ASSERT("sock >= 0", sock >= 0);

	while (1) {
		STATUS_t status;
		BaseType_t received = xQueueReceive(xQueueStatus, &status, 0);
		ESP_LOGD(TAG, "received=%d", received);
		if (received) {
			int currentFrequence = round(status.currentFrequence * 10);
			ESP_LOGI(TAG, "currentFrequence=%d stereoMode=%d signalLevel=%d/15 muteStatus=%d",
				currentFrequence, status.stereoMode, status.signalLevel, status.muteStatus);

			// Serialize
			cJSON *root;
			root = cJSON_CreateObject();
			cJSON_AddNumberToObject(root, "currentFrequence", currentFrequence);
			cJSON_AddNumberToObject(root, "stereoMode", status.stereoMode);
			cJSON_AddNumberToObject(root, "signalLevel", status.signalLevel);
			cJSON_AddNumberToObject(root, "muteStatus", status.muteStatus);
			char *json_string = cJSON_Print(root);
			ESP_LOGD(TAG, "json_string\n%s",json_string);
			cJSON_Delete(root);

			// UDP send
			int buflen = strlen(json_string);
			int ret = lwip_sendto(sock, json_string, buflen, 0, (struct sockaddr *)&addr, sizeof(addr));
			LWIP_ASSERT("ret == buflen", ret == buflen);
			ESP_LOGD(TAG, "lwip_sendto ret=%d",ret);
			cJSON_free(json_string);
		}
		vTaskDelay(100);
	} // end while

	// Never reach here
	lwip_close(sock);
	vTaskDelete( NULL );
}

