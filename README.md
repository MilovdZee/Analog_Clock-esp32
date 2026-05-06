# Analog Clock

<img src="https://raw.githubusercontent.com/MilovdZee/Analog_Clock-esp32/main/images/clocks.jpg" width=500/>

## All in one

run `./compile.sh`. This will do the following:
- download the arduino-cli
- download old version of the ESP32 core (otherwise it won't fit)
- create the SPIFFS partition data
- compile the sketch
- upload the data and sketch to the board

## Update CA
- echo | openssl s_client -servername raw.githubusercontent.com -connect raw.githubusercontent.com:443 -showcerts | grep -E "subject=|issuer="
- Take highest depth: depth=2 C=US, O=Internet Security Research Group, CN=ISRG Root X1
  => ISRG Root X1
  => cat /etc/ssl/certs/ISRG_Root_X1.pem
- use this cert as the CA

## Setup

- Write the LittleFS
- Write the program using the Arduino IDE
- Connect your computer or phone to th 'RondKlokje' wifi network
- Open http://192.168.4.1
- Goto 'wifi' and enter the credentials

## 3D print files

The 3D print STL files are in the 3D folder. The 'printhouder' is meant for a 'D1 mini esp32'.

<img src="https://raw.githubusercontent.com/MilovdZee/Analog_Clock-esp32/main/images/d1-mini-esp32.jpg" width=200/>

## Setup LittleFS filesystem

```
~/.arduino15/packages/esp32/tools/mklittlefs/4.0.2-db0513a/mklittlefs \
  --page 256 \
  --size 0x1d0000 \
  --block 4096 \
  --create data data.img
```

```
~/.arduino15/packages/esp32/tools/esptool_py/5.2.0/esptool \
  --port /dev/ttyACM0 \
  --baud 921600 \
  write_flash 0x230000 data.img
```
