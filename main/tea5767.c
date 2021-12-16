#include <stdio.h>
#include <string.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/i2c.h"
#include "esp_log.h"

#include "tea5767.h"

#define I2C_NUM I2C_NUM_0
//#define I2C_NUM I2C_NUM_1

// The 3-wire bus controls the write/read, clock and data lines and operates at a maximum clock frequency of 400 kHz.
//#define I2C_MASTER_FREQ_HZ 400000 /*!< I2C master clock frequency. no higher than 1MHz for now */
#define I2C_MASTER_FREQ_HZ 200000 /*!< I2C master clock frequency. no higher than 1MHz for now */

#define	I2C_ADDRESS 0x60 

static const char *TAG = "TAE5767";

void radio_init(TEA5767_t * ctrl_data, int16_t sda, int16_t scl) {

	i2c_config_t i2c_config = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = sda,
		.scl_io_num = scl,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = I2C_MASTER_FREQ_HZ
	};
	ESP_ERROR_CHECK(i2c_param_config(I2C_NUM, &i2c_config));
	ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM, I2C_MODE_MASTER, 0, 0, 0));

	ctrl_data->_address = I2C_ADDRESS;
	ctrl_data->port1 = 1;
	ctrl_data->port2 = 1;
	ctrl_data->high_cut = 1;
	ctrl_data->st_noise = 1;
	ctrl_data->soft_mute = 1;
	ctrl_data->deemph_75 = 0;
	ctrl_data->japan_band = 0;
	ctrl_data->pllref = 0;
	ctrl_data->HILO = 1;

	//unsigned long freq = 87500000;
	//set_frequency((float) freq / 1000000);
	
}

//calculate the optimial hi or lo injection mode for the freq is in hz
//return 1 if high is the best, or 0 for low injection
int radio_hilo_optimal (TEA5767_t * ctrl_data, unsigned long freq) {

	int signal_high = 0;
	int signal_low = 0;
	unsigned char buf[5];

	radio_set_frequency_internal (ctrl_data, 1, (double) (freq + 450000) / 1000000);
	delay_ms (30);
	
	// Read the signal level
	if (radio_read_status (ctrl_data, buf) == 1) {
		signal_high = radio_signal_level (ctrl_data, buf);
	}

	radio_set_frequency_internal (ctrl_data, 0, (double) (freq - 450000) / 1000000);
	delay_ms (30);

	if (radio_read_status (ctrl_data, buf) == 1) {
		signal_low = radio_signal_level (ctrl_data, buf);
	}

	return (signal_high < signal_low) ? 1 : 0;
}

/* Set Band Limits to Japanese */
void radio_set_japanese_band (TEA5767_t * ctrl_data) {
	ctrl_data->japan_band = 1;
}

void radio_set_frequency_internal (TEA5767_t * ctrl_data, int hilo, double freq) {
	unsigned char buffer[5];
	unsigned div;
	ESP_LOGD(TAG, "radio_set_frequency_internal hiki=%d freeq=%f", hilo, freq);

	memset (buffer, 0, 5);

	buffer[2] = 0;
	buffer[2] |= TEA5767_PORT1_HIGH;

	if (hilo == 1)
		buffer[2] |= TEA5767_HIGH_LO_INJECT;
	
	buffer[3] = 0;

	if (ctrl_data->port2)
		buffer[3] |= TEA5767_PORT2_HIGH;

	if (ctrl_data->high_cut)
		buffer[3] |= TEA5767_HIGH_CUT_CTRL;

	if (ctrl_data->st_noise)
		buffer[3] |= TEA5767_ST_NOISE_CTL;

	if (ctrl_data->soft_mute)
		buffer[3] |= TEA5767_SOFT_MUTE;

	if (ctrl_data->japan_band)
		buffer[3] |= TEA5767_JAPAN_BAND;

	buffer[3] |= TEA5767_XTAL_32768;
	buffer[4] = 0;

	if (ctrl_data->deemph_75)
		buffer[4] |= TEA5767_DEEMPH_75;

	if (ctrl_data->pllref)
		buffer[4] |= TEA5767_PLLREF_ENABLE;

	if (hilo == 1)
		div = (4 * (freq * 1000 + 225)) / 32.768;
	else
		div = (4 * (freq * 1000 - 225)) / 32.768;

	buffer[0] = (div >> 8) & 0x3f;
	buffer[1] = div & 0xff;

	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (ctrl_data->_address << 1) | I2C_MASTER_WRITE, true);
	for (int i=0; i<5; i++) {
		i2c_master_write_byte(cmd, buffer[i], true);
	}
	i2c_master_stop(cmd);
	esp_err_t espRc = i2c_master_cmd_begin(I2C_NUM, cmd, 10/portTICK_PERIOD_MS);
	if (espRc == ESP_OK) {
		ESP_LOGD(TAG, "radio_set_frequency_internal successfully");
	} else {
		ESP_LOGE(TAG, "radio_set_frequency_internal failed. code: 0x%.2X", espRc);
	}
	i2c_cmd_link_delete(cmd);

}

/* Freq should be specifyed at X M hz */
void radio_set_frequency (TEA5767_t * ctrl_data, double freq) {
	ctrl_data->HILO = radio_hilo_optimal (ctrl_data, (unsigned long) (freq * 1000000));
	radio_set_frequency_internal(ctrl_data, ctrl_data->HILO, freq);
}

// Read Status
int radio_read_status (TEA5767_t * ctrl_data, unsigned char *buf) {
	int ret;
	memset (buf, 0, 5);
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (ctrl_data->_address << 1) | I2C_MASTER_READ, true);
	i2c_master_read(cmd, buf, 5, I2C_MASTER_ACK);
	i2c_master_stop(cmd);
	esp_err_t espRc = i2c_master_cmd_begin(I2C_NUM, cmd, 10/portTICK_PERIOD_MS);
	if (espRc == ESP_OK) {
		ESP_LOGD(TAG, "radio_read_status successfully");
		ESP_LOG_BUFFER_HEXDUMP(TAG, buf, sizeof(buf), ESP_LOG_DEBUG);
		ret = 1;
	} else {
		ESP_LOGE(TAG, "radio_read_status failed. code: 0x%.2X", espRc);
		ret = 0;
	}
	i2c_cmd_link_delete(cmd);
	return ret;
}

int radio_signal_level (TEA5767_t * ctrl_data, unsigned char *buf) {
	int signal = ((buf[3] & TEA5767_ADC_LEVEL_MASK) >> 4);
	return signal;
}

int radio_stereo (TEA5767_t * ctrl_data, unsigned char *buf) {
	int stereo = (buf[2] & TEA5767_STEREO_MASK);
	return stereo ? 1 : 0;
}

//returns 1 if tuning completed or BL reached
int radio_ready (TEA5767_t * ctrl_data, unsigned char *buf) {
	return (buf[0] & 0x80) ? 1 : 0;
}

//returns 1 if band limit is reached during searching
int radio_bl_reached (TEA5767_t * ctrl_data, unsigned char *buf) {
	return (buf[0] & 0x40) ? 1 : 0;
}

//returns freq available in Hz
double radio_frequency_available (TEA5767_t * ctrl_data, unsigned char *buf) {
	double freq_available;
	if (ctrl_data->HILO == 1)
		freq_available = (((buf[0] & 0x3F) << 8) + buf[1]) * 32768 / 4 - 225000;
	else
		freq_available = (((buf[0] & 0x3F) << 8) + buf[1]) * 32768 / 4 + 225000;
	return freq_available;
}

void radio_search_up (TEA5767_t * ctrl_data, unsigned char *buf) {
	unsigned div;
	double freq_av;

	freq_av = radio_frequency_available (ctrl_data, buf);

	div = (4 * (((freq_av + 98304) / 1000000) * 1000000 + 225000)) / 32768;

	buf[0] = (div >> 8) & 0x3f;
	buf[1] = div & 0xff;

	buf[0] |= TEA5767_SEARCH;

	buf[2] = 0;

	buf[2] |= TEA5767_SEARCH_UP;
	buf[2] |= TEA5767_SRCH_MID_LVL;
	//buf[2] |= TEA5767_SRCH_HIGH_LVL;
	buf[2] |= TEA5767_HIGH_LO_INJECT;

	//buf[3] = 0x18;
	buf[3] = 0;

	if (ctrl_data->port2)
		buf[3] |= TEA5767_PORT2_HIGH;

	if (ctrl_data->high_cut)
		buf[3] |= TEA5767_HIGH_CUT_CTRL;

	if (ctrl_data->st_noise)
		buf[3] |= TEA5767_ST_NOISE_CTL;

	if (ctrl_data->soft_mute)
		buf[3] |= TEA5767_SOFT_MUTE;

	if (ctrl_data->japan_band)
		buf[3] |= TEA5767_JAPAN_BAND;

	buf[3] |= TEA5767_XTAL_32768;

	buf[4] = 0;

	if (ctrl_data->deemph_75)
		buf[4] |= TEA5767_DEEMPH_75;

	if (ctrl_data->pllref)
		buf[4] |= TEA5767_PLLREF_ENABLE;

	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (ctrl_data->_address << 1) | I2C_MASTER_WRITE, true);
	for (int i=0; i<5; i++) {
		i2c_master_write_byte(cmd, buf[i], true);
	}
	i2c_master_stop(cmd);
	esp_err_t espRc = i2c_master_cmd_begin(I2C_NUM, cmd, 10/portTICK_PERIOD_MS);
	if (espRc == ESP_OK) {
		ESP_LOGD(TAG, "radio_set_frequency_internal successfully");
	} else {
		ESP_LOGE(TAG, "radio_set_frequency_internal failed. code: 0x%.2X", espRc);
	}
	i2c_cmd_link_delete(cmd);

	ctrl_data->HILO = 1;
}

void radio_search_down (TEA5767_t * ctrl_data, unsigned char *buf)
{
	unsigned div;
	double freq_av;

	freq_av = radio_frequency_available (ctrl_data, buf);

	div = (4 * (((freq_av - 98304) / 1000000) * 1000000 + 225000)) / 32768;

	buf[0] = (div >> 8) & 0x3f;
	buf[1] = div & 0xff;

	buf[0] |= TEA5767_SEARCH;

	buf[2] = 0;

	buf[2] |= TEA5767_SEARCH_DOWN;
	buf[2] |= TEA5767_SRCH_MID_LVL;
	buf[2] |= TEA5767_HIGH_LO_INJECT;

	buf[3] = 0;

	if (ctrl_data->port2)
		buf[3] |= TEA5767_PORT2_HIGH;

	if (ctrl_data->high_cut)
		buf[3] |= TEA5767_HIGH_CUT_CTRL;

	if (ctrl_data->st_noise)
		buf[3] |= TEA5767_ST_NOISE_CTL;

	if (ctrl_data->soft_mute)
		buf[3] |= TEA5767_SOFT_MUTE;

	if (ctrl_data->japan_band)
		buf[3] |= TEA5767_JAPAN_BAND;

	buf[3] |= TEA5767_XTAL_32768;

	buf[4] = 0;

	if (ctrl_data->deemph_75)
		buf[4] |= TEA5767_DEEMPH_75;

	if (ctrl_data->pllref)
	buf[4] |= TEA5767_PLLREF_ENABLE;

	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (ctrl_data->_address << 1) | I2C_MASTER_WRITE, true);
	for (int i=0; i<5; i++) {
		i2c_master_write_byte(cmd, buf[i], true);
	}
	i2c_master_stop(cmd);
	esp_err_t espRc = i2c_master_cmd_begin(I2C_NUM, cmd, 10/portTICK_PERIOD_MS);
	if (espRc == ESP_OK) {
		ESP_LOGD(TAG, "radio_set_frequency_internal successfully");
	} else {
		ESP_LOGE(TAG, "radio_set_frequency_internal failed. code: 0x%.2X", espRc);
	}
	i2c_cmd_link_delete(cmd);

	ctrl_data->HILO = 1;
}

//Returns 1 if search is finished, 0 if wrapped and new search initiated
//TODO - To prevent endless looping add a static variable to abort if it has searched for more than 2 loops
int radio_process_search (TEA5767_t * ctrl_data, unsigned char *buf, int search_dir)
{
	if (radio_ready (ctrl_data, buf) == 1) {
		if (radio_bl_reached (ctrl_data, buf) == 1) {
			if (search_dir == TEA5767_SEARCH_DIR_UP) {
				//wrap down
				if (ctrl_data->japan_band) {
					radio_set_frequency (ctrl_data, TEA5767_JP_FM_BAND_MIN); // 76MHz
				} else {
					radio_set_frequency (ctrl_data, TEA5767_US_FM_BAND_MIN); // 87.5MHz
				}
				radio_read_status (ctrl_data, buf);
				radio_search_up (ctrl_data, buf);
				return 0;
			} else if (search_dir == TEA5767_SEARCH_DIR_DOWN) {
				//wrap up
				if (ctrl_data->japan_band) {
					radio_set_frequency (ctrl_data, TEA5767_JP_FM_BAND_MAX); // 91MHz
				} else {
					radio_set_frequency (ctrl_data, TEA5767_US_FM_BAND_MAX); // 108MHz
				}
				radio_read_status (ctrl_data, buf);
				radio_search_down (ctrl_data, buf);
				return 0;
			}
		} else {
			// search finished - round up the pll word and feed it back as recommended
			double rounded_freq;
			double freq_available = radio_frequency_available (ctrl_data, buf);
			rounded_freq = floor (freq_available / 100000 + .5) / 10;
			radio_set_frequency (ctrl_data, rounded_freq);
			return 1;
		}
	}
	return 0;
}

