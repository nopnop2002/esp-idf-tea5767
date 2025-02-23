/* The example of TEA5767
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_vfs.h"
#include "esp_spiffs.h"
#include "esp_log.h"

#include "tea5767.h"

static char *TAG = "RADIO";
static char *DEFAULT_FREQ = "DEFAULT_FREQ";

extern QueueHandle_t xQueueCommand;

#define MAX_PRESET 20
#define MAX_NAME 32
typedef struct {
	char name[MAX_NAME+1];
	float frequency;
} PRESET_t;

esp_err_t NVS_read_int16(char * key, int16_t *value) {
	// Check key length
	ESP_LOGD(TAG, "NVS_KEY_NAME_MAX_SIZE=%d", NVS_KEY_NAME_MAX_SIZE);
	if (strlen(key) > NVS_KEY_NAME_MAX_SIZE-1) {
		ESP_LOGE(TAG, "Maximal key length is %d", NVS_KEY_NAME_MAX_SIZE-1);
		return ESP_ERR_INVALID_ARG;
	}

	// Open NVS
	ESP_LOGI(TAG, "Opening Non-Volatile Storage handle");
	nvs_handle_t my_handle;
	esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "Error (%s) opening NVS handle", esp_err_to_name(err));
		return ESP_FAIL;
	}
	ESP_LOGD(TAG, "nvs_open Done");

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
	// Check key length
	ESP_LOGD(TAG, "NVS_KEY_NAME_MAX_SIZE=%d", NVS_KEY_NAME_MAX_SIZE);
	if (strlen(key) > NVS_KEY_NAME_MAX_SIZE-1) {
		ESP_LOGE(TAG, "Maximal key length is %d", NVS_KEY_NAME_MAX_SIZE-1);
		return ESP_ERR_INVALID_ARG;
	}

	// Open NVS
	ESP_LOGI(TAG, "Opening Non-Volatile Storage handle");
	nvs_handle_t my_handle;
	esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "Error (%s) opening NVS handle", esp_err_to_name(err));
		return ESP_FAIL;
	}
	ESP_LOGD(TAG, "nvs_open Done");

	ESP_LOGI(TAG, "NVS_write_int16 Writing [%s] to NVS ... ", key);
	err = nvs_set_i16(my_handle, key, value);
	ESP_LOGI(TAG, "nvs_set_i16 err=%d", err);
	if (err == ESP_OK) {
		ESP_LOGI(TAG, "NVS_write_int16 Done. [%s] = %d", key, value);
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

static int parseLine(char *line, int size1, int size2, char arr[size1][size2])
{
	ESP_LOGD(__FUNCTION__, "line=[%s]", line);
	int dst = 0;
	int pos = 0;
	int llen = strlen(line);
	bool inq = false;

	for(int src=0;src<llen;src++) {
		char c = line[src];
		ESP_LOGD(__FUNCTION__, "src=%d c=%c", src, c);
		if (c == ',') {
			if (inq) {
				if (pos == (size2-1)) continue;
				arr[dst][pos++] = line[src];
				arr[dst][pos] = 0;
			} else {
				ESP_LOGD(__FUNCTION__, "arr[%d]=[%s]",dst,arr[dst]);
				dst++;
				if (dst == size1) break;
				pos = 0;
			}

		} else if (c == ';') {
			if (inq) {
				if (pos == (size2-1)) continue;
				arr[dst][pos++] = line[src];
				arr[dst][pos] = 0;
			} else {
				ESP_LOGD(__FUNCTION__, "arr[%d]=[%s]",dst,arr[dst]);
				dst++;
				break;
			}

		} else if (c == '"') {
			inq = !inq;

		} else if (c == '\'') {
			inq = !inq;

		} else {
			if (pos == (size2-1)) continue;
			arr[dst][pos++] = line[src];
			arr[dst][pos] = 0;
		}
	}

	return dst;
}

static int readPresetFile(char *filename, PRESET_t *preset, size_t maxLine, size_t maxText) {
	int readLine = 0;
	ESP_LOGD(__FUNCTION__, "Reading file:maxText=%d",maxText);
	FILE* f = fopen(filename, "r");
	if (f == NULL) {
		ESP_LOGE(__FUNCTION__, "Failed to open define file for reading");
		return 0;
	}
	char line[64];
	char result[10][32];
	while (1){
		if ( fgets(line, sizeof(line) ,f) == 0 ) break;
		// strip newline
		char* pos = strchr(line, '\n');
		if (pos) {
			*pos = '\0';
		}
		ESP_LOGD(__FUNCTION__, "line=[%s]", line);
		if (strlen(line) == 0) continue;
		if (line[0] == '#') continue;

		int ret = parseLine(line, 10, 32, result);
		ESP_LOGD(__FUNCTION__, "parseLine=%d", ret);
		for(int i=0;i<ret;i++) ESP_LOGD(__FUNCTION__, "result[%d]=[%s]", i, &result[i][0]);
		strlcpy(preset[readLine].name, &result[0][0], maxText+1);
		preset[readLine].frequency = strtof(&result[1][0], NULL);
		ESP_LOGD(__FUNCTION__, "name=[%s] frequency=%f", preset[readLine].name, preset[readLine].frequency);

		readLine++;
		if (readLine == maxLine) break;
	}
	fclose(f);
	return readLine;
}

void radio(void *pvParameters)
{
	char *base_path = (char *)pvParameters;
	ESP_LOGI(TAG, "Start base_path=[%s]", base_path);

	// Reading preset frequency
	char filename[64];
	sprintf(filename, "%s/preset.def", base_path);
	ESP_LOGI(TAG, "filename=[%s]", filename);
	PRESET_t preset[MAX_PRESET];
	int presets = readPresetFile(filename, preset, MAX_PRESET, MAX_NAME);
	if (presets == 0) {
		ESP_LOGE(TAG, "You need to set preset");
		vTaskDelete(NULL);
	}
	for(int i=0;i<presets;i++) {
		ESP_LOGI(TAG, "preset[%d] name=[%s] frequency=%f", i, preset[i].name, preset[i].frequency);
	}

	// Read default frequency
	int16_t defaultFrequence = 0;
	double currentFrequence;
	esp_err_t err = NVS_read_int16(DEFAULT_FREQ, &defaultFrequence);
	ESP_LOGI(TAG, "NVS_read_int16=%d defaultFrequence=%d", err, defaultFrequence);
	if (err == ESP_OK) {
		currentFrequence = defaultFrequence / 10.0; // go to default frequency
	} else {
		currentFrequence = preset[0].frequency;
	}

	// Set current radio index
	int current_preset_index = 0;
	for (int i=0;i<presets;i++) {
		ESP_LOGI(TAG, "preset[%d] name=[%s] frequency=%f", i, preset[i].name, preset[i].frequency);
		int wk1 = round(currentFrequence * 10);
		int wk2 = round(preset[i].frequency * 10);
		ESP_LOGI(TAG, "wk1=%d wk2=%d", wk1, wk2);
		if (wk1 == wk2) current_preset_index = i;
	}
	ESP_LOGI(TAG, "current_preset_index=%d", current_preset_index);

	// Initialize radio
	TEA5767_t ctrl_data;
	ESP_LOGI(TAG, "CONFIG_SDA_GPIO=%d",CONFIG_SDA_GPIO);
	ESP_LOGI(TAG, "CONFIG_SCL_GPIO=%d",CONFIG_SCL_GPIO);
	radio_init(&ctrl_data, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO);

	// Set current frequency
	ESP_LOGI(TAG, "currentFrequence=%f", currentFrequence);
	radio_set_frequency(&ctrl_data, currentFrequence);

	unsigned char buf[5];

	while(1) {
		char ch;
		BaseType_t readBytes = xQueueReceive(xQueueCommand, &ch, 1000/portTICK_PERIOD_MS);
		ESP_LOGD(TAG, "readBytes=%d", readBytes);
		if (readBytes == 0) {
			//radio_read_status(&ctrl_data, buf);
			if (radio_read_status(&ctrl_data, buf) == 1) {
				//double currentFrequence =	floor (radio_frequency_available (&ctrl_data, buf) / 100000 + .5) / 10;
				currentFrequence = floor (radio_frequency_available (&ctrl_data, buf) / 100000 + .5) / 10;
				int stereoMode = radio_stereo(&ctrl_data, buf);
				int signalLevel = radio_signal_level(&ctrl_data, buf);
				ESP_LOGI(TAG, "currentFrequence=%f stereoMode=%d signalLevel=%d/15 mute=%d", currentFrequence, stereoMode, signalLevel, ctrl_data.mute);
			}

		} else {
			ESP_LOGD(TAG, "ch=0x%x", ch);
			if (ch == 0x4d) { // 'M'
				ctrl_data.mute = !ctrl_data.mute;
				radio_set_frequency(&ctrl_data, currentFrequence);

			// Change radio station
			} else if (ch == 0x53) { // 'S'
				current_preset_index++;
				if (current_preset_index == presets) current_preset_index=0;
				currentFrequence = preset[current_preset_index].frequency;
				ESP_LOGI(TAG, "current_preset_index=%d", current_preset_index);
				ESP_LOGI(TAG, "currentFrequence=%f", currentFrequence);
				radio_set_frequency(&ctrl_data, currentFrequence);

				// Set default frequency
				defaultFrequence = round(currentFrequence * 10);
				ESP_LOGI(TAG, "defaultFrequence=%d", defaultFrequence);
				err = NVS_write_int16(DEFAULT_FREQ, defaultFrequence);
			}
		}
	} // end while

	// Never reach here
	vTaskDelete(NULL);
}
