/* The example of TEA5767
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

static char *TAG = "KEYIN";

extern QueueHandle_t xQueueCommand;

void keyin(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(NULL), "Start");

	uint16_t ch;
	while (1) {
		ch = fgetc(stdin);
		if (ch == 0xffff) {
			vTaskDelay(10);
			continue;
		}
		ESP_LOGD(TAG, "ch=0x%x", ch);
		xQueueSend(xQueueCommand, &ch, portMAX_DELAY);
	}

	// Never reach here
	vTaskDelete( NULL );
}
