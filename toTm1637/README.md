# toTm1637
Displays current radio frequency on 8 segment LED.   
I use [this](https://github.com/nopnop2002/esp-idf-tm1637) component.

- 0.36 inch segment   
![Image](https://github.com/user-attachments/assets/6aef7880-8f6d-4b65-ba99-b9457d8f0349)
- 0.56 inch segment   
![Image](https://github.com/user-attachments/assets/c30bd6e9-04a9-4771-ba52-11c04bb174da)

# Hardware requirements
TM1637 8 segment Digital Display Tube 4 digit LED module.   
There are several products with different segments and different sizes.   
I used dot segments.   
![Image](https://github.com/user-attachments/assets/d1004d68-9a17-4bc2-8f01-33c7c873db9c)

# Installation
```
git clone https://github.com/nopnop2002/esp-idf-tea5767
cd esp-idf-tea5767/toTm1637
idf.py menuconfig
idf.py flash
```

When using the USB port provided by the USB Serial/JTAG Controller Console, you need to enable the following line in sdkconfig.   
```
CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG=y
```


# Configuration for TM1637
Use GPIO that does not overlap with TEA5767.   
![Image](https://github.com/user-attachments/assets/e7f94ddf-807a-4a26-9b96-19bc6657efc9)
![Image](https://github.com/user-attachments/assets/9e6fd01b-c5bc-44cd-b4b4-d17c6eaa94a6)

# Wiring example
|TEA5767|TM1637||ESP32|ESP32-S2/S3|ESP32-C2/C3/C6||
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|SCL||--|GPIO22|GPIO0|GPIO0|(*1)|
|SDA||--|GPIO21|GPIO1|GPIO1|(*1)|
||CLK|--|GPIO17|GPIO2|GPIO2|(*1)|
||DIO|--|GPIO16|GPIO3|GPIO3|(*1)|
|GND|GND|--|GND|GND|GND||
|VCC(*2)|VCC|--|3.3V|3.3V|3.3V||

(*1)   
You can change it to any pin using menuconfig.   

(*2)   
The PCB marking is +5V, but it works at 3.3V.   

# How to use   
Same as [this](https://github.com/nopnop2002/esp-idf-tea5767/tree/main/withKeys).   
