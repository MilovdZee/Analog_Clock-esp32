# Analog Clock

<img src="https://raw.githubusercontent.com/MilovdZee/Analog_Clock-esp32/main/images/clocks.jpg" width=500/>

## 3D print files

The 3D print STL files are in the 3D folder. The 'printhouder' is meant for a 'D1 mini esp32'.

<img src="https://raw.githubusercontent.com/MilovdZee/Analog_Clock-esp32/main/images/d1-mini-esp32.jpg" width=200/>

## Setup LittleFS filesystem

```
~/.arduino15/packages/esp32/tools/mklittlefs/3.0.0-gnu12-dc7f933/mklittlefs \
  --page 256 \
  --size 0x1C2000 \
  --block 4096 \
  --create data data.img
```

```
python ~/.arduino15/packages/esp32/tools/esptool_py/4.5.1/esptool.py \
  --port /dev/ttyACM0 \
  --baud 921600 \
  write_flash 0x1F0000 data.img
```
