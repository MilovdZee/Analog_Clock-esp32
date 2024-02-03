# Analog Clock

## Setup LITTLEFS filesystem

~/.arduino15/packages/esp32/tools/mklittlefs/3.0.0-gnu12-dc7f933/mklittlefs \
  --page 256 \
  --size 0x1C2000 \
  --block 4096 \
  --create data data.img

python ~/.arduino15/packages/esp32/tools/esptool_py/4.5.1/esptool.py \
  --port /dev/ttyACM0 \
  --baud 921600 \
  write_flash 0x1F0000 data.img

