# withGaradeDoorRemote
Operate the radio using the Garade Door Remote.   
![Image](https://github.com/user-attachments/assets/851969e3-fb69-4be6-a727-bd8ea58a3c4f)

# Installation
```
git clone https://github.com/nopnop2002/esp-idf-tea5767
cd esp-idf-tea5767/withGaradeDoorRemote
idf.py menuconfig
idf.py flash
```

# Hardware requirements   
- 315MHz/433MHz Garade Door Remote

- 315MHz/433MHz Receiver   
	This project supports these receivers.   
	H3V3/H3V4   
	SYN480R   
	LR43B/LR33B   
	SRX882/SRX887   

	I used these receivers. From left: H3V4F(433MHz), H3V3E(315MHz), SYN480R(433MHz), SYN480R(315MHz)   
	![Image](https://github.com/user-attachments/assets/f91a7f59-7f5a-49d9-89a1-1d7e3fcf684d)   
	![Image](https://github.com/user-attachments/assets/bc268d1c-89cb-450f-b7c1-bc8ab141f372)   
	![Image](https://github.com/user-attachments/assets/fdd5481a-674b-4f37-8296-552561eff4b0)   

# How to use   
- Radio station preset   
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

- Remote teaching   
	We need to tell ESP32 what kind of wireless signal to use.   
	This project uses two signals.   
	One is used to switch mute.   
	The other is used to select radio stations.

- Play radio   
	Once the teaching is complete, you can use the remote control to operate the radio.  

# Configuration for teaching
![Image](https://github.com/user-attachments/assets/7f6094fa-1905-4bdd-83a3-146162b8a4c9)
![Image](https://github.com/user-attachments/assets/115cffd9-152b-4c15-b9c9-eb489d15b806)

At this timing, press the button used for mute switching.   
![Image](https://github.com/user-attachments/assets/c1da8758-59bb-4e94-a436-616e1ae83979)   
At this timing, press the button used to select the radio station.   
![Image](https://github.com/user-attachments/assets/cda24337-eb54-4d81-aea9-863b9aa47569)   
Teaching is now complete.   
![Image](https://github.com/user-attachments/assets/5275686c-b2dd-4764-adf2-4a914d5a9d71)   


# Configuration for play

![Image](https://github.com/user-attachments/assets/d01bc5f1-b1ae-4872-9733-d80402e659ce)
![Image](https://github.com/user-attachments/assets/98fd909d-164c-4f73-a078-8be682de846b)

