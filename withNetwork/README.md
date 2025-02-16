# withNetwork
Operate the radio using http/mqtt.   
This project requires a WiFi connection.   

# Installation
```
git clone https://github.com/nopnop2002/esp-idf-tea5767
cd esp-idf-tea5767/withNetwork
idf.py menuconfig
idf.py flash
```

# Configuration
![Image](https://github.com/user-attachments/assets/f869af69-bb54-4935-887f-61abce648f98)
![Image](https://github.com/user-attachments/assets/430ce51d-2e02-4da1-9d6c-3f5e5c1c0f34)

## WiFi Setting   
Set the information of your access point.   
![Image](https://github.com/user-attachments/assets/76a6c710-1c6c-4417-850e-4c572c8b871f)

You can connect using the mDNS hostname instead of the IP address.   
![Image](https://github.com/user-attachments/assets/02e5c814-bcb3-41a4-b211-5caa014a5b28)

You can use static IP.   
![Image](https://github.com/user-attachments/assets/5395edfd-dcdf-418c-8fdb-54c4bdabddb0)

## Network protocol Setting
Using HTTP   
![Image](https://github.com/user-attachments/assets/b31eeab2-1581-4067-8e3f-84c2b56f711e)

Using MQTT   
![Image](https://github.com/user-attachments/assets/127f99f1-7940-485d-83e8-c673856201d9)

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

# API for HTTP

- Search radio station   
	- Search for radio stations upwards.   
	```curl -X POST http://esp32-server.local:8080/api/search/up```   
	- Search for radio stations downwards.   
	```curl -X POST http://esp32-server.local:8080/api/search/down```   

- Select a preset radio station   
	- Select the first preset.   
	```curl -X POST http://esp32-server.local:8080/api/preset/0```   
	- Select the second preset.   
	```curl -X POST http://esp32-server.local:8080/api/preset/1```   
	- Select the 9th preset.   
	```curl -X POST http://esp32-server.local:8080/api/preset/9```   
	- Set current radio station to default.   
	```curl -X POST http://esp32-server.local:8080/api/preset/default```   
	Record the current radio station to NVS.   
	At boot time, it read from NVS and set to the current Radio station.   
	If you want to erase the defaults, use ```idf.py Erase-flash```.   

- Change frequence   
	- Decrease the frequence by -1.0Mhz.   
	```curl -X POST http://esp32-server.local:8080/api/freq/--```   
	- Increase the frequence by +1.0Mhz.   
	```curl -X POST http://esp32-server.local:8080/api/freq/++```   
	- Decrease the frequence by -0.1Mhz.   
	```curl -X POST http://esp32-server.local:8080/api/freq/-```   
	- Increase the frequence by +0.1Mhz.   
	```curl -X POST http://esp32-server.local:8080/api/freq/+```   
	- Setting the frequency directly.   
	```curl -X POST http://esp32-server.local:8080/api/freq/80.7```   

- Mute audio   
	- Mute L and R audio.   
	```curl -X POST http://esp32-server.local:8080/api/mute/on```   
	```curl -X POST http://esp32-server.local:8080/api/mute/1```   
	- Unmute L and R audio.   
	```curl -X POST http://esp32-server.local:8080/api/mute/off```   
	```curl -X POST http://esp32-server.local:8080/api/mute/0```   

- Get current status   
 ```curl http://esp32-server.local:8080/```


# API for MQTT

- Search radio station   
	- Search for radio stations upwards.   
	```mosquitto_pub -h broker.emqx.io -p 1883 -t "/api/search/up" -m ""```   
	- Search for radio stations downwards.   
	```mosquitto_pub -h broker.emqx.io -p 1883 -t "/api/search/down" -m ""```   

- Select a preset radio station   
	- Select the first preset.   
	```mosquitto_pub -h broker.emqx.io -p 1883 -t "/api/preset/0" -m ""```   
	- Select the second preset.   
	```mosquitto_pub -h broker.emqx.io -p 1883 -t "/api/preset/1" -m ""```   
	- Select the 9th preset.   
	```mosquitto_pub -h broker.emqx.io -p 1883 -t "/api/preset/9" -m ""```   
	- Set current radio station to default.   
	```mosquitto_pub -h broker.emqx.io -p 1883 -t "/api/preset/default" -m ""```   
	Record the current radio station to NVS.   
	At boot time, it read from NVS and set to the current Radio station.   
	If you want to erase the defaults, use ```idf.py Erase-flash```.   

- Change frequence   
	- Decrease the frequence by -1.0Mhz.   
	```mosquitto_pub -h broker.emqx.io -p 1883 -t "/api/freq/--" -m ""```   
	- Increase the frequence by +1.0Mhz.   
	```mosquitto_pub -h broker.emqx.io -p 1883 -t "/api/freq/++" -m ""```   
	- Decrease the frequence by -0.1Mhz.   
```mosquitto_pub -h broker.emqx.io -p 1883 -t "/api/freq/-" -m ""```   
	- Increase the frequence by +0.1Mhz.
	```mosquitto_pub -h broker.emqx.io -p 1883 -t "/api/freq/+" -m ""```   
	- Setting the frequency directly.   
	```mosquitto_pub -h broker.emqx.io -p 1883 -t "/api/freq/80.7" -m ""```   

- Mute audio   
	- Mute L and R audio.   
	```mosquitto_pub -h broker.emqx.io -p 1883 -t "/api/mute/on" -m ""```   
	```mosquitto_pub -h broker.emqx.io -p 1883 -t "/api/mute/1" -m ""```   
	- Unmute L and R audio.   
	```mosquitto_pub -h broker.emqx.io -p 1883 -t "/api/mute/off" -m ""```   
	```mosquitto_pub -h broker.emqx.io -p 1883 -t "/api/mute/0" -m ""```   
