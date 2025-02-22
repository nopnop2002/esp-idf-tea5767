#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include <hd44780.h>
#include <esp_idf_lib_helpers.h>
#include "status.h"

extern QueueHandle_t xQueueStatus;

static const char *TAG = "LCD";

static const uint8_t level0[8] = { 0b11100, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11100};
static const uint8_t level1[8] = { 0b00111, 0b01111, 0b01111, 0b01111, 0b01111, 0b01111, 0b01111, 0b00111};
static const uint8_t level2[8] = { 0b11111, 0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000};
static const uint8_t level3[8] = { 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111};
static const uint8_t level4[8] = { 0b11111, 0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111};
static const uint8_t level5[8] = { 0b01111, 0b00111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00011, 0b00111};
static const uint8_t level6[8] = { 0b11110, 0b11100, 0b00000, 0b00000, 0b00000, 0b00000, 0b11000, 0b11100};
static const uint8_t level7[8] = { 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00111, 0b01111};

#define NONE 0x20

static const uint8_t pattern[] = {
	0x09, 0x0a, 0x08, 0x09, 0x0b, 0x08, //0
	NONE, NONE, 0x08, NONE, NONE, 0x08, //1
	0x0d, 0x0c, 0x08, 0x09, 0x0b, 0x0b, //2
	0x0d, 0x0c, 0x08, 0x0f, 0x0b, 0x08, //3
	0x09, 0x0b, 0x08, NONE, NONE, 0x08, //4
	0x09, 0x0c, 0x0e, 0x0f, 0x0b, 0x08, //5
	0x09, 0x0c, 0x0e, 0x09, 0x0b, 0x08, //6
	0x09, 0x0a, 0x08, NONE, NONE, 0x08, //7
	0x09, 0x0c, 0x08, 0x09, 0x0b, 0x08, //8
	0x09, 0x0c, 0x08, 0x0f, 0x0b, 0x08, //9
};


void lcdNumber(hd44780_t lcd, int num, int pos)
{
	if(num == 10) { // Minus
		hd44780_gotoxy(&lcd, pos, 0);
		hd44780_putc(&lcd, pattern[57]);
		hd44780_gotoxy(&lcd, pos+1, 0);
		hd44780_putc(&lcd, pattern[58]);
		hd44780_gotoxy(&lcd, pos+2, 0);
		hd44780_putc(&lcd, pattern[58]);
		hd44780_gotoxy(&lcd, pos, 1);
		hd44780_putc(&lcd, pattern[7]);
		hd44780_gotoxy(&lcd, pos+1, 1);
		hd44780_putc(&lcd, pattern[7]);
		hd44780_gotoxy(&lcd, pos+2, 1);
		hd44780_putc(&lcd, pattern[7]);
	} else if(num == 11) { // Space
		hd44780_gotoxy(&lcd, pos, 0);
		hd44780_putc(&lcd, ' ');
		hd44780_gotoxy(&lcd, pos+1, 0);
		hd44780_putc(&lcd, ' ');
		hd44780_gotoxy(&lcd, pos+2, 0);
		hd44780_putc(&lcd, ' ');
		hd44780_gotoxy(&lcd, pos, 1);
		hd44780_putc(&lcd, ' ');
		hd44780_gotoxy(&lcd, pos+1, 1);
		hd44780_putc(&lcd, ' ');
		hd44780_gotoxy(&lcd, pos+2, 1);
		hd44780_putc(&lcd, ' ');
   } else {
		hd44780_gotoxy(&lcd, pos, 0);
		hd44780_putc(&lcd, pattern[num*6]);
		hd44780_gotoxy(&lcd, pos+1, 0);
		hd44780_putc(&lcd, pattern[num*6+1]);
		hd44780_gotoxy(&lcd, pos+2, 0);
		hd44780_putc(&lcd, pattern[num*6+2]);
		hd44780_gotoxy(&lcd, pos, 1);
		hd44780_putc(&lcd, pattern[num*6+3]);
		hd44780_gotoxy(&lcd, pos+1, 1);
		hd44780_putc(&lcd, pattern[num*6+4]);
		hd44780_gotoxy(&lcd, pos+2, 1);
		hd44780_putc(&lcd, pattern[num*6+5]);
	}
}

void hd44780(void *pvParameters)
{
	ESP_LOGI(TAG, "Start");
	hd44780_t lcd = {
		.write_cb = NULL,
		.font = HD44780_FONT_5X8,
		.lines = 2,
		.pins = {
			.rs = CONFIG_RS_GPIO,
			.e	= CONFIG_E_GPIO,
			.d4 = CONFIG_D4_GPIO,
			.d5 = CONFIG_D5_GPIO,
			.d6 = CONFIG_D6_GPIO,
			.d7 = CONFIG_D7_GPIO,
			.bl = HD44780_NOT_USED
		}
	};

	// Enable backlight control
	if (CONFIG_BL_GPIO != -1) {
		lcd.pins.bl = CONFIG_BL_GPIO;
		ESP_LOGW(pcTaskGetName(0), "GPIO%d is used for backlight control.", CONFIG_BL_GPIO);
	}

	// Intialize lcd
	ESP_ERROR_CHECK(hd44780_init(&lcd));

	// Create special character
	hd44780_upload_character(&lcd, 0, level0);
	hd44780_upload_character(&lcd, 1, level1);
	hd44780_upload_character(&lcd, 2, level2);
	hd44780_upload_character(&lcd, 3, level3);
	hd44780_upload_character(&lcd, 4, level4);
	hd44780_upload_character(&lcd, 5, level5);
	hd44780_upload_character(&lcd, 6, level6);
	hd44780_upload_character(&lcd, 7, level7);

	while(1) {
		STATUS_t status;
		BaseType_t received = xQueueReceive(xQueueStatus, &status, 0);
		ESP_LOGD(TAG, "received=%d", received);
		if (received) {
			int currentFrequence = round(status.currentFrequence * 10);
			ESP_LOGI(TAG, "currentFrequence=%d stereoMode=%d signalLevel=%d/15 muteStatus=%d",
				currentFrequence, status.stereoMode, status.signalLevel, status.muteStatus);

			if (status.muteStatus == 0) {
				if (CONFIG_BL_GPIO != -1) {
					hd44780_switch_backlight(&lcd, true);
				}
				int digit1 = currentFrequence / 100;
				int digit2 = (currentFrequence / 10) % 10;
				int digit3 = currentFrequence % 10;
				ESP_LOGI(TAG, "digit1=%d digit2=%d digit3=%d", digit1, digit2, digit3);
				hd44780_clear(&lcd);
				if (currentFrequence >= 1000) lcdNumber(lcd, 1, 3);
				lcdNumber(lcd, digit1, 6);
				lcdNumber(lcd, digit2, 9);
				hd44780_gotoxy(&lcd, 12, 1);
				hd44780_putc(&lcd, '.');
				lcdNumber(lcd, digit3, 13);

				hd44780_gotoxy(&lcd, 0, 0);
				char wk[16];
				sprintf(wk, "%d/15", status.signalLevel);
				hd44780_puts(&lcd, wk);

				hd44780_gotoxy(&lcd, 0, 1);
				hd44780_putc(&lcd, '@');
				if (status.stereoMode == 1) {
					hd44780_gotoxy(&lcd, 1, 1);
					hd44780_puts(&lcd, "-@");
				}
			} else {
				hd44780_clear(&lcd);
				if (CONFIG_BL_GPIO != -1) {
					hd44780_switch_backlight(&lcd, false);
				}
			}
		}
		vTaskDelay(100);
	} // end while

	// Never reach here
	vTaskDelete( NULL );
}

