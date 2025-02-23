# esp-idf-tea5767
TEA5767 FM Stereo Radio Driver for esp-idf.

I ported from [here](https://github.com/andykarpov/TEA5767).   

# Software requirements
ESP-IDF V4.4/V5.x.   
ESP-IDF V5.0 is required when using ESP32-C2.   
ESP-IDF V5.1 is required when using ESP32-C6.   

# Hardware requirements   
- TEA5767 FM Stereo Radio Module   
	I bought this on AliExpress about $4.   

	![tea5767-1](https://user-images.githubusercontent.com/6020549/146292319-adf96f9a-f076-4b4f-be9f-2a2928c0b92f.JPG)
	![tea5767-2](https://user-images.githubusercontent.com/6020549/146292325-c70aaddb-6f61-45ca-8de3-42ba3f375876.JPG)

- External antena   
	The module has a standard antenna, but if you want to use it in a room, you need a long antenna.   
	With a long antenna, you can get more signals.   
	I used an AC power cable as extended antena.   
	![tea5767-3](https://user-images.githubusercontent.com/6020549/146294473-9b514cf8-ca94-49d8-a723-ec67185ec119.JPG)

- Speaker   
	A speaker with a 3.5mm 3-pole mini plug input is required.   
	I bought this for $5.   
	![Image](https://github.com/user-attachments/assets/a9d3712c-3834-4acd-9746-2909fd19f16a)

- Wire Cable   
	3.5mm 3-pole mini plug Male to Male wire cable.   
	Used to connect to speakers.   

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

# Configuration for Radio   

![config-top](https://user-images.githubusercontent.com/6020549/146292879-4be4bc9b-6a2e-4cb9-b0a8-bdad5fae8615.jpg)
![config-tea5767](https://user-images.githubusercontent.com/6020549/146292884-e29e45a4-4f99-4314-bb20-4f03bacbe2f7.jpg)

- CONFIG_SCL_GPIO   
 GPIO number(IOxx) to SCL.
- CONFIG_SDA_GPIO   
 GPIO number(IOxx) to SDA.
- CONFIG_FM_BAND   
 In US/EU it ranges from 87.5 MHz to 108 MHz.   
 In Japan it ranges from 76.1 MHz to 94.9 MHz.   
 Used when wrapping in a search.   

# Wireing

|TEA5767||ESP32|ESP32-S2/S3|ESP32-C2/C3/C6||
|:-:|:-:|:-:|:-:|:-:|:-:|
|SCL|--|GPIO22|GPIO4|GPIO6|(*1)|
|SDA|--|GPIO21|GPIO3|GPIO5|(*1)|
|GND|--|GND|GND|GND||
|VCC(*2)|--|3.3V|3.3V|3.3V||

(*1)   
You can change it to any pin using menuconfig.   

(*2)   
The PCB marking is +5V, but it works at 3.3V.   

# Example project
- Operate the radio using the keyboard   
	Read [this](https://github.com/nopnop2002/esp-idf-tea5767/tree/main/withKeys).   

-	Operate the radio using network   
	Read [this](https://github.com/nopnop2002/esp-idf-tea5767/tree/main/withNetwork).   

-	Operate the radio using Garade Door Remote   
	Read [this](https://github.com/nopnop2002/esp-idf-tea5767/tree/main/withGaradeDoorRemote).   

- Operate the radio using Web Browser   
	Read [this](https://github.com/nopnop2002/esp-idf-tea5767/tree/main/WebSocket).   

- Display radio station frequency on 8 segment LCD   
	Read [this](https://github.com/nopnop2002/esp-idf-tea5767/tree/main/toTm1637).   

- Display radio station frequency on 16x02 line character display   
	Read [this](https://github.com/nopnop2002/esp-idf-tea5767/tree/main/to1602lcd).   

- Display radio station frequency on the remote display   
	Read [this](https://github.com/nopnop2002/esp-idf-tea5767/tree/main/toRemoteDisplay).   

# API
Refer to tea5767.h.   

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

