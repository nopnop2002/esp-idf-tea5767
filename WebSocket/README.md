# WebSocket
Operate the radio using Web Browser.   
![Image](https://github.com/user-attachments/assets/8d35ab6b-19c8-4f84-88f8-e167282031b5)

I used [this](https://github.com/Molorius/esp32-websocket) component.   
This component can communicate directly with the browser.   
There is an example of using the component [here](https://github.com/Molorius/ESP32-Examples).
It's a great job.   

I used [this](https://github.com/CodeDrome/seven-segment-display-javascript) for segment display.   

# Installation
```
git clone https://github.com/nopnop2002/esp-idf-tea5767
cd esp-idf-tea5767/WebSocket
idf.py menuconfig
idf.py flash
```


# Configuration   
![config-top](https://user-images.githubusercontent.com/6020549/146466041-44d8769e-955f-4ff2-a820-19d7462baa21.jpg)

Set the information of your access point.
![config-wifi-1](https://user-images.githubusercontent.com/6020549/146466210-9d808b99-7782-412d-ac11-fc69a31f66c1.jpg)

You can use Static IP.   
![config-wifi-2](https://user-images.githubusercontent.com/6020549/146466213-bc88ec7a-0a60-4ff5-83d0-332eac07a28b.jpg)

You can use the MDNS hostname instead of the IP address.   
![config-wifi-3](https://user-images.githubusercontent.com/6020549/146466214-1a076345-7f39-4a13-b472-27eeeff3485c.jpg)


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

- Open browser   

- Enter the esp32 address in your browser's address bar   

- Search Up   
 Search for radio stations upwards.   

- Search Down   
 Search for radio stations downwards.   

- Segment Color   
 Change segment color.   

- Goto   
 Goto preset station.   

- As system default   
 Record the current radio station to NVS.   
 At boot time, it read from NVS and set to the current Radio station.   
 If you want to erase the defaults, use ```idf.py Erase-flash```.   
