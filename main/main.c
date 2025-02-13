/* The example of TEA5767
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <inttypes.h>
#include "math.h"
#include "string.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/message_buffer.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include "cJSON.h"

#include "tea5767.h"

static char *TAG = "MAIN";
static char *KEY = "preset_freq";

MessageBufferHandle_t xMessageBufferMain;

void keyin(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(NULL), "Start");

	uint16_t c;
	while (1) {
		c = fgetc(stdin);
		if (c == 0xffff) {
			vTaskDelay(10);
			continue;
		}
		ESP_LOGI(pcTaskGetName(NULL), "c=0x%x", c);
		if (c == 0x2b) {
			ESP_LOGI(TAG, "Push +");
			cJSON *request;
			request = cJSON_CreateObject();
			cJSON_AddStringToObject(request, "id", "searchup-request");
			char *my_json_string = cJSON_Print(request);
			ESP_LOGD(TAG, "my_json_string\n%s",my_json_string);
			size_t xBytesSent = xMessageBufferSend(xMessageBufferMain, my_json_string, strlen(my_json_string), portMAX_DELAY);
			if (xBytesSent != strlen(my_json_string)) {
				ESP_LOGE(TAG, "xMessageBufferSend fail");
			}
			cJSON_Delete(request);
			cJSON_free(my_json_string);

		} else if (c == 0x2d) {
			ESP_LOGI(TAG, "Push -");
			cJSON *request;
			request = cJSON_CreateObject();
			cJSON_AddStringToObject(request, "id", "searchdown-request");
			char *my_json_string = cJSON_Print(request);
			ESP_LOGD(TAG, "my_json_string\n%s",my_json_string);
			size_t xBytesSent = xMessageBufferSend(xMessageBufferMain, my_json_string, strlen(my_json_string), portMAX_DELAY);
			if (xBytesSent != strlen(my_json_string)) {
				ESP_LOGE(TAG, "xMessageBufferSend fail");
			}
			cJSON_Delete(request);
			cJSON_free(my_json_string);

		} else if (c == 0x2a) {
			ESP_LOGI(TAG, "Push *");
			cJSON *request;
			request = cJSON_CreateObject();
			cJSON_AddStringToObject(request, "id", "preset-request");
			char *my_json_string = cJSON_Print(request);
			ESP_LOGD(TAG, "my_json_string\n%s",my_json_string);
			size_t xBytesSent = xMessageBufferSend(xMessageBufferMain, my_json_string, strlen(my_json_string), portMAX_DELAY);
			if (xBytesSent != strlen(my_json_string)) {
				ESP_LOGE(TAG, "xMessageBufferSend fail");
			}
			cJSON_Delete(request);
			cJSON_free(my_json_string);

		} else if (c == 0x44) {
			ESP_LOGI(TAG, "Push D");
			cJSON *request;
			request = cJSON_CreateObject();
			cJSON_AddStringToObject(request, "id", "minus1.0-request");
			char *my_json_string = cJSON_Print(request);
			ESP_LOGD(TAG, "my_json_string\n%s",my_json_string);
			size_t xBytesSent = xMessageBufferSend(xMessageBufferMain, my_json_string, strlen(my_json_string), portMAX_DELAY);
			if (xBytesSent != strlen(my_json_string)) {
				ESP_LOGE(TAG, "xMessageBufferSend fail");
			}
			cJSON_Delete(request);
			cJSON_free(my_json_string);

		} else if (c == 0x64) {
			ESP_LOGI(TAG, "Push d");
			cJSON *request;
			request = cJSON_CreateObject();
			cJSON_AddStringToObject(request, "id", "minus0.1-request");
			char *my_json_string = cJSON_Print(request);
			ESP_LOGD(TAG, "my_json_string\n%s",my_json_string);
			size_t xBytesSent = xMessageBufferSend(xMessageBufferMain, my_json_string, strlen(my_json_string), portMAX_DELAY);
			if (xBytesSent != strlen(my_json_string)) {
				ESP_LOGE(TAG, "xMessageBufferSend fail");
			}
			cJSON_Delete(request);
			cJSON_free(my_json_string);

		} else if (c == 0x55) {
			ESP_LOGI(TAG, "Push U");
			cJSON *request;
			request = cJSON_CreateObject();
			cJSON_AddStringToObject(request, "id", "plus1.0-request");
			char *my_json_string = cJSON_Print(request);
			ESP_LOGD(TAG, "my_json_string\n%s",my_json_string);
			size_t xBytesSent = xMessageBufferSend(xMessageBufferMain, my_json_string, strlen(my_json_string), portMAX_DELAY);
			if (xBytesSent != strlen(my_json_string)) {
				ESP_LOGE(TAG, "xMessageBufferSend fail");
			}
			cJSON_Delete(request);
			cJSON_free(my_json_string);

		} else if (c == 0x75) {
			ESP_LOGI(TAG, "Push u");
			cJSON *request;
			request = cJSON_CreateObject();
			cJSON_AddStringToObject(request, "id", "plus0.1-request");
			char *my_json_string = cJSON_Print(request);
			ESP_LOGD(TAG, "my_json_string\n%s",my_json_string);
			size_t xBytesSent = xMessageBufferSend(xMessageBufferMain, my_json_string, strlen(my_json_string), portMAX_DELAY);
			if (xBytesSent != strlen(my_json_string)) {
				ESP_LOGE(TAG, "xMessageBufferSend fail");
			}
			cJSON_Delete(request);
			cJSON_free(my_json_string);
		}


	}

	/* Never reach */
	vTaskDelete( NULL );
}

esp_err_t NVS_read_int16(char * key, int16_t *value) {
	// Open NVS
	ESP_LOGI(TAG, "Opening Non-Volatile Storage handle");
	nvs_handle_t my_handle;
	esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "Error (%s) opening NVS handle", esp_err_to_name(err));
		return ESP_FAIL;
	}
	ESP_LOGI(TAG, "nvs_open Done");

	ESP_LOGD(TAG, "NVS_KEY_NAME_MAX_SIZE=%d", NVS_KEY_NAME_MAX_SIZE);
	if (strlen(key) > NVS_KEY_NAME_MAX_SIZE-1) {
		ESP_LOGE(TAG, "Maximal key length is %d", NVS_KEY_NAME_MAX_SIZE-1);
		return ESP_ERR_INVALID_ARG;
	}

	ESP_LOGI(TAG, "NVS_read_int16 Reading [%s] from NVS ... ", key);
	int16_t _value = 0; // value will default to 0, if not set yet in NVS
	err = nvs_get_i16(my_handle, key, &_value);
	switch (err) {
		case ESP_OK:
			ESP_LOGI(TAG, "NVS_read_int16 Done. [%s] = %d", key, _value);
			*value = _value;
			break;
		case ESP_ERR_NVS_NOT_FOUND:
			ESP_LOGW(TAG, "The value is not initialized yet!");
			break;
		default :
			ESP_LOGE(TAG, "Error (%s) read!", esp_err_to_name(err));
			break;
	}

	nvs_close(my_handle);
	return err;
}

esp_err_t NVS_write_int16(char * key, int16_t value) {
	// Open NVS
	ESP_LOGI(TAG, "Opening Non-Volatile Storage handle");
	nvs_handle_t my_handle;
	esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "Error (%s) opening NVS handle", esp_err_to_name(err));
		return ESP_FAIL;
	}
	ESP_LOGI(TAG, "nvs_open Done");

	ESP_LOGD(TAG, "NVS_KEY_NAME_MAX_SIZE=%d", NVS_KEY_NAME_MAX_SIZE);
	if (strlen(key) > NVS_KEY_NAME_MAX_SIZE-1) {
		ESP_LOGE(TAG, "Maximal key length is %d", NVS_KEY_NAME_MAX_SIZE-1);
		return ESP_ERR_INVALID_ARG;
	}

	err = nvs_set_i16(my_handle, key, value);
	ESP_LOGI(TAG, "nvs_set_i16 err=%d", err);
	if (err == ESP_OK) {
		// Commit written value.
		// After setting any values, nvs_commit() must be called to ensure changes are written
		// to flash storage. Implementations may write to storage at other times,
		// but this is not guaranteed.
		ESP_LOGI(TAG, "Committing updates in NVS ... ");
		err = nvs_commit(my_handle);
		ESP_LOGI(TAG, "nvs_commit err=%d", err);
		if (err != ESP_OK) {
			ESP_LOGE(TAG, "Error (%s) commit!", esp_err_to_name(err));
		}
	} else {
		ESP_LOGE(TAG, "Error (%s) write!", esp_err_to_name(err));
	}

	nvs_close(my_handle);
	return err;
}

void app_main()
{
	// Initialize NVS
	esp_err_t err = nvs_flash_init();
	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		// NVS partition was truncated and needs to be erased
		// Retry nvs_flash_init
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
	}
	ESP_ERROR_CHECK( err );

	// Reading preset frequency
	int16_t presetFrequence = 0;
	double current_freq;
	err = NVS_read_int16(KEY, &presetFrequence);
	ESP_LOGI(TAG, "NVS_read_int16=%d presetFrequence=%d", err, presetFrequence);
	if (err == ESP_OK) {
		current_freq = presetFrequence / 10.0; // go to preset frequency
	} else {
#if CONFIG_FM_BAND_US
		current_freq = TEA5767_US_FM_BAND_MIN; // go to station 87.5MHz
#else
		current_freq = TEA5767_JP_FM_BAND_MIN; // go to station 76.0MHz
#endif
	}

	// Create Message Buffer
	xMessageBufferMain = xMessageBufferCreate(1024);
	configASSERT( xMessageBufferMain );

	// Create Task
	xTaskCreate(keyin, "KEYIN", 1024*4, NULL, 2, NULL);

	TEA5767_t ctrl_data;
	ESP_LOGI(TAG, "CONFIG_SDA_GPIO=%d",CONFIG_SDA_GPIO);
	ESP_LOGI(TAG, "CONFIG_SCL_GPIO=%d",CONFIG_SCL_GPIO);
	radio_init(&ctrl_data, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO);

	ESP_LOGI(TAG, "current_freq=%f", current_freq);
	radio_set_frequency(&ctrl_data, current_freq);

	int search_mode = 0;
	int search_direction = 0;
	unsigned char buf[5];
	char cRxBuffer[512];

	while(1) {
		size_t readBytes = xMessageBufferReceive(xMessageBufferMain, cRxBuffer, sizeof(cRxBuffer), 1000/portTICK_PERIOD_MS);
		if (readBytes == 0) {
			//radio_read_status(&ctrl_data, buf);
			if (radio_read_status(&ctrl_data, buf) == 1) {
				//double current_freq =	floor (radio_frequency_available (&ctrl_data, buf) / 100000 + .5) / 10;
				current_freq = floor (radio_frequency_available (&ctrl_data, buf) / 100000 + .5) / 10;
				int stereo = radio_stereo(&ctrl_data, buf);
				int signal_level = radio_signal_level(&ctrl_data, buf);
				ESP_LOGI(TAG, "current_freq=%f stereo=%d signal_level=%d/15", current_freq, stereo, signal_level);
			}

			if (search_mode == 1) {
				ESP_LOGI(TAG, "search_direction=%d", search_direction);
				if (radio_process_search (&ctrl_data, buf, search_direction) == 1) {
					ESP_LOGI(TAG, "process_search success");
					search_mode = 0;
				} else {
					ESP_LOGW(TAG, "process_search fail");
				}
			}
		} else {
			ESP_LOGD(TAG, "cRxBuffer=[%.*s]", readBytes, cRxBuffer);
			cJSON *root = cJSON_Parse(cRxBuffer);
			if (cJSON_GetObjectItem(root, "id")) {
				char *id = cJSON_GetObjectItem(root,"id")->valuestring;
				ESP_LOGI(TAG, "id=[%s]",id);
				if ( strcmp (id, "searchup-request") == 0) {
					search_mode = 1;
					search_direction = TEA5767_SEARCH_DIR_UP;
					radio_search_up(&ctrl_data, buf);
				}

				if ( strcmp (id, "searchdown-request") == 0) {
					search_mode = 1;
					search_direction = TEA5767_SEARCH_DIR_DOWN;
					radio_search_down(&ctrl_data, buf);
				}

				if ( strcmp (id, "preset-request") == 0) {
					presetFrequence = current_freq * 10;
					err = NVS_write_int16(KEY, presetFrequence);
					ESP_LOGI(TAG, "NVS_write_int16=%d, presetFrequence=%d current_freq=%f", err, presetFrequence, current_freq);
				}

				if ( strcmp (id, "minus1.0-request") == 0) {
					double min_freq;
#if CONFIG_FM_BAND_US
					min_freq = TEA5767_US_FM_BAND_MIN;
#endif
#if CONFIG_FM_BAND_JP
					min_freq = TEA5767_JP_FM_BAND_MIN;
#endif
					if (current_freq - 1.0 >= min_freq) {
						current_freq = current_freq - 1.0;
						radio_set_frequency(&ctrl_data, current_freq);
					}
				}

				if ( strcmp (id, "plus1.0-request") == 0) {
					double max_freq;
#if CONFIG_FM_BAND_US
					max_freq = TEA5767_US_FM_BAND_MAX;
#endif
#if CONFIG_FM_BAND_JP
					max_freq = TEA5767_JP_FM_BAND_MAX;
#endif
					if (current_freq + 1.0 <= max_freq) {
						current_freq = current_freq + 1.0;
						radio_set_frequency(&ctrl_data, current_freq);
					}
				}

				if ( strcmp (id, "minus0.1-request") == 0) {
					double min_freq;
#if CONFIG_FM_BAND_US
					min_freq = TEA5767_US_FM_BAND_MIN;
#endif
#if CONFIG_FM_BAND_JP
					min_freq = TEA5767_JP_FM_BAND_MIN;
#endif
					if (current_freq - 0.1 >= min_freq) {
						current_freq = current_freq - 0.1;
						radio_set_frequency(&ctrl_data, current_freq);
					}
				}

				if ( strcmp (id, "plus0.1-request") == 0) {
					double max_freq;
#if CONFIG_FM_BAND_US
					max_freq = TEA5767_US_FM_BAND_MAX;
#endif
#if CONFIG_FM_BAND_JP
					max_freq = TEA5767_JP_FM_BAND_MAX;
#endif
					if (current_freq + 0.1 <= max_freq) {
						current_freq = current_freq + 0.1;
						radio_set_frequency(&ctrl_data, current_freq);
					}
				}

			} // end if
			// Delete a cJSON structure
			cJSON_Delete(root);
		}
	} // end while

}
