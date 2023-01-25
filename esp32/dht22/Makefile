killall_screen:
	killall screen || true

build:
	idf.py build

flash:
	idf.py -p /dev/ttyUSB0 flash

all: killall_screen build flash
