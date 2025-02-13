/*
 * TEA5767.h
 * defintions for TEA5767 FM radio module.
 * Created by Andrey Karpov
 * Based on this project: http://kalum.posterous.com/arduino-with-tea5767-single-chip-radio-and-no
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef _TEA5767_H_
#define _TEA5767_H_

/******************************
 * Write mode register values *
 ******************************/

/* First register */
#define TEA5767_MUTE             0x80    /* Mutes output */
#define TEA5767_SEARCH           0x40    /* Activates station search */
/* Bits 0-5 for divider MSB */

/* Second register */
/* Bits 0-7 for divider LSB */

/* Third register */

/* Station search from botton to up */
#define TEA5767_SEARCH_DOWN      0x00
#define TEA5767_SEARCH_UP        0x80

/* Searches with ADC output = 10 */
#define TEA5767_SRCH_HIGH_LVL    0x60

/* Searches with ADC output = 7 */
#define TEA5767_SRCH_MID_LVL     0x40

/* Searches with ADC output = 5 */
#define TEA5767_SRCH_LOW_LVL     0x20

/* if on, div=4*(Frf+Fif)/Fref otherwise, div=4*(Frf-Fif)/Freq) */
#define TEA5767_HIGH_LO_INJECT   0x10

/* Disable stereo */
#define TEA5767_MONO             0x08

/* Disable right channel and turns to mono */
#define TEA5767_MUTE_RIGHT       0x04

/* Disable left channel and turns to mono */
#define TEA5767_MUTE_LEFT        0x02

#define TEA5767_PORT1_HIGH       0x01

/* Fourth register */
#define TEA5767_PORT2_HIGH       0x80
/* Chips stops working. Only I2C bus remains on */
#define TEA5767_STDBY            0x40

/* Japan freq (76-108 MHz. If disabled, 87.5-108 MHz */
#define TEA5767_JAPAN_BAND       0x20

/* Unselected means 32.768 KHz freq as reference. Otherwise Xtal at 13 MHz */
#define TEA5767_XTAL_32768       0x10

/* Cuts weak signals */
#define TEA5767_SOFT_MUTE        0x08

/* Activates high cut control */
#define TEA5767_HIGH_CUT_CTRL    0x04

/* Activates stereo noise control */
#define TEA5767_ST_NOISE_CTL     0x02

/* If activate PORT 1 indicates SEARCH or else it is used as PORT1 */
#define TEA5767_SRCH_IND         0x01

/* Fifth register */

/* By activating, it will use Xtal at 13 MHz as reference for divider */
#define TEA5767_PLLREF_ENABLE    0x80

/* By activating, deemphasis=50, or else, deemphasis of 50us */
#define TEA5767_DEEMPH_75        0X40

/* US/Europe FM band (87.5 MHz to 108 MHz) */
#define TEA5767_US_FM_BAND_MIN   87.5
#define TEA5767_US_FM_BAND_MAX   108.0

/* Japanese FM band (76 MHz to 91 MHz) FM Band */
#define TEA5767_JP_FM_BAND_MIN   76.0
#define TEA5767_JP_FM_BAND_MAX   91.0

/*****************************
 * Read mode register values *
 *****************************/

/* First register */
#define TEA5767_READY_FLAG_MASK  0x80
#define TEA5767_BAND_LIMIT_MASK  0X40
/* Bits 0-5 for divider MSB after search or preset */

/* Second register */
/* Bits 0-7 for divider LSB after search or preset */

/* Third register */
#define TEA5767_STEREO_MASK      0x80
#define TEA5767_IF_CNTR_MASK     0x7f

/* Fourth register */
#define TEA5767_ADC_LEVEL_MASK   0xf0

/* should be 0 */
#define TEA5767_CHIP_ID_MASK     0x0f

/* Fifth register */
/* Reserved for future extensions */
#define TEA5767_RESERVED_MASK    0xff

/* internal constants */
#define TEA5767_SEARCH_DIR_UP    1
#define TEA5767_SEARCH_DIR_DOWN  2

//#include <Arduino.h>

typedef struct tea5767_ctrl
{
	int _address;
  int port1;
  int port2;
  int high_cut;
  int st_noise;
  int soft_mute;
  int japan_band;
  int deemph_75;
  int pllref;
  int xtal_freq;
	int HILO;
} TEA5767_t;


#define delay_ms(delay_time) vTaskDelay(delay_time/portTICK_PERIOD_MS)

int radio_hilo_optimal (TEA5767_t * ctrl_data, unsigned long freq);
int radio_ready (TEA5767_t * ctrl_data, unsigned char *buf);
int radio_bl_reached (TEA5767_t * ctrl_data, unsigned char *buf);

void radio_set_japanese_band (TEA5767_t * ctrl_data);
void radio_set_frequency_internal (TEA5767_t * ctrl_data, int hilo, double freq);
void radio_set_frequency (TEA5767_t * ctrl_data, double freq);
int radio_read_status (TEA5767_t * ctrl_data, unsigned char *buf);
int radio_signal_level (TEA5767_t * ctrl_data, unsigned char *buf);
int radio_stereo (TEA5767_t * ctrl_data, unsigned char *buf);
double radio_frequency_available (TEA5767_t * ctrl_data, unsigned char *buf);
void radio_search_up (TEA5767_t * ctrl_data, unsigned char *buf);
void radio_search_down (TEA5767_t * ctrl_data, unsigned char *buf);
int radio_process_search (TEA5767_t * ctrl_data, unsigned char *buf, int search_dir);
void radio_init(TEA5767_t * ctrl_data, int16_t sda, int16_t scl);

#endif  // _TEA5767_H_

