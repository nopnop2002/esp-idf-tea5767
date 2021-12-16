# esp-idf-tea5767
TEA5767 FM Stereo Radio Driver for esp-idf.

I ported from [here](https://github.com/andykarpov/TEA5767).   

# Hardware requirements   
TEA5767 FM Stereo Radio Module.   
I bought this on AliExpress about $4.   

![tea5767-1](https://user-images.githubusercontent.com/6020549/146292319-adf96f9a-f076-4b4f-be9f-2a2928c0b92f.JPG)
![tea5767-2](https://user-images.githubusercontent.com/6020549/146292325-c70aaddb-6f61-45ca-8de3-42ba3f375876.JPG)

The module has a standard antenna, but if you want to use it in a room, you need a long antenna.   
With a long antenna, you can get more signals.   
I used an AC power cable.   

![tea5767-3](https://user-images.githubusercontent.com/6020549/146294473-9b514cf8-ca94-49d8-a723-ec67185ec119.JPG)


# Installation for ESP32

```
git clone https://github.com/nopnop2002/esp-idf-tea5767
cd esp-idf-tea5767
idf.py set-target esp32
idf.py menuconfig
idf.py flash
```

# Installation for ESP32-S2

```
git clone https://github.com/nopnop2002/esp-idf-tea5767
cd esp-idf-tea5767
idf.py set-target esp32s2
idf.py menuconfig
idf.py flash
```

# Installation for ESP32-C3

```
git clone https://github.com/nopnop2002/esp-idf-tea5767
cd esp-idf-tea5767
idf.py set-target esp32c3
idf.py menuconfig
idf.py flash
```

# Configuration   
You have to set this config value with menuconfig.   
- CONFIG_SCL_GPIO   
 GPIO number(IOxx) to SCL.
- CONFIG_SDA_GPIO   
 GPIO number(IOxx) to SDA.
- CONFIG_FM_BAND   
 In US/EU it ranges from 87.5 MHz to 108 MHz.   
 In Japan it ranges from 76 MHz to 91 MHz.   
 Used when wrapping in a search.   

![config-top](https://user-images.githubusercontent.com/6020549/146292879-4be4bc9b-6a2e-4cb9-b0a8-bdad5fae8615.jpg)
![config-tea5767](https://user-images.githubusercontent.com/6020549/146292884-e29e45a4-4f99-4314-bb20-4f03bacbe2f7.jpg)

# Wireing

|TEA5767||ESP32|ESP32-S2|ESP32-C3|
|:-:|:-:|:-:|:-:|:-:|
|SCL|--|GPIO22|GPIO12|GPIO9|
|SDA|--|GPIO21|GPIO11|GPIO10|
|GND|--|GND|GND|GND|
|VCC|--|3.3V|3.3V|3.3V|

__You can change it to any pin using menuconfig.__   
__But it may not work with other GPIOs.__


# API
Refer to tea5767.h.   

# How to use   

- Search radio station   
 Press the + key to search for radio stations upwards.   
 Press the - key to search for radio stations downwards.   

- Preset radio station   
 Press the * key to record the current radio station in NVS.   
 At boot time, it read from NVS and set to the current Radio station.   

- Clear preset   
 idf.py erase-flash   

