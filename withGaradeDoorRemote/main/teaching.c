/* The example of TEA5767
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include "RCSwitch.h"

static const char *TAG = "TEACHING";

void teaching(void *pvParameters)
{
	ESP_LOGI(TAG, "Start");

	// Initialize RF module
	RCSWITCH_t RCSwitch;
	initSwich(&RCSwitch);
	enableReceive(&RCSwitch, CONFIG_RF_GPIO);

	ESP_LOGW(TAG, "Press the MUTE button");
	uint32_t ValueMute = 0;
	uint16_t BitsMute = 0;
	uint16_t ProtocolMute = 0;
	while(1) {
		if (available(&RCSwitch)) {
			ValueMute = getReceivedValue(&RCSwitch);
			BitsMute = getReceivedBitlength(&RCSwitch);
			ProtocolMute = getReceivedProtocol(&RCSwitch);
			ESP_LOGI(TAG, "Received %"PRIu32" / %dbit Protocol: %d", ValueMute, BitsMute, ProtocolMute);
			resetAvailable(&RCSwitch);
			break;
		} else {
			vTaskDelay(1);
		}
	} // end while

	// Clear RF
	for (int i=0;i<100;i++) {
		if (available(&RCSwitch)) {
			getReceivedValue(&RCSwitch);
			resetAvailable(&RCSwitch);
		} else {
			vTaskDelay(1);
		}
	}

	ESP_LOGW(TAG, "Press the STATION button");
	uint32_t ValueStation = 0;
	uint16_t BitsStation = 0;
	uint16_t ProtocolStation = 0;
	while(1) {
		if (available(&RCSwitch)) {
			ValueStation = getReceivedValue(&RCSwitch);
			BitsStation = getReceivedBitlength(&RCSwitch);
			ProtocolStation = getReceivedProtocol(&RCSwitch);
			ESP_LOGI(TAG, "Received %"PRIu32" / %dbit Protocol: %d", ValueStation, BitsStation, ProtocolStation);
			resetAvailable(&RCSwitch);
			break;
		} else {
			vTaskDelay(1);
		}
	} // end while

	// Open NVS
	nvs_handle_t my_handle;
	esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "nvs_open error (%s)", esp_err_to_name(err));
		vTaskDelete(NULL);
	}

	// Set NVS
	err = nvs_set_u32(my_handle, "ValueMute", ValueMute);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "ValueMute set failed");
		vTaskDelete(NULL);
	}

	err = nvs_set_u16(my_handle, "BitsMute", BitsMute);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "BitsMute set failed");
		vTaskDelete(NULL);
	}

	err = nvs_set_u16(my_handle, "ProtocolMute", ProtocolMute);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "ProtocolMute set failed");
		vTaskDelete(NULL);
	}

	err = nvs_set_u32(my_handle, "ValueStation", ValueStation);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "ValueStation set failed");
		vTaskDelete(NULL);
	}

	err = nvs_set_u16(my_handle, "BitsStation", BitsStation);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "BitsStation set failed");
		vTaskDelete(NULL);
	}

	err = nvs_set_u16(my_handle, "ProtocolStation", ProtocolStation);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "ProtocolStation set failed");
		vTaskDelete(NULL);
	}

	// Commit written value.
	// After setting any values, nvs_commit() must be called to ensure changes are written
	// to flash storage. Implementations may write to storage at other times,
	// but this is not guaranteed.
	err = nvs_commit(my_handle);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "nvs_commit failed");
		vTaskDelete(NULL);
	}

	// Close NVS
	nvs_close(my_handle);
	ESP_LOGI(TAG, "All done");
	vTaskDelete(NULL);
}
