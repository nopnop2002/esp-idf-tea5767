/* The example of TEA5767
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include "RCSwitch.h"

static char *TAG = "RCSWITCH";

extern QueueHandle_t xQueueCommand;

void rcswitch(void *pvParameters)
{
	ESP_LOGI(TAG, "Start");
	uint32_t ValueMute = 0;
	uint16_t BitsMute = 0;
	uint16_t ProtocolMute = 0;
	uint32_t ValueStation = 0;
	uint16_t BitsStation = 0;
	uint16_t ProtocolStation = 0;

	// Open NVS
	nvs_handle_t my_handle;
	esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "nvs_open error (%s)", esp_err_to_name(err));
		vTaskDelete(NULL);
	}

	// Get NVS
	err = nvs_get_u32(my_handle, "ValueMute", &ValueMute);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "ValueMute get failed");
		vTaskDelete(NULL);
	}

	err = nvs_get_u16(my_handle, "BitsMute", &BitsMute);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "BitsMute get failed");
		vTaskDelete(NULL);
	}

	err = nvs_get_u16(my_handle, "ProtocolMute", &ProtocolMute);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "ProtocolMute get failed");
		vTaskDelete(NULL);
	}

	err = nvs_get_u32(my_handle, "ValueStation", &ValueStation);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "ValueStation get failed");
		vTaskDelete(NULL);
	}

	err = nvs_get_u16(my_handle, "BitsStation", &BitsStation);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "BitsStation get failed");
		vTaskDelete(NULL);
	}

	err = nvs_get_u16(my_handle, "ProtocolStation", &ProtocolStation);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "ProtocolStation get failed");
		vTaskDelete(NULL);
	}

	// Close NVS
	nvs_close(my_handle);
	ESP_LOGI(TAG, "MUTE %"PRIu32" / %dbit Protocol: %d", ValueMute, BitsMute, ProtocolMute);
	ESP_LOGI(TAG, "STATION %"PRIu32" / %dbit Protocol: %d", ValueStation, BitsStation, ProtocolStation);

	// Initialize RF module
	RCSWITCH_t RCSwitch;
	initSwich(&RCSwitch);
	enableReceive(&RCSwitch, CONFIG_RF_GPIO);

	uint32_t Value = 0;
	uint16_t Bits = 0;
	uint16_t Protocol = 0;
	uint16_t ch;
	TickType_t lastTick = 0;
	while (1) {
		if (available(&RCSwitch)) {
			Value = getReceivedValue(&RCSwitch);
			Bits = getReceivedBitlength(&RCSwitch);
			Protocol = getReceivedProtocol(&RCSwitch);
			ESP_LOGI(TAG, "Received %"PRIu32" / %dbit Protocol: %d", Value, Bits, Protocol);
			resetAvailable(&RCSwitch);
			TickType_t nowTick = xTaskGetTickCount();
			TickType_t diffTick = nowTick - lastTick;
			ESP_LOGI(TAG, "diffTick=%"PRIu32, diffTick);
			// Since the transmitter sends the same code multiple times, consecutive receptions are ignored.
			if (diffTick < 100) continue;

			if (Value == ValueMute && Bits == BitsMute && Protocol == ProtocolMute) {
				lastTick = nowTick;
				ch = 'M';
				xQueueSend(xQueueCommand, &ch, portMAX_DELAY);
			}
			if (Value == ValueStation && Bits == BitsStation && Protocol == ProtocolStation) {
				lastTick = nowTick;
				ch = 'S';
				xQueueSend(xQueueCommand, &ch, portMAX_DELAY);
			}
		} else {
			vTaskDelay(1);
		}
	} // end while

	// Never reach here
	vTaskDelete( NULL );
}
