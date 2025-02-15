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
#include "status.h"

static char *TAG = "RADIO";
static char *KEY = "preset_freq";

extern QueueHandle_t xQueueCommand;
extern QueueHandle_t xQueueStatus;

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
	ESP_LOGI(TAG, "nvs_open Done");

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
	ESP_LOGI(TAG, "nvs_open Done");

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

static int parseLine(char *line, int size1, int size2, char arr[size1][size2])
{
	ESP_LOGD(TAG, "line=[%s]", line);
	int dst = 0;
	int pos = 0;
	int llen = strlen(line);
	bool inq = false;

	for(int src=0;src<llen;src++) {
		char c = line[src];
		ESP_LOGD(TAG, "src=%d c=%c", src, c);
		if (c == ',') {
			if (inq) {
				if (pos == (size2-1)) continue;
				arr[dst][pos++] = line[src];
				arr[dst][pos] = 0;
			} else {
				ESP_LOGD(TAG, "arr[%d]=[%s]",dst,arr[dst]);
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
				ESP_LOGD(TAG, "arr[%d]=[%s]",dst,arr[dst]);
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
	ESP_LOGI(pcTaskGetName(0), "Reading file:maxText=%d",maxText);
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
		ESP_LOGI(__FUNCTION__, "line=[%s]", line);
		if (strlen(line) == 0) continue;
		if (line[0] == '#') continue;

		int ret = parseLine(line, 10, 32, result);
		ESP_LOGI(TAG, "parseLine=%d", ret);
		for(int i=0;i<ret;i++) ESP_LOGI(__FUNCTION__, "result[%d]=[%s]", i, &result[i][0]);
		strlcpy(preset[readLine].name, &result[0][0], maxText+1);
		preset[readLine].frequency = strtof(&result[1][0], NULL);
		ESP_LOGI(__FUNCTION__, "name=[%s] frequency=%f", preset[readLine].name, preset[readLine].frequency);

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

	// Reading default frequency
	int16_t presetFrequence = 0;
	double current_freq;
	esp_err_t err = NVS_read_int16(KEY, &presetFrequence);
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

	// Set frequency limit
	double min_freq;
	double max_freq;
#if CONFIG_FM_BAND_US
		min_freq = TEA5767_US_FM_BAND_MIN;
		max_freq = TEA5767_US_FM_BAND_MAX;
#endif
#if CONFIG_FM_BAND_JP
		min_freq = TEA5767_JP_FM_BAND_MIN;
		max_freq = TEA5767_JP_FM_BAND_MAX;
#endif

	// Reading preset frequency
	char filename[64];
	sprintf(filename, "%s/preset.def", base_path);
	ESP_LOGI(TAG, "filename=[%s]", filename);
	PRESET_t preset[MAX_PRESET];
	int presets = readPresetFile(filename, preset, MAX_PRESET, MAX_NAME);

	// Initialize radio
	TEA5767_t ctrl_data;
	ESP_LOGI(TAG, "CONFIG_SDA_GPIO=%d",CONFIG_SDA_GPIO);
	ESP_LOGI(TAG, "CONFIG_SCL_GPIO=%d",CONFIG_SCL_GPIO);
	radio_init(&ctrl_data, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO);

	// Set current frequency
	ESP_LOGI(TAG, "current_freq=%f", current_freq);
	radio_set_frequency(&ctrl_data, current_freq);

	int search_mode = 0;
	int search_direction = 0;
	unsigned char buf[5];

	while(1) {
		char ch;
		BaseType_t readBytes = xQueueReceive(xQueueCommand, &ch, 1000/portTICK_PERIOD_MS);
		ESP_LOGD(TAG, "readBytes=%d", readBytes);
		if (readBytes == 0) {
			//radio_read_status(&ctrl_data, buf);
			if (radio_read_status(&ctrl_data, buf) == 1) {
				//double current_freq =	floor (radio_frequency_available (&ctrl_data, buf) / 100000 + .5) / 10;
				current_freq = floor (radio_frequency_available (&ctrl_data, buf) / 100000 + .5) / 10;
				int stereo = radio_stereo(&ctrl_data, buf);
				int signal_level = radio_signal_level(&ctrl_data, buf);
				ESP_LOGI(TAG, "current_freq=%f stereo=%d signal_level=%d/15 mute=%d", current_freq, stereo, signal_level, ctrl_data.mute);

				STATUS_t status;
				status.current_freq = current_freq;
				status.stereo = stereo;
				status.signal_level = signal_level;
				status.mute = ctrl_data.mute;
				xQueueOverwrite(xQueueStatus, &status);
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
			ESP_LOGD(TAG, "ch=0x%x", ch);
			if (ch == 0x2b) { // +
				search_mode = 1;
				search_direction = TEA5767_SEARCH_DIR_UP;
				radio_search_up(&ctrl_data, buf);
			} else if (ch == 0x2d) { // -
				search_mode = 1;
				search_direction = TEA5767_SEARCH_DIR_DOWN;
				radio_search_down(&ctrl_data, buf);
			} else if (ch == 0x2a) { // *
				presetFrequence = current_freq * 10;
				err = NVS_write_int16(KEY, presetFrequence);
				ESP_LOGI(TAG, "NVS_write_int16=%d, presetFrequence=%d current_freq=%f", err, presetFrequence, current_freq);
			} else if (ch == 0x44) { // D
				if (current_freq - 1.0 >= min_freq) {
					current_freq = current_freq - 1.0;
					radio_set_frequency(&ctrl_data, current_freq);
				}
			} else if (ch == 0x55) { // U
				if (current_freq + 1.0 <= max_freq) {
					current_freq = current_freq + 1.0;
					radio_set_frequency(&ctrl_data, current_freq);
				}
			} else if (ch == 0x64) { // d
				if (current_freq - 0.1 >= min_freq) {
					current_freq = current_freq - 0.1;
					radio_set_frequency(&ctrl_data, current_freq);
				}
			} else if (ch == 0x75) { // u
				if (current_freq + 0.1 <= max_freq) {
					current_freq = current_freq + 0.1;
					radio_set_frequency(&ctrl_data, current_freq);
				}
			} else if (ch == 0x3f) { // ?
				for (int i=0;i<presets;i++) {
					ESP_LOGI(TAG, "preset[%d] name=[%s] frequency=%f", i, preset[i].name, preset[i].frequency);
				}
			} else if (ch >= 0x30 && ch <= 0x39) { // 0-9
				int index = ch - 0x30;
				if (index < presets) {
					ESP_LOGI(TAG, "preset[%d] name=[%s] frequency=%f", index, preset[index].name, preset[index].frequency);
					current_freq = preset[index].frequency;
					radio_set_frequency(&ctrl_data, current_freq);
				} else {
					ESP_LOGW(TAG, "preset[%d] not found", index);
				}
			} else if (ch == 0x4d) { // M
				ctrl_data.mute = false;
				radio_set_frequency(&ctrl_data, current_freq);
			} else if (ch == 0x6d) { // m
				ctrl_data.mute = true;
				radio_set_frequency(&ctrl_data, current_freq);
			} // end if
		}
	} // end while

	// Never reach here
	vTaskDelete(NULL);
}
