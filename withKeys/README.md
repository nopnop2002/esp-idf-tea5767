# withKey
Operate the radio using the keyboard.   

# Installation
```
git clone https://github.com/nopnop2002/esp-idf-tea5767
cd esp-idf-tea5767/withKey
idf.py menuconfig
idf.py flash
```

# How to use   
- Radio staion preset   
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

- Preset radio station   
 ? key to display a list of presets.   
 1 key to select the first preset.   
 2 key to select the second preset.   
 9 key to select the 9th preset.   
 \* key to record the current radio station in NVS.   
 At boot time, it read from NVS and set to the current Radio station.   

- Change frequence   
 D key to decrease the frequence by -1.0Mhz.   
 U key to increase the frequence by +1.0Mhz.   
 d key to decrease the frequence by -0.1Mhz.   
 u key to increase the frequence by +0.1Mhz.   

- Mute audio    
 m key to mute L and R audio.   
 M key to unmute L and R audio.   

- Clear NVS   
 ```idf.py erase-flash```   

