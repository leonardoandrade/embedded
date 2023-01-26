setup:
	. ~/software/esp/esp-idf/export.sh
clean:
	killall screen || true

build: */*.c
	idf.py build

flash:
	idf.py -p /dev/ttyUSB0 flash

all: clean build flash
