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
	ESP_LOGI(TAG, "Start. CLK_PIN=%d DIO_PIN=%d", CONFIG_TM1637_CLK_PIN, CONFIG_TM1637_DIO_PIN);
	tm1637_led_t * led = tm1637_init(CONFIG_TM1637_CLK_PIN, CONFIG_TM1637_DIO_PIN);
	if (led == NULL) {
		ESP_LOGE(TAG, "tm1637_init fail");
		vTaskDelete(NULL);
	}
	tm1637_set_brightness(led, CONFIG_TM1637_BRIGHTNESS);

#if CONFIG_TM1637_4_SEGMENT
	int startDigit = 2;
#else
	int startDigit = 0;
#endif

	// Test segment control
	uint8_t seg_data[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20};
	for (uint8_t x=0; x<32; ++x)
	{
		uint8_t v_seg_data = seg_data[x%6];
		if (startDigit == 0) {
			tm1637_set_segment_fixed(led, led->segment_idx[0], v_seg_data);
			tm1637_set_segment_fixed(led, led->segment_idx[1], v_seg_data);
		}
		tm1637_set_segment_fixed(led, led->segment_idx[2], v_seg_data);
		tm1637_set_segment_fixed(led, led->segment_idx[3], v_seg_data);
		tm1637_set_segment_fixed(led, led->segment_idx[4], v_seg_data);
		tm1637_set_segment_fixed(led, led->segment_idx[5], v_seg_data);
		vTaskDelay(10);
	}

	// Clear all segment
	for (int index=startDigit;index<6;index++) {
		tm1637_set_segment_fixed(led, led->segment_idx[index], 0x00);
	}

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
				char wk[16];
				if (currentFrequence < 1000) {
					sprintf(wk, " %d", currentFrequence);
				} else {
					sprintf(wk, "%d", currentFrequence);
				}
				// Wait 1000 mill sec in this function
				tm1637_set_segment_ascii_with_time(led, wk, 0x02, 1000); // 123.4
			} else {
				// Clear all segment
				for (int index=startDigit;index<6;index++) {
					tm1637_set_segment_fixed(led, led->segment_idx[index], 0x00);
				}
				vTaskDelay(100);
			}
		} else {
			vTaskDelay(100);
		}
	} // end while

	// Never reach here
	vTaskDelete(NULL);
}

