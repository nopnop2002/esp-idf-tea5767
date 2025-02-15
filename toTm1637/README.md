# toTm1637
Display frequency and signal level on 8 segment LCD.   
I use [this](https://github.com/nopnop2002/esp-idf-tm1637) component.

![Image](https://github.com/user-attachments/assets/df16a991-69cc-4c42-99b6-1512548d0938)
![Image](https://github.com/user-attachments/assets/2b135c03-ea3a-40dd-ba99-c4a5e21b1312)

# Hardware requirements
TM1637 8 segment Digital Display Tube 4 digit LED module.   
There are several products with different segments and different sizes.   
We use 0.36 Inch dot segment.   
![Image](https://github.com/user-attachments/assets/d1004d68-9a17-4bc2-8f01-33c7c873db9c)

# Installation
```
git clone https://github.com/nopnop2002/esp-idf-tea5767
cd esp-idf-tea5767/toTm1637
idf.py menuconfig
idf.py flash
```

# Configuration for TM1637
Use GPIO that does not overlap with TEA1637.   
![Image](https://github.com/user-attachments/assets/e7f94ddf-807a-4a26-9b96-19bc6657efc9)
![Image](https://github.com/user-attachments/assets/9e6fd01b-c5bc-44cd-b4b4-d17c6eaa94a6)

# Typical Wireing
|TEA5767|TM1637||ESP32|ESP32-S2/S3|ESP32-C2/C3/C6||
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|SCL||--|GPIO22|GPIO4|GPIO6|(*1)|
|SDA||--|GPIO21|GPIO3|GPIO5|(*1)|
||CLK|--|GPIO17|GPIO12|GPIO4|(*1)|
||DIO|--|GPIO16|GPIO11|GPIO3|(*1)|
|GND|GND|--|GND|GND|GND||
|VCC|VCC|--|3.3V|3.3V|3.3V||

(*1)   
You can change it to any pin using menuconfig.   
__But it may not work with other GPIOs.__


# How to use   
Same as [this](https://github.com/nopnop2002/esp-idf-tea5767/tree/main/withKeys).   
