# Sensors for ESP32 microcontroler

To build & deploy:
1. go to project root
2. configure environment: `source ${ESP_ROOT}/esp-idf/export.sh` (ESP_ROOT is where the [esp-idf](https://github.com/espressif/esp-idf) project was cloned into 
3. set the target:  `idf.py set-target esp32`
4. build the project:  `idf.py build` (if it fails due to previous builds, do a `idf.py fullclean`)
5. plug the development board to the USB
6. find where is connected with `dmesg  | grep usb | grep tty`
5. flash the microcontroler: `idf.py -p /dev/ttyUSB0 flash`


To monitor the serial uart: 

screen /dev/tty/USB0 115200
