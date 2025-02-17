# withKey
Operate the radio using the keyboard.   

# Installation
```
git clone https://github.com/nopnop2002/esp-idf-tea5767
cd esp-idf-tea5767/withKey
idf.py menuconfig
idf.py flash
```

When using the USB port provided by the USB Serial/JTAG Controller Console, you need to enable the following line in sdkconfig.   
```
CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG=y
```


# How to use   
- Radio station preset   
	You have to edit preset.def in preset directory.   
	Preset.def have radio frequency which you want to select.   
	After which, Build this project and flash it to board.   
	example of preset.def:   
	```
	# This is the radio station preset definition file
	# name,frequency; comment
	ZIP-FM,77.8; JOQV-FM 10kw
	FM-AICHI,80.7; JOCU-FM 10kw
	FM-NHK,82.5; JOCK-FM 10kw
	```

- Search radio station   
	\+ key to search for radio stations upwards.   
	\- key to search for radio stations downwards.   

- Select a preset radio station   
	? key to display a list of presets.   
	0 key to select the first preset.   
	1 key to select the second preset.   
	9 key to select the 9th preset.   
	\* key to set current radio station to default.   
	Record the current radio station to NVS.   
	At boot time, it read from NVS and set to the current Radio station.   
	```idf.py erase-flash``` if clearing NVS.   

- Change frequence   
	D key to decrease the frequence by -1.0Mhz.   
	U key to increase the frequence by +1.0Mhz.   
	d key to decrease the frequence by -0.1Mhz.   
	u key to increase the frequence by +0.1Mhz.   

- Mute audio    
	m key to mute L and R audio.   
	M key to unmute L and R audio.   


