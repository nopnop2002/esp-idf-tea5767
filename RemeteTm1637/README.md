# esp-idf-remote-hd44780
Displays the current radio frequency on the remote display.   
![Image](https://github.com/user-attachments/assets/110ffa59-4673-4f5e-ac65-3d5f67ec5a0b)

# Hardware requirements
TM1637 8 segment Digital Display Tube 4 digit LED module.   
There are several products with different segments and different sizes.   
The clock segment module cannot display real numbers.   
![Image](https://github.com/user-attachments/assets/d1004d68-9a17-4bc2-8f01-33c7c873db9c)

# Installation
```
git clone https://github.com/nopnop2002/esp-idf-tea5767
cd esp-idf-tea5767/RemoteTm1637
idf.py menuconfig
idf.py flash
```


# Configuration
![Image](https://github.com/user-attachments/assets/c4485b72-cb09-4efe-b19a-4ae25f43cca7)
![Image](https://github.com/user-attachments/assets/65b6a801-3ebb-4220-a939-d20670f4dd06)


## WiFi Setting   
Set the information of your access point.   
![Image](https://github.com/user-attachments/assets/fd112275-f686-4a16-ac12-573d81fd7f39)


## Device Setting
![Image](https://github.com/user-attachments/assets/b7c3ba54-dcdc-4d9c-8421-66ce1ef4ce58)
![Image](https://github.com/user-attachments/assets/ec53bacd-641c-4654-aed5-752bc5296b94)

# Wiring
|TM1637||ESP32|ESP32-S2/S3|ESP32-C2/C3/C6||
|:-:|:-:|:-:|:-:|:-:|:-:|
|CLK|--|GPIO22|GPIO12|GPIO6|(*1)|
|DIO|--|GPIO21|GPIO11|GPIO5|(*1)|
|GND|--|GND|GND|GND||
|VCC|--|3.3V|3.3V|3.3V||

(*1)   
The TM1637's interface is similar to I2C, but it is not I2C.   
You can change it to any pin using menuconfig.   
