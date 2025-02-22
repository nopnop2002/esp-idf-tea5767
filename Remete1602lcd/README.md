# Remote1602lcd
Displays the current radio frequency on the remote display.   
Use with [this](https://github.com/nopnop2002/esp-idf-tea5767/tree/main/toRemoteDisplay).   
I'm listening to 92.9MHz as a stereo signal.   
![Image](https://github.com/user-attachments/assets/184d5160-53b4-4248-9488-a93fa019533e)
Sometimes it's a mono signal.   
![Image](https://github.com/user-attachments/assets/8c09811b-9db0-4019-a1fe-7f577694a163)

# Hardware requirements
Character display using HD44780.   
Usually called 1602LCD/1604LCD.   

# Installation
```
git clone https://github.com/nopnop2002/esp-idf-tea5767
cd esp-idf-tea5767/Remote1602lcd
idf.py menuconfig
idf.py flash
```

# Configuration
![Image](https://github.com/user-attachments/assets/a9de5538-28b4-4155-8aa0-946594ce6505)
![Image](https://github.com/user-attachments/assets/445d5375-b7b9-4212-b737-56b1138b23bd)

## WiFi Setting   
Set the information of your access point.   
![Image](https://github.com/user-attachments/assets/1e58895b-f568-4301-8ac0-2a21c9b423cf)

## Device Setting
![Image](https://github.com/user-attachments/assets/de560fa5-fc0e-45c7-b0a2-b6fdd0d083ad)

# Wiring
|16x02 LCD||ESP32|ESP32-S2/S3|ESP32-C2/C3/C6||
|:-:|:-:|:-:|:-:|:-:|:-:|
|VSS|--|GND|GND|GND||
|VDD|--|3.3V/5V|3.3V/5V|3.3V/5V|(*2)|
|VO|--||||Variable resistor output|
|RS|--|GPIO19|GPIO2|GPIO2|(*1)|
|RW|--|GND|GND|GND||
|E|--|GPIO18|GPIO3|GPIO3|(*1)|
|D0|--|N/C|N/C|N/C||
|D1|--|N/C|N/C|N/C||
|D2|--|N/C|N/C|N/C||
|D3|--|N/C|N/C|N/C||
|D4|--|GPIO5|GPIO4|GPIO4|(*1)|
|D5|--|GPIO17|GPIO5|GPIO5|(*1)|
|D6|--|GPIO16|GPIO6|GPIO6|(*1)|
|D7|--|GPIO4|GPIO7|GPIO7|(*1)|
|A|--|3.3V/5V|3.3V/5V|3.3V/5V|(*2)|
|K|--|GND|GND|GND|(*3)|

(*1)   
You can change it to any pin using menuconfig.   

(*2)   
5V for 5V modules. 3.3V for 3.3V modules.   

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

