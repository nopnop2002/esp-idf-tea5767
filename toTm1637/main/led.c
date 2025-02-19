#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "tm1637.h"
#include "status.h"

static char *TAG = "LED";

extern QueueHandle_t xQueueStatus;

void led(void * arg)
{
	ESP_LOGI(TAG, "Start");
	tm1637_led_t * lcd = tm1637_init(CONFIG_TM1637_CLK_PIN, CONFIG_TM1637_DIO_PIN);
	tm1637_set_brightness(lcd, CONFIG_TM1637_BRIGHTNESS);

	while (1) {
		STATUS_t status;
		BaseType_t received = xQueueReceive(xQueueStatus, &status, 0);
		ESP_LOGD(TAG, "received=%d", received);
		if (received) {
			int currentFrequence = round(status.currentFrequence * 10);
			ESP_LOGI(TAG, "currentFrequence=%d stereoMode=%d signalLevel=%d/15 muteStatus=%d",
				currentFrequence, status.stereoMode, status.signalLevel, status.muteStatus);
			if (status.muteStatus == 0) {
				// Show frequency
				tm1637_set_number(lcd, currentFrequence, false, 0x02); // _23.4
				vTaskDelay(100);
				// Show signal level
				tm1637_set_number(lcd, status.signalLevel, false, 0x00);
			} else {
				// Clear lcd
				for (int index=0;index<6;index++) {
					tm1637_set_segment_fixed(lcd, lcd->segment_idx[index], 0x00);
				}
			}
		}
		vTaskDelay(100);
	} // end while

	// Never reach here
	vTaskDelete(NULL);
}

