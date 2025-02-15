# esp-idf-tea5767
TEA5767 FM Stereo Radio Driver for esp-idf.

I ported from [here](https://github.com/andykarpov/TEA5767).   

# Software requirements
ESP-IDF V4.4/V5.x.   
ESP-IDF V5.0 is required when using ESP32-C2.   
ESP-IDF V5.1 is required when using ESP32-C6.   

# Hardware requirements   
TEA5767 FM Stereo Radio Module.   
I bought this on AliExpress about $4.   

![tea5767-1](https://user-images.githubusercontent.com/6020549/146292319-adf96f9a-f076-4b4f-be9f-2a2928c0b92f.JPG)
![tea5767-2](https://user-images.githubusercontent.com/6020549/146292325-c70aaddb-6f61-45ca-8de3-42ba3f375876.JPG)

The module has a standard antenna, but if you want to use it in a room, you need a long antenna.   
With a long antenna, you can get more signals.   
I used an AC power cable as extended antena.   
![tea5767-3](https://user-images.githubusercontent.com/6020549/146294473-9b514cf8-ca94-49d8-a723-ec67185ec119.JPG)


# Installation
```
git clone https://github.com/nopnop2002/esp-idf-tea5767
cd esp-idf-tea5767
idf.py menuconfig
idf.py flash
```

# Configuration   

![config-top](https://user-images.githubusercontent.com/6020549/146292879-4be4bc9b-6a2e-4cb9-b0a8-bdad5fae8615.jpg)
![config-tea5767](https://user-images.githubusercontent.com/6020549/146292884-e29e45a4-4f99-4314-bb20-4f03bacbe2f7.jpg)

- CONFIG_SCL_GPIO   
 GPIO number(IOxx) to SCL.
- CONFIG_SDA_GPIO   
 GPIO number(IOxx) to SDA.
- CONFIG_FM_BAND   
 In US/EU it ranges from 87.5 MHz to 108 MHz.   
 In Japan it ranges from 76 MHz to 91 MHz.   
 Used when wrapping in a search.   

# Wireing

|TEA5767||ESP32|ESP32-S2/S3|ESP32-C2/C3/C6||
|:-:|:-:|:-:|:-:|:-:|:-:|
|SCL|--|GPIO22|GPIO4|GPIO6|(*1)|
|SDA|--|GPIO21|GPIO3|GPIO5|(*1)|
|GND|--|GND|GND|GND||
|VCC|--|3.3V|3.3V|3.3V|(*2)|

(*1)   
You can change it to any pin using menuconfig.   
__But it may not work with other GPIOs.__

(*2)   
The PCB marking is +5V, but it works at 3.3V.   


# API
Refer to tea5767.h.   

# Radio staion preset   
You have to edit preset.def in preset directory.   
Preset.def have radio frequency which you want to select.   
After which, Build this project and flash it to board.   

example of preset.def.   
```
# This is the radio station preset definition file
# name,frequency; comment
ZIP-FM,77.8; JOQV-FM 10kw
FM-AICHI,80.7; JOCU-FM 10kw
FM-NHK,82.5; JOCK-FM 10kw
```

# How to use   

- Search radio station   
 \+ key to search for radio stations upwards.   
 \- key to search for radio stations downwards.   

- Preset radio station   
 ? key to display a list of presets.   
 1 key to select the first preset.   
 2 key to select the second preset.   
 9 key to select the 9threset.   
 \* key to record the current radio station in NVS.   
 At boot time, it read from NVS and set to the current Radio station.   

- Change frequence   
 D key to decrease the frequence by -1.0Mhz.   
 U key to increase the frequence by +1.0Mhz.   
 d key to decrease the frequence by -0.1Mhz.   
 u key to increase the frequence by +0.1Mhz.   

- Mute audio    
 m key to L and R audio are muted.   
 M key to L and R audio are not muted.   

- Clear NVS   
 ```idf.py erase-flash```   

# How to use this component in your project   
Create idf_component.yml in the same directory as main.c.   
```
YourProject --+-- CMakeLists.txt
              +-- main --+-- main.c
                         +-- CMakeLists.txt
                         +-- idf_component.yml
```

Contents of idf_component.yml.
```
dependencies:
  nopnop2002/tea5767:
    path: components/tea5767/
    git: https://github.com/nopnop2002/esp-idf-tea5767.git
```

When you build a projects esp-idf will automaticly fetch repository to managed_components dir and link with your code.   
```
YourProject --+-- CMakeLists.txt
              +-- main --+-- main.c
              |          +-- CMakeLists.txt
              |          +-- idf_component.yml
              +-- managed_components ----- nopnop2002__tea5767
```

# Reference   
https://github.com/nopnop2002/esp-idf-fm-radio
