# to1602lcd
Display frequency and signal level on 16x02 line character display.   
I use [this](https://github.com/UncleRus/esp-idf-lib/tree/master/components/hd44780) component.

I'm listening to 80.7MHz.   
8/15 is signal level.   
@ is is monaural signal.   
![Image](https://github.com/user-attachments/assets/fa2a27e0-bcf0-425c-b633-53eeb1902ceb)

I'm listening to 92.9MHz.   
10/15 is signal level.   
@-@ is is streo signal.   
![Image](https://github.com/user-attachments/assets/e4363811-f03e-4319-b5e8-9a88eaaf6897)

# Hardware requirements
Character display using HD44780.   
Usually called 1602LCD/1604LCD.   

# Installation
```
git clone https://github.com/nopnop2002/esp-idf-tea5767
cd esp-idf-tea5767/to1602lcd
idf.py menuconfig
idf.py flash
```

When using the USB port provided by the USB Serial/JTAG Controller Console, you need to enable the following line in sdkconfig.   
```
CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG=y
```


# Configuration for 16x02 line character display
Use GPIO that does not overlap with TEA5767.   
![Image](https://github.com/user-attachments/assets/ee025a5a-3be4-41d3-a7b3-b980f9003a20)
![Image](https://github.com/user-attachments/assets/b4fbb81a-4325-4bc9-b97f-c89d6cb0920b)

# Typical Wireing
|TEA5767|16x02 LCD||ESP32|ESP32-S2/S3|ESP32-C2/C3/C6||
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|SCL||--|GPIO22|GPIO0|GPIO1|(*1)|
|SDA||--|GPIO21|GPIO1|GPIO1|(*1)|
||VSS|--|GND|GND|GND||
||VDD|--|3.3V/5V|3.3V/5V|3.3V/5V|(*2)|
||VO|--||||Variable resistor output|
||RS|--|GPIO19|GPIO2|GPIO2|(*1)|
||RW|--|GND|GND|GND||
||E|--|GPIO18|GPIO3|GPIO3|(*1)|
||D0|--|N/C|N/C|N/C||
||D1|--|N/C|N/C|N/C||
||D2|--|N/C|N/C|N/C||
||D3|--|N/C|N/C|N/C||
||D4|--|GPIO5|GPIO4|GPIO4|(*1)|
||D5|--|GPIO17|GPIO5|GPIO5|(*1)|
||D6|--|GPIO16|GPIO6|GPIO6|(*1)|
||D7|--|GPIO4|GPIO7|GPIO7|(*1)|
||A|--|3.3V/5V|3.3V/5V|3.3V/5V|(*2)|
||K|--|GND|GND|GND|(*3)|

(*1)   
You can change it to any pin using menuconfig.   

(*2)   
The PCB marking is +5V, but it works at 3.3V.   

(*3)   
___A transistor is required for backlight control.___   
```
ESP32 3.3V/5V --------------------- LCD A
                           +------- LCD K
                           |collector
                           |
                     base /
ESP32 BL  -----^^^--------|    SS8050
              100ohm      \
                           |emitter
ESP32 GND -----------------+
```

You can use Digital transistor.   
```
ESP32 3.3V/5V --------------------- LCD A
                           +------- LCD K
                           |OUT
                           |
                       IN /
ESP32 BL  ----------------|    Digital transistor
                           \
                           |GND
ESP32 GND -----------------+
```


# How to use   
Same as [this](https://github.com/nopnop2002/esp-idf-tea5767/tree/main/withKeys).   
