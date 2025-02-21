# toRemoteDisplay
Displays the current radio frequency on the remote display.   

![Image](https://github.com/user-attachments/assets/0678d8c2-5d03-49b8-8ae3-b1d7cc4a8207)
![Image](https://github.com/user-attachments/assets/fd1faf72-d2ea-48ef-80f1-3733756e0366)

# Hardware requirements
Two ESP32s are required.   

# Installation
```
git clone https://github.com/nopnop2002/esp-idf-tea5767
cd esp-idf-tea5767/toRemoteDisplay
idf.py menuconfig
idf.py flash
```

When using the USB port provided by the USB Serial/JTAG Controller Console, you need to enable the following line in sdkconfig.   
```
CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG=y
```


# Configuration
![Image](https://github.com/user-attachments/assets/cf8f5a34-fb74-4577-965b-291e66fd0392)   
Set the information of your access point.   
![Image](https://github.com/user-attachments/assets/b8112ba2-63ad-4cd6-8b62-0ab8235032f8)   


# How to use   
Same as [this](https://github.com/nopnop2002/esp-idf-tea5767/tree/main/withKeys).   
This repository works as a UDP client.   
Requires a UDP listener.   

## TM1637 udp listener
https://github.com/nopnop2002/esp-idf-tea5767/tree/main/RemeteTm1637

